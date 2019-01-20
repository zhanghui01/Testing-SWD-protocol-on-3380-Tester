/******************************************************************************
 * @file     sysctrl.c
 * @brief    System control driver source code file for RN821x series SOC
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

#define FL_TW_32K  (0 << 16)
#define FL_TW_1M   (0 << 16)
#define FL_TW_3M   (0 << 16)
#define FL_TW_7M   (2 << 16)
#define FL_TW_14M  (2 << 16)
#define FL_TW_29M  (5 << 16)

#define EE_TW_32K  (0 << 16)
#define EE_TW_1M   (0 << 16)
#define EE_TW_3M   (0 << 16)
#define EE_TW_7M   (0 << 16)
#define EE_TW_14M  (2 << 16)
#define EE_TW_29M  (3 << 16)
//#define EE_TW_29M  (5 << 16)

/*
const uint32_t getSysclk(void){
    uint32_t  freq;
	
    uint32_t  val = (SYSCTL->OSC_CTRL1 >> 8) & 0x7;
	  uint32_t  mode = (SYSCTL->OSC_CTRL1 >> 4) & 0x7;
	
    if(val == 0x3)
      freq = 32768;
    else if(val == 0x2)
		{
			if(mode == 0x2)
        freq = 1843200;
			else
				freq = 1843200;
		}
    else if(val == 0x0)
	    freq = 7372800;
    else if(val == 0x1)
	    freq = 3686400;
	  else if(val == 0x4)
	    freq = 14745600;
	  else
	    freq = 29491200;
  return freq;
}

const void systickDelay(uint32_t time)
{
	uint32_t  i, freq;
	
	if(time == 0)
	{
		return;
	}
	
	freq = getSysclk();
	
	SysTick->LOAD = 0xffffff; 
  SysTick->VAL  = 0x0;
  SysTick->CTRL = 0x5;
	
	for(i = 0; i < (time * (freq / 32768)); i++)
  {
    if((0xffffff - SysTick->VAL) >= time * (freq / 32768))
    {
      SysTick->CTRL = 0x0;
      return;
    }
  }
  
  SysTick->CTRL = 0x0;
	
  return;	
}

const waitFlag(uint32_t addr, uint32_t val, uint32_t timeout)
{
	uint16_t i;
	uint32_t freq;
 
  if(timeout == 0)
    return;
  
  freq = getSysclk();
	
	SysTick->LOAD = 0xffffff; 
  SysTick->VAL  = 0x0;
  SysTick->CTRL = 0x5;
  
  while(1)
  {
    
  }
  
  return;
}
*/
/* mode = LC(0), RC(1), PLL(2), HOSC(3) */
const void sysmodeWrite(uint8_t mode)
{
  uint8_t  flag;
  
  flag = (mode == 0) ? 4 : (mode == 1) ? 5 : 6;
    
  if(mode == 2)
  {
    SYSCTL->OSC_CTRL2 &= (~(1 << 4));         /* pll as source */
    SYSCTL->OSC_CTRL1 &= (~1);                /* enable pll */
    while(!(SYSCTL->OSC_CTRL1 & (1 << 7)))    /* wait pll lock */
    {
      ;
    }
    
    while(!(SYSCTL->OSC_CTRL1 & (1 << 12)))    /* wait pll flag lock */
    {
      ;
    }
  }
  else if(mode == 3)
  {
		/* enable pll for rtc in hosc mode */
		//SYSCTL->OSC_CTRL2 &= (~(1 << 4));         /* pll as source */
    SYSCTL->OSC_CTRL1 &= (~1);                /* enable pll */
    while(!(SYSCTL->OSC_CTRL1 & (1 << 7)))    /* wait pll lock */
    {
      ;
    }
    
    while(!(SYSCTL->OSC_CTRL1 & (1 << 12)))    /* wait pll flag lock */
    {
      ;
    }
		
    SYSCTL->MOD1_EN |= (1 << 5);
    GPIO->PCA1 |= (0xff << 12);                                       /* PAD as osc */
    SYSCTL->OSC_CTRL2 = (SYSCTL->OSC_CTRL2 & (~(0x1 << 3))) | (1 << 4); /* osc as source, enable osc */
		while(!(SYSCTL->OSC_CTRL1 & (1 << 15)))
		{
			;
		}
  }
      
  SYSCTL->SYS_MODE = (mode == 0) ? 0xF : (mode == 1) ? 0xE : 0xD;
  while(!(SYSCTL->OSC_CTRL1 & (1 << flag)))
  {
    ;
  }
	
  if(mode != 1)
  {
    SYSCTL->OSC_CTRL1 |= (1 << 1);     /* disable RC */
  }
}

