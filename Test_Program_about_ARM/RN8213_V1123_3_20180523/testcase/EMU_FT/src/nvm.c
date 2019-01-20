/******************************************************************************
 * @file     nvm.c
 * @brief    non-valatile memory driver source code file for RN821x series SOC
 * @author   Renergy Technology
 * @version  V1.00
 * @date     2012-11-29
 *
 * @note
 * Copyright (C) 2008, Renergy Technology Inc. All rights reserved.
 *
 *
 ******************************************************************************/

#include "RN821x.h"
#include "system_RN821x.h"
#include "nvm.h"

/*
IAPÃüÁî	      ÃüÁî´úÂë
FlashÒ³²Á³ý	  0x50
Flash¿é²Á³ý	  0x51
flashÆ¬²Á³ý	  0x52
Flash¿é²é¿Õ	  0x53

EEPROMÒ³²Á³ý	0x54
EEPROM¿é²Á³ý	0x55
EEPROMÆ¬²Á³ý	0x56
EEPROM¿é²é¿Õ	0x57

Flash±à³Ì	    0x58
EEPROM±à³Ì	  0x59

NVMÊ¹ÄÜ       0x5a
Ä£ÄâÈí¼þ¸´Î»  0x5b
*/
#define  FL_PAGE_ERASE   0x50
#define  FL_SECT_ERASE   0x51
#define  FL_CHIP_ERASE   0x52

#define  EE_PAGE_ERASE   0x54
#define  EE_SECT_ERASE   0x55
#define  EE_CHIP_ERASE   0x56

#define  FL_PROG         0x58
#define  EE_PROG         0x59

#define  FL_TYPE         0
#define  EE_TYPE         1

/* IAP Call */
typedef void (*IAP_Entry) (unsigned long *cmd, unsigned long *stat);
#define IAP_Call ((IAP_Entry) 0x18001c01)

uint8_t nvmWrEnable(uint8_t nvm_type)
{  
  struct sIAP {                  // IAP Structure
    unsigned long cmd;           // Command
    unsigned long par[4];        // Parameters
    unsigned long stat;          // Status
    unsigned long res[2];        // Result
  } IAP;
  IAP.cmd = 0x5a;
  IAP.par[0] = nvm_type;
  IAP_Call(&IAP.cmd, &IAP.stat);
  return (uint8_t)IAP.stat;
}

uint8_t flashPageErase(uint32_t pg)
{  
  struct sIAP {
    unsigned long cmd;
    unsigned long par[4];
    unsigned long stat;
    unsigned long res[2];
  } IAP;
  IAP.cmd = FL_PAGE_ERASE;
  IAP.par[0] = pg;
  
  nvmWrEnable(FL_TYPE);
  IAP_Call(&IAP.cmd, &IAP.stat);
  return (uint8_t)IAP.stat;
}

uint8_t flashSectorErase(uint32_t sec)
{  
  struct sIAP {
    unsigned long cmd;
    unsigned long par[4];
    unsigned long stat;
    unsigned long res[2];
  } IAP;
  IAP.cmd = FL_SECT_ERASE;
  IAP.par[0] = sec;
  
  nvmWrEnable(FL_TYPE);
  IAP_Call(&IAP.cmd, &IAP.stat);
  return (uint8_t)IAP.stat;
}

uint8_t flashChipErase(void)
{  
  struct sIAP {
    unsigned long cmd;
    unsigned long par[4];
    unsigned long stat;
    unsigned long res[2];
  } IAP;
  IAP.cmd = FL_CHIP_ERASE;
  
  nvmWrEnable(FL_TYPE);
  IAP_Call(&IAP.cmd, &IAP.stat);
  return (uint8_t)IAP.stat;
}

uint8_t eepromPageErase(uint32_t pg)
{  
  struct sIAP {                  // IAP Structure
    unsigned long cmd;           // Command
    unsigned long par[4];        // Parameters
    unsigned long stat;          // Status
    unsigned long res[2];        // Result
  } IAP;
  IAP.cmd = EE_PAGE_ERASE;
  IAP.par[0] = pg;
  
  nvmWrEnable(EE_TYPE);
  IAP_Call(&IAP.cmd, &IAP.stat);
  return (uint8_t)IAP.stat;
}

