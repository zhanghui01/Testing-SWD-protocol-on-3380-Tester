#ifndef  _SPI_H
#define  _SPI_H

#ifdef   _SPI_GLOBALS
	#define  SPI_EXT
#else
	#define  SPI_EXT  extern 
#endif


#include "lpc17xx.h"

#define SPICS_SET0 LPC_GPIO0->FIOCLR = (1 << 16) //P0_16 = 0 
#define SPICS_SET1 LPC_GPIO0->FIOSET = (1 << 16) //P0_16 = 1 

#define DIROUTPUT_SET0 LPC_GPIO1->FIOCLR = (1 << 29)  //P1_29  = 0 
#define DIROUTPUT_SET1 LPC_GPIO1->FIOSET = (1 << 29)  //P1_29  = 1 
#define DIRINPUT_SET0  LPC_GPIO1->FIOCLR = (1 << 28)  //P1_28  = 0 
#define DIRINPUT_SET1  LPC_GPIO1->FIOSET = (1 << 28)  //P1_28  = 1 
#define TR0_EN_SET0    LPC_GPIO2->FIOCLR = (1 << 8)   //P2_8  = 0 
#define TR0_EN_SET1    LPC_GPIO2->FIOSET = (1 << 8)   //P2_8  = 1 

extern void spiInit(void);
extern uint8_t spiSendByte(uint8_t data);
extern uint8_t spiReceByte(void);

#endif 
