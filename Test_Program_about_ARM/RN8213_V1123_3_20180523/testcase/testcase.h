#ifndef __TESTCASE__H
#define __TESTCASE_H

#include "swd_atf.h"
#include "list.h"

const s_atf_img sys_atf_tbl[]={
 {2, 0, SizeOfAES_FTImg, "AES_FT", 2000, (unsigned char *)pImgOfAES_FT},
 {2, 0, SizeOfALL_REG_WR_QCImg, "ALL_REG_WR_QC", 2000, (unsigned char *)pImgOfALL_REG_WR_QC},
 {2, 0, SizeOfASRAM_MARCHImg, "ASRAM_MARCH", 2000, (unsigned char *)pImgOfASRAM_MARCH},
 {2, 0, SizeOfCache_nonF0_QCImg, "Cache_nonF0_QC", 4000, (unsigned char *)pImgOfCache_nonF0_QC},
 {2, 0, SizeOfClock_Switch_QCImg, "Clock_Switch_QC", 2000, (unsigned char *)pImgOfClock_Switch_QC},
 {2, 0, SizeOfCM0_INST_FTImg, "CM0_INST_FT", 2000, (unsigned char *)pImgOfCM0_INST_FT},
 {2, 0, SizeOfEEPROM_FTImg, "EEPROM_FT", 5000, (unsigned char *)pImgOfEEPROM_FT},
 {2, 0, SizeOfEMU_FTImg, "EMU_FT", 4000, (unsigned char *)pImgOfEMU_FT},
 {2, 0, SizeOfGPIO_FTImg, "GPIO_FT", 1000, (unsigned char *)pImgOfGPIO_FT},
 {2, 0, SizeOfIFLASH_FTImg, "IFLASH_FT", 5000, (unsigned char *)pImgOfIFLASH_FT},
 {2, 0, SizeOfMIP_FTImg, "MIP_FT", 1000, (unsigned char *)pImgOfMIP_FT},
 {2, 0, SizeOfSocLowPwr_sramImg, "SocLowPwr_sram", 2000, (unsigned char *)pImgOfSocLowPwr_sram},
 {0, 0, SizeOfSocLowPwr_sysImg, "SocLowPwr_sys", 2000, (unsigned char *)pImgOfSocLowPwr_sys},
 {0, 0, SizeOfSoc_DISLowPwr_sysImg, "Soc_DISLowPwr_sys", 2000, (unsigned char *)pImgOfSoc_DISLowPwr_sys},
 {0, 0, SizeOfSofeWareImg, "SofeWare", 90000, (unsigned char *)pImgOfSofeWare},
 {2, 0, SizeOfZPLL_1Hz_FTImg, "ZPLL_1Hz_FT", 3000, (unsigned char *)pImgOfZPLL_1Hz_FT},
};

#endif
