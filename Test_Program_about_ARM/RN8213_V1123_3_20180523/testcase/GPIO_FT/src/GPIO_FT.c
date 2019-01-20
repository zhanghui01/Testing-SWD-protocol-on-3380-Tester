#include "drv.h"

void FT_Result( uint32_t result ) ;
uint8_t GPIO_Test(void) ;

int32_t main()
{
	uint8_t  result ;
  uint32_t * sram_res;
  uint32_t remap ;
	
//   SysModeChg(2, 0);
  remap = (SYSCTL->MAP_CTRL & 0x3);
	sram_res = (uint32_t *)(((remap == 2) ? 0x0 : 0x10000000) + RES_OFFSET);
	*sram_res = TEST_START;
	
	SysModeChg(2, 0);
	result = GPIO_Test() ;
	FT_Result(result) ;
	
}

void FT_Result( uint32_t result )
{
  uint32_t  * sram_res ;
  uint32_t  remap ;
  
  remap    = (SYSCTL->MAP_CTRL & 0x3) ;
	sram_res = (uint32_t *)(((remap == 2) ? 0x0 : 0x10000000) + RES_OFFSET) ;
	
	if( result == FAIL )
	{
		*sram_res = TEST_FAIL;
	}
	else
	{
		*sram_res = TEST_PASS;
	}
	while(1)
		WdtFeed() ;	
	
}

/******************************************************************************************************
** RN8213 GPIO test
** PA[]={ P02 ,P03 ,P04 ,P10 ,P11 ,P20~P23 ,P26 ,P27 ,P32}
** PB[]={ P40~P47 ,P50~P55 ,P57}
** PB_LCD[]={ P60~P67 ,P70~P77 }
** PC_LCD[]={ P80~P87 ,P90~P95 }
**
******************************************************************************************************/
#define PA_H      0x04cf031c
#define PB_IO_H   0xbeff
#define PB_LCD_H  0xffff0000
#define PC_LCD_H  0x3fff

