/******************************************************************************
 * @file     sysctrl.c
 * @brief    System control driver source code file for RN821x series SOC
 * @author   Renergy Technology
 * @version  V1.3
 * @date     2017-07-20
 *
 * @note
 * Copyright (C) 2008, Renergy Technology Inc. All rights reserved.
 *
 * History:
 *  2015-8-24   change for RA9103D0
 *       1. discriminate RA9103A0/RA9103D0 automatically
 *       2. use SYS_TC instead of SYSTICK while RA9103D0
 *  2017-4-22
 *       1. new FLASh/EEPROM TW value
 *  2017-5-03 v1.2.1
*        1. update struction of .h
*   2017-7-20 v1.3
    1. modify the bug that sys_ps was not 0x82 when opening sys_tick's APB clock.
    2017-9-20 v1.3.1
    1. add a break in function setMipTW for case 32768
 ******************************************************************************/

#include "RN821x.h"
#include "system_RN821x.h"
#include "rn8xxx_sysctrl.h"
#include "rn8xxx_sysctrl_reg.h"
#include <stdio.h>

//=====================================================================================
//
// SysModeChg() lib
//
//=====================================================================================
#define IAP_FP 0x50        
#define IAP_FS 0x51         
#define IAP_FC 0x52         
#define IAP_FQ 0x53
#define IAP_EP 0x54         
#define IAP_ES 0x55         
#define IAP_EC 0x56         
#define IAP_EQ 0x57         
#define IAP_FW 0x58         
#define IAP_EW 0x59   
#define IAP_NV 0x5a      
#define IAP_SR 0x5b  

#define FLASH_NVM  0x0
#define EEPROM_NVM 0x1

#define SPL1_ADDR 0x08c08040

//us unit
#define PLL_TIMEOUT   3000000
#define HOSC_TIMEOUT  100000
#define RCH_TIMEOUT   10000
#define LOSC_TIMEOUT  3000000
#define MIP_TIMEOUT   7000
#define RTC_TIMEOUT   1000

#define TEST_TIMEOUT  10000000

#define FEED_WDT               WDT->EN = 0xbb

#define LOSC_PWD      (SYSCTL->OSC_CTRL2 & (1 << 8))
#define RTC_LCNT_ADDR 0x4003c080
#define RTC_LCNT      (*(volatile uint32_t *)(RTC_LCNT_ADDR))

#define TICK_START(chipid)   { \
                               if(chipid == CHIP_RA9103A){ \
                                 SysTick->CTRL = 0x5;\
                               }else{\
                                 SYS_TC->CTRL = 3;\
                               }\
                             }
#define TICK_STOP(chipid)    { \
                               if(chipid == CHIP_RA9103A){ \
                                 SysTick->LOAD = 0xffffff;\
                                 SysTick->CTRL = 0x4;\
                                 SysTick->VAL  = 0x0;\
                               }else{\
                                 SYS_TC->CTRL = 2;\
                               }\
                             }
#define TICK_TIME_SET(chipid, ticks)   {\
                                         if(chipid == CHIP_RA9103A){ \
                                           SysTick->LOAD = ticks;\
                                         }else{\
                                           SYS_TC->LOAD = ticks;\
                                         }\
                                       }
/*
 * return:
 *   0 = RA9103A
 *   1 = RA9103D
 *   2 = RA9103C
 *   3 = RA9103E
 *   4 = RA9103F
 */
