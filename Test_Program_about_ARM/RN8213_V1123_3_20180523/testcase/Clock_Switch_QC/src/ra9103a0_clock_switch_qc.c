#include "rn821x.h"
#include "common.h"
#include "sysctrl.h"

#define  RES_OFFSET     0x37f8
#define  CHIPID_OFFSET  0x37f0

#define  TEST_START    0xa5a5a5a5
#define  TEST_FAIL     0x12345678
#define  TEST_PASS     0x98765432

void qc_fail(uint32_t *sram_res);
volatile uint32_t  * sram_chipid;

int32_t main(void)
{ 
  uint32_t  * sram_res;
  uint32_t  remap, mode, div;
   
// 	TRIG_INIT();
// 	TRIG_LOW();
// 	TRIG_HIGH();
	
	WdtInit();
	//uartInit(0, 9600, 0, 0, 0);
  MSG("SOC Clock Switch testing...\n");
  
  remap = (SYSCTL->MAP_CTRL & 0x3);
	sram_res = (uint32_t *)(((remap == 2) ? 0x0 : 0x10000000) + RES_OFFSET);
	*sram_res = TEST_START;
	sram_chipid = (uint32_t *)(((remap == 2) ? 0x0 : 0x10000000) + CHIPID_OFFSET);
	
	SYSCTL->OSC_CTRL2 = 3;  //29.4912Mhz OSC
	for(mode = 0; mode < 4; mode++)
	{
	  for(div = 0; div < 4; div++)
	  {
	    if(SysModeChg(mode, div))
	    {
	      qc_fail(sram_res);
	    }
	    if((mode == 0) || (mode == 1))
	    {
	      div = 3;
	    }
	  }
	}
	
	MSG("pass\n");
	*sram_res = TEST_PASS;
// 	TRIG_LOW();
  while(1)
  {
    WDT->EN = 0xbb;
  }
}

void qc_fail(uint32_t *sram_res)
{
  *sram_res = TEST_FAIL;
  MSG("test fail!\n");
  while(1)
  {
    ;
  }
}
