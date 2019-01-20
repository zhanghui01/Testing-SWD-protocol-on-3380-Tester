//Dear:
// 
//SD提供测试pattern注意：
// 
//      start 信号定义为：P63，P64
// 
//上电通过测试模式写入程序
//程序执行，P63，P64配置为输入，P60，P61，P62口初始化配置为输出0,0,0。循环检测P63，P64的输入信号。
//当测试机给P63,P64灌入1,0后程序往下执行测试。
// 
// 
// 
//      result信号定义为：P60，P61，P62
// 
//P60，P61，P62配置为输出
//P60，P61，P62口初始化配置为0,0,0
// 
//FAIL：P60，P61，P62输出0，1，0
//PASS：P60，P61，P62输出1，0，1

#include "RN8215.h"

#define FLASH_PAGE_SIZE   128
#define FLASH_CHIP_SIZE   (128*1024)
#define WDT_CLR  WDT->EN = 0xbb

//FAIL：P60，P61，P62输出0，1，0
//PASS：P60，P61，P62输出1，0，1
void testFail(void)
{
  GPIO->PB  &= (~((1<< 16) | (1 << 18)));
  GPIO->PB |= (1 << 17);
	while(1)
		;
}

void testPass(void)
{
  GPIO->PB  &= (~(1 << 17));
  GPIO->PB |= ((1 << 16) | (1 << 18));
	while(1)
		;
}


//上电通过测试模式写入程序
//程序执行，P63，P64配置为输入，P60，P61，P62口初始化配置为输出0,0,0。循环检测P63，P64的输入信号。
//当测试机给P63,P64灌入1,0后程序往下执行测试。
void waitStart(void)
{
  SYSCTL->SYS_PS = 0x82;
  SYSCTL->MOD1_EN |= (1 << 5);
  GPIO->PCB &= (~((0x1f << 16)));
  GPIO->PMB &= (~((0x07 << 16)));
	GPIO->PMB |= ((0x03 << 19));
  GPIO->PB  &= (~((0x1f << 16)));
  
	GPIO->PIEB &= (~((0x1f << 16)));
  while(((GPIO->PB >> 19) & 0x3) != 0x1)
  {
		WDT_CLR;
  }
}


uint32_t fl_chip_check_empty(void)
{
  uint32_t i;
	volatile uint32_t fl_addr;
	
	fl_addr = SYSCTL->MAP_CTRL & 0x3;
	fl_addr = (fl_addr == 0x0) ? 0x0 : (fl_addr == 0x1) ? 0x08000000 : (fl_addr == 0x2) ? 0x10000000 : 0x18000000;
	
  for(i = 0; i < FLASH_CHIP_SIZE / 4; i++)
  {
		//WDT_CLR;
    if(*(uint32_t *)(fl_addr + 4 * i) != 0x0)
    {
      return 1;
    }
  }
  
  return 0;
}

uint32_t fl_chip_check_data(uint8_t *data)
{
  uint32_t i, j ;
	volatile uint32_t fl_addr;
	
	fl_addr = SYSCTL->MAP_CTRL & 0x3;
	fl_addr = (fl_addr == 0x0) ? 0x0 : (fl_addr == 0x1) ? 0x08000000 : (fl_addr == 0x2) ? 0x10000000 : 0x18000000;
	
  for(i = 0; i < (FLASH_CHIP_SIZE / FLASH_PAGE_SIZE); i++)
  {
    //WDT_CLR;
    
    for(j = 0; j < FLASH_PAGE_SIZE; j++)
    {
      if(*(uint8_t *)(fl_addr + i * FLASH_PAGE_SIZE + j) != data[j])
      {
				return 1 ;
      }
    }
  }
  
  return 0;
}

