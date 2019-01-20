#include "rn821x.h"
#include "common.h"
#include <stdio.h>  
#include <sysctrl.h>

// #define SWD_EN

#define  RES_OFFSET     0x1ffc
#define  CHIPID_OFFSET  0x1ff8

typedef enum 
{
  I1_CH = 1,
	I2_CH = 2,
	U_CH  = 4,
	ALL_CH = 7
}ADC_CH;

#define   APB_SPI   (1 << 15)
#define   APB_I2C   (1 << 14)
#define   APB_7816  (1 << 13)
#define   APB_IR38K (1 << 12)
#define   APB_UART3 (1 << 11)
#define   APB_UART2 (1 << 10)
#define   APB_UART1 (1 << 9)
#define   APB_UART0 (1 << 8)
#define   APB_UART5 (1 << 7)
#define   APB_UART4 (1 << 6)
#define   APB_TC1   (1 << 5)
#define   APB_TC0   (1 << 4)
#define   APB_CT    (1 << 3)
#define   APB_EE    (1 << 2)

#define   APB_SAR   (1 << 11)
#define   APB_RTC   (1 << 10)
#define   APB_WDT   (1 << 9)
#define   APB_NVM   (1 << 8)
#define   APB_EMU   (1 << 7)
#define   APB_LCD   (1 << 6)
#define   APB_GPIO  (1 << 5)
#define   APB_DMA   (1 << 4)
#define   APB_DMA1  (1 << 1)
#define   APB_DMA0  (1 << 0)

void eeStandby(void);
void eeWakeup(void);
void cacheOn(void);
void gpioPwrDown(void);
void All_Reg_WR0(void);
void busPwrDown(void);
void anaPwrDown(void);
void emuEnable(ADC_CH ch);
void LVDEnable(void);
void CMP2Enable(void);
void CMP1Enable(void);
//void lcdEnable(uint8_t bgr);
void lcdEnable(void);
void RCEnable(void);
void PLLEnable(uint8_t dll);
void HOSCEnable(uint32_t freq);
void LBGREnable(void);
void BGREnable(void);
void BGRDisable(void);
void socSleep(void);
void socIdle(void);
void eeChipErase(void);
void flPageErase(void);
void singleMeter(void);
void loscPwrDown(void);
void loscBiasmin(void);
void loscBiasmax(void);	
void flStandby(void);
void clrTrim(void);
void rtcOut(void);

volatile uint32_t  * sram_chipid;
int32_t main(void)
{ 	
  uint32_t  i;
	uint32_t  * sram_res;
  uint32_t  remap;
  SYSCTL->SYS_PS = 0x82;
 
  remap = (SYSCTL->MAP_CTRL & 0x3);
	sram_res = (uint32_t *)(((remap == 2) ? 0x0 : 0x10000000) + RES_OFFSET);
	*sram_res = 0xa5a5a5a5;
	sram_chipid = (uint32_t *)(((remap == 2) ? 0x0 : 0x10000000) + CHIPID_OFFSET);
	//*sram_chipid = 0xa0008213;
	
  WdtInit();             /* set wdt to 8s and enable irq to feed wdt */
	    
	mDelay(10);
	SYSCTL->SYS_PS = 0x75;
	if(SYSCTL->DIE_VER == 0xe0)
	{
		SYSCTL->SYS_PS = 0x82;
		SYSCTL->MOD1_EN |= (1 << 10);
		MADC->ANA_CTL = 0x10000;
		MADC->ANA_CTL = 0x10000; //rch & bgr off
		MADC->ANA_TEST = (0xa << 13) | (0x5 << 9) | (1 << 0);
		MADC->ANA_TEST = (0xa << 13) | (0x5 << 9) | (1 << 0);
	}
	SYSCTL->SYS_PS = 0x82;
  eeStandby();
  cacheOn();
	gpioPwrDown();
	All_Reg_WR0();
  busPwrDown();
	anaPwrDown();	

  SYSCTL->SYS_PS = 0x82;
  SYSCTL->MOD1_EN |= (1 << 10);
  RTC->CTRL = 0x100;
  RTC->CTRL = 0x100;
  RTC->CTRL = 0x100;
  for(i = 0; i < 100; i++)
  {
    ;
  }
  SYSCTL->MOD1_EN &= (~(1 << 10));
  
 	SysModeChg(0,0);   //0:losc,1:RC
// 	SYSCTL->MOD0_EN |= (1 << 2);	
// 	clrTrim();
//   eeStandby();	
	
	//*(volatile uint32_t *)(0x10800000) = 0xfeed0007;  //flash standby
	flStandby();
	SYSCTL->SYS_PD &= (~(1 << 5));//CMP2 pwr on
	socSleep();
}