uint8_t getChipid(void)
{
  uint8_t   chipid;
    uint16_t  die_ver;
    uint16_t  sys_ps_bak;
  
    sys_ps_bak = SYSCTL->SYS_PS;
    
    SYSCTL->SYS_PS = 0x82;
  SYSCTL->MOD0_EN |= (1 << 0);
    
    SYSCTL->SYS_PS = 0x75;
    die_ver = *(volatile uint32_t *)(0x40034074);
    
    switch(die_ver)
    {
        case 0xc0:
            chipid = CHIP_RA9103C;
          break;
        case 0xe0:
            chipid = CHIP_RA9103E;
          break;
        case 0xf0:
            chipid = CHIP_RA9103F;
          break;
        default:
            *(volatile uint32_t *)(0x50010004) = 0xffffffff;
          if(*(volatile uint32_t *)(0x50010004) == 0xffffffff)
            {
                *(volatile uint32_t *)(0x50010004) = 0x0;
                if(*(volatile uint32_t *)(0x50010004) == 0x0)
                {
                  chipid = CHIP_RA9103D;
                }
                else
                {
                    chipid = CHIP_RA9103A;
                }
            }
            else
            {
                chipid = CHIP_RA9103A;
            }
            break;
    }
    
  SYSCTL->SYS_PS = sys_ps_bak;  
  return chipid;
}
                
static uint32_t currentClk(void){
  uint32_t  freq;  
  uint32_t  val = (SYSCTL->OSC_CTRL1 >> 8) & 0x7;
  
  if(val == 0x3)
    freq = 32768;
  else if(val == 0x2)
    freq = 0;//1843200;
  else if(val == 0x0)
    freq = 2;//7372800;
  else if(val == 0x1)
    freq = 1;//3686400;
  else if(val == 0x4)
    freq = 3;//14745600;
  else
    freq = 4;//29491200;
  return freq;
}

/* mode = 0: FLASH, mode = 1: EEPROM */
static uint32_t  _getMipAddr(uint8_t mode)
{
  uint32_t addr = 0x0;
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
    default:
      addr =  (mode == 0) ? 0x00000000 : 0x08000000;
      break;
  }
  return addr;
}

static uint8_t TICK_STAT_GET(uint8_t chipid)
{
  uint8_t  res;
  if(chipid == CHIP_RA9103A)
  {
    res = ((SysTick->CTRL >> 16) & 0x1);
  }
  else
  {
    res = ((SYS_TC->CTRL >> 1) & 0x1);
  }
  
  return res;
}

const uint32_t  us_freq[] = {9100000, 4550000, 2274000, 1137000, 568000};//0xffffff = us_freq[x] us
const uint32_t  us_cycle[] = {2,4,7,15,29};  //1us = us_cycle[x] cycles
//timeout unit: us
//sign = 1, wait while value is 1
//sign = 0, wait while value is 0
static void usTimeout(uint32_t addr, uint32_t val, uint32_t sign, uint32_t timeout, uint8_t chipid)
{
    uint32_t i, loops=0;
    uint32_t freq;
  uint32_t ticks;
 
  if(timeout == 0)
    return;
  
  //SysTick range: 29M: 568ms; 14M:1.137s; 7M:2.274s; 3M:4.55s; 1M:9.1s; 32K: 511.7s
  freq = currentClk();
  
  if(freq == 32768)
  {
    loops = 1;
  }
  else
  {
    loops = 1 + timeout / us_freq[freq];
  }
  
  for(i = 0; i < loops; i++)
  {
    TICK_STOP(chipid);
    if(i != (loops - 1))
    {
      ticks = 0xffffff;
    }
    else
    {
      if(freq == 32768)
      {
        ticks = timeout / 32;
      }
      else
      {
        //ticks = (((1843200 << freq)* (timeout - (loops - 1) * us_freq[freq]) / 1000000) + 1); 
                ticks = (timeout - (loops-1)*us_freq[freq])*us_cycle[freq];
        if(ticks == 0)
        {
          break;
        }
      }
    }
    TICK_TIME_SET(chipid, ticks);
    TICK_START(chipid);
    while(1)
    {
      if(TICK_STAT_GET(chipid))
      {
        break;
      }
      if(((sign == 1) && (((*(uint32_t *)addr) & val) != val)) ||    //wait while value is 1
         ((sign == 0) && (((*(uint32_t *)addr) & val) == val)))      //wait while value is 0
      {
        break;
      }
      FEED_WDT;
    }
  }
  
  TICK_STOP(chipid);
  return;
}

