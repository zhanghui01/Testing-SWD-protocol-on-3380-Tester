#ifndef  _TIMER_H
#define  _TIMER_H

#ifdef   _TIMER_GLOBALS
	#define  TIMER_EXT
#else
	#define  TIMER_EXT  extern 
#endif

#include "lpc17xx.h"

TIMER_EXT uint16_t	sys_stamp;

extern void timer3Init(void);
extern uint16_t stampThrough(uint16_t ago);

#endif