uint8_t eepromSectorErase(uint32_t sec)
{  
  struct sIAP {
    unsigned long cmd;
    unsigned long par[4];
    unsigned long stat;
    unsigned long res[2];
  } IAP;
  IAP.cmd = EE_SECT_ERASE;
  IAP.par[0] = sec;
  
  nvmWrEnable(EE_TYPE);
  IAP_Call(&IAP.cmd, &IAP.stat);
  return (uint8_t)IAP.stat;
}

uint8_t eepromChipErase(void)
{  
  struct sIAP {
    unsigned long cmd;
    unsigned long par[4];
    unsigned long stat;
    unsigned long res[2];
  } IAP;
  IAP.cmd = EE_CHIP_ERASE;
  
  nvmWrEnable(EE_TYPE);
  IAP_Call(&IAP.cmd, &IAP.stat);
  return (uint8_t)IAP.stat;
}

uint8_t flashProgram(uint32_t dst_addr, uint32_t src_addr, uint32_t len)
{ 
  struct sIAP {
    unsigned long cmd;
    unsigned long par[4];
    unsigned long stat;
    unsigned long res[2];
  } IAP;
  
  IAP.cmd = FL_PROG;
	IAP.par[0] = dst_addr;
	IAP.par[1] = src_addr;
	IAP.par[2] = len;
  
  nvmWrEnable(FL_TYPE);
	IAP_Call(&IAP.cmd, &IAP.stat);
  return (uint8_t)IAP.stat;
}

uint8_t eepromProgram(uint32_t dst_addr, uint32_t src_addr, uint32_t len)
{ 
  struct sIAP {
    unsigned long cmd;
    unsigned long par[4];
    unsigned long stat;
    unsigned long res[2];
  } IAP;
  
  IAP.cmd = EE_PROG;
	IAP.par[0] = dst_addr;
	IAP.par[1] = src_addr;
	IAP.par[2] = len;
  
  nvmWrEnable(EE_TYPE);
	IAP_Call(&IAP.cmd, &IAP.stat);
  return (uint8_t)IAP.stat;
}

/* mode = 0: FLASH, mode = 1: EEPROM */
uint32_t  getMipAddr(uint8_t mode)
{
  uint32_t addr;
  switch (*(uint32_t *)(0x40034018) & 0x7)
  {
    case 0:
      addr =  (mode == 0) ? 0x00800000 : 0x08800000;
      break;
    case 1:
      addr = (mode == 0) ? 0x08800000 : 0x00800000;
      break;
    case 2:
      addr = (mode == 0) ? 0x10800000 : 0x08800000;
      break;
    case 3:
      addr = (mode == 0) ? 0x18800000 : 0x08800000;
      break;
  }
  return addr;
}

void    eepromStandby(void)
{
  uint32_t reg;
  
  reg = getMipAddr(1);
  *(uint32_t *)(reg + 0x0) = 0xfeed0007;
  while(*(uint32_t *)(reg + 0x8) & 0x1)
  {
    ;
  }
}

void    eepromWakeup(void)
{
  uint32_t reg;
  
  reg = getMipAddr(1);
  *(uint32_t *)(reg + 0x0) = 0xfeed0004;
  while(*(uint32_t *)(reg + 0x8) & 0x1)
  {
    ;
  }
}

void    cacheOff(void)
{
  uint32_t reg;
  
  reg = getMipAddr(0);
  *(uint32_t *)(reg + 0x1c) = 0xfeed0000;
  while(*(uint32_t *)(reg + 0x8) & 0x1)
  {
    ;
  }
}

void    cacheOn(void)
{
  uint32_t reg;
  
  reg = getMipAddr(0);
  *(uint32_t *)(reg + 0x1c) = 0xfeed0001;
  while(*(uint32_t *)(reg + 0x8) & 0x1)
  {
    ;
  }
}