uint8_t GPIO_Test(void)
{
	uint32_t i ,j ;
	uint8_t GPIO_A[] = {2,3,4,10,11,20,21,22,23,26,27,32} ;
	uint8_t PAD_Num ;
	
	PAD_Num = sizeof(GPIO_A)/ sizeof(uint8_t) ;
	
	SYSCTL->SYS_PS = 0x82 ;
	SYSCTL->MOD1_EN |= (1<<5) ;  // GPIO CLK enable 
	
  // ALL GPIO as input	
	// PA 
	GPIO->PMA = GPIO->PMA | (0x7<<2) | (0x3<<8) | (0xcf<<16) | (1<<26) ;  // P02~P04 ,P10~P11 ,P20~P23 ,P26 ,P27 ,P32 as input
	GPIO->PCA0 = (1<<29) ;  // P00~P27 as GPIO ,P24 ,P25 as SWD
	GPIO->PCA1 = (0xf<<12) ;  // P32 as GPIO ,P36,P37 as HOSC
	GPIO->PIEA = ~((0x7<<2) | (0x3<<8) | (0xcf<<16) | (1<<26)) ;  // P02~P04 ,P10~P11 ,P20~23 ,P26 ,P27 ,P32 input enable

	// PB  
	GPIO->PMB  |= 0xbeff ;  // P40~P47 ,P50~P55 ,P57 as input
	GPIO->PCB  &= (~0xbeff) ;  // P40~P47 ,P50~P55 ,P57 as GPIO
	GPIO->PIEB &= (~0xbeff) ;  // P40~P47 ,P50~P55 ,P57 input enable
	
	// PB LCD 
	GPIO->PMB  |= (0xffffu<<16) ;  // P60~67 ,P70~P77 as input
	GPIO->PCB  &= (~(0xffffu<<16)) ;  // P60~67 ,P70~P77 as GPIO
	GPIO->PIEB &= (~(0xffffu<<16)) ;  // P60~67 ,P70~P77 input enable
	
	// PC 
	GPIO->PMC  |= (0x3fff<<0) ;  // P80~P87 ,P90~P95 as input
	GPIO->PCB  &= (~(0x3fff<<0)) ;  // P80~P87 ,P90~P95 as GPIO
	GPIO->PIEC &= (~(0x3fff<<0)) ;  // P80~P87 ,P90~P95 input enable
	
	// PA test
	for( i=0;i<PAD_Num;i++ )  // PAD num is 12
	{
		GPIO->PMA = GPIO->PMA | (0x7<<2) | (0x3<<8) | (0xcf<<16) | (1<<26) ;  // P02~P04 ,P10~P11 ,P20~P23 ,P26 ,P27 ,P32 as input
		
		j = GPIO_A[i]%10 + (GPIO_A[i]/10)*8  ;
		
		GPIO->PMA &= (~(1<<j)) ;  // PINx as output ;
		GPIO->PA  |= (1<<j)    ;  // PINx = 1
		uDelay(100) ;
	  if( (GPIO->PA & PA_H) != PA_H )
	  	return FAIL ;
	  	
	  GPIO->PA &= (~(1<<j)) ;  // PINx = 0 ;
	  uDelay(100) ;
	  if( (GPIO->PA & PA_H) != 0 )
	  	return FAIL ; 
		
	}
	GPIO->PMA = GPIO->PMA | (0x7<<2) | (0x3<<8) | (0xcf<<16) | (1<<26) ;  // P02~P04 ,P10~P11 ,P20~P23 ,P26 ,P27 ,P32 as input
	
	// PB GPIO test
	GPIO->PMB  |= 0xbeff ;  // P40~P47 ,P50~P55 ,P57 as input
	GPIO->PCB  &= (~0xbeff) ;  // P40~P47 ,P50~P55 ,P57 as GPIO
	GPIO->PIEB &= (~0xbeff) ;  // P40~P47 ,P50~P55 ,P57 input enable
	for( i=0;i<16;i++ )
	{
		if( (i==8) || (i==14) )  // P50 ,P56 ignored
			continue ;
		GPIO->PMB  |= 0xbeff ;  // P40~P47 ,P51~P55 ,P57 as input
		
		GPIO->PMB &= (~(1<<i)) ;  // PB GPIO as output
		GPIO->PB  |= (1<<i)   ;  // PB GPIO = 1
		uDelay(100) ;
		if( (GPIO->PB & PB_IO_H) != PB_IO_H )
			return FAIL ;
			
		GPIO->PB &= (~(1<<i)) ; // PB GPIO = 0
		uDelay(100) ;
		if( (GPIO->PB & PB_IO_H) != 0 )
			return FAIL ;
			
	}
	GPIO->PMB  |= 0xbfff ;  // P40~P47 ,P50~P55 ,P57 as input
	
	// PB LCD test
	GPIO->PMB  |= (0xffffu<<16) ;  // P60~67 ,P70~P77 as input
	GPIO->PCB  &= (~(0xffffu<<16)) ;  // P60~67 ,P70~P77 as GPIO
	GPIO->PIEB &= (~(0xffffu<<16)) ;  // P60~67 ,P70~P77 input enable
	for( i=0;i<16;i++ )
	{
		GPIO->PMB  |= (0xffffu<<16) ;  // P60~67 ,P70~P77 as input
		
		GPIO->PMB &= (~(1<<(i+16))) ;  // PB LCD as output
		GPIO->PB  |= (1<<(i+16))   ;  // PB LCD = 1
		uDelay(100) ;
		if( (GPIO->PB & PB_LCD_H) != PB_LCD_H )
			return FAIL ;
			
		GPIO->PB &= (~(1<<(i+16))) ; // PB GPIO = 0
		uDelay(100) ;
		if( (GPIO->PB & PB_LCD_H) != 0 )
			return FAIL ;
			
	}
	GPIO->PMB  |= (0xffffu<<16) ;  // P60~67 ,P70~P77 as input
	
	// PC test
	GPIO->PMC  |= (0x3fff<<0) ;  // P80~P87 ,P90~P95 as input
	GPIO->PCB  &= (~(0x3fff<<0)) ;  // P80~P87 ,P90~P95 as GPIO
	GPIO->PIEC &= (~(0x3fff<<0)) ;  // P80~P87 ,P90~P95 input enable
	for( i=0;i<12;i++ )
	{
		GPIO->PMC |= (0x3fff<<0) ;  // P80~P87 ,P90~P95 input enable
		
		GPIO->PMC &= (~(1<<i)) ;  // P8x or P9x as output
		GPIO->PC  |= (1<<i) ;  // P8x or P9x = 1
		uDelay(100) ;
		if( (GPIO->PC & PC_LCD_H) != PC_LCD_H )
		  return FAIL ;
		  
		GPIO->PC &= (~(1<<i)) ;  // P8x or P9x = 0
		uDelay(100) ;
		if( (GPIO->PC & PC_LCD_H) != 0 )
			return FAIL ;
	}
	GPIO->PMC |= (0x3fff<<0) ;  // P80~P87 ,P90~P95 input enable
	
	return SUCCESS ;
	
}



