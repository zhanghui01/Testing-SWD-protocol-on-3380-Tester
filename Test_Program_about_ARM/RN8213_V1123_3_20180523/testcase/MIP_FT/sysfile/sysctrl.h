/******************************************************************************
 * @file     sysctrl.h
 * @brief    System control driver header file for RN821x series SOC
 * @author   Renergy Technology
 * @version  V1.00
 * @date     2012-11-28
 *
 * @note
 * Copyright (C) 2008, Renergy Technology Inc. All rights reserved.
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
 *    in uint8_t mode: system mode, support LC(0), RC(1), PLL(2) and HC(3)
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
 *    none
 *
 */
void SysUpdate(void);


#ifdef __cplusplusw
}
#endif

#endif
