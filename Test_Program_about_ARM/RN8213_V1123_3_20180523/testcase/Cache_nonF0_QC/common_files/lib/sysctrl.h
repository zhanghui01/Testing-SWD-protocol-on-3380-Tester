/******************************************************************************
 * @file     sysctrl.h
 * @brief    System control driver header file for RN821x series SOC
 * @author   Renergy Technology
 * @version  V1.2
 * @date     2017-04-21
 *
 * @note
 * Copyright (C) , Renergy Technology Inc. All rights reserved.
 *
 *
 ******************************************************************************/
#ifndef __SYSCTRL_H
#define __SYSCTRL_H

#ifdef __cplusplus
extern "C" {
#endif

#define SYSCTRL_PASS                      0
#define SYSCTRL_FAIL                      1

/**
 * @brief SysModeChg
 *
 * @param 
 *    in uint8_t mode: system mode, support 0=LC(32.768Khz), 1=RC(typ. 1.6Mhz), 2=PLL(7.3728Mhz), 3=HC(depends on OSC_CTRL2[2:0]), 4=RC(typ. 3.2Mhz) and 5=PLL(14.7456Mhz)
 * @param 
 *    in uint8_t div: the divisor of HC mode, support no-div(0), 2-div(1), 4-div(2) and 8-div(3)
 * @return 
 *    0: system mode change pass
 *    1: system mode change fail
 *
 */
uint8_t  SysModeChg(uint8_t mode, uint8_t div);

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
uint8_t  Remap4Sel(uint8_t sel);

/**
 * @brief SysUpdate
 *   Update all parameters stored in eeprom splines if configuration regs test fail
 * @param 
 *    none
 * @return 
 *    0: vbat parameters init pass
 *    1: vbat parameters init fail
 *
 */
uint8_t VbatParaInit(void);


#ifdef __cplusplusw
}
#endif

#endif
