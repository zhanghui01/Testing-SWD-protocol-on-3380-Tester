#include "standby.h"
#include "common.h"

#define  SWD_EN
#define  BOR18_OFF
static void eeStandby(void)
{	
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->MOD0_EN |= (1 << 2);
	
	if((SYSCTL->MAP_CTRL & 0x7) == 1)
	{
    *(uint32_t *)(0x00800000) = 0xfeed0007;
  }
  else
	{
    *(uint32_t *)(0x08800000) = 0xfeed0007;
  }
  
	SYSCTL->MOD0_EN &= (~(1 << 2));
}

static void cacheOn(void)
{	
  uint32_t map;
  
  map = SYSCTL->MAP_CTRL & 0x7;
  
  switch (map)
  {
    case 0:
      *(uint32_t *)(0x0080001c) = 0xfeed0001;
      break;
    case 1:
      *(uint32_t *)(0x0880001c) = 0xfeed0001;
      break;
    case 2:
      *(uint32_t *)(0x1080001c) = 0xfeed0001;
      break;
    case 3:
      *(uint32_t *)(0x1880001c) = 0xfeed0001;
      break;
  }  
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
	
	SYSCTL->MOD0_EN = 0x0;
	SYSCTL->MOD1_EN = (1 << 9);
	SYSCTL->KBI_EN = 0x0;
	SYSCTL->INTC_EN = 0x0;
}

void anaPwrDown(void)
{
#ifdef  BOR18_OFF
  SYSCTL->SYS_PS = 0x75;
  SYSCTL->ANA_PSW = 0xa5790000 | (1 << 8) | (1 << 4);
#endif
  
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->SYS_PD = 0xff;
}

void sysStandby(void)
{
  eeStandby();
	cacheOn();
	gpioPwrDown();
  busPwrDown();
	anaPwrDown();
	
	SysModeChg(0,0);
}
