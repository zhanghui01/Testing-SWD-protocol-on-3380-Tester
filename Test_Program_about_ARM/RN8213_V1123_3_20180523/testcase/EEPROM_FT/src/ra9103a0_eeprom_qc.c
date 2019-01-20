#include "rn821x.h"
#include "common.h"
#include "sysctrl.h"
#include "eeprom_driver.h"
#include "stdio.h"

#define  RES_OFFSET     0x37f8
#define  CHIPID_OFFSET  0x37f0

#define  TEST_START    0xa5a5a5a5
#define  TEST_FAIL     0x1234eeee
#define  TEST_PASS     0x98765432

#ifdef PAGE_SIZE
#undef PAGE_SIZE
#undef PAGE_SIZE_C
#define  PAGE_SIZE     64
#define  PAGE_SIZE_C   128
#endif

#define  CHIP_SIZE     (32 * 1024)

void qc_fail(uint32_t *sram_res);
uint32_t ee_chip_check_empty( void ) ;
uint32_t ee_chip_check_data( uint8_t *data ) ;
uint32_t ee_page_check_empty( uint32_t pg )  ;
uint32_t ee_page_check_data( uint32_t pg, uint8_t *data ) ;

volatile uint32_t  * sram_chipid;

int32_t main(void)
{ 
  uint32_t  * sram_res;
  uint32_t  remap, i,j,k;
	uint32_t  ee_page_size;
	uint32_t  buf[128/4];
   
	//初始化EEPROM,POR TBC参数
	*(volatile uint32_t *) (0x08800000) = 0xfeed0003; 
	
	SYSCTL->SYS_PS = 0x82;
	WdtInit();
	
	SYSCTL->OSC_CTRL2 = 0x13;
	SysModeChg(3,0);
	//*(volatile uint32_t *)(0x08800018) = 0x10000;
	//uartInit(0, 9600, 0, 0, 0);
  //MSG("SOC CM0 Peripheral testing...\n");
  //tcInit(0xffffffff);
  remap = (SYSCTL->MAP_CTRL & 0x3);
	sram_res = (uint32_t *)(((remap == 2) ? 0x0 : 0x10000000) + RES_OFFSET);
	*sram_res = TEST_START;
	sram_chipid = (uint32_t *)(((remap == 2) ? 0x0 : 0x10000000) + CHIPID_OFFSET);
	
	switch((*sram_chipid & 0xf0000000))
	{
		case 0xc0000000:
		case 0xe0000000:
			ee_page_size = 128;
		  *(volatile uint32_t *)(0x08800000) = 0xfeed0003;
			break;
		default:
			ee_page_size = 64;
			break;
	}
		
   if((*sram_chipid & 0xf0000000) == 0xe0000000)  //remap=2
	{
	  *(volatile uint32_t *)(0x10000000) = *(volatile uint32_t *)(0x10000000);  //E版，OSC电流默认来自FLASH，操作FLASH地址，唤醒flash
	}   
	
  /* 1. EEPROM CHIP ERASE */
  ee_chip_erase();
  if(ee_chip_check_empty())
  {
    MSG("eeprom chip erase fail!!!\n");
    qc_fail(sram_res);
  } 
	
	/* 2.EEPROM PAGE PROGRAM */
	for(i = 0x0; i < CHIP_SIZE; i*=2)
	{
		for(j = 0; j < ee_page_size/4; j++)
		{
			k = (i/ee_page_size)+j;			
			buf[j] = (k&0xff) | (((~k) & 0xff) << 8) | (((k+0x55) & 0xff) << 16) | (((k+0xaa) & 0xff) << 24);
		}
		ee_page_prog(i/ee_page_size, (uint8_t *)buf);
		WDT->EN = 0xbb;
		if(i==0)
			i=ee_page_size/2;
	}

	for(i = 0x0; i < CHIP_SIZE; i*=2)
	{
		for(j = 0; j < ee_page_size/4; j++)
		{
			k = (i/ee_page_size)+j;			
			buf[j] = (k&0xff) | (((~k) & 0xff) << 8) | (((k+0x55) & 0xff) << 16) | (((k+0xaa) & 0xff) << 24);
			if(*(volatile uint32_t *)(0x08000000 + 4*j + i) != buf[j])
			{
        MSG("eeprom page program fail!!!\n");
				qc_fail(sram_res);
			}
		}		
		if(i==0)
			i=ee_page_size/2;
	}
	
	/* 3. EEPROM CHIP PROG */
  ee_chip_prog((uint8_t *)buf);
  if(ee_chip_check_data((uint8_t *)buf))
  {
    MSG("eeprom chip program fail!!!\n");
		qc_fail(sram_res);
  }
	/* 4. EEPROM CHIP ERASE */
  ee_chip_erase();
  
  //i=TC0->CNT;
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->SYS_RST = SYSCTL->SYS_RST;
	//MSG("\n<< CNT %f(%x) >>\n",(float)i/29491200, SYSCTL->SYS_RST);
	//MSG("pass\n");
	*sram_res = TEST_PASS;
	//TRIG_LOW();
  while(1)
  {
    WDT->EN = 0xbb;
  }
}

