/******************************************************************************
 * @file     sysupdate.c
 * @brief    system update source code file for RN821x series SOC
 * @author   Renergy Technology
 * @version  V1.00
 * @date     2012-11-28
 *
 * @note
 * Copyright (C) 2008, Renergy Technology Inc. All rights reserved.
 *
 *
 ******************************************************************************/

#include "RN821x.h"
#include "system_RN821x.h"
#include "sysctrl.h"
#include <stdio.h>

#define SPL0_OFFSET  0x00c08000
#define PAGE_SIZE    64

/* reg offset address in eeprom spline */
#define ANAADC_OFFSET      8
#define ANACLK1_OFFSET     9
#define ANACLK2_OFFSET     10
#define ANALDO_OFFSET      11
#define ANABGR_OFFSET      12
#define ANAPSW_OFFSET      13
//#define RTCTADJ_OFFSET     14
//#define RTCZT_OFFSET       15
#define TPSCTRL_OFFSET     16
#define ADCTEST_OFFSET     17
#define CACHECTRL_OFFSET   18

#define RTCMODE_OFFSET     4
#define RTCDOTA0_OFFSET    5
#define RTCALPHAL_OFFSET   6
#define RTCALPHAH_OFFSET   7
#define RTCXT0_OFFSET      8
#define RTCBETA_OFFSET     9
#define WDTCTRL_OFFSET     10
#define EMAP_OFFSET        11
#define WDTHALT_OFFSET     12
#define WDTSTBY_OFFSET     13	
#define RTCTADJ_OFFSET     14
#define RTCZT_OFFSET       15
#define EMAP4_OFFSET       30

/* reg address */
#define ANAADC_REG         *(uint32_t *)(0x40034048)
#define ANACLK1_REG        *(uint32_t *)(0x4003404c)
#define ANACLK2_REG        *(uint32_t *)(0x40034050)
#define ANALDO_REG         *(uint32_t *)(0x40034054)
#define ANABGR_REG         *(uint32_t *)(0x40034058)
#define ANAPSW_REG         *(uint32_t *)(0x4003405c)
#define RTCTADJ_REG        *(uint32_t *)(0x4003C058)
#define RTCZT_REG          *(uint32_t *)(0x4003C060)
#define TPSCTRL_REG        *(uint32_t *)(0x4002C018)
#define ADCTEST_REG        *(uint32_t *)(0x4002C01c)

#define RTCMODE_REG        *(uint32_t *)(0x4003C044)
#define RTCDOTA0_REG       *(uint32_t *)(0x4003C048)
#define RTCALPHAL_REG      *(uint32_t *)(0x4003C04c)
#define RTCALPHAH_REG      *(uint32_t *)(0x4003C050)
#define RTCXT0_REG         *(uint32_t *)(0x4003C054)
#define RTCBETA_REG        *(uint32_t *)(0x4003C05c)
#define WDTCTRL_REG        *(uint32_t *)(0x40030004)
#define WDTHALT_REG        *(uint32_t *)(0x40030014)
#define WDTSTBY_REG        *(uint32_t *)(0x40030018)
#define EMAP_REG           *(uint32_t *)(0x40034018)

/* reg default value */
#define ANAADC_REG_DEF         0x40
#define ANACLK1_REG_DEF        0x420
#define ANACLK2_REG_DEF        0x14
#define ANALDO_REG_DEF         0x58
#define ANABGR_REG_DEF         0x6158
#define ANAPSW_REG_DEF         0x10
#define TPSCTRL_REG_DEF        0x7bb
#define ADCTEST_REG_DEF        0x28
#define CACHECTRL_REG_DEF      0x1

#define RTCMODE_REG_DEF        0x1
#define RTCDOTA0_REG_DEF       0x0
#define RTCALPHAL_REG_DEF      0x404
#define RTCALPHAH_REG_DEF      0x4c9
#define RTCXT0_REG_DEF         0x64
#define RTCBETA_REG_DEF        0xff
#define WDTCTRL_REG_DEF        0x6e
#define WDTHALT_REG_DEF        0x7e5a
#define WDTSTBY_REG_DEF        0xa538
#define EMAP_REG_DEF           0x0
#define RTCTADJ_REG_DEF        0x66a
#define RTCZT_REG_DEF          0x116

