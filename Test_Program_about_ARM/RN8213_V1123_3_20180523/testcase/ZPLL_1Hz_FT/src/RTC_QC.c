/****************************************Copyright (c)****************************************************
**                            REnergy Micro-Electronics Co.,LTD.
**
**                                 http://www.renergy-me.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified:       2011-03-16
** Last Version:        V1.0
** Descriptions:        User Programn Entry
**                      
**--------------------------------------------------------------------------------------------------------
** Created by:          
** Created date:        2011-03-16
** Version:             V1.0
** Descriptions:        first creation
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
**
*********************************************************************************************************/

#include "rn821x.h"
// #include "system.h"
// #include "serial.h"
#include <stdio.h>
#include "sysctrl.h"
//#include "common.h"
#define  CHIPID_OFFSET  0x1ff8		//0x1ff8
#define  RES_OFFSET     0x37f8		//0x1ffc
#define  PLL1HZ_OFFSET  0x1fc4		//0x1fc4

uint32_t intc_flag = 0;
volatile uint32_t  * sram_chipid;
volatile uint32_t  * sram_res;
volatile uint32_t  * sram_pll1hz;
volatile uint32_t  pll1hz=0;

void testPass(void);
void testFail(uint32_t res);
void systemInit(uint8_t div);
void swDelay(uint32_t time);
/*********************************************************************************************************
** Function name:       hex2bcd
** Descriptions:        Hex To BCD
** input parameters:    Hex data (0-99)
** output parameters:   BCD data
** Returned value:      none
*********************************************************************************************************/
uint32_t hex2bcd (uint32_t data) {
  uint32_t result;
  
  if(data>=90)
  {
    result = (9<<4) | (data-90);
  }
  if((data>=80) && (data<90))
  {
    result = (8<<4) | (data-80);
  }
  if((data>=70) && (data<80))
  {
    result = (7<<4) | (data-70);
  }
  if((data>=60) && (data<70))
  {
    result = (6<<4) | (data-60);
  }
  if((data>=50) && (data<60))
  {
    result = (5<<4) | (data-50);
  }
  if((data>=40) && (data<50))
  {
    result = (4<<4) | (data-40);
  }
  if((data>=30) && (data<40))
  {
    result = (3<<4) | (data-30);
  }
  if((data>=20) && (data<30))
  {
    result = (2<<4) | (data-20);
  }
  if((data>=10) && (data<20))
  {
    result = (1<<4) | (data-10);
  }  
  if(data<10)
  {
    result = data;
  } 
  
  return result;         
}

/*********************************************************************************************************
** Function name:       RTC_IRQHandler
** Descriptions:        RTC Interrupt Manager
** input parameters:    none
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
void    RTC_HANDLER                             (void){ 
    
    if((RTC->IF & 0x8) != 0x8)
    {
        //printf("\tIF = 0x%08x.\n",RTC->IFLAG);
        ;//testFail(1);
    }
        
    if((RTC->IF & 0x8) == 0x8)            // 
    {
        intc_flag = 0x01;
    }     
       
    RTC->IF = 0xff;        //Clear Pending Bit  
    while(RTC->CTRL & 0x200);
} 

void systemInit(uint8_t div)
{
	*sram_res = 0xa5a5a5a5;
	
	SYSCTL->SYS_PS = 0x82;
  //Enable WDT Clock
  SYSCTL->MOD1_EN = 0xa5800000 | (SYSCTL->MOD1_EN | 0x200);  
  
  //Close WDT
  WDT->PASS = 0x70;
  WDT->PASS = 0x7e;
  WDT->CTRL  = 0xee;
  while(WDT->EN&0x100);
  WDT->PASS = 0x00;

	__enable_irq();
  NVIC_EnableIRQ(WDT_IRQn);
	
	//swDelay(32*2000);	
// 	SYSCTL->OSC_CTRL1 &= (~(1<<0));
// 	SYSCTL->OSC_CTRL1 &= (~(3<<2));
// 	SYSCTL->OSC_CTRL2 =0x13;
// 	swDelay(8000);	
// 	SYSCTL->SYS_MODE = 0xd;
  SysModeChg(2,1);  
}
void testPass(void){
  //printf("Testing PASS\n");
  *sram_res = 0x98765432;
	* sram_pll1hz = pll1hz;
  while(1)
		WDT->EN = 0xbb;
}

void testFail(uint32_t res){
 // *sram_res = 0x1234567a;
	* sram_pll1hz = pll1hz;
  while(1)
		WDT->EN = 0xbb;
}

/*********************************************************************************************************
** Function name:       acqCorefre
** Descriptions:        Acquire Core Frequecy
** input parameters:    None　　
** output parameters:   result
** Returned value:      None
*********************************************************************************************************/
uint32_t acqCorefre(void)
{
  uint32_t result;
  uint8_t sysclk_state;
  
  sysclk_state = (SYSCTL->OSC_CTRL1 & 0x700) >> 8;
  
  if(sysclk_state == 0x0)
  {
  	result = 7372800;
  }
  if(sysclk_state == 0x1)
  {
  	result = 3686400;
  }
  if(sysclk_state == 0x2)
  {
  	result = 1843200;
  }
  if(sysclk_state == 0x3)
  {
  	result = 32768;
  }      
  if(sysclk_state == 0x4)
  {
  	result = 14745600;
  }
  if(sysclk_state == 0x5)
  {
  	result = 29491200;
  }
  return result;
}