void apbClk0En(uint32_t id)
{
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->MOD0_EN |= id;
}

void apbClk1En(uint32_t id)
{
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->MOD1_EN |= id;
}

void singleMeter(void)
{
	SYSCTL->SYS_PS = 0x82;
	/*
	SYSCTL->MOD0_EN = (1 << 13) | //iso7816
	                  (1 << 12) | //uart38k
	                  (1 << 8) | //uart0
	                  (1 << 9) | //uart1 
	                  (1 << 5) | //tc0
	                  (1 << 4) | //tc1
	                  (1 << 2);
	                  
	SYSCTL->MOD1_EN =  (1 << 11) | //sar
	                   (1 << 10) | //rtc
	                   (1 << 9) | //wdt
	                   (1 << 7) | //emu
	                   (1 << 6) | //lcd
	                   (1 << 5); //gpio
	*/
	apbClk0En(APB_7816);
	apbClk0En(APB_IR38K);
	apbClk0En(APB_UART0);
	apbClk0En(APB_UART1);
	apbClk0En(APB_TC0);
	apbClk0En(APB_TC1);
	apbClk0En(APB_EE);
	
	apbClk1En(APB_SAR);
	apbClk1En(APB_RTC);
	apbClk1En(APB_WDT);
	apbClk1En(APB_EMU);
	apbClk1En(APB_LCD);
	apbClk1En(APB_GPIO);
	
  SYSCTL->INTC_EN = 0x1ff;  //intc
	SYSCTL->KBI_EN = 0x1ff;   //kbi	
	
	eeWakeup();
}


void gpioPwrDown(void)
{
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->MOD1_EN |= (1 << 5);
	
#ifdef SWD_EN
  GPIO->PCA0 = (1 << 29);   /* keep SWD */
	GPIO->PUA = 0x00300000;
#else
  GPIO->PCA0 = 0x0;
	GPIO->PUA = 0x0;
#endif
  GPIO->PCA1 = 0x0;       /* GPIO mode */  
  GPIO->PMA = 0xffffffff; /* PA input mode */
  GPIO->PIEA = 0xffffffff;/* PA input disable */
  
  GPIO->PCB = 0x0;
  GPIO->PMB = 0xffffffff;
  GPIO->PIEB = 0xffffffff;
  
  GPIO->PCC = 0x0;
  GPIO->PMC = 0xffffffff;
  GPIO->PIEC = 0xffffffff;
	
	SYSCTL->MOD1_EN &= (~(1 << 5));
}

void All_Reg_WR0(void)
{
	uint32_t i;
	
	SYSCTL->MOD0_EN = 0xffff;
	SYSCTL->MOD1_EN = 0xffff;
	SYSCTL->OSC_CTRL1 = 0x1;
	SYSCTL->OSC_CTRL2 = 0x8;
	SYSCTL->INTC_EN = 0x1ff;
	SYSCTL->KBI_EN = 0x1ff;

	//INTC
	INTC->CTRL = 0x0;
	INTC->MODE = 0x0;
	INTC->MASK = 0x0;
	
	//TC
	TC0->PS   = 0x0;
	TC0->DN   = 0x0;
	TC0->CCD0 = 0x0;
	TC0->CCD1 = 0x0;
	TC0->CCFG = 0x0;
	TC0->CTRL = 0x0;
	TC0->CM0  = 0x0;
	TC0->CM1  = 0x0;
	TC0->IE   = 0x0;
	
	TC1->PS   = 0x0;
	TC1->DN   = 0x0;
	TC1->CCD0 = 0x0;
	TC1->CCD1 = 0x0;
	TC1->CCFG = 0x0;
	TC1->CTRL = 0x0;
	TC1->CM0  = 0x0;
	TC1->CM1  = 0x0;
	TC1->IE   = 0x0;
	
	//RTC
	RTC->CTRL = 0x0;
	mDelay(10);
  RTC->CTRL = 0x0;
	mDelay(10);
	RTC->CTRL = 0x0;
	mDelay(10);
	RTC->CNT1 = 0x0;
	RTC->CNT2 = 0x0;
	
	//SAR
	MADC->SAR_CTRL = 0x0;
	MADC->LVD_CTRL = 0x0;
	
	//LCD
	LCD->CTRL = 0x0;
	LCD->CLKDIV = 0x0;
	LCD->BLINK = 0x0;
	LCD->PS = 0x0;
	for(i=0;i<10;i++){
	  LCD->BUF[i] = 0x0;
	}
	
	//UART
	UART0->CTRL = 0x0;
	UART0->BAUD = 0x0;
	UART0->FDIV = 0x0;
	UART1->CTRL = 0x0;
	UART1->BAUD = 0x0;
	UART1->FDIV = 0x0;
	UART2->CTRL = 0x0;
	UART2->BAUD = 0x0;
	UART2->FDIV = 0x0;
	UART3->CTRL = 0x0;
	UART3->BAUD = 0x0;
	UART3->FDIV = 0x0;
	UART4->CTRL = 0x0;
	UART4->BAUD = 0x0;
	UART4->FDIV = 0x0;
	UART5->CTRL = 0x0;
	UART5->BAUD = 0x0;
	UART5->FDIV = 0x0;
	
	//ISO7816
	ISO7816->CTRL0 = 0x0;
	ISO7816->CTRL1 = 0x0;
	ISO7816->CLK = 0x0;
	ISO7816->BDDIV0 = 0x0;
	ISO7816->BDDIV1 = 0x0;
	
	//IIC
	I2C->CTRL = 0x0;
	I2C->CLK = 0x0;
	
	//SPI
	SPI->CTRL = 0x0;
	
	//KBI
	KBI->CTRL = 0x0;
	KBI->SEL = 0x0;
	KBI->MASK = 0x0;	
	
}