const   uint32_t  fl_tw_98ns[] =  {FL_TW_32K_98NS,  FL_TW_1M_98NS,  FL_TW_3M_98NS,  FL_TW_7M_98NS,  FL_TW_14M_98NS,  FL_TW_29M_98NS};
const   uint32_t  fl_tw_198ns[] = {FL_TW_32K_198NS, FL_TW_1M_198NS, FL_TW_3M_198NS, FL_TW_7M_198NS, FL_TW_14M_198NS, FL_TW_29M_198NS};
const   uint32_t  ee_tw_98ns[] =  {EE_TW_32K_98NS,  EE_TW_1M_98NS,  EE_TW_3M_98NS,  EE_TW_7M_98NS,  EE_TW_14M_98NS,  EE_TW_29M_98NS};
const   uint32_t  ee_tw_198ns[] = {EE_TW_32K_198NS, EE_TW_1M_198NS, EE_TW_3M_198NS, EE_TW_7M_198NS, EE_TW_14M_198NS, EE_TW_29M_198NS};      
static void setMipTW(uint32_t freq, uint32_t chipid)
{
  volatile uint32_t   *fltw_addr;
    volatile uint32_t   *eetw_addr;
    uint32_t            *eetw_ptemp;
  uint32_t            *fltw_value;
    uint32_t            eetw_value;
  
  if((chipid == CHIP_RA9103A) || (chipid == CHIP_RA9103D))
  {
        fltw_addr  = (uint32_t *)(_getMipAddr(0) + 0x00800018);
    eetw_addr =  (uint32_t *)(_getMipAddr(1) + 0x00800018);
    fltw_value = (uint32_t *)fl_tw_98ns;
        eetw_ptemp = (uint32_t *)ee_tw_98ns;
  }
  else if((chipid == CHIP_RA9103E) || (chipid == CHIP_RA9103C))
  {
    fltw_addr  = (uint32_t *)(_getMipAddr(0) + 0x00800018);
    eetw_addr =  (uint32_t *)(_getMipAddr(1) + 0x00800018);
        fltw_value = (uint32_t *)fl_tw_198ns;
        eetw_ptemp = (uint32_t *)ee_tw_198ns;
  }
    else
    {
        fltw_addr  = (uint32_t *)(0x50014040);
    fltw_value = (uint32_t *)fl_tw_198ns;
        eetw_addr  = (uint32_t *)(0x50014040);
    eetw_ptemp = (uint32_t *)fl_tw_198ns;
    }

  switch (freq)
  {
    case 32768:
      *fltw_addr  = fltw_value[0] << 16;
      eetw_value = eetw_ptemp[0] << 16 ;
      break;
    case 1843200:
      *fltw_addr  = fltw_value[1] << 16;
      eetw_value = eetw_ptemp[1] << 16 ;
      break;
    case 3686400:
      *fltw_addr  = fltw_value[2] << 16;
      eetw_value = eetw_ptemp[2] << 16 ;
      break;
    case 7372800:
      *fltw_addr  = fltw_value[3] << 16;
      eetw_value = eetw_ptemp[3] << 16 ;
      break;
    case 14745600:
      *fltw_addr  = fltw_value[4] << 16;
      eetw_value = eetw_ptemp[4] << 16 ;
      break;
    case 29491200:
      *fltw_addr  = fltw_value[5] << 16;
      eetw_value = eetw_ptemp[5] << 16 ;
      break;          
  }
    
    if(chipid != CHIP_RA9103F)
    {
        *eetw_addr = eetw_value;
    }
}