/*********************************************************************************************************
** Function name:       swDelay for 30.5 us
** Descriptions:        Software Delay
** input parameters:    None
** output parameters:   None
** Returned value:      None
*********************************************************************************************************/

void swDelay(uint32_t time)
{
  uint32_t i;
  uint32_t cur_freq;
  
  cur_freq = acqCorefre();
  
  if(time == 0)
  {
  	return;
  }
  
  //SysTick Start Here
  SysTick->LOAD = 0xffffff; 
  SysTick->VAL  = 0x0;
  SysTick->CTRL = 0x5;

  //while((0xffffff - SysTick->VAL) < time * (core_freq / 1024))
  //;
  
  for(i=0;i<(time * (cur_freq / 32768));i++)
  {
    if((0xffffff - SysTick->VAL) >= time * (cur_freq / 32768))
    {
      SysTick->CTRL = 0x0;
      return;
    }   
  }
  
  SysTick->CTRL = 0x0;
  return;
  
//  while(time--)
//  {
//    for(i=0;i<0x390;i++) //912*4*274=1ms
//    ;
//  }
}
/*===========================================================
利用TC内部捕获RTC PLL1HZ
===========================================================*/
void Pll1hz_out(void)
{
	uint32_t mode, loop;
	uint32_t fout_cnt[5];
	uint32_t cur_freq;
  //WdtInit();
	WDT->PASS = 0x70;
	WDT->PASS = 0x7e;
	WDT->CTRL = 0x6e;	
  
	cur_freq = acqCorefre();
	//uartInit(0, 115200, 0, 0, 0);
	
	SYSCTL->SYS_PS = 0x82;
  SYSCTL->MOD1_EN = 0xa5800000 | (SYSCTL->MOD1_EN | 0x400);    
	SYSCTL->MOD0_EN |=(1<<4);  //TC0 EN
	
	RTC->CTRL |= (1 << 8);
	swDelay(5);

		WDT->EN = 0xbb;
		mode = 1;
		RTC->CTRL = 1 << 8;
		swDelay(5);
		RTC->CTRL = (mode << 0);
		swDelay(100);
		TC0->CTRL = 0;
		TC0->STA = TC0->STA;
		TC0->DN  = 0xffffffff;
		TC0->CM0 = (0x13 << 20) | (1 << 0);   //rtcfout,pos edge
		TC0->CM1 = (0x13 << 20) | (1 << 9) | (1 << 0);   //rtcfout,neg edge
		swDelay(2000);
		TC0->CTRL = 1;
		for(loop = 0; loop < 2; loop++)
		{
			while(!(TC0->STA & (1 << 1)))
			{
				if( TC0->CNT > (cur_freq*2.4))  //2s timeout
				{
			    pll1hz = TC0->CNT;
					pll1hz = 0;
					*sram_res = 0x1234567a;
          testFail(1);
        }
				WDT->EN = 0xbb;
			}
			TC0->STA = TC0->STA;
			fout_cnt[loop] = TC0->CCD0;
		}
		pll1hz = (fout_cnt[1] - fout_cnt[0]);
		if(((((float)pll1hz) / ((float)cur_freq)) <0.999) || ((((float)pll1hz) / ((float)cur_freq))>1.001))
		{
				*sram_res = 0x1234567b;
      testFail(1);
    }
	
}
/*********************************************************************************************************
** Function name:       main
** Descriptions:        User Programn Entry Example
** input parameters:    none
** output parameters:   none
** Returned value:      none/never return
*********************************************************************************************************/
int main (void)
{
  uint32_t i;
  uint32_t temp;
  intc_flag = 0;  
	
	systemInit(0);

  //Enable RTC Clock
	SYSCTL->SYS_PS = 0x82;
  SYSCTL->MOD1_EN = 0xa5800000 | (SYSCTL->MOD1_EN | 0x400);    
	sram_chipid = (uint32_t *)((((SYSCTL->MAP_CTRL & 0x3) == 2) ? 0x0 : 0x10000000) + CHIPID_OFFSET);
	sram_pll1hz = (uint32_t *)((((SYSCTL->MAP_CTRL & 0x3) == 2) ? 0x0 : 0x10000000) + PLL1HZ_OFFSET); 
  sram_res = (uint32_t *)((((SYSCTL->MAP_CTRL & 0x3) == 2) ? 0x0 : 0x10000000) + RES_OFFSET);
		*sram_res = 0xa5a5a5a5;
  //Enable RTC INT
  __enable_irq();
  NVIC_EnableIRQ(RTC_IRQn); 
  Pll1hz_out();                                   
  testPass();
}

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
