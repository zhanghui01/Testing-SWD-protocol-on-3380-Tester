/******************************************************************************
 * @file     nvm.h
 * @brief    non-valatile memory driver header file for RN821x series SOC
 * @author   Renergy Technology
 * @version  V1.00
 * @date     2012-11-29
 *
 * @note
 * Copyright (C) 2008, Renergy Technology Inc. All rights reserved.
 *
 *
 ******************************************************************************/
#ifndef __NVM_H
#define __NVM_H

#ifdef __cplusplus
extern "C" {
#endif

#define NVM_PASS                      0
#define INVALID_PAGE                  8
#define SRC_ADDR_NOT_ALIGN            10
#define SRC_ADDR_NOT_MAPPED           11
#define DST_ADDR_NOT_ALIGN            12
#define DST_ADDR_NOT_MAPPED           13

uint8_t flashPageErase(uint32_t pg);
uint8_t flashSectorErase(uint32_t sec);

uint8_t eepromPageErase(uint32_t pg);
uint8_t eepromSectorErase(uint32_t sec);

uint8_t flashProgram(uint32_t dst_addr, uint32_t src_addr, uint32_t len);
uint8_t eepromProgram(uint32_t dst_addr, uint32_t src_addr, uint32_t len);

void    eepromStandby(void);
void    eepromWakeup(void);

void    cacheOn(void);
void    cacheOff(void);

#ifdef __cplusplusw
}
#endif

#endif
