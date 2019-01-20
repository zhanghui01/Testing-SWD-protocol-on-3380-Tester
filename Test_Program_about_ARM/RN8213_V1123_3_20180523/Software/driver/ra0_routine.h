#ifndef  _RA0_ROUTINE_H
#define  _RA0_ROUTINE_H

#ifdef   _RA0_ROUTINE_GLOBALS
	#define  _RA0_ROUTINE_EXT
#else
	#define  _RA0_ROUTINE_EXT  extern 
#endif

#include "LPC17xx.h"
#include "swd_drv.h"
#include "ads_acquire.h"
#include "eeprom.h"
#include "timer.h"

// #define FAIL    0
// #define SUCCESS 1

_RA0_ROUTINE_EXT uint32_t tadj;
_RA0_ROUTINE_EXT uint32_t std_connect_flag;

extern void stdInitial(void);
extern uint32_t temperLinePro(void);
extern uint32_t ra0TempAdjust(uint16_t std_temper);
extern uint32_t average(uint32_t * pdata,uint32_t num);
extern void swDelay(uint16_t delay_time);
extern uint32_t sumGen32(uint32_t* p_data,uint32_t length);
extern uint16_t stdTempAcquire(void);
extern void ra9103ADCInit(void);
extern uint8_t swdInitial(uint8_t swd_id);

#endif
