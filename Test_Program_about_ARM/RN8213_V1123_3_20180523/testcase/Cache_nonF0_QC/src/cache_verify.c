#include "ra9103.h"
#include "flash_driver.h"

#define  RES_OFFSET     0x37f8
#define  CHIPID_OFFSET  0x37fc

#define  TEST_START    0xa5a5a5a5
#define  TEST_FAIL     0x123cac4e
#define  TEST_PASS     0x98765432

#define  CACHE_SIZE         (2*1024)
#define  CACHE_GROUPS       16
#define  CACHE_LINES        4
#define  CACHE_BYTES        32

#define  CACHELOOPS         2
#define  FLASH_START_ADDR   0//0x27400

uint8_t *cache_data;// = (uint8_t *)0x10003800;
uint32_t verify_data[CACHE_SIZE/4];
uint32_t fl_base;

static   uint32_t get_fl_base(void);
uint32_t mem_cmp(uint8_t *src, uint8_t *dst, uint32_t len);
void     generate_buf(void);
uint8_t  lru_verify(uint8_t line);

uint32_t  fl_page_size_g, fl_chip_size_g;
int32_t main(void){
  uint32_t i, j, line, group;
	uint32_t cache_loops;
	uint32_t data;
	uint8_t  fl_data[128];
	uint8_t  div;
	volatile uint32_t temp;
	uint32_t sram_base;
  
	SYSCTL->SYS_PS = 0x82;
	
	sram_base = ((SYSCTL->MAP_CTRL & 0x7) == 2) ? 0x0 : 0x10000000;
	*(volatile uint32_t *)(sram_base + RES_OFFSET) = TEST_START;
	
	fl_base = get_fl_base();
	*(volatile uint32_t *)(fl_base + 0x00800000) = 0xfeed0003;
	
	SYSCTL->SYS_PS = 0x75;
	if(SYSCTL->DIE_VER == 0xf0)
	{
		fl_page_size_g = 256;
		fl_chip_size_g = 512*1024;
		cache_data = (uint8_t *)(sram_base + 0x7800);
	}
	else if((SYSCTL->DIE_VER == 0xc0) || (SYSCTL->DIE_VER == 0xe0))
	{
		fl_page_size_g = 128;
		fl_chip_size_g = 128*1024;
		cache_data = (uint8_t *)(sram_base + 0x2000);
	}
	else
	{
		fl_page_size_g = 128;
		fl_chip_size_g = 192*1024;
		cache_data = (uint8_t *)(sram_base + 0x3800);
	}
	SYSCTL->SYS_PS = 0x82;
	
	SYSCTL->MOD1_EN |= (1 << 9);  //wdt
	SYSCTL->MOD1_EN |= (1 << 5);  //gpio
										
	fl_chip_erase();
	for(i = FLASH_START_ADDR/fl_page_size_g; i < ((FLASH_START_ADDR+CACHELOOPS*CACHE_SIZE) / fl_page_size_g); i++)
	{
		for(j = 0; j < fl_page_size_g/4; j++)
	  {
			data = 4*j+i*fl_page_size_g;
			fl_data[4*j]   = data&0xff;
			fl_data[4*j+1] = (data>>8)&0xff;
			fl_data[4*j+2] = (data>>16)&0xff;
			fl_data[4*j+3] = (data>>24)&0xff;
		}
		fl_page_prog(i, fl_data);
		WDT->EN = 0xbb;
	}
															//  0			1		  	2     3
	for(div = 0; div < 2; div++)//29MHz  14MHz   7MHz  3.5MHz
	{
	  *(volatile uint32_t *)(fl_base + 0x0080001c) = 0xfeed0000; //cache off
		
		for(i = 0; i < CACHE_SIZE; i++)
		{
			((uint8_t *)verify_data)[i] = 0x0;
		}
		
		SYSCTL->SYS_MODE = 0xE;		
		GPIO->PCA1 |= (0xf << 12);
		SYSCTL->OSC_CTRL2 = 0x13;
 		SYSCTL->OSC_CTRL1 = div << 2;
		*(volatile uint32_t *)(fl_base + 0x00800018) = (div == 0) ? (5 << 16) : (((div == 1) || (div == 2)) ? (2 << 16) : 0);
		SYSCTL->SYS_MODE = 0xD;	
			
		*(volatile uint32_t *)(fl_base + 0x0080001c) = 0xfeed0001; //cache on		
		for(i = 0; i < fl_chip_size_g; i+=4)
		{
			temp = *(volatile uint32_t *)(fl_base + i);
		}
		
		for(cache_loops = 0; cache_loops < CACHELOOPS; cache_loops++)
		{
			for(i = 0; i < 64; i++)
			{
				group = i / 4;
				line = i % 4;
				SYSCTL->CHIP_ID = *(uint8_t *)(fl_base + FLASH_START_ADDR + cache_loops*CACHE_SIZE + 32*i);
				SYSCTL->CHIP_ID = *(uint8_t *)(fl_base + FLASH_START_ADDR + cache_loops*CACHE_SIZE + 32*i + 1);
				for(j = 0; j < (CACHE_BYTES/4); j++)
				{
					verify_data[group*32+line*8+j] = FLASH_START_ADDR + ((group+line*16)*8 + j)*4 + cache_loops*CACHE_SIZE;			
				}
			}
					
			if(mem_cmp(cache_data, (uint8_t *)verify_data, CACHE_SIZE))
			{
				*(uint32_t *)(sram_base + RES_OFFSET-0x4) = 0x11;
				goto fail_label;
			}
			
			for(i = 0; i < 64; i++)
			{
				SYSCTL->CHIP_ID = *(uint8_t *)(fl_base + FLASH_START_ADDR + cache_loops*CACHE_SIZE + 32*i);
				SYSCTL->CHIP_ID = *(uint8_t *)(fl_base + FLASH_START_ADDR + cache_loops*CACHE_SIZE + 32*i + 1);
			}
			if(mem_cmp(cache_data, (uint8_t *)verify_data, CACHE_SIZE))
			{
				*(uint32_t *)(sram_base + RES_OFFSET-4) = 0x22;
				goto fail_label;
			}		
		}
		
		for(i = 0; i < 4; i++)
		{
			if(lru_verify(i))
			{
				*(uint32_t *)(sram_base + RES_OFFSET-4) = 0x33+i;
				goto fail_label;
			}
		}
		WDT->EN = 0xbb;
	}
	
	*(uint32_t *)(sram_base + RES_OFFSET) = TEST_PASS;
  while(1)
	{
		WDT->EN = 0xbb;
	}
	
fail_label:
	*(uint32_t *)(sram_base + RES_OFFSET) = TEST_FAIL;
  while(1)
	{
		WDT->EN = 0xbb;
	}
}

