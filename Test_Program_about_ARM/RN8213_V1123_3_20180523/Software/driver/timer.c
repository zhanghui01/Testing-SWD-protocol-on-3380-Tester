#define _TIMER_GLOBALS

#include "timer.h"

void TIMER3_IRQHandler(void)    //系统主节拍.1ms
{
  LPC_TIM3->IR = 0x1;        //Clear MRO Interrupt
  sys_stamp++;                //1MS
}

void timer3Init(void)
{
  LPC_SC->PCONP    |= (1 << 23);                                       //Enable Time3 Power
  LPC_SC->PCLKSEL1 &= (~(3 << 14));                                   /* clk = cclk / 4 (25Mhz)*/
  
  LPC_TIM3->CTCR   = 0x0;                                             //Time Mode
  LPC_TIM3->PR     = 0x0;                                             //PreScale Counter Register.
  LPC_TIM3->MR0    = SystemCoreClock / 4 / 1000;                      //Match Register,1ms  
  LPC_TIM3->MCR    = 0x3;                                             //Interrupt and Reset on MR0
  
  LPC_TIM3->TCR    = 0x1;                                             //Counter Enable
  
  sys_stamp = 0x0;
  
  //Enable T3 INT
  NVIC_EnableIRQ(TIMER3_IRQn);  
}

//函数功能：计算时戳(每个定时器节拍，SysStamp累加一次)
//入口参数：Ago需要比较的节拍
//出口参数：返回SysStamp与Ago间的节拍数
uint16_t stampThrough(uint16_t ago)
{
  if (sys_stamp >= ago)
    {return (sys_stamp - ago);}
  else
    {return ( (0xffff - ago) + sys_stamp);}
}