const void setMipTW(uint32_t *fl_tw, uint32_t *ee_tw, uint32_t freq)
{
  switch (freq)
  {
    case 1843200:
      *fl_tw = FL_TW_1M;
      *ee_tw = EE_TW_1M;
      break;
    case 3686400:
      *fl_tw = FL_TW_3M;
      *ee_tw = EE_TW_3M;
      break;
    case 7372800:
      *fl_tw = FL_TW_7M;
      *ee_tw = EE_TW_7M;
      break;
    case 14745600:
      *fl_tw = FL_TW_14M;
      *ee_tw = EE_TW_14M;
      break;
    case 29491200:
      *fl_tw = FL_TW_29M;
      *ee_tw = EE_TW_29M;
      break;          
  }
}

/**
 * @brief SysModeChg
 *
 * @param 
 *    in uint8_t mode: system mode, support LC(0), RC(1), PLL(2) and HC(3)
 * @param 
 *    in uint8_t div: the divisor of HC mode, support no-div(0), 2-div(1), 4-div(2) and 8-div(3)
 * @return 
 *    0: system mode change pass
 *    1: system mode change fail
 *
 */
uint8_t  SysModeChg(uint8_t mode, uint8_t div)
{
  uint32_t   old_freq, new_freq, hosc_sel, sysmap, sys_ps, timeout;     /* max. timeout = 7ms*/
  uint32_t   *flash_tw, *eeprom_tw;
  
  sys_ps = SYSCTL->SYS_PS;
  SYSCTL->SYS_PS = 0x82;
      
  //===============================
  // always enable RTC apb clock
  //===============================
  SYSCTL->MOD1_EN |= (1 << 10);
  
  sysmap = SYSCTL->MAP_CTRL;
  switch (sysmap)
  {
    case 0x0:
      flash_tw =  (uint32_t *)(0x00800000 + 0x18);
      eeprom_tw = (uint32_t *)(0x08800000 + 0x18);
      break;
    case 0x1:
      flash_tw =  (uint32_t *)(0x08800000 + 0x18);
      eeprom_tw = (uint32_t *)(0x00800000 + 0x18);
      break;
    case 0x2:
      flash_tw =  (uint32_t *)(0x10800000 + 0x18);
      eeprom_tw = (uint32_t *)(0x08800000 + 0x18);
      break;
    case 0x3:
      flash_tw =  (uint32_t *)(0x18800000 + 0x18);
      eeprom_tw = (uint32_t *)(0x08800000 + 0x18);
      break;
    case 0x4:
      flash_tw =  (uint32_t *)(0x00800000 + 0x18);
      eeprom_tw = (uint32_t *)(0x08800000 + 0x18);
      break;      
  }
  
  switch ((SYSCTL->OSC_CTRL1 >> 8) & 0x7)
  {
    case 0:
      old_freq = 7372800;
      timeout = 51610;
      break;
    case 1:
      old_freq = 3686400;
      timeout = 25805;
      break;
    case 2:
      old_freq = 1843200;
      timeout = 12902;
      break;
    case 3:
      old_freq = 32768;
      timeout = 230;
      break;
    case 4:
      old_freq = 14745600;
      timeout = 103220;
      break;
    case 5:
      old_freq = 29491200;
      timeout = 206440;
      break;
  }
  
  while(SYSCTL->SYS_MODE & 0x30)
  {
    timeout--;
    if(timeout == 0)
    {
      return 1;    /* timeout */
    }
  }  
  
  if(mode == 0)  /* LC mode */
  {
    sysmodeWrite(mode);    
    *flash_tw = FL_TW_32K;
    *eeprom_tw = EE_TW_32K;
    SYSCTL->OSC_CTRL1 |= (3 << 0);   /* close RC&PLL */
    SYSCTL->OSC_CTRL2 |= (1 << 3);   /* close HOSC */
  }
  else if(mode == 1)  /* RC mode */
  {
    sysmodeWrite(mode);    
    *flash_tw = FL_TW_1M;
    *eeprom_tw = EE_TW_1M;
    SYSCTL->OSC_CTRL1 |= (1 << 0);  /* close PLL */
    SYSCTL->OSC_CTRL2 |= (1 << 3);  /* close HOSC */
  }
  else
  {
    hosc_sel = (mode == 2) ? 1 : ((SYSCTL->OSC_CTRL2 & 0x7) + 1);
    if(hosc_sel > 2)
    {
      hosc_sel = 4;
    }
    
    switch (div)
    {
      case 0x0:
        new_freq = 7372800 * hosc_sel;
        break;
      case 0x1:
        new_freq = 7372800 * hosc_sel / 2;
        break;
      case 0x2:
        new_freq = 7372800 * hosc_sel / 4;
        break;
      case 0x3:
        new_freq = 7372800 * hosc_sel / 8;
        if(hosc_sel == 1)
          return 1;          /* invalid divisor parameter */
        break;
      default:
        return 1;       /* invalid divisor parameter */
    }
    
    if(new_freq > old_freq)    /* from Low-frequency to High-frequency switching */
    {
      setMipTW(flash_tw, eeprom_tw, new_freq);
      sysmodeWrite(1);
			SYSCTL->OSC_CTRL1 = SYSCTL->OSC_CTRL1 & (~(3 << 2)) |(div << 2);
      sysmodeWrite(mode);
    }
    else
    {
      sysmodeWrite(1);
			SYSCTL->OSC_CTRL1 = SYSCTL->OSC_CTRL1 & (~(3 << 2)) |(div << 2);
      sysmodeWrite(mode);
      setMipTW(flash_tw, eeprom_tw, new_freq);
    }    
  } 
	
	SYSCTL->SYS_PS = sys_ps;
	return 0;
}

