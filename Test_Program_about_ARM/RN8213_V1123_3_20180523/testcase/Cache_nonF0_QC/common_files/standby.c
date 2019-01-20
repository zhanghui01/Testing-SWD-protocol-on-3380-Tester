#include "standby.h"
#include "common.h"
#include "sysctrl.h"

#define  SWD_EN
#define  BOR18_OFF
#define  BOR5_OFF
//#define  RCL_OFF

static void cacheOnOff(uint32_t c)
{	
  FLASH->CC_CTRL = c ? 0xfeed0001 : 0xfeed0000;
}

static void gpioPwrDown(void)
{
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->MOD1_EN |= (1 << 5);
	
#ifdef SWD_EN
  GPIO->PCA0 = (1 << 29);   /* keep SWD */
#else
  GPIO->PCA0 = 0x0;
#endif
  GPIO->PCA1 = 0x0;       /* GPIO mode */  
  GPIO->PMA = 0xffffffff; /* PA input mode */
  GPIO->PUA = 0x00300000;
  GPIO->PIEA = 0xffffffff;/* PA input disable */
  
  GPIO->PCB = 0x0;
  GPIO->PMB = 0xffffffff;
  GPIO->PUB = 0x0;
  GPIO->PIEB = 0xffffffff;
  
  GPIO->PCC = 0x0;
  GPIO->PMC = 0xffffffff;
  GPIO->PUC = 0x0;
  GPIO->PIEC = 0xffffffff;
	
	SYSCTL->MOD1_EN &= (~(1 << 5));
}

static void busPwrDown(void)
{
	SYSCTL->SYS_PS = 0x82;
	
	SYSCTL->MOD1_EN = (1 << 9);  //WDT
	SYSCTL->KBI_EN = 0x0;
	SYSCTL->INTC_EN = 0x0;
}

void anaPwrDown(void)
{
#ifdef  BOR18_OFF
  SYSCTL->SYS_PS = 0x75;
  SYSCTL->ANA_SYS |= (0xa5370000 | (1 << 10));
#endif

#ifdef  BOR5_OFF
  SYSCTL->SYS_PS = 0x75;
  SYSCTL->ANA_SYS |= (0xa5370000 | (1 << 11));
#endif
  
#ifdef  RCL_OFF
  SYSCTL->SYS_PS = 0x75;
  SYSCTL->ANA_SYS |= (0xa5370000 | (1 << 14));
#endif
	
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->SYS_PD = 0xff;
}

void digOff(void)
{
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->SYS_PD |= (1 << 13);
	SYSCTL->SYS_PD |= (1 << 12);
}

void digLowVol(uint32_t vol)
{
	SYSCTL->SYS_PS = 0x75;
	SYSCTL->ANA_LDO = 0xa5580000 | ((SYSCTL->ANA_LDO & (~(0xf << 0))) | ((vol & 0xf) << 0));
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->SYS_PD |= (1 << 15);
	SYSCTL->SYS_PD |= (1 << 14);
}

void sysStandby(void)
{
	cacheOnOff(1);
	gpioPwrDown();
  busPwrDown();
	anaPwrDown();
	
// 	digOff();
	
	SysModeChg(0,0);
}
