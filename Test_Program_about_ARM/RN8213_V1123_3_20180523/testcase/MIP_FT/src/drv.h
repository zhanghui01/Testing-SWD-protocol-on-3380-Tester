#ifndef __DRV_H
#define __DRV_H

// global
#define  RES_OFFSET    0x37f8
#define  TEST_START    0xa5a5a5a5
#define  TEST_FAIL     0x12345677
#define  TEST_PASS     0x98765432

#define  SUCCESS       0xAA
#define  FAIL          0x55

// ROM
#define ROM_CHKSUM_A1     0xE99044BC
#define ROM_CHKSUM_A2     0xe9902f4c  // Add 2014-7-30 by caiqb
#define rom_chksum_D0     0x5a82002f

// EEPROM
#define  EE_PAGE_SIZE     64
#define  EE_CHIP_SIZE     (32 * 1024)
#define EE_PROT_SIZE      (2 * 1024)
#define EE_SECTOR_SIZE    (64 * PAGE_SIZE)
#define EE_MEM_BASE  (((SYSCTL->MAP_CTRL & 0x7) == 0) ? 0x08000000 : ((SYSCTL->MAP_CTRL & 0x7) == 1) ? 0x0: ((SYSCTL->MAP_CTRL & 0x7) == 2) ? 0x08000000 : 0x08000000)

// FLASH
#define FL_CHIP_SIZE             (192 * 1024)
#define FL_PAGE_SIZE             (128)
#define FL_PROT_SIZE             (16 * 1024)
#define FL_SECTOR_SIZE           (16 * 1024)
#define FL_MEM_BASE  ((SYSCTL->MAP_CTRL & 0x7) == 0) ? 0x0 : ((SYSCTL->MAP_CTRL & 0x7) == 1) ? 0x08000000 : ((SYSCTL->MAP_CTRL & 0x7) == 2) ? 0x10000000 : 0x18000000



#include "RN821x.h"
#include "common.h"
#include "sysctrl.h"
#include "nvm.h"
#include "system_RN821x.h"
#include "ROM_FT.h"
#include "flash_driver.h"
#include "FLASH_FT.h"
#include "eeprom_driver.h" 
#include "EEPROM_FT.h"



#endif

