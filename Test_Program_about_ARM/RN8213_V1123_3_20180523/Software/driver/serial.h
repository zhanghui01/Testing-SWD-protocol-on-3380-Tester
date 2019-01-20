/*----------------------------------------------------------------------------
 * Name:    Serial.h
 * Purpose: Low level serial definitions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2010 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

//#define  DEBUG_ON
//
//#ifdef   DEBUG_ON
//#define  DBG   printf
//#else
//#define  DBG(info,...)
//#endif
//
//#define  DBG_ERR(info,...)   printf("File: "__FILE__", line: %05d: "info"\n", __LINE__, ##__VA_ARGS__)

// #define DEBUG_LEVEL 3  //0:ERR, 1:WARNNING, 2:ALL ,3:NONE
// #if DEBUG_LEVEL == 0

// #define DBG_ERR(info,...)   printf("File: "__FILE__", line: %05d: "info"\n", __LINE__, ##__VA_ARGS__)
// #define DBG_WARNNING(info,...)
// #define DBG_MSG(info,...)

// #elseif DEBUG_LEVEL == 1

// #define DBG_ERR(info,...)   printf("File: "__FILE__", line: %05d: "info"\n", __LINE__, ##__VA_ARGS__)
// #define DBG_WARNNING        printf
// #define DBG_MSG(info,...)

// #elseif DEBUG_LEVEL == 2

// #define DBG_ERR(info,...)   printf("File: "__FILE__", line: %05d: "info"\n", __LINE__, ##__VA_ARGS__)
// #define DBG_WARNNING        printf
// #define DBG_MSG             printf

// #else 

// #define DBG_ERR(info,...)   
// #define DBG_WARNNING(info,...)        
// #define DBG_MSG(info,...)             

// #endif
#define DBG_ERR(info,...)   printf("File: "__FILE__", line: %05d: "info"\n", __LINE__, ##__VA_ARGS__)
#define DBG_WARNNING        printf
#define DBG_MSG             printf

extern void SER_Init      (void);
extern int  SER_GetChar   (void);
extern int  SER_PutChar   (int c);

void uartInit(void);

#endif