/* use CLK_TRIM to check LOSC */
//500ms总时间，每1ms检查losc能否计数超过10个节拍，如是，则认为losc存在，如否，超过500ms总时间后错误返回
static uint32_t loscCalc(uint8_t chipid)
{
    uint32_t trim_cfg1_bak, trim_start_bak;
    uint32_t res = SYSCTRL_PASS;
    
    trim_cfg1_bak = SYSCTL->TRIM_CFG1;
    trim_start_bak = SYSCTL->TRIM_START;
    
    SYSCTL->TRIM_CFG1 = (1 << 26) | (0 << 24) | (10 << 0);  //use losc to calc rcl
    SYSCTL->TRIM_START = 1 << 27;
    SYSCTL->TRIM_START = 0x7 << 24;
    usTimeout((uint32_t)(&SYSCTL->TRIM_START), (1 << 24), 0, RTC_TIMEOUT, chipid);
    if(!(SYSCTL->TRIM_START & (1 << 24)))
    {
        res = SYSCTRL_FAIL;
    }
    SYSCTL->TRIM_START = 0x1 << 27;
    SYSCTL->TRIM_START = 0x3 << 24;
    
    SYSCTL->TRIM_CFG1 = trim_cfg1_bak;
    SYSCTL->TRIM_START = trim_start_bak;
    return res;
}

static uint32_t loscCheck(uint8_t chipid)
{
    uint32_t i;
    
    if(chipid != CHIP_RA9103A)
    {
        for(i = 0; i < 500; i++)
        {
            if(SYSCTRL_PASS == loscCalc(chipid))
            {
                return SYSCTRL_PASS;
            }
        }
    }
    else
    {
        usTimeout((uint32_t)(&SYSCTL->OSC_CTRL1), (1 << 11), 0, LOSC_TIMEOUT, chipid); /* wait LOSC flag*/
        if(SYSCTL->OSC_CTRL1 & (1 << 11))
        {
            return SYSCTRL_PASS;
        }
    }
//  if((LOSC_PWD) && loscCalc(chipid))
//  {
//      return 1;
//  }
//  else
//  {
//      usTimeout((uint32_t)(&SYSCTL->OSC_CTRL1), (1 << 11), 0, LOSC_TIMEOUT, chipid); /* wait LOSC flag*/
//      if(!(SYSCTL->OSC_CTRL1 & (1 << 11)))
//      {
//          return 1;
//      }
//  }
    return SYSCTRL_FAIL;
}

/* check: PLL lock and LOSC flag(if LOSC pwr-down, check RTC write) */
static uint8_t PllEnable(uint8_t chipid)
{
  uint32_t flag, osc1_ctrl_bak, osc2_ctrl_bak;
  uint8_t  res = SYSCTRL_PASS;
    
  /* enable pll for rtc in hosc mode */
    osc1_ctrl_bak = SYSCTL->OSC_CTRL1;
    osc2_ctrl_bak = SYSCTL->OSC_CTRL2;
    
    SYSCTL->OSC_CTRL2 &= (~(1 << 4));         /* pll as source */
  SYSCTL->OSC_CTRL1 &= (~1);                /* enable pll */
  usTimeout((uint32_t)(&SYSCTL->OSC_CTRL1), (1 << 7), 0, PLL_TIMEOUT, chipid);   /* wait PLL lock */
  
  if(!(SYSCTL->OSC_CTRL1 & (1 << 7)))  //unlock
  {
    SYSCTL->OSC_CTRL2 = osc2_ctrl_bak;
    SYSCTL->OSC_CTRL1 = osc1_ctrl_bak;
    res = SYSCTRL_FAIL;
  }
    else
    {
        if(SYSCTRL_FAIL == loscCheck(chipid))
        {
            SYSCTL->OSC_CTRL2 = osc2_ctrl_bak;
      SYSCTL->OSC_CTRL1 = osc1_ctrl_bak;
            res = SYSCTRL_FAIL;
        }
        else
        {
      usTimeout((uint32_t)(&SYSCTL->OSC_CTRL1), (1 << 12), 0, PLL_TIMEOUT, chipid);  /* wait PLL flag*/
          flag = SYSCTL->OSC_CTRL1;
          if(!(flag & (1 << 12)))  //cgf
          {
              SYSCTL->OSC_CTRL2 = osc2_ctrl_bak;
        SYSCTL->OSC_CTRL1 = osc1_ctrl_bak;
              res = SYSCTRL_FAIL;
          }
        }
    }
  
  return res;
}

