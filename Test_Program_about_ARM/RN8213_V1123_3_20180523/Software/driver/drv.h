#ifndef __DRV_H
#define __DRV_H

#include <lpc17xx.h>
#include "serial.h"
#include "swd_drv.h"
#include "swd_atf.h"
#include "board.h"
#include "test_all.h"
#include "LCD_Pump.h"
#include "Voltage.h"
#include "adc.h"
#include "ra9103_reg.h"
#include "rn821x.h"
#include "ads_acquire.h"
// #include "eeprom.h"
#include "ra0_routine.h"
#include "spi.h"
#include "spline.h"
#include "timer.h"
#include "temper_process.h"
#include "EMU.h"
#include "ldo18_trim.h"
#include "rcl_trim.h"
#include "sar.h"


#define FAIL    0
#define SUCCESS 1
#define PASS    1 
#define TEMPER_ERR 0x55


// P1.28 SET 1
#define DIR2_SET1 {LPC_PINCON->PINSEL3 &= (0x3<<24) ;LPC_GPIO1->FIOMASK &= (1<<28) ;LPC_GPIO1->FIODIR |= (1<<28) ;LPC_GPIO1->FIOSET |= (1<<28) ;}
// P1.28 CLR 0
#define DIR2_CLR0 {LPC_PINCON->PINSEL3 &= (0x3<<24) ;LPC_GPIO1->FIOMASK &= (1<<28) ;LPC_GPIO1->FIODIR |= (1<<28) ;LPC_GPIO1->FIOCLR |= (1<<28) ;}
// P1.29 SET 1
#define DIR1_SET1 {LPC_PINCON->PINSEL3 &= (0x3<<26) ;LPC_GPIO1->FIOMASK &= (1<<29) ;LPC_GPIO1->FIODIR |= (1<<29) ;LPC_GPIO1->FIOSET |= (1<<29) ;}
// P1.29 CLR 0
#define DIR1_CLR0 {LPC_PINCON->PINSEL3 &= (0x3<<26) ;LPC_GPIO1->FIOMASK &= (1<<29) ;LPC_GPIO1->FIODIR |= (1<<29) ;LPC_GPIO1->FIOCLR |= (1<<29) ;}


// 164245 disable
#define BUS_OFF {LPC_PINCON->PINSEL4 &= (~(0x3 << 16));LPC_GPIO2->FIOMASK = ~(1 << 8);LPC_GPIO2->FIODIR |= (1<<8) ;LPC_GPIO2->FIOSET |= (1<<8) ;}
// 164245 enable
#define BUS_ON  {LPC_PINCON->PINSEL4 &= (~(0x3 << 16));LPC_GPIO2->FIOMASK = ~(1 << 8);LPC_GPIO2->FIODIR |= (1<<8) ;LPC_GPIO2->FIOCLR |= (1<<8) ;}

// P2.10
#define FLUSH_INI {LPC_PINCON->PINSEL4 &= (~(0x3<<20));LPC_GPIO2->FIOMASK = ~(1 << 10);LPC_GPIO2->FIODIR |= (1<<10) ;LPC_GPIO2->FIOSET |= (1<<10) ;  }
// #define FLUSH {LPC_GPIO2->FIOPIN = LPC_GPIO2->FIOPIN & (~(1<<10)) | ((1<<10) & (~(LPC_GPIO2->FIOPIN & (1<<10)))) ;}
#define FLUSH {LPC_GPIO2->FIOPIN = LPC_GPIO2->FIOPIN & (~(1<<10)) | ((~(LPC_GPIO2->FIOPIN & (1<<10))) & (1<<10)) ;}
#define FLUSH_H {LPC_GPIO2->FIOSET |= (1<<10) ;}
#define FLUSH_L {LPC_GPIO2->FIOCLR |= (1<<10) ;}

#define TESTFAIL 0x3

#define  CGF_2015_05_23

#endif