void line_access(uint8_t line)
{
	uint32_t i;
	volatile uint32_t temp;
	
	for(i = 0; i < CACHE_SIZE; i+=32)
	{
		if((i >= (line*CACHE_GROUPS*32)) && (i < ((line+1)*CACHE_GROUPS*32)))
		{
			temp = *(volatile uint32_t *)(fl_base+ FLASH_START_ADDR + i);
		}
	}
}

/*
 *  A(0), B(1), C(2), D(3)  AB(01) /AB(10)
 *
 *  A=10&20&30
 *  B=01&21&31
 *  C=02&03&32
 *  D=03&13&23
 *
 */
const	uint8_t  linea_lru[]={0,1,0,2,0,3,0,1,2,1,3,1,2,3,2};
const uint8_t  lineb_lru[]={1,0,1,2,1,3,1,0,2,0,3,0,2,3,2};
const uint8_t  linec_lru[]={2,0,2,3,2,0,1,0,2,0,3,0,1,3,1};
const uint8_t  lined_lru[]={3,0,3,1,3,2,3,0,1,0,2,0,1,2,1};	
// const	uint8_t  linea_lru[]={0,1,2,3, 1,3,0, 3,2,1};
// const uint8_t  lineb_lru[]={0,1,2,3, 1,3,0, 3,2,0};
// const uint8_t  linec_lru[]={0,1,2,3, 1,3,0, 0,3,1};
// const uint8_t  lined_lru[]={0,1,2,3, 1,3,0, 0,2,1};	
uint8_t lru_verify(uint8_t line)
{
	uint32_t i, j;
	volatile uint32_t temp;
  uint8_t  *line_lru;
	
	*(volatile uint32_t *)(fl_base + 0x0080001c) = 0xfeed0000; //cache off
	*(volatile uint32_t *)(fl_base + 0x0080001c) = 0xfeed0001; //cache on
	//1.fill cache
	for(i = 0; i < CACHE_SIZE; i+=32)
	{
		temp = *(volatile uint32_t *)(fl_base+ FLASH_START_ADDR + i);
	}
	
	//2. !line access once more
	line_lru = (uint8_t *)((line == 0)? linea_lru : ((line==1) ? lineb_lru : ((line==2) ? linec_lru : lined_lru)));
	for(i = 0; i < sizeof(linea_lru)/sizeof(uint8_t); i++)
	{
		line_access(line_lru[i]);
	}
	
	//3.make cache miss
	for(i = 0; i < CACHE_GROUPS*32; i+=32)
	{
		temp = *(volatile uint32_t *)(fl_base+fl_chip_size_g-CACHE_SIZE+i);
	}
	
	//4.generator cache sram data	
	generate_buf();
	for(i = 0; i < 16; i++)
	{
	  for(j = 0; j < (CACHE_BYTES/4); j++)
	  {
	    verify_data[(4*i+line)*8+j] = 0; //+0:line0, +1:line1, +2:line2, +3:line3
	  }
	}
	
	if(mem_cmp(cache_data, (uint8_t *)verify_data, CACHE_SIZE))
	{
	  return 1;
  }	
	return 0;
}

void generate_buf(void)
{
	uint32_t i, j, group, line;
	for(i = 0; i < 64; i++)
	{
		group = i / 4;
		line = i % 4;
		for(j = 0; j < (CACHE_BYTES/4); j++)
		{
			verify_data[group*32+line*8+j] = FLASH_START_ADDR + ((group+line*16)*8 + j)*4;			
		}
	}	
}

static uint32_t get_fl_base(void)
{
	uint32_t remap, fl_addr;
	
	remap = SYSCTL->MAP_CTRL;
	switch (remap & 0x7)
	{
		case 0:
			fl_addr = 0x00000000;
		  break;
		case 1:
			fl_addr = 0x08000000;
		  break;
		case 2:
			fl_addr = 0x10000000;
		  break;
		case 3:
			fl_addr = 0x18000000;
		  break;
		default:
			fl_addr = 0x08000000;
		  break;
	}
	
	return fl_addr;
}

uint32_t mem_cmp(uint8_t *src, uint8_t *dst, uint32_t len)
{
	uint32_t i;
	for(i = 0; i < len; i++)
	{
		if(src[i] != dst[i])
		{
			*(uint32_t *)(0x0 + RES_OFFSET-0x8) = src[i];
			*(uint32_t *)(0x0 + RES_OFFSET-0xc) = dst[i];
			*(uint32_t *)(0x0 + RES_OFFSET-0x10) = i;
			return 1;
		}
	}
	
	return 0;
}
