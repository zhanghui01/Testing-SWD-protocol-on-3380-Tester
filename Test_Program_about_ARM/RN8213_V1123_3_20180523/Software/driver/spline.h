#ifndef  _SPLINE_H
#define  _SPLINE_H

#ifdef   _SPLINE_GLOBALS
	#define  _SPLINE_EXT
#else
	#define  _SPLINE_EXT  extern 
#endif

#include "LPC17xx.h"
#include "swd_drv.h"
#include "eeprom.h"
#include "ra0_routine.h"

#define CP0_CODE 0x0
#define CP1_CODE 0x12345678
#define CP2_CODE 0x87654321
#define CP3_CODE 0x4e647370

extern uint32_t sp0LinePro(void);
extern uint32_t temperLinePro(void);
extern uint32_t UID_check(void);
extern uint32_t FT_UID_write(void);

#endif