#define SPL0_OFFSET  0x00c08000
#define PAGE_SIZE    64

#define ANAADC_OFFSET      8
#define ANACLK1_OFFSET     9
#define ANACLK2_OFFSET     10
#define ANALDO_OFFSET      11
#define ANABGR_OFFSET      12
#define ANAPSW_OFFSET      13
#define RTCTADJ_OFFSET     14
#define RTCZT_OFFSET       15
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
#define CACHECTRL_REG      *(uint32_t *)()

#define RTCMODE_REG        *(uint32_t *)(0x4003C044)
#define RTCDOTA0_REG       *(uint32_t *)(0x4003C048)
#define RTCALPHAL_REG      *(uint32_t *)(0x4003C04c)
#define RTCALPHAH_REG      *(uint32_t *)(0x4003C050)
#define RTCXT0_REG         *(uint32_t *)(0x4003C054)
#define RTCBETA_REG        *(uint32_t *)(0x4003C05c)
#define WDTCTRL_REG        *(uint32_t *)(0x40030004)
#define EMAP_REG           *(uint32_t *)(0x40034018)

const uint8_t   eeChkSum(uint16_t *ee_addr)
{
  uint32_t  chksum = 0;
  uint32_t  i;
  
  for(i = 0; i < ((PAGE_SIZE / 2) - 2); i += 2)
  {
    chksum += (ee_addr[i] | (ee_addr[i + 1] << 16));
  }
  
  if((chksum + (ee_addr[i] | (ee_addr[i + 1] << 16))) != 0)  /* checksum err */
  {
    return 1;
  }
  
  return 0;
}

/* mode = 0: FLASH, mode = 1: EEPROM */
const uint32_t  _getMipAddr(uint8_t mode)
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

