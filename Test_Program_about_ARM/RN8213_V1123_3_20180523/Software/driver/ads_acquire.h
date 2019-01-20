#ifndef  _ADS_ACQUIRE_H
#define  _ADS_ACQUIRE_H

#ifdef   _ADS_ACQUIRE_GLOBALS
	#define  _ADS_ACQUIRE_EXT
#else
	#define  _ADS_ACQUIRE_EXT  extern 
#endif

#include "lpc17xx.h"
#include <stdio.h>
#include "spi.h"
#include "ra0_routine.h"

// #define EMU_ENABLE_FIRST

#define ACQUIRE_TIMES 3

#define  MACHINE_TEMP_DIFF  (3*8)

//RN8213_FT_3#   A/B°æ  //20170817
#ifdef SIZE1
#1
//#define ADS_RATIO (1.0080+0.0049+0.0006+0.0025)
#define ADS_RATIO 1.0077
#define ADS_VERF 2.5000
#define CHIP_TEMPERTRIM_OFFSET -10
#define TEMPER_OFFSET 12
#endif

#ifdef SIZE2
#2
//#define ADS_RATIO (1.0067+0.0040+0.0023-0.0008)
//#define ADS_RATIO 1.0115
#define ADS_RATIO 1.0073
#define ADS_VERF  2.5002
#define CHIP_TEMPERTRIM_OFFSET -7
#define TEMPER_OFFSET 12
#endif

#define CHAN_RA1   ((0x0 << 4) | (0x1))
#define CHAN_RA2   ((0x2 << 4) | (0x3))
#define CHAN_RA3   ((0x4 << 4) | (0x5))
#define CHAN_RA4   ((0x6 << 4) | (0x7))


#define SPIDRDYN  (LPC_GPIO2->FIOPIN & 0x10)

#define ADS_SETUP 0x0
#define ADS_MUX   0x1
#define ADS_ACR   0x2
#define ADS_ODAC  0x3
#define ADS_DIO   0x4
#define ADS_DIR   0x5
#define ADS_IOCON 0x6
#define ADS_OCR0  0x7
#define ADS_OCR1  0x8
#define ADS_OCR2  0x9
#define ADS_FSR0  0xA
#define ADS_FSR1  0xB
#define ADS_FSR2  0xC
#define ADS_DOR2  0xD
#define ADS_DOR1  0xE
#define ADS_DOR0  0xF

#define ADS_SELFCAL   0xf0
#define ADS_SELFOCAL  0xf1
#define ADS_SELFGCAL  0xf2
#define ADS_SYSOCAL   0xf3
#define ADS_SYSOGAL   0xf4
#define ADS_WAKEUP    0xfb
#define ADS_DSYNC     0xfc
#define ADS_SLEEP     0xfd
#define ADS_RESET     0xfe

uint32_t adsDataRead(void) ;
extern void adsInit(void);
extern void adsAcquireInit(uint32_t channel);
extern uint32_t adsAcquire(void);
extern void adsOffsetCal(void);
extern void adsInitvdd(uint32_t channel);

#endif