static void PllOn(void)
{
  SYSCTL->OSC_CTRL1 &= (~1);                /* enable pll */  
}

static uint8_t HoscEnable(uint8_t chipid)
{
  uint32_t flag, mod1_en;
    uint32_t gpio_pca1_bak, osc_ctrl2_bak;
  uint8_t  res = SYSCTRL_PASS;
    
  /* enable pll for rtc in hosc mode */
  PllOn();

  mod1_en = SYSCTL->MOD1_EN;
  
  SYSCTL->MOD1_EN |= (1 << 5);
    osc_ctrl2_bak = SYSCTL->OSC_CTRL2;
    gpio_pca1_bak = GPIO->PCA1;
    
  GPIO->PCA1 |= (0xff << 12);                                       /* PAD as osc */
  SYSCTL->OSC_CTRL2 = (SYSCTL->OSC_CTRL2 & (~(0x1 << 3))) | (1 << 4); /* osc as source, enable osc */
  usTimeout((uint32_t)(&SYSCTL->OSC_CTRL1), 1 << 15, 0, HOSC_TIMEOUT, chipid);   /* wait HOSC flag */
  
  flag = SYSCTL->OSC_CTRL1;
  if(!(flag & (1 << 15)))  //only check HOSC flag(pll bypass)
  {
        SYSCTL->OSC_CTRL2 = osc_ctrl2_bak;
        GPIO->PCA1 = gpio_pca1_bak;
    res = SYSCTRL_FAIL;
  }
  
  SYSCTL->MOD1_EN = mod1_en;
  
  return res;
}

/* mode = LC(0), RC(1), PLL(2), HOSC(3) */
static uint8_t sysmodeWrite(uint8_t mode, uint8_t chipid, uint32_t cur_freq)
{
  if((mode == 0) && (SYSCTRL_FAIL == loscCheck(chipid)))
  {
        return SYSCTRL_FAIL;
  }
  else if((mode == 1) || (mode == 4))
    {
        SYSCTL->OSC_CTRL1 &= (~(1 << 1));
        usTimeout((uint32_t)(&SYSCTL->OSC_CTRL1), (1 << 13), 0, RCH_TIMEOUT, chipid); /* wait RCH flag */
        if(!(SYSCTL->OSC_CTRL1 & (1 << 13)))
        {
            return SYSCTRL_FAIL;
        }
    }
  else if(((mode == 2) || (mode == 5)) && (SYSCTRL_FAIL == loscCheck(chipid) || (SYSCTRL_FAIL == PllEnable(chipid))))
  {
    return SYSCTRL_FAIL;
  }
  else if((mode == 3) && (SYSCTRL_FAIL == HoscEnable(chipid)))
  {
    return SYSCTRL_FAIL;
  }
  
    
  if(mode == 0)
  {
    SYSCTL->SYS_MODE = 0xF;
    setMipTW(32768, chipid);
    SYSCTL->OSC_CTRL1 |= (3 << 0);   /* close RC&PLL */
    SYSCTL->OSC_CTRL2 |= (1 << 3);   /* close HOSC */
  }
  else if((mode == 1) || (mode == 4))
  {
        if((SYSCTL->OSC_CTRL2 & (1 << 16)) && (cur_freq < 3686400))
        {
      setMipTW(3686400, chipid);
        }
    SYSCTL->SYS_MODE = 0xE;     
        if(SYSCTL->OSC_CTRL2 & (1 << 16))
        {
      setMipTW(3686400, chipid);
        }
        else
        {
            setMipTW(1843200, chipid);
        }
  }
  else if((mode == 2) || (mode == 5))
  {
    SYSCTL->SYS_MODE = 0xD;
    SYSCTL->OSC_CTRL1 |= (1 << 1);     /* disable RC */
  }
  else if(mode == 3)
  {
    SYSCTL->SYS_MODE = 0xD;
    SYSCTL->OSC_CTRL1 |= (1 << 1);     /* disable RC */
  }
  else
  {
    return SYSCTRL_FAIL;
  }
  
  return SYSCTRL_PASS;
}