void eeStandby(void)
{	
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->MOD0_EN |= (1 << 2);
	
	SYSCTL->SYS_PS = 0x75;
	if(((SYSCTL->DIE_VER & 0xff) == 0xc0) ||
		 ((SYSCTL->DIE_VER & 0xff) == 0xe0))
	{
    *(uint32_t *)(0x08800000) = 0xfeed000f;
	}
	else
	{
		*(uint32_t *)(0x08800000) = 0xfeed0007;
		
		SYSCTL->SYS_PS = 0x82;
		SYSCTL->MOD0_EN &= (~(1 << 2));	
	}
	SYSCTL->SYS_PS = 0x82;
}

void eeWakeup(void)
{	
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->MOD0_EN |= (1 << 2);
	
  *(uint32_t *)(0x08800000) = 0xfeed0004;
	
	SYSCTL->MOD0_EN &= (~(1 << 2));
}

void cacheOn(void)
{	
    if((SYSCTL->MAP_CTRL & 0x3) == 0x3)
	{
	  *(uint32_t *)(0x1880001c) = 0xfeed0001;
    }
	else if((SYSCTL->MAP_CTRL & 0x3) == 0x2)
	{
	  *(uint32_t *)(0x1080001c) = 0xfeed0001;
	}
	else 
	{
	  *(uint32_t *)(0x0080001c) = 0xfeed0001;
	}	

}


void flStandby(void)
{	
	SYSCTL->SYS_PS = 0x82;
	
  if((SYSCTL->MAP_CTRL & 0x3) == 0x3)
	{
	  *(uint32_t *)(0x18800000) = 0xfeed0007;
    }
	else if((SYSCTL->MAP_CTRL & 0x3) == 0x2)
	{
	  *(uint32_t *)(0x10800000) = 0xfeed0007;
	}
	else 
	{
	  *(uint32_t *)(0x00800000) = 0xfeed0007;
	}	
}

void busPwrDown(void)
{

	SYSCTL->SYS_PS = 0x75;
	if((SYSCTL->DIE_VER & 0xff) == 0xc0)
	{	
		SYSCTL->SYS_PS = 0x82;
  	SYSCTL->MOD0_EN = 0x4;
	}
	else
	{	
		SYSCTL->SYS_PS = 0x82;
  	SYSCTL->MOD0_EN = 0x0;
	}
	SYSCTL->MOD1_EN = (1 << 9);		//wdton
	SYSCTL->KBI_EN = 0x0;
	SYSCTL->INTC_EN = 0x0;
}

