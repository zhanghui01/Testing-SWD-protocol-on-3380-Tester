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


//=====================================================================================
//
// SysModeChg() lib
//
//=====================================================================================
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

#define FEED_WDT               WDT->EN = 0xbb

static uint32_t currentClk(void){
  uint32_t  freq;  
  uint32_t  val = (SYSCTL->OSC_CTRL1 >> 8) & 0x7;
  
  if(val == 0x3)
    freq = 32768;
  else if(val == 0x2)
    freq = 1843200;
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

//timeout unit: us
//sign = 1, wait while value is 1
//sign = 0, wait while value is 0
static void usTimeout(uint32_t addr, uint32_t val, uint32_t sign, uint32_t timeout)
{
	uint32_t i, loops;
	uint32_t freq;
 
  if(timeout == 0)
    return;
  
  //SysTick range: 29M: 568ms; 14M:1.137s; 7M:2.274s; 3M:4.55s; 1M:9.1s; 32K: 511.7s
  freq = currentClk();
  
	switch (freq)
  {
    case 32768:
      loops = 0;
		  break;
    case 1843200:
      loops = timeout / 9100000;
		  break;
    case 3686400:
      loops = timeout / 4550000;
		  break;
    case 7372800:
      loops = timeout / 2274000;
		  break;
    case 14745600:
      loops = timeout / 1137000;
      break;
    case 29491200:
      loops = timeout / 568000;
      break;          
  }
  loops++;
  
  for(i = 0; i < loops; i++)
  {
	  SysTick->LOAD = 0xffffff;   
    SysTick->CTRL = 0x4;
    SysTick->VAL  = 0x0;
	  if(freq == 32768)
	  	SysTick->LOAD = 33 * timeout;
	  else
      SysTick->LOAD = freq / 1000000 * timeout;
    
    SysTick->CTRL = 0x5;
    while(!(SysTick->CTRL & 0x10000))
    {
      if(((sign == 1) && (((*(uint32_t *)addr) & val) != val)) ||    //wait while value is 1
         ((sign == 0) && (((*(uint32_t *)addr) & val) == val)))      //wait while value is 0
      {
        SysTick->CTRL = 4;
        return;
      }
      FEED_WDT;
    }
  }
  
  SysTick->CTRL = 4;
  return;
}

static void setMipTW(uint32_t freq)
{
  uint32_t   *fl_tw, *ee_tw;
  
  fl_tw  = (uint32_t *)(_getMipAddr(0) + 0x00800018);
  ee_tw = (uint32_t *)(_getMipAddr(1) + 0x00800018);
  
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

static uint8_t PllEnable(void)
{
  uint32_t flag;
  /* enable pll for rtc in hosc mode */
	//SYSCTL->OSC_CTRL2 &= (~(1 << 4));         /* pll as source */
  SYSCTL->OSC_CTRL1 &= (~1);                /* enable pll */
  usTimeout((uint32_t)(&SYSCTL->OSC_CTRL1), (1 << 7), 0, PLL_TIMEOUT);   /* wait PLL lock */
  usTimeout((uint32_t)(&SYSCTL->OSC_CTRL1), (1 << 12), 0, PLL_TIMEOUT);  /* wait PLL lock & flag */

  flag = SYSCTL->OSC_CTRL1;
  if(!(flag & (1 << 12)))
  {
    return 1;
  }
  
  return 0;
}

static uint8_t HoscEnable(void)
{
  uint32_t flag, mod1_en;
  /* enable pll for rtc in hosc mode */
	PllEnable();

  mod1_en = SYSCTL->MOD1_EN;
  
  SYSCTL->MOD1_EN |= (1 << 5);
  GPIO->PCA1 |= (0xff << 12);                                       /* PAD as osc */
  SYSCTL->OSC_CTRL2 = (SYSCTL->OSC_CTRL2 & (~(0x1 << 3))) | (1 << 4); /* osc as source, enable osc */
  usTimeout((uint32_t)(&SYSCTL->OSC_CTRL1), 1 << 15, 0, HOSC_TIMEOUT);   /* wait HOSC flag */
  
  flag = SYSCTL->OSC_CTRL1;
  if(!((flag & (1 << 15)) && (flag & (1 << 12))))
  {
    SYSCTL->MOD1_EN = mod1_en;
    return 1;
  }
  
  SYSCTL->MOD1_EN = mod1_en;
  return 0;
}

/* mode = LC(0), RC(1), PLL(2), HOSC(3) */
static uint8_t sysmodeWrite(uint8_t mode)
{
  uint8_t  clk_flag;
  //uint32_t mod1_en;
  
//  mod1_en = SYSCTL->MOD1_EN;
  
  if(mode == 0)
  {
    usTimeout((uint32_t)(&SYSCTL->OSC_CTRL1), (1 << 11), 0, LOSC_TIMEOUT); /* wait RCH flag */
  }
  else if(mode == 1)
	{
		SYSCTL->OSC_CTRL1 &= (~(1 << 1));
		usTimeout((uint32_t)(&SYSCTL->OSC_CTRL1), (1 << 13), 0, RCH_TIMEOUT); /* wait RCH flag */
	}
  else if(mode == 2)
  {
    PllEnable();  //open again for OSC_SEL write
#if 0
    SYSCTL->OSC_CTRL2 &= (~(1 << 4));         /* pll as source */
    SYSCTL->OSC_CTRL1 &= (~1);                /* enable pll */
    usTimeout((uint32_t)(&SYSCTL->OSC_CTRL1), (1 << 7), 0, PLL_TIMEOUT);   /* wait PLL lock */
    usTimeout((uint32_t)(&SYSCTL->OSC_CTRL1), (1 << 12), 0, PLL_TIMEOUT);  /* wait PLL lock & flag */
#endif
  }
  else if(mode == 3)
  {
    HoscEnable();
#if 0
		/* enable pll for rtc in hosc mode */
		//SYSCTL->OSC_CTRL2 &= (~(1 << 4));         /* pll as source */
    SYSCTL->OSC_CTRL1 &= (~1);                /* enable pll */
    usTimeout((uint32_t)(&SYSCTL->OSC_CTRL1), (1 << 7), 0, PLL_TIMEOUT);   /* wait PLL lock */
    usTimeout((uint32_t)(&SYSCTL->OSC_CTRL1), (1 << 12), 0, PLL_TIMEOUT);  /* wait PLL lock & flag */

    SYSCTL->MOD1_EN |= (1 << 5);
    GPIO->PCA1 |= (0xff << 12);                                       /* PAD as osc */
    SYSCTL->OSC_CTRL2 = (SYSCTL->OSC_CTRL2 & (~(0x1 << 3))) | (1 << 4); /* osc as source, enable osc */
    usTimeout((uint32_t)(&SYSCTL->OSC_CTRL1), 1 << 15, 0, HOSC_TIMEOUT);   /* wait HOSC flag */
#endif
  }
  
  clk_flag = (SYSCTL->OSC_CTRL1 >> 11) & 0x1f;
  
  if((mode == 0) && (clk_flag & 0x1))
  {
    SYSCTL->SYS_MODE = 0xF;
    setMipTW(32768);
    SYSCTL->OSC_CTRL1 |= (3 << 0);   /* close RC&PLL */
    SYSCTL->OSC_CTRL2 |= (1 << 3);   /* close HOSC */
    return 0;
  }
  else if((mode == 1) && (clk_flag & 0x4))
  {
    SYSCTL->SYS_MODE = 0xE;
    setMipTW(1843200);
//    SYSCTL->OSC_CTRL1 |= (1 << 0);  /* close PLL */
//    SYSCTL->OSC_CTRL2 |= (1 << 3);  /* close HOSC */
    return 0;
  }
  else if((mode == 2) && (clk_flag & 0x2))
  {
    SYSCTL->SYS_MODE = 0xD;
    SYSCTL->OSC_CTRL1 |= (1 << 1);     /* disable RC */
    return 0;
  }
  else if((mode == 3) && (clk_flag & 0x10))
  {
    SYSCTL->SYS_MODE = 0xD;
    //SYSCTL->MOD1_EN = mod1_en;
    SYSCTL->OSC_CTRL1 |= (1 << 1);     /* disable RC */
    return 0;
  }
  
  //SYSCTL->MOD1_EN = mod1_en;
  return 1;
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
  uint32_t   new_freq, hosc_sel, sys_ps;
  
  sys_ps = SYSCTL->SYS_PS;
  SYSCTL->SYS_PS = 0x82;
  
  //===============================
  // always enable RTC/WDT apb clock
  //===============================
  SYSCTL->MOD1_EN |= ((1 << 10) | (1 << 9));
    
  FEED_WDT;
  usTimeout((uint32_t)(&SYSCTL->SYS_MODE), 0x10, 1, MIP_TIMEOUT);   /* check MIP busy */
  usTimeout((uint32_t)(&SYSCTL->SYS_MODE), 0x20, 1, MIP_TIMEOUT);
  
  if(mode == 0)  /* LC mode */
  {
    if(sysmodeWrite(mode))
    {
      return 1;
    }
  }
  else if(mode == 1)  /* RC mode */
  {
    if(sysmodeWrite(mode))
    {
      return 1;
    }
    else
    {
      SYSCTL->OSC_CTRL1 |= (1 << 0);  /* close PLL */
      SYSCTL->OSC_CTRL2 |= (1 << 3);  /* close HOSC */
    }
  }
  else if(mode < 4)
  {
		if((mode == 2) && PllEnable())
		{
			return 1;
		}
		else if((mode == 3) && HoscEnable())
    {
      return 1;
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
            //return 1;          /* invalid divisor parameter */
            new_freq = 1843200;
          break;
        default:
          return 1;       /* invalid divisor parameter */
      }
      
      sysmodeWrite(1);
		  setMipTW(new_freq);
		  SYSCTL->OSC_CTRL1 = SYSCTL->OSC_CTRL1 & (~(3 << 2)) |(div << 2);
      if(sysmodeWrite(mode))
      {
        return 1;
      }
    }
  }
  else
  {
    return 1;    /* invalid mode parameter */
  }
	
  FEED_WDT;
	SYSCTL->SYS_PS = sys_ps;
	return 0;
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
  if (IAP.stat) return (1);                    // Command Failed		
	else return (0);
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
	  return 1;
	}
	
  IAP.cmd    = IAP_EW;                         
  IAP.par[0] = ee_addr + SPL0_OFFSET + 2 * PAGE_SIZE - 4; // Destination Flash Address
  IAP.par[1] = (uint32_t)(&remap_value);                  // Source RAM Address
  IAP.par[2] = 4;                                         // Fixed Byte Size
  IAP_Call (&IAP.cmd, &IAP.stat);                         // Call IAP Command	

	if (IAP.stat)          // Command Failed
	{
		return 1;
	}
	
	remap_value = *(uint32_t *)(ee_addr + SPL0_OFFSET + 2 * PAGE_SIZE - 4);
	if(((sel == 1) && (remap_value == 0)) ||
	   ((sel == 0) && (remap_value == ((((~0x7e5au) & 0xffff) << 16) | 0x7e5a))))
	{
	  return 0;
	}
	else
	{
		return 1;
	}
}