/**
 * @brief SysModeChg
 *
 * @param 
 *    in uint8_t mode: system mode, support 0=LC(32.768Khz), 1=RC(under 1.8432Mhz), 2=PLL(7.3728Mhz), 3=HC(depends on OSC_CTRL2[2:0]), 4=RC(under 3.6864Mhz) and 5=PLL(14.7456Mhz)
 * @param 
 *    in uint8_t div: the divisor of HC mode, support no-div(0), 2-div(1), 4-div(2) and 8-div(3)
 * @return 
 *    0: system mode change pass
 *    1: system mode change fail
 *
 */
uint8_t  SysModeChg(uint8_t mode, uint8_t div)
{
  uint32_t   old_freq, new_freq, hosc_sel;
    uint32_t   sys_ps, res = SYSCTRL_PASS;
  uint8_t    chipid = 0;
  
  chipid = getChipid();
    
    if((chipid == CHIP_RA9103A) || (chipid == CHIP_RA9103D))
    {
    if((((mode == 0) || (mode == 1)) && (div > 0)) ||
        ((mode == 2) && (div > 2)) ||
        ((mode == 3) && (div > 3)) ||
            ((mode == 3) && (div == 3) && ((SYSCTL->OSC_CTRL2 & 0x7) == 0x0)) ||
            (mode > 3))
    {
      return SYSCTRL_FAIL;
    }
    }
    else
    {
        if((((mode == 0) || (mode == 1) || (mode == 4)) && (div > 0)) ||
        ((mode == 2) && (div > 2)) ||
        (((mode == 3) || (mode == 5)) && (div > 3)) ||
            ((mode == 3) && (div == 3) && ((SYSCTL->OSC_CTRL2 & 0x7) == 0x0)) ||
            (mode > 5))
    {
      return SYSCTRL_FAIL;
    }
    }

  sys_ps = SYSCTL->SYS_PS;
  SYSCTL->SYS_PS = 0x82;
    
  old_freq = currentClk();
  old_freq = (old_freq == 32768)? 32768 : (1843200 << old_freq);  
  //===============================
  // always enable RTC/WDT apb clock
  //===============================
  SYSCTL->MOD1_EN |= ((1 << 10) | (1 << 9));
    
  FEED_WDT;
  usTimeout((uint32_t)(&SYSCTL->SYS_MODE), 0x10, 1, MIP_TIMEOUT, chipid);   /* check MIP busy */
  usTimeout((uint32_t)(&SYSCTL->SYS_MODE), 0x20, 1, MIP_TIMEOUT, chipid);
      
  if(mode == 0)  /* LC mode */
  {
    if(SYSCTRL_FAIL == sysmodeWrite(mode, chipid, old_freq))
    {
      res = SYSCTRL_FAIL;
    }
  }
  else if((mode == 1) || (mode == 4)) /* RC mode */
  {
        if(mode == 4)
        {
            SYSCTL->OSC_CTRL2 |= (1 << 16);  //3.6864Mhz RCH
            setMipTW(3686400, chipid);
        }
        
    if(SYSCTRL_FAIL == sysmodeWrite(mode, chipid, old_freq))
    {
      res = SYSCTRL_FAIL;
    }
    else
    {
      SYSCTL->OSC_CTRL1 |= (1 << 0);  /* close PLL */
      SYSCTL->OSC_CTRL2 |= (1 << 3);  /* close HOSC */
    }
  }
  else  /* PLL(7/14M) or HOSC */
  {     
        hosc_sel = (mode == 2) ? 1 : ((mode == 5) ? 2 : ((SYSCTL->OSC_CTRL2 & 0x7) + 1));
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
                    new_freq = 1843200; /* invalid divisor parameter */
                break;
            default:
                return SYSCTRL_FAIL;       /* invalid divisor parameter */
        }
        
        if(SYSCTRL_FAIL == sysmodeWrite(1, chipid, old_freq))
        {
            res = SYSCTRL_FAIL;
        }
        else
        {
            if(mode == 5)
            {
                SYSCTL->OSC_CTRL2 = (SYSCTL->OSC_CTRL2 & (~(0x7 << 5))) | (0x1 << 5);  //14.7456Mhz PLL
            }
            else
            {
                SYSCTL->OSC_CTRL2 = (SYSCTL->OSC_CTRL2 & (~(0x7 << 5)));  //7.3728M PLL
            }
            if(new_freq >= 3686400)
            {
              setMipTW(new_freq, chipid);
            }
            SYSCTL->OSC_CTRL1 = (SYSCTL->OSC_CTRL1 & (~(3 << 2))) | (div << 2);
            if(SYSCTRL_FAIL == sysmodeWrite(mode, chipid, old_freq))
            {
                res = SYSCTRL_FAIL;
            }
            if(new_freq < 3686400)
            {
              setMipTW(new_freq, chipid);
            }
        }
  } 
    
    if(res == SYSCTRL_FAIL)
    {
        old_freq = 32768 ? 32768 : (1843200 << old_freq);
        setMipTW(new_freq, chipid);
    }
  FEED_WDT;
    SYSCTL->SYS_PS = sys_ps;
    return res;
}


