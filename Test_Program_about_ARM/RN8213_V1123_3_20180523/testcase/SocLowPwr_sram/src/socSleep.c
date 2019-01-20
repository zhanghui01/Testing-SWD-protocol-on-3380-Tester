#include "rn821x.h"

void socSleep(void)
    __attribute__ ((section (".ARM.__at_0x1a00")));
		
extern volatile uint32_t  * sram_chipid;
void socSleep(void)
{
	uint32_t i,codesize,ramsize,chipid;
	
	chipid = *sram_chipid;
	//1.清除低端SRAM
	for(i = 0; i < 0x1a00/4; i++)
	{
		*(volatile uint32_t *)(0x0 + 4*i) = 0;
	}
//   	*(volatile uint32_t *)(0x10000e08) = 0xffffefff;
	
	//2.保留100字节的验证代码段
	codesize = 160;
	if((chipid & 0xf0000000) == 0xf0000000)
	{
		ramsize = 0x8000;
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
		ramsize = 0x4000;
	}
	  WDT->EN = 0xbb;
	//3.清除高端SRAM（包含CACHE空间）
	for(i = 0; i < (ramsize-0x1a00-codesize)/4; i++)
	{
		*(volatile uint32_t *)(0x1a00+codesize+4*i)= 0;
	}
	
	while(1)
	{
	SYSCTL->ANA_ADC = 0x0; 
		__WFI();
	}
}