void fl_check(uint32_t base_addr)
{
	while((*(volatile uint32_t *)(base_addr + 0x00800008)) & 0x1)
	{
		;//WDT_CLR;
	}
}
//3.8
void fl_chip_erase(void)
{
  uint32_t prot;
	uint32_t fl_addr;
	
	fl_addr = SYSCTL->MAP_CTRL & 0x3;
	fl_addr = (fl_addr == 0x0) ? 0x0 : (fl_addr == 0x1) ? 0x08000000 : (fl_addr == 0x2) ? 0x10000000 : 0x18000000;
		
  fl_check(fl_addr);
  
	*(volatile uint32_t *)(fl_addr + 0x00800024) = 3800/4/1.085-1;  //tms
	*(volatile uint32_t *)(fl_addr + 0x0080002c) = 3;  //te
  
  *(volatile uint32_t *)(fl_addr + 0x00800014) = 0xfeed0000;
  *(volatile uint32_t *)(fl_addr + 0x00800000) = 0xfeed0009;  //CHIP ERASE
  fl_check(fl_addr);
}
//1.8
void fl_chip_prog(uint8_t * data)
{
  uint32_t prot, i;
	uint32_t fl_addr;
	
	fl_addr = SYSCTL->MAP_CTRL & 0x3;
	fl_addr = (fl_addr == 0x0) ? 0x0 : (fl_addr == 0x1) ? 0x08000000 : (fl_addr == 0x2) ? 0x10000000 : 0x18000000;
		
  fl_check(fl_addr);
  
	*(volatile uint32_t *)(fl_addr + 0x00800024) = 3800/4/1.085-1;  //tms
	*(volatile uint32_t *)(fl_addr + 0x0080002c) = 3;  //te
  *(volatile uint32_t *)(fl_addr + 0x00800014) = 0xfeed0000;
  *(volatile uint32_t *)(fl_addr + 0x00800000) = 0xfeed0009;  //CHIP ERASE
  fl_check(fl_addr);  
  *(volatile uint32_t *)(fl_addr + 0x00800000) = 0xfeed0004;
  fl_check(fl_addr);  
  
  for(i = 0; i < FLASH_PAGE_SIZE; i++)
	{
	  *(uint8_t *)(fl_addr + i) = ((uint8_t *)data)[i];
	}
	*(volatile uint32_t *)(fl_addr + 0x00800024) = 1800/2/1.085-1;  //tms
	*(volatile uint32_t *)(fl_addr + 0x00800028) = 1;  //tp
	*(volatile uint32_t *)(fl_addr + 0x00800000) = 0xfeed000c;  //CHIP PROG
  fl_check(fl_addr);
}

int main()
{
  uint32_t  i, sum = 0;
  uint8_t   data[FLASH_PAGE_SIZE];
	uint32_t fl_addr;
	
	SYSCTL->SYS_PS = 0x82;
	fl_addr = SYSCTL->MAP_CTRL & 0x3;
	fl_addr = (fl_addr == 0x0) ? 0x0 : (fl_addr == 0x1) ? 0x08000000 : (fl_addr == 0x2) ? 0x10000000 : 0x18000000;
		
  *(volatile uint32_t *)(0x40030008) = 0x70;	
	*(volatile uint32_t *)(0x40030008) = 0x7e;
	*(volatile uint32_t *)(0x40030004) = 0xee;
	NVIC_EnableIRQ(WDT_IRQn);
	__enable_irq();
  
  //Enable RTC/WDT/GPIO/LCD Clock
	SYSCTL->OSC_CTRL1 = 0x0;
	SYSCTL->SYS_MODE = 0xD;
  SYSCTL->MOD1_EN |= (1<<5) | (1<<9) | (1<<10) | (1<<11);
  SYSCTL->MOD1_EN |= 0x460; 

  *(volatile uint32_t *)(0x40030008) = 0x70;	
	*(volatile uint32_t *)(0x40030008) = 0x7e;
	*(volatile uint32_t *)(0x40030004) = 0xee;
	NVIC_EnableIRQ(WDT_IRQn);
	__enable_irq();
	waitStart();
	
 *(volatile uint32_t *)(fl_addr + 0x00800020) = 0x7;  //tus
 *(volatile uint32_t *)(fl_addr + 0x00800018) = 0x20000;  //tw
	for(i = 0; i < FLASH_PAGE_SIZE; i++)
  {
    data[i] = i; 
  }
    
  /* 1. FLASH CHIP ERASE */
  fl_chip_erase();
  if(fl_chip_check_empty())
  {
    testFail();
  }
  
  /* 2. FLASH CHIP PROG */
  fl_chip_prog(data);
  if(fl_chip_check_data(data))
  {
		testFail();
  }
    
	testPass();
}

void WDT_HANDLER(void)
{
	WDT->EN = 0xbb;
}
