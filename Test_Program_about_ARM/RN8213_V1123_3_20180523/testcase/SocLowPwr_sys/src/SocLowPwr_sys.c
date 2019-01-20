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
void cacheOn(uint32_t die_ver);
void gpioPwrDown(void);
void All_Reg_WR1(void);
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

void rtcOut(void);

volatile uint32_t  * sram_chipid;
int32_t main(void)
{ 	
  uint32_t  i;
	uint32_t  * sram_res;
  uint32_t  remap;
	uint32_t  die_ver;
	
  SYSCTL->SYS_PS = 0x82;
 
  remap = (SYSCTL->MAP_CTRL & 0x3);
	sram_res = (uint32_t *)(((remap == 2) ? 0x0 : 0x10000000) + RES_OFFSET);
	*sram_res = 0xa5a5a5a5;
	sram_chipid = (uint32_t *)(((remap == 2) ? 0x0 : 0x10000000) + CHIPID_OFFSET);
	
  WdtInit();             /* set wdt to 8s and enable irq to feed wdt */
	mDelay(100);   
	SYSCTL->SYS_PS = 0x75;
	die_ver = SYSCTL->DIE_VER;
	if((die_ver == 0xe0) || (die_ver == 0xf0))
	{
		SYSCTL->SYS_PS = 0x82;
		SYSCTL->MOD1_EN |= (1 << 10);
		MADC->ANA_CTL = 0x10000;
		MADC->ANA_CTL = 0x10000; //rch & bgr off
		MADC->ANA_TEST = (0xa << 13) | (0x5 << 9) | (1 << 0);
		MADC->ANA_TEST = (0xa << 13) | (0x5 << 9) | (1 << 0);
	}
	SYSCTL->SYS_PS = 0x82;
	if(die_ver != 0xf0)
	{
    eeStandby();
	}
	if(die_ver == 0xf0)
	{
    cacheOn(die_ver);//cache spl load fail
	}
	gpioPwrDown();
	All_Reg_WR1();
  busPwrDown();
	anaPwrDown();	

  SYSCTL->SYS_PS = 0x82;
  SYSCTL->MOD1_EN |= (1 << 10);
//   RTC->CTRL = 0x100;
//   RTC->CTRL = 0x100;
//   RTC->CTRL = 0x100;
  for(i = 0; i < 100; i++)
  {
    ;
  }
  SYSCTL->MOD1_EN &= (~(1 << 10));
  
 	SysModeChg(0,0);   //0:losc,1:RC
  if(die_ver != 0xf0)
	{
    eeStandby();
	}
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

void All_Reg_WR1(void)
{
	uint32_t i;
	
	SYSCTL->MOD0_EN = 0xffff;
	SYSCTL->MOD1_EN = 0xffff;
	SYSCTL->OSC_CTRL1 |= 0xf;
	SYSCTL->OSC_CTRL2 |= 0x1f;
	SYSCTL->INTC_EN = 0x1ff;
	SYSCTL->KBI_EN = 0x1ff;

	//INTC
	INTC->CTRL = 0xff;
	INTC->MODE = 0xffff;
	INTC->MASK = 0xff;
	
	//TC
	TC0->PS   = 0xffff;
	TC0->DN   = 0xffffffff;
	TC0->CCD0 = 0xffffffff;
	TC0->CCD1 = 0xffffffff;
	TC0->CCFG = 0xffffffff;
	TC0->CTRL = 0xffffffff;
	TC0->CM0  = 0xffffffff;
	TC0->CM1  = 0xffffffff;
	TC0->IE   = 0xf;
	
	TC1->PS   = 0xffff;
	TC1->DN   = 0xffffffff;
	TC1->CCD0 = 0xffffffff;
	TC1->CCD1 = 0xffffffff;
	TC1->CCFG = 0xffffffff;
	TC1->CTRL = 0xffffffff;
	TC1->CM0  = 0xffffffff;
	TC1->CM1  = 0xffffffff;
	TC1->IE   = 0xf;
	
	//RTC
	RTC->CTRL = (1<<8) | (3<<6) | (7<<3);
	mDelay(10);
  RTC->CTRL = (1<<8) | (3<<6) | (7<<3);
	mDelay(10);
	RTC->CTRL = (1<<8) | (3<<6) | (7<<3);
	mDelay(10);
	RTC->CNT1 = 0xff;
	RTC->CNT2 = 0xff;
	
	//SAR
	MADC->SAR_CTRL = 0xffffffff;
	MADC->LVD_CTRL = 0xf;
	
	//LCD
	LCD->CTRL = 0xffff;
	LCD->CLKDIV = 0xff;
	LCD->BLINK = 0xffffffff;
	LCD->PS = 0xfff;
	for(i=0;i<10;i++){
	  LCD->BUF[i] = 0xfffffff;
	}
	
	//UART
	UART0->CTRL = 0xffffffff;
	UART0->BAUD = 0xfff;
	UART0->FDIV = 0x3fff;
	UART1->CTRL = 0xffffffff;
	UART1->BAUD = 0xfff;
	UART1->FDIV = 0x3fff;
	UART2->CTRL = 0xffffffff;
	UART2->BAUD = 0xfff;
	UART2->FDIV = 0x3fff;
	UART3->CTRL = 0xffffffff;
	UART3->BAUD = 0xfff;
	UART3->FDIV = 0x3fff;
	UART4->CTRL = 0xffffffff;
	UART4->BAUD = 0xfff;
	UART4->FDIV = 0x3fff;
	UART5->CTRL = 0xffffffff;
	UART5->BAUD = 0xfff;
	UART5->FDIV = 0x3fff;
	
	//ISO7816
	ISO7816->CTRL0 = 0xffffffff;
	ISO7816->CTRL1 = 0xffffffff;
	ISO7816->CLK = 0xf;
	ISO7816->BDDIV0 = 0xffffffff;
	ISO7816->BDDIV1 = 0xffffffff;
	
	//IIC
	I2C->CTRL = 0x3f;
	I2C->CLK = 0x7;
	
	//SPI
	SPI->CTRL = 0xffffffff;
	
	//KBI
	KBI->CTRL = 0xff;
	KBI->SEL = 0xff;
	KBI->MASK = 0xff;	
	
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
		SYSCTL->SYS_PS = 0x82;
		*(uint32_t *)(0x08800000) = 0xfeed0007;
		
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

void cacheOn(uint32_t die_ver)
{	
	SYSCTL->SYS_PS = 0x82;
	if(die_ver == 0xf0)
	{
		*(volatile uint32_t *)(0x50014060) = 0xfeed0001;
	}
	else
	{
			if((SYSCTL->MAP_CTRL & 0x3) == 0x3)
			{
				*(uint32_t *)(0x1880001c) = 0xfeed0001;
				}
			else
			{
				*(uint32_t *)(0x0080001c) = 0xfeed0001;
		}
	}

}


void flStandby(void)
{	
	SYSCTL->SYS_PS = 0x82;
	
  if((SYSCTL->MAP_CTRL & 0x3) == 0x3)
	{
	  *(uint32_t *)(0x18800000) = 0xfeed0007;
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
		//SYSCTL->ANA_SYS = 0XA5370000u | SYSCTL->ANA_SYS | (1 << 10) | (1 << 11) | (1 << 14);//��SPL load
		
		SYSCTL->SYS_PS = 0x82;
		SYSCTL->SYS_PD |= 0x3ff;
	}
	else if((chip_ver == 0xe0) || (chip_ver == 0xf0))
	{
		//SYSCTL->ANA_SYS = 0XA5370000u | SYSCTL->ANA_SYS | (1 << 10) | (1 << 11);//��SPL load
		
		SYSCTL->SYS_PS = 0x82;
		SYSCTL->SYS_PD |= 0x3ff;
	}
	else
	{
		SYSCTL->SYS_PS = 0x82;
	  SYSCTL->SYS_PD |= 0xff;
	}
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->SYS_PD &= (~(1 << 5));
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

void socSleep(void)
{
		uint32_t i,codesize,ramsize,chipid;
	
	chipid = *sram_chipid;
	if((chipid & 0xf0000000) == 0xf0000000)
	{
		ramsize = 0x3800;
	}
	else if((chipid & 0xf0000000) == 0xc0000000)
	{
		ramsize = 0x2800;
	}
	else if((chipid & 0xf0000000) == 0xe0000000)
	{
		ramsize = 0x2800;
	}	
	else
	{
		ramsize = 0x3800;
	}
		WDT->EN = 0xbb;
	//1.����Ͷ�SRAM
	for(i = 0; i < 0xe00; i++)
	{
		*(volatile uint32_t *)(0x10000000 + 4*i) = 0xffffffff;
	}
//   	*(volatile uint32_t *)(0x10000e08) = 0xffffefff;
	
// 	//2.����200�ֽڵ���֤�����
// 	codesize = 0;
// 	WDT->EN = 0xbb;
// 	//3.����߶�SRAM������CACHE�ռ䣩
// 	for(i = 0; i < (ramsize-0x1a00-codesize)/4; i++)
// 	{
// 		*(volatile uint32_t *)(0x1a00+codesize+4*i)= 0xffffffff;
// 	}
	
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->SYS_MODE = 0xf;	
	while(!(SYSCTL->SYS_MODE & (1<<0)))
	  ;
	while(!(SYSCTL->OSC_CTRL1 & (1<<4)))
		;
	SYSCTL->OSC_CTRL1 |= (1<<1);
	while(1)
	{
	  SYSCTL->ANA_ADC = 0;
		__WFI();
	}
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