void anaPwrDown(void)
{
	uint32_t  chip_ver;
	SYSCTL->SYS_PS = 0x75;
	chip_ver = SYSCTL->DIE_VER & 0xff;
	if(chip_ver == 0xc0)
	{	
		SYSCTL->ANA_SYS = 0XA5370000u | SYSCTL->ANA_SYS | (1 << 10) | (1 << 11) | (1 << 14);
		
		SYSCTL->SYS_PS = 0x82;
		SYSCTL->SYS_PD |= 0x3df;
	}
	else if(chip_ver == 0xe0)
	{
		SYSCTL->ANA_SYS = 0XA5370000u | SYSCTL->ANA_SYS | (1 << 10) | (1 << 11);
		
		SYSCTL->SYS_PS = 0x82;
		SYSCTL->SYS_PD |= 0x3df;
	}
	else
	{
		SYSCTL->SYS_PS = 0x82;
	  SYSCTL->SYS_PD |= 0xdf;
	}
}

void emuEnable(ADC_CH ch)
{
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->MOD1_EN |= (1 << 7);
	SYSCTL->SYS_PD &= (~(1 << 6));  //BGR ON
	//SYSCTL->SYS_PS = 0x75;
	//*(uint32_t *)(0x40034000 + 0x58) = 0xa5280000 + (0x820 << 4) + 1;
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->SYS_PD = (SYSCTL->SYS_PD | 0x7) & (~(ch)); //ADC ON
	//SYSCTL->SYS_PS = 0x75;
	//*(uint32_t *)(0x40034000 + 0x48) = 0xa5770000 + (1 << 6) + (1 << 4);
	
	SYSCTL->SYS_PD = (SYSCTL->SYS_PD | 0x7) & (~(6)); //
	
	SYSCTL->SYS_PD = (SYSCTL->SYS_PD | 0x7) & (~(ch)); //ADC ON
	SYSCTL->SYS_PD = (SYSCTL->SYS_PD | 0x7) & (~(5)); //
	
	SYSCTL->SYS_PD = (SYSCTL->SYS_PD | 0x7) & (~(ch)); //ADC ON
	SYSCTL->SYS_PD = (SYSCTL->SYS_PD | 0x7) & (~(3)); //
	
	SYSCTL->SYS_PD = (SYSCTL->SYS_PD | 0x7) & (~(0));
	SYSCTL->SYS_PD = SYSCTL->SYS_PD | (1 << 6); //BGR OFF
	
	SYSCTL->MOD1_EN &= (~(1 << 7));
	
	SYSCTL->SYS_PD = 0x3f;
	SYSCTL->SYS_PD = 0x7f;
}

/* bgr = 1: LDO18 as reference, bgr = 0: LBGR as reference */
//void lcdEnable(uint8_t bgr)
void lcdEnable(void)
{
	SYSCTL->SYS_PS = 0x82;
	
	//SYSCTL->SYS_PD &= (~(1 << 7));  //auto enable
	SYSCTL->MOD1_EN |= (1 << 6);
	
	//SYSCTL->SYS_PS = 0x75;
	//SYSCTL->ANA_PSW = 0xa5790000 | (bgr << 5);
	
	LCD->BLINK = 0x1;
	LCD->CTRL = (1 << 4) 
	          | (0x5 << 1)
	          | (1 << 0);
	
	SYSCTL->MOD1_EN &= (~(1 << 6));
}

void RCEnable(void)
{
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->OSC_CTRL1 &= (~(1 << 1));
}

/* dll = 1: disable dll, dll = 0: enable dll */
void PLLEnable(uint8_t dll)
{
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->OSC_CTRL1 &= (~(1 << 0));
	
	SYSCTL->SYS_PS = 0x75;
	//SYSCTL->ANA_CLK1 = 0xa5270400 | (dll << 9);  /* DLL = 2* PLL */
	SYSCTL->ANA_CLK1 = 0xa5270000 | (1 << 9);    /* DLL = PLL (bypass DLL) */
	SYSCTL->ANA_CLK1 = 0xa5270000 | (dll << 9);    /* DLL = PLL (bypass DLL) */
	SYSCTL->ANA_CLK1 = 0xa5270000;    /* DLL = PLL (bypass DLL) */
}

void HOSCEnable(uint32_t freq)
{
	SYSCTL->SYS_PS = 0x82;
	switch (freq)
	{
		case 7372800:
			SYSCTL->OSC_CTRL2 = (1 << 4) | (0 << 0);
		  break;
		case 14745600:
			SYSCTL->OSC_CTRL2 = (1 << 4) | (1 << 0);
		  break;
		case 29491200:
			SYSCTL->OSC_CTRL2 = (1 << 4) | (2 << 0);
		  break;
	}	
}

void LBGREnable(void)
{
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->SYS_PD &= (~(1 << 7));
}