//=====================================================================================
//
// Remap4Sel() lib
//
//=====================================================================================
#define SPL0_OFFSET  0x00c08000
#define PAGE_SIZE    64

static struct sIAP {                  // IAP Structure
  unsigned long cmd;           // Command
  unsigned long par[4];        // Parameters
  unsigned long stat;          // Status
  unsigned long res[2];        // Result
} IAP;

/* IAP Call */
typedef void (*IAP_Entry) (unsigned long *cmd, unsigned long *stat);
#define IAP_Call ((IAP_Entry) 0x18001c01)

static int unlockNVM(int flag)
{
  IAP.cmd    = IAP_NV;                         
  IAP.par[0] = flag;                              
  IAP_Call (&IAP.cmd, &IAP.stat);              // Call IAP Command
  if (IAP.stat) return (SYSCTRL_FAIL);                    // Command Failed     
    else return (SYSCTRL_PASS);
}

/**
 * @brief Remap4Sel
 *
 * @param 
 *    in uint8_t sel: select remap4(sel = 0) or remap0(sel = 1)
 * @return 
 *    0: set remap pass
 *    1: set remap fail
 *
 */
uint8_t  Remap4Sel(uint8_t sel)
{
  uint32_t  remap_value, ee_addr;
  
  //Special Line 1 Update=======>REMAP4
  remap_value = (sel == 0) ? ((((~0x7e5au) & 0xffff) << 16) | 0x7e5a) : 0x0;
  ee_addr = _getMipAddr(1);
  
    if(unlockNVM(EEPROM_NVM))
    {
      return SYSCTRL_FAIL;
    }
    
  IAP.cmd    = IAP_EW;                         
  IAP.par[0] = ee_addr + SPL0_OFFSET + 2 * PAGE_SIZE - 4; // Destination Flash Address
  IAP.par[1] = (uint32_t)(&remap_value);                  // Source RAM Address
  IAP.par[2] = 4;                                         // Fixed Byte Size
  IAP_Call (&IAP.cmd, &IAP.stat);                         // Call IAP Command   

    if (IAP.stat)          // Command Failed
    {
        return SYSCTRL_FAIL;
    }
    
    remap_value = *(uint32_t *)(ee_addr + SPL0_OFFSET + 2 * PAGE_SIZE - 4);
    if(((sel == 1) && (remap_value == 0)) ||
       ((sel == 0) && (remap_value == ((((~0x7e5au) & 0xffff) << 16) | 0x7e5a))))
    {
      return SYSCTRL_PASS;
    }
    else
    {
        return SYSCTRL_FAIL;
    }
}