void qc_fail(uint32_t *sram_res)
{
  *sram_res = TEST_FAIL;
  //MSG("test fail!\n");
  while(1)
  {
    WDT->EN = 0xbb;
  }
}

uint32_t ee_chip_check_empty(void)
{
  uint32_t i;
	uint32_t ee_addr;
	
	ee_addr = ((SYSCTL->MAP_CTRL & 0x3) == 0x1) ? 0x0 : 0x08000000;
	
  for(i = 0; i < CHIP_SIZE / 4; i++)
  {
		WDT->EN = 0xbb;
    if(*(uint32_t *)(ee_addr + 4 * i) != 0x0)
    {
      //printf("chip check empty fail: Byte[%i] = %d\n", i, *(uint32_t *)(ee_addr + 4 * i));
      return 1;
    }
  }
  
  return 0;
}

uint32_t ee_chip_check_data(uint8_t *data)
{
  uint32_t i, j ;
	uint8_t  flag ;
	uint32_t ee_addr;
	uint32_t  ee_page_size;
	switch((*sram_chipid & 0xf0000000))
	{
		case 0xc0000000:
		case 0xe0000000:
			ee_page_size = 128;
			break;
		default:
			ee_page_size = 64;
			break;
	}
	ee_addr = ((SYSCTL->MAP_CTRL & 0x3) == 0x1) ? 0x0 : 0x08000000;
	flag = 0 ;
  
  for(i = 0; i < (CHIP_SIZE / ee_page_size); i++)
  {
    WDT->EN = 0xbb;
    
    for(j = 0; j < ee_page_size; j++)
    {
      if(*(uint8_t *)(ee_addr + i * ee_page_size + j) != data[j])
      {
        //printf("chip check data fail: Adrr[0x%x] = 0x%x, shoude be 0x%x\n", (ee_addr + i * ee_page_size + j), *(uint8_t *)(ee_addr + i * ee_page_size + j ), data[j]);
				flag = 1 ;				
		    *sram_chipid = (data[j] << 16) | (*(uint8_t *)(ee_addr + i * ee_page_size + j));
				return flag ;
      }
    }
  }
  
  return flag;
}

uint32_t ee_page_check_empty(uint32_t pg)
{
  uint32_t i;
	uint32_t ee_addr;
	uint32_t  ee_page_size;
	
	ee_addr = ((SYSCTL->MAP_CTRL & 0x3) == 0x1) ? 0x0 : 0x08000000;
	switch((*sram_chipid & 0xf0000000))
	{
		case 0xe0000000:
		case 0xc0000000:
			ee_page_size = 128;
			break;
		default:
			ee_page_size = 64;
			break;
	}
  for(i = 0; i < ee_page_size; i++)
  {
    if(*(uint8_t *)(ee_addr + pg * PAGE_SIZE + i) != 0x0)
    {
      //printf("page[%d] check empty fail: Byte[%i] = %d\n", pg, i, *(uint8_t *)(ee_addr + pg * PAGE_SIZE + i));
      return 1;
    }
  }
  
  WDT->EN = 0xbb;
  
  return 0;
}

uint32_t ee_page_check_data(uint32_t pg, uint8_t *data)
{
  uint32_t i;
	uint32_t ee_addr;
	uint32_t  ee_page_size;
	switch((*sram_chipid & 0xf0000000))
	{
		case 0xe0000000:
		case 0xc0000000:
			ee_page_size = 128;
			break;
		default:
			ee_page_size = 64;
			break;
	}
	ee_addr = ((SYSCTL->MAP_CTRL & 0x3) == 0x1) ? 0x0 : 0x08000000;
  
  for(i = 0; i < ee_page_size; i++)
  {
    if(*(uint8_t *)(ee_addr + pg * ee_page_size + i) != data[i])
    {
      //printf("page[%d] check data fail: Byte[%i] = %d, shoude be %d\n", pg, i, *(uint32_t *)(ee_addr + pg * ee_page_size + i), data[i]);
      return 1;
    }
  }
  
  WDT->EN = 0xbb;
  
  return 0;
}
