#include "rn821x.h"
#include "common.h"
#include "sysctrl.h"
#include "flash_driver.h"
#include "stdio.h"

#define  RES_OFFSET       0x37f8
#define  CHIPID_OFFSET    0x37f0

#define  TEST_START    0xa5a5a5a5
#define  TEST_FAIL     0x1234ffff
#define  TEST_PASS     0x98765432

// #ifdef PAGE_SIZE
// #undef PAGE_SIZE
// #define  PAGE_SIZE     128
// #endif

#define  CHIP_SIZE     (192 * 1024)
#define  CHIP_SIZE_C   (128 * 1024)
void qc_fail(uint32_t *sram_res);
uint32_t fl_chip_check_empty( void ) ;
uint32_t fl_chip_check_data( uint8_t *data ) ;
uint32_t fl_page_check_empty( uint32_t pg )  ;
uint32_t fl_page_check_data( uint32_t pg, uint8_t *data ) ;

volatile uint32_t  * sram_chipid;
uint32_t  fl_page_size_g, fl_chip_size_g;
int32_t main(void)
{ 
  uint32_t  * sram_res;
  uint32_t  remap, i,j,k;
	uint32_t  buf[128/4];
	uint32_t  flash_base_addr,time=0;
   
	*(volatile uint32_t *) (0x10800000) = 0xfeed0003; 
	
	WdtInit();
	
	SYSCTL->SYS_PS =0x82;
	SYSCTL->OSC_CTRL2 = 0x13;
	SysModeChg(3, 0);
	//uartInit(0, 115200, 0, 0, 0);
  MSG("SOC CM0 Peripheral testing...\n");
  
	//tcInit(0xffffffff);
  remap = (SYSCTL->MAP_CTRL & 0x3);
	sram_res = (uint32_t *)(((remap == 2) ? 0x0 : 0x10000000) + RES_OFFSET);
	*sram_res = TEST_START;
	sram_chipid = (uint32_t *)(((remap == 2) ? 0x0 : 0x10000000) + CHIPID_OFFSET);
	flash_base_addr = (remap == 0x0) ? 0x0 : (remap == 0x1) ? 0x08000000 : (remap == 0x2) ? 0x10000000 : 0x18000000;
	
	SYSCTL->SYS_PS = 0x75;
	if(SYSCTL->DIE_VER == 0xf0)
	{
		fl_page_size_g = 256;
		fl_chip_size_g = 512*1024;
	}
	else if((SYSCTL->DIE_VER == 0xc0) || (SYSCTL->DIE_VER == 0xe0))
	{
		fl_page_size_g = 128;
		fl_chip_size_g = 128*1024;
	}
	else
	{
		fl_page_size_g = 128;
		fl_chip_size_g = 192*1024;
	}
	
	SYSCTL->SYS_PS = 0x82;
	
  /* 1. FLASH CHIP ERASE */
  fl_chip_erase();
  if(fl_chip_check_empty())
  {
    MSG("flash chip erase fail!!!\n");
    qc_fail(sram_res);
  }
	
#if 0
	for(i = 0; i < fl_page_size_g; i++)
  {
    ((uint8_t *)buf)[i] = i; 
  }    
  
  /* 2. FLASH CHIP PROG */
  fl_chip_prog(((uint8_t *)buf));
  if(fl_chip_check_data(((uint8_t *)buf)))
  {
    MSG("flash chip program fail!!!\n");
		qc_fail(sram_res);
  }
#else	
	/* 2. FLASH PAGE PROG */
	for(i = 0x0; i < fl_chip_size_g; i*=2)
	{
		for(j = 0; j < fl_page_size_g/4; j++)
		{
			k = (i/fl_page_size_g)+j;			
			buf[j] = (k&0xff) | (((~k) & 0xff) << 8) | (((k+0x55) & 0xff) << 16) | (((k+0xaa) & 0xff) << 24);
		}
		fl_page_prog(i/fl_page_size_g, (uint8_t *)buf);
		WDT->EN = 0xbb;
		if(i==0)
			i=fl_page_size_g/2;
	}

	for(i = 0x0; i < fl_chip_size_g; i*=2)
	{
		for(j = 0; j < fl_page_size_g/4; j++)
		{
			k = (i/fl_page_size_g)+j;			
			buf[j] = (k&0xff) | (((~k) & 0xff) << 8) | (((k+0x55) & 0xff) << 16) | (((k+0xaa) & 0xff) << 24);
			if(*(volatile uint32_t *)(flash_base_addr + 4*j + i) != buf[j])
			{
        MSG("flash page program fail!!!\n");
		    qc_fail(sram_res);
			}
		}		
		if(i==0)
			i=fl_page_size_g/2;
	}
	
	/* 3. FLASH CHIP PROG */
  fl_chip_prog((uint8_t *)buf);
  if(fl_chip_check_data((uint8_t *)buf))
  {
    MSG("flash chip program fail!!!\n");
		qc_fail(sram_res);
  }
#endif
	/* 4. FLASH CHIP ERASE */
 	fl_chip_erase();
  
  time = TC0->CNT;
	*sram_res =time;
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->SYS_RST = SYSCTL->SYS_RST;
	MSG("\n<< CNT %d(%x) >>\n", time, SYSCTL->SYS_RST);  
	
	*sram_res = TEST_PASS;
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
    WDT->EN = 0xbb;
  }
}