#define FEED_WDT               WDT->EN = 0xbb

static uint8_t   eeChkSum(uint16_t *ee_addr, uint8_t line)
{
  uint32_t  chksum = 0, stop_pos;
  uint32_t  i;
  
  if(line == 1)
  {
    stop_pos = (PAGE_SIZE / 2) - 4;
  }
  else
  {
    stop_pos = (PAGE_SIZE / 2) - 2;
  }
  
  for(i = 0; i < stop_pos; i += 2)
  {
    chksum += (ee_addr[i] | (ee_addr[i + 1] << 16));
  }
  
  if(((chksum + (ee_addr[i] | (ee_addr[i + 1] << 16))) != 0) ||
     (ee_addr[PAGE_SIZE/2 - 5] != 0xffff) ||
     (ee_addr[PAGE_SIZE/2 - 6] != 0xffff)) /* checksum err */
  {
    return 1;
  }
  
  return 0;
}

/* mode = 0: FLASH, mode = 1: EEPROM */
static uint32_t  _getMipAddr(uint8_t mode)
{
  uint32_t addr;
  switch (*(uint32_t *)(0x40034018) & 0x7)
  {
    case 0:
      addr =  (mode == 0) ? 0x00000000 : 0x08000000;
      break;
    case 1:
      addr = (mode == 0) ? 0x08000000 : 0x00000000;
      break;
    case 2:
      addr = (mode == 0) ? 0x10000000 : 0x08000000;
      break;
    case 3:
      addr = (mode == 0) ? 0x18000000 : 0x08000000;
      break;
  }
  return addr;
}

/* RTC sync time:6LOSC_Cycle(32K) */
static void RtcWait(void)
{
  uint32_t   i;
  for(i = 0; i < 2000; i++)
  {
    if(!(RTC->CTRL & (1 << 9)))
    {
      break;
    }
  }
}

/* RTC sync time:6RCL_Cycle(8.75K/4) */
static void WdtWait(void)
{
  uint32_t   i;
  for(i = 0; i < 28000; i++)
  {
    if(!(WDT->EN & (1 << 9)))
    {
      break;
    }
  }
}

