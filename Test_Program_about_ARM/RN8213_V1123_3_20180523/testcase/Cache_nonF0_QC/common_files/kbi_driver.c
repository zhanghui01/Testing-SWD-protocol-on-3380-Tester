#include "ra9103.h"
//#include "common.h"

uint8_t  key = 0;

void kbi_init()
{
	SYSCTL->SYS_PS = 0x82;
  SYSCTL->MOD1_EN |= (1 << 5);     /* enable GPIO clk */
  SYSCTL->KBI_EN = 0xFFFF;                    /* enable KBI clk */
  GPIO->PCA0 = GPIO->PCA0 & (~(0xffff << 9)) | (0x5555 << 9); /* configure kbi pad */ 
  
  KBI->CTRL = 0xff;
  KBI->SEL = 0x0;    //posedge
  KBI->DATA = 0xff;
  KBI->MASK = 0xff;
  
  key = 0;
  
  NVIC_ClearPendingIRQ(KBI_IRQn);
  NVIC_EnableIRQ(KBI_IRQn);
  __enable_irq();
  
}

void KBI_HANDLER(){
	GPIO->PA |= (1 << 25);     //output high
  key = KBI->DATA;
  KBI->DATA = KBI->DATA;
}