uint32_t fl_chip_check_empty(void)
{
  uint32_t i;
	uint32_t fl_addr;
	
	fl_addr = SYSCTL->MAP_CTRL & 0x3;
	fl_addr = (fl_addr == 0x0) ? 0x0 : (fl_addr == 0x1) ? 0x08000000 : (fl_addr == 0x2) ? 0x10000000 : 0x18000000;
	
	if(((*sram_chipid & 0xf0000000) == 0xc0000000) || ((*sram_chipid & 0xf0000000) == 0xe0000000))
	{
		for(i = 0; i < CHIP_SIZE_C / 4; i++)
    {
		  WdtFeed();
      if(*(uint32_t *)(fl_addr + 4 * i) != 0x0)
      {
        MSG("chip check empty fail: Byte[%i] = %d\n", i, *(uint32_t *)(fl_addr + 4 * i));
        return 1;
      }
    }
	}
	else
	{
		for(i = 0; i < CHIP_SIZE / 4; i++)
    {
		  WdtFeed();
      if(*(uint32_t *)(fl_addr + 4 * i) != 0x0)
      {
        MSG("chip check empty fail: Byte[%i] = %d\n", i, *(uint32_t *)(fl_addr + 4 * i));
        return 1;
      }
    }
	}
	
  for(i = 0; i < CHIP_SIZE / 4; i++)
  {
		WdtFeed();
    if(*(uint32_t *)(fl_addr + 4 * i) != 0x0)
    {
      MSG("chip check empty fail: Byte[%i] = %d\n", i, *(uint32_t *)(fl_addr + 4 * i));
      return 1;
    }
  }
  
  return 0;
}

uint32_t fl_chip_check_data(uint8_t *data)
{
  uint32_t i, j ;
	uint8_t  flag ;
	uint32_t fl_addr;
	
	fl_addr = SYSCTL->MAP_CTRL & 0x3;
	fl_addr = (fl_addr == 0x0) ? 0x0 : (fl_addr == 0x1) ? 0x08000000 : (fl_addr == 0x2) ? 0x10000000 : 0x18000000;
	flag = 0 ;
  
  for(i = 0; i < (CHIP_SIZE / fl_page_size_g); i++)
  {
    WdtFeed();
    
    for(j = 0; j < fl_page_size_g; j++)
    {
      if(*(uint8_t *)(fl_addr + i * fl_page_size_g + j) != data[j])
      {
        MSG("chip check data fail: Adrr[0x%x] = 0x%x, shoude be 0x%x\n", (fl_addr + i * fl_page_size_g + j), *(uint8_t *)(fl_addr + i * fl_page_size_g + j ), data[j]);
				flag = 1 ;
				return flag ;
      }
    }
  }
  
  return flag;
}

uint32_t fl_page_check_empty(uint32_t pg)
{
  uint32_t i;
	uint32_t fl_addr;
	
	fl_addr = SYSCTL->MAP_CTRL & 0x3;
	fl_addr = (fl_addr == 0x0) ? 0x0 : (fl_addr == 0x1) ? 0x08000000 : (fl_addr == 0x2) ? 0x10000000 : 0x18000000;
	
  for(i = 0; i < fl_page_size_g; i++)
  {
    if(*(uint8_t *)(fl_addr + pg * fl_page_size_g + i) != 0x0)
    {
      MSG("page[%d] check empty fail: Byte[%i] = %d\n", pg, i, *(uint8_t *)(fl_addr + pg * fl_page_size_g + i));
      return 1;
    }
  }
  
  WdtFeed();
  
  return 0;
}

uint32_t fl_page_check_data(uint32_t pg, uint8_t *data)
{
  uint32_t i;
	uint32_t fl_addr;
	
	fl_addr = SYSCTL->MAP_CTRL & 0x3;
	fl_addr = (fl_addr == 0x0) ? 0x0 : (fl_addr == 0x1) ? 0x08000000 : (fl_addr == 0x2) ? 0x10000000 : 0x18000000;
  
  for(i = 0; i < fl_page_size_g; i++)
  {
    if(*(uint8_t *)(fl_addr + pg * fl_page_size_g + i) != data[i])
    {
      MSG("page[%d] check data fail: Byte[%i] = %d, shoude be %d\n", pg, i, *(uint32_t *)(fl_addr + pg * fl_page_size_g + i), data[i]);
      return 1;
    }
  }
  
  WdtFeed();
  
  return 0;
}