void BGREnable(void)
{
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->SYS_PD &= (~(1 << 6));
}

void BGRDisable(void)
{
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->SYS_PD |= ((1 << 6));
}

void CMP2Enable(void)
{
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->SYS_PD &= (~(0x1 << 5));
}
void CMP1Enable(void)
{
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->SYS_PD &= (~(1 << 4));
}

void LVDEnable(void)
{
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->SYS_PD &= (~(1 << 3));
}

void socIdle(void)
{
	while(1)
	{
		WdtFeed();
	}
}

void eeChipErase(void)
{
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->MOD0_EN |= (1 << 2);
	
	*(uint32_t *)(0x08800014) = 0xfeed0000;
	*(uint32_t *)(0x08800020) = 1;
	*(uint32_t *)(0x08800024) = 860;
	*(uint32_t *)(0x0880002c) = 2;
	while(1)
	{
	  *(uint32_t *)(0x08800000) = 0xfeed0009;
	}
}

void flPageErase(void)
{
	*(uint32_t *)(0x00800014) = 0xfeed0000;
	*(uint32_t *)(0x00800004) = 768;
	*(uint32_t *)(0x00800020) = 1;
	*(uint32_t *)(0x00800024) = 855;
	*(uint32_t *)(0x0080002c) = 6;
	while(1)
	{
	  *(uint32_t *)(0x00800000) = 0xfeed000b;
	}
}
void loscPwrDown(void)		 //add losc off
{	

	SYSCTL->SYS_PS = 0x82;
	SYSCTL->OSC_CTRL2 |= (1 << 9);		   /*enable loscPwrDown*/
	SYSCTL->OSC_CTRL2 |= ((1 << 8) | (1 << 9));	/*loscPwrDown*/
}
void loscBiasmin(void)		 //
{	
	SYSCTL->SYS_PS = 0x75;
	SYSCTL->ANA_CLK2 = 0xa5360000;    /* Imin=SHmin*IBSmin */
																			
}
void loscBiasmax(void)		// 
{	
	SYSCTL->SYS_PS = 0x75;
	SYSCTL->ANA_CLK2 = 0xa5360000 | (0x7) | (4<<3);    /* Imax=SHmax*IBSmax */
																			
}

void rtcOut(void)
{
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->MOD1_EN |= (1 << 10);  //rtc
	while(RTC->CTRL & (1 << 9))
		WDT->EN = 0xbb;
	RTC->CTRL |= (1 << 8);
	while(RTC->CTRL & (1 << 9))
		WDT->EN = 0xbb;
	RTC->CTRL = 0x141;//0x159;
}


/* mode = 0: FLASH, mode = 1: EEPROM */
static uint32_t  _getMipAddr(uint8_t mode)
{
  uint32_t addr;
  switch (*(uint32_t *)(0x40034018) & 0x7)
  {
    case 0:
      addr =  (mode == 0) ? 0x00000000 : 0x08000000;
      break;
    case 1:
      addr = (mode == 0) ? 0x08000000 : 0x00000000;
      break;
    case 2:
      addr = (mode == 0) ? 0x10000000 : 0x08000000;
      break;
    case 3:
      addr = (mode == 0) ? 0x18000000 : 0x08000000;
      break;
  }
  return addr;
}

void clrTrim(void)
{
	volatile uint32_t   *fl_base, *ee_base;
  
	fl_base  = (uint32_t *)_getMipAddr(0);
	ee_base  = (uint32_t *)_getMipAddr(1);
	  
  *(volatile uint32_t *)((uint32_t)fl_base + 0x00800000u + 0xcu) = (*(volatile uint32_t *)((uint32_t)fl_base + 0x00800000u + 0xcu)) & (~(0xffffu << 8));
  *(volatile uint32_t *)((uint32_t)ee_base + 0x00800000u + 0xcu) = (*(volatile uint32_t *)((uint32_t)ee_base + 0x00800000u + 0xcu)) & (~(0xffffu << 8));
	
	*(volatile uint32_t *)((uint32_t)fl_base + 0x00800000u + 0x0u) = 0xfeed0002u;//POR+??+TBC????
	*(volatile uint32_t *)((uint32_t)ee_base + 0x00800000u + 0x0u) = 0xfeed0002u;
	
	*(volatile uint32_t *)((uint32_t)ee_base + 0x00800000u + 0x0u) = 0xfeed0007u;//低功耗模式2（待机模式）
}