void SysUpdate(void)
{
  uint16_t  *ee_addr;
  uint32_t  sys_ps;
  uint32_t  mod0_en, mod1_en;
  uint8_t   spl_fail;
  uint32_t  rtc_ctrl;
  
  sys_ps = SYSCTL->SYS_PS;
  
  SYSCTL->SYS_PS = 0x82;
  SYSCTL->MOD1_EN |= (1 << 9);   //WDT APB
  FEED_WDT;
  
  mod0_en = SYSCTL->MOD0_EN;
  mod1_en = SYSCTL->MOD1_EN;
  
  ee_addr = (uint16_t *)(_getMipAddr(1) + SPL0_OFFSET);
  spl_fail = eeChkSum(ee_addr, 0);
  
  SYSCTL->SYS_PS = 0x75;
  
  if(spl_fail)
  {
    ANAADC_REG = 0xa5770000 | ANAADC_REG_DEF;
  }
  else if(ee_addr[ANAADC_OFFSET] != ANAADC_REG)
  {
    ANAADC_REG = 0xa5770000 | ee_addr[ANAADC_OFFSET];
  }
  
  if(spl_fail)
  {
    ANACLK1_REG = 0xa5270000 | (ANACLK1_REG | (1 << 9));  /* power down first */   
    ANACLK1_REG = 0xa5270000 | ANACLK1_REG_DEF;
  }
  else if(ee_addr[ANACLK1_OFFSET] != ANACLK1_REG)
  {
    ANACLK1_REG = 0xa5270000 | (ANACLK1_REG | (1 << 9));  /* power down first */   
    ANACLK1_REG = 0xa5270000 | ee_addr[ANACLK1_OFFSET];
  }
  
  if(spl_fail)
  {
    ANACLK2_REG = 0xa5360000 | ANACLK2_REG_DEF;
  }
  else if(ee_addr[ANACLK2_OFFSET] != ANACLK2_REG)
  {
    ANACLK2_REG = 0xa5360000 | ee_addr[ANACLK2_OFFSET];
  }
  
  if(spl_fail)
  {
    ANALDO_REG = 0xa5580000 | ANALDO_REG_DEF;
  }
  else if(ee_addr[ANALDO_OFFSET] != ANALDO_REG)
  {
    ANALDO_REG = 0xa5580000 | ee_addr[ANALDO_OFFSET];
  }
  
  if(spl_fail)
  {
    ANABGR_REG = 0xa5280000 | ANABGR_REG_DEF;
  }
  else if(ee_addr[ANABGR_OFFSET] != ANABGR_REG)
  {
    ANABGR_REG = 0xa5280000 | ee_addr[ANABGR_OFFSET];
  }
  
  if(spl_fail)
  {
    ANAPSW_REG = 0xa5790000 | ANAPSW_REG_DEF;
  }
  else if(ee_addr[ANAPSW_OFFSET] != ANAPSW_REG)
  {
    ANAPSW_REG = 0xa5790000 | ee_addr[ANAPSW_OFFSET];
  }
      
  SYSCTL->SYS_PS = 0x82;
  SYSCTL->MOD1_EN |= (1 << 11);  /* SAR */
  if(spl_fail)
  {
		TPSCTRL_REG |= (0x7 << 13);
    TPSCTRL_REG = TPSCTRL_REG_DEF;
  }
  else if(ee_addr[TPSCTRL_OFFSET] != TPSCTRL_REG)
  {
		TPSCTRL_REG |= (0x7 << 13);
    TPSCTRL_REG = ee_addr[TPSCTRL_OFFSET];
  }
      
  if(spl_fail)
  {
		ADCTEST_REG |= (0xa << 8);
    ADCTEST_REG = ADCTEST_REG_DEF;
  }
  else if(ee_addr[ADCTEST_OFFSET] != ADCTEST_REG)
  {
		ADCTEST_REG |= (0xa << 8);
    ADCTEST_REG = ee_addr[ADCTEST_OFFSET];
  }
  
  {
    uint32_t  *cc_addr;
    cc_addr = (uint32_t *)(_getMipAddr(0) + 0x0080001c);
    
    if(spl_fail)
    {
      *cc_addr = 0xfeed0000 | CACHECTRL_REG_DEF;
    }
    else if(ee_addr[CACHECTRL_OFFSET] != *cc_addr)
    {
      *cc_addr = 0xfeed0000 | ee_addr[CACHECTRL_OFFSET];
    }
  }
  
  ee_addr = (uint16_t *)(_getMipAddr(1) + SPL0_OFFSET + PAGE_SIZE);
  spl_fail = eeChkSum(ee_addr, 1);
  
  /*
  SYSCTL->SYS_PS = 0x75;
  if(ee_addr[EMAP_OFFSET] != (EMAP_REG & 0x10))
  {
    if(spl_fail)
    {
      EMAP_REG = EMAP_REG_DEF;
    }
    else
    {
      EMAP_REG = ee_addr[EMAP_OFFSET];
    }
  }
  
  SYSCTL->SYS_PS = 0x82;
  if((ee_addr[EMAP4_OFFSET] == ((~0x7e5a) & 0xffff)) && (ee_addr[EMAP4_OFFSET + 1] == 0x7e5a))
  {
    if((EMAP_REG & 0x7) != 0x4)
    {
      EMAP_REG = 0x4;
    }
  }
  else
  {
    if((EMAP_REG & 0x7) != 0x0)
    {
      EMAP_REG = 0x0;
    }
  }
  */
  
  SYSCTL->SYS_PS = 0x82;
  SYSCTL->MOD1_EN |= (1 << 9);   /* WDT */
  *(uint32_t *)(0x40030008) = 0x70;
  *(uint32_t *)(0x40030008) = 0x7E;
  WdtWait();
  if(spl_fail)
  {
    WDTCTRL_REG = WDTCTRL_REG_DEF;
  }
  else if(ee_addr[WDTCTRL_OFFSET] != WDTCTRL_REG)
  {
    WDTCTRL_REG = ee_addr[WDTCTRL_OFFSET];
  }
  WdtWait();
  
  WdtWait();
  if(spl_fail)
  {
    WDTHALT_REG = WDTHALT_REG_DEF;
  }
  else if(ee_addr[WDTHALT_OFFSET] != WDTHALT_REG)
  {
    WDTHALT_REG = ee_addr[WDTHALT_OFFSET];
  }
  WdtWait();
  
  WdtWait();
  if(spl_fail)
  {
    WDTSTBY_REG = WDTSTBY_REG_DEF;
  }
  else if(ee_addr[WDTSTBY_OFFSET] != WDTSTBY_REG)
  {
    WDTSTBY_REG = ee_addr[WDTSTBY_OFFSET];
  }
  WdtWait();
  *(uint32_t *)(0x40030008) = 0x0;
            
  SYSCTL->SYS_PS = 0x82;
  SYSCTL->MOD0_EN |= (1 << 9);  
  rtc_ctrl = RTC->CTRL;
  
  RtcWait();
  RTC->CTRL = 0x100;
  RtcWait();
  
  *(uint32_t *)(0x4003C040) = 0x8e;
  RtcWait();
  
  if(spl_fail)
  {
    RTCMODE_REG = RTCMODE_REG_DEF;
  }
  else if(ee_addr[RTCMODE_OFFSET] != RTCMODE_REG)
  {
    RTCMODE_REG = ee_addr[RTCMODE_OFFSET];
  }
  /*   comment@2013-1-29 18:18:06, changed by user
  if(ee_addr[RTCDOTA0_OFFSET] != RTCDOTA0_REG)
  {
    RTCDOTA0_REG = ee_addr[RTCDOTA0_OFFSET];
  }
  */
  if(spl_fail)
  {
    RTCALPHAL_REG = RTCALPHAL_REG_DEF;
  }
  else if(ee_addr[RTCALPHAL_OFFSET] != RTCALPHAL_REG)
  {
    RTCALPHAL_REG = ee_addr[RTCALPHAL_OFFSET];
  }
  
  if(spl_fail)
  {
    RTCALPHAH_REG = RTCALPHAH_REG_DEF;
  }
  else if(ee_addr[RTCALPHAH_OFFSET] != RTCALPHAH_REG)
  {
    RTCALPHAH_REG = ee_addr[RTCALPHAH_OFFSET];
  }
  
  if(spl_fail)
  {
    RTCXT0_REG = RTCXT0_REG_DEF;
  }
  else if(ee_addr[RTCXT0_OFFSET] != RTCXT0_REG)
  {
    RTCXT0_REG = ee_addr[RTCXT0_OFFSET];
  }
  
  if(spl_fail)
  {
    RTCBETA_REG = RTCBETA_REG_DEF;
  }
  else if(ee_addr[RTCBETA_OFFSET] != RTCBETA_REG)
  {
    RTCBETA_REG = ee_addr[RTCBETA_OFFSET];
  }
       
  if(spl_fail)
  { 
    RTCTADJ_REG = RTCTADJ_REG_DEF;
  }
  else if(ee_addr[RTCTADJ_OFFSET] != RTCTADJ_REG)
  {
    RTCTADJ_REG = ee_addr[RTCTADJ_OFFSET];
  }
  
  if(spl_fail)
  {
    RTCZT_REG = RTCZT_REG_DEF;
  }
  else if(ee_addr[RTCZT_OFFSET] != RTCZT_REG)
  {
    RTCZT_REG = ee_addr[RTCZT_OFFSET];
  }
  
  *(uint32_t *)(0x4003C040) = 0x0;
  RtcWait();
  RTC->CTRL = rtc_ctrl;
  RtcWait();
    
  FEED_WDT;
  
  SYSCTL->SYS_PS = 0x82;
  SYSCTL->MOD0_EN = mod0_en;
  SYSCTL->MOD1_EN = mod1_en;  
  
  SYSCTL->SYS_PS = sys_ps;
}