void sysUpdate(void)
{
/*
  uint8_t   spl0[PAGE_SIZE] ={
		F1 & 0xff, (F1 >> 8) & 0xff, 
		(F1 >> 16) & 0xff, (F1 >> 24) & 0xff, 
		F2 & 0xff, (F2 >> 8) & 0xff, 
		(F2 >> 16) & 0xff, (F2 >> 24) & 0xff, 
		F3 & 0xff, (F3 >> 8) & 0xff, 
		(F3 >> 16) & 0xff, (F3 >> 24) & 0xff, 
		F4 & 0xff, (F4 >> 8) & 0xff, 
		(F4 >> 16) & 0xff, (F4 >> 24) & 0xff, 
		ANAADC & 0xff, (ANAADC >> 8) & 0xff, 
		ANACLK1 & 0xff, (ANACLK1 >> 8) & 0xff, 
		ANACLK2 & 0xff, (ANACLK2 >> 8) & 0xff,   
		ANALDO & 0xff, (ANALDO >> 8) & 0xff, 
		ANABGR & 0xff, (ANABGR >> 8) & 0xff, 
		ANAPSW & 0xff, (ANAPSW >> 8) & 0xff, 
		RTCTADJ & 0xff, (RTCTADJ >> 8) & 0xff, 
		RTCZT & 0xff, (RTCZT >> 8) & 0xff, 
		TPSCTRL & 0xff, (TPSCTRL >> 8) & 0xff, 
		ADCTEST & 0xff, (ADCTEST >> 8) & 0xff, 
		CACHECTRL & 0xff, (CACHECTRL >> 8) & 0xff,
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
  };
  //12 half-word
  uint8_t   spl1[PAGE_SIZE] = {
		CP0 & 0xff, (CP0 >> 8) & 0xff, 
		(CP0 >> 16) & 0xff, (CP0 >> 24) & 0xff, 
		PASS & 0xff, (PASS >> 8) & 0xff, 
		(PASS >> 16) & 0xff, (PASS >> 24) & 0xff, 
		RTCMODE & 0xff, (RTCMODE >> 8) & 0xff, 
		RTCDOTA0 & 0xff, (RTCDOTA0 >> 8) & 0xff, 
		RTCALPHAL & 0xff, (RTCALPHAL >> 8) & 0xff, 
		RTCALPHAH & 0xff, (RTCALPHAH >> 8) & 0xff, 
		RTCXT0 & 0xff, (RTCXT0 >> 8) & 0xff, 
		RTCBETA & 0xff, (RTCBETA >> 8) & 0xff, 
		WDTCTRL & 0xff, (WDTCTRL >> 8) & 0xff, 
		EMAP & 0xff, (EMAP >> 8) & 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
		0xff, 0xff, 
  };
*/
  uint16_t  *ee_addr;
  uint32_t  sys_ps;
  uint32_t  mod0_en, mod1_en;
  
  sys_ps = SYSCTL->SYS_PS;
  
  SYSCTL->SYS_PS = 0x82;
  mod0_en = SYSCTL->MOD0_EN;
  mod1_en = SYSCTL->MOD1_EN;
  
  ee_addr = (uint16_t *)(_getMipAddr(1) + SPL0_OFFSET);
  
  if(eeChkSum(ee_addr))
  {
    SYSCTL->SYS_PS = 0x75;
    if(ee_addr[ANAADC_OFFSET] != ANAADC_REG)
    {
      ANAADC_REG = 0xa5770000 | ee_addr[ANAADC_OFFSET];
    }
    
    if(ee_addr[ANACLK1_OFFSET] != ANACLK1_REG)
    {
      if((ee_addr[ANACLK1_OFFSET] & (1 << 10)) != (ANACLK1_REG & (1 << 10)))
      {
        ANACLK1_REG = 0xa5270000 | (ANACLK1_REG | (1 << 9));  /* power down first */        
      }
      ANACLK1_REG = 0xa5270000 | ee_addr[ANACLK1_OFFSET];
    }
    
    if(ee_addr[ANACLK2_OFFSET] != ANACLK2_REG)
    {
      ANACLK2_REG = 0xa5360000 | ee_addr[ANACLK2_OFFSET];
    }
    
    if(ee_addr[ANALDO_OFFSET] != ANALDO_REG)
    {
      ANALDO_REG = 0xa5580000 | ee_addr[ANALDO_OFFSET];
    }
    
    if(ee_addr[ANABGR_OFFSET] != ANABGR_REG)
    {
      ANABGR_REG = 0xa5280000 | ee_addr[ANABGR_OFFSET];
    }
    
    if(ee_addr[ANAPSW_OFFSET] != ANAPSW_REG)
    {
      ANAPSW_REG = 0xa5790000 | ee_addr[ANAPSW_OFFSET];
    }
    
    {
      uint32_t  rtc_ctrl;
      uint32_t  i_cnt = 0;
      
      SYSCTL->SYS_PS = 0x82;
      SYSCTL->MOD1_EN |= (1 << 10);  /* RTC */
      
      rtc_ctrl = RTC->CTRL;
      
      while(RTC->CTRL & 0x200)
      {
        i_cnt ++;
        if(i_cnt > 0x200)
        {
          break;
        }
      }
      RTC->CTRL = 0x100;
      i_cnt = 0;
      while(RTC->CTRL & 0x200)
      {
        i_cnt ++;
        if(i_cnt > 0x200)
        {
          break;
        }
      }
      RTC->CTRL = 0x100;
      
      *(uint32_t *)(0x4003C040) = 0x8e;
      
      if(ee_addr[RTCTADJ_OFFSET] != RTCTADJ_REG)
      {
        RTCTADJ_REG = ee_addr[RTCTADJ_OFFSET];
      }
      
      if(ee_addr[RTCZT_OFFSET] != RTCZT_REG)
      {
        RTCZT_REG = ee_addr[RTCZT_OFFSET];
      }
      
      *(uint32_t *)(0x4003C040) = 0x0;
      i_cnt = 0;
      while(RTC->CTRL & 0x200)
      {
        i_cnt ++;
        if(i_cnt > 0x200)
        {
          break;
        }
      }
      RTC->CTRL = rtc_ctrl;
    }
    
    SYSCTL->MOD1_EN |= (1 << 11);  /* SAR */
    if(ee_addr[TPSCTRL_OFFSET] != TPSCTRL_REG)
    {
      TPSCTRL_REG = ee_addr[TPSCTRL_OFFSET];
    }
        
    if(ee_addr[ADCTEST_OFFSET] != ADCTEST_REG)
    {
      ADCTEST_REG = ee_addr[ADCTEST_OFFSET];
    }
    
    {
      uint32_t  *cc_addr;
      cc_addr = (uint32_t *)(_getMipAddr(0) + 0x0080001c);
      
      if(ee_addr[CACHECTRL_OFFSET] != *cc_addr)
      {
        *cc_addr = 0xfeed0000 | ee_addr[CACHECTRL_OFFSET];
      }
    }
  }
  
  ee_addr = (uint16_t *)(_getMipAddr(1) + SPL0_OFFSET + PAGE_SIZE);
  if(eeChkSum(ee_addr))
  {
    SYSCTL->SYS_PS = 0x75;
    if(ee_addr[EMAP_OFFSET] != EMAP_REG)
    {
      EMAP_REG = ee_addr[EMAP_OFFSET];
    }
    
    SYSCTL->SYS_PS = 0x82;
    SYSCTL->MOD1_EN |= (1 << 9);   /* WDT */
    if(ee_addr[WDTCTRL_OFFSET] != WDTCTRL_REG)
    {
      *(uint32_t *)(0x40030008) = 0x70;
      *(uint32_t *)(0x40030008) = 0x7E;
      {
        uint32_t  i_cnt = 0;
        while(WDT->EN & 0x200)
        {
          i_cnt ++;
          if(i_cnt > 0x200)
          {
            break;
          }
        }
        WDTCTRL_REG = ee_addr[WDTCTRL_OFFSET];
      }
    }
    
    {
      uint32_t  rtc_ctrl;
      uint32_t  i_cnt = 0;
      
      SYSCTL->SYS_PS = 0x82;
      SYSCTL->MOD0_EN |= (1 << 9);
      
      rtc_ctrl = RTC->CTRL;
      
      while(RTC->CTRL & 0x200)
      {
        i_cnt ++;
        if(i_cnt > 0x200)
        {
          break;
        }
      }
      RTC->CTRL = 0x100;
      i_cnt = 0;
      while(RTC->CTRL & 0x200)
      {
        i_cnt ++;
        if(i_cnt > 0x200)
        {
          break;
        }
      }
      RTC->CTRL = 0x100;
      
      *(uint32_t *)(0x4003C040) = 0x8e;
      
      if(ee_addr[RTCMODE_OFFSET] != RTCMODE_REG)
      {
        RTCMODE_REG = ee_addr[RTCMODE_OFFSET];
      }
      
      if(ee_addr[RTCDOTA0_OFFSET] != RTCDOTA0_REG)
      {
        RTCDOTA0_REG = ee_addr[RTCDOTA0_OFFSET];
      }
      
      if(ee_addr[RTCALPHAL_OFFSET] != RTCALPHAL_REG)
      {
        RTCALPHAL_REG = ee_addr[RTCALPHAL_OFFSET];
      }
      
      if(ee_addr[RTCALPHAH_OFFSET] != RTCALPHAH_REG)
      {
        RTCALPHAH_REG = ee_addr[RTCALPHAH_OFFSET];
      }
      
      if(ee_addr[RTCXT0_OFFSET] != RTCXT0_REG)
      {
        RTCXT0_REG = ee_addr[RTCXT0_OFFSET];
      }
      
      if(ee_addr[RTCBETA_OFFSET] != RTCBETA_REG)
      {
        RTCBETA_REG = ee_addr[RTCBETA_OFFSET];
      }
      
      *(uint32_t *)(0x4003C040) = 0x0;
      i_cnt = 0;
      while(RTC->CTRL & 0x200)
      {
        i_cnt ++;
        if(i_cnt > 0x200)
        {
          break;
        }
      }
      RTC->CTRL = rtc_ctrl;
    }    
  }
    
  SYSCTL->SYS_PS = 0x82;
  SYSCTL->MOD0_EN = mod0_en;
  SYSCTL->MOD1_EN = mod1_en;
  
  SYSCTL->SYS_PS = sys_ps;
}
