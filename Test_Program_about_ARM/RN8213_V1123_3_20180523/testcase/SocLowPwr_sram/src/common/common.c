#include <common.h>
#include "clk_calc.h"
//#include <sysctrl.h>

/**
 * @brief get system clock
 * @param none
 * @return uint32_t freq: 32768/1843200/3686400/7372800
 * get system clock. LC mode only support 32.768Khz, RC mode only support 1.8432Mhz, HC mode support 1.8432/3.6864/7.3728Mhz)
 */
uint32_t getSysclk(void){
    uint32_t  freq;
    uint32_t  val = (SYSCTL->OSC_CTRL1 >> 8) & 0x7;
	  
	  switch (val)
	  {
	    case 0x3:
        freq = 32768;
        break;
      case 0x2:
  			freq = 1843200;
        break;
      case 0x0:
  	    freq = 7372800;
        break;
      case 0x1:
  	    freq = 3686400;
        break;
  	  case 0x4:
  	    freq = 14745600;
        break;
  	  default:
  	    freq = 29491200;
        break;
	  }
	      
		//return 3686400;
    return freq;
}

uint32_t sysclk_loops = 0;
/**
 * @brief loop system clock setting
 * @param none
 * @return uint32_t status: 0 pass
 *                          1 fail
 * loop system clock setting, from HC to LC
 */
uint32_t sysclkLoop(){
	//SysModeChg(sysclk_loops/4, sysclk_loops%4); 
  sysclk_loops++;
	if(sysclk_loops > 15)
	{
		return 1;
	}
	else
	{
    return 0;
	}
}

/**
 * @brief set UARTx baudrate
 * @param uint32_t id: 0/1/2/3 means UART0/1/2/3/4/5
 *        uint32_t value: UARTx baudrate
 * @return uint32_t status: 0 means successful, 1 means fail
 * Set UARTx baudrate
 */
extern volatile uint32_t  * sram_chipid;
uint32_t setBaud(uint32_t id, uint32_t value){
  int32_t div, status = 0;
  float   sys_freq;
  uint32_t fdiv;
  
  if(((SYSCTL->OSC_CTRL1 >> 4) & 0x7) == 0x2)
  {
    sys_freq = clk_freq(CALC_RCH) / 2.0;
  }
  else
  {
    sys_freq = getSysclk();
  }
  
  sys_freq = sys_freq / (16 * value);
  div = ((uint32_t)sys_freq) - 1;
	if((*sram_chipid & 0xf0000000) != 0xa0000000)
	{
    fdiv = (uint32_t)((float)(sys_freq - (uint32_t)sys_freq) * 16384.0 + 0.5);
	}
  
  
  //div = getSysclk()/(value*8);
	//div = (div & 0x1) ? (div >> 1) : ((div >> 1) - 1);
  
  if(id > 5){
    status = 1;
    *(uint32_t *)(&(UART0->BAUD) + id * (&(UART1->BAUD) - &(UART0->BAUD))) = 0;
  }else if(id < 4)
  {
    *(uint32_t *)(&(UART0->BAUD) + id * (&(UART1->BAUD) - &(UART0->BAUD))) = div;
		if((*sram_chipid & 0xf0000000) != 0xa0000000)
		{
      *(uint32_t *)(&(UART0->FDIV) + id * (&(UART1->FDIV) - &(UART0->FDIV))) = fdiv;
		}
  }
  else
  {
    *(uint32_t *)(&(UART4->BAUD) + (id - 4) * (&(UART5->BAUD) - &(UART4->BAUD))) = div;
		if((*sram_chipid & 0xf0000000) != 0xa0000000){
      *(uint32_t *)(&(UART4->FDIV) + (id - 4) * (&(UART5->FDIV) - &(UART4->FDIV))) = fdiv;
		}
  }
  
  return status;
}

/**
 * @brief UARTx initialization
 * @param uint32_t id: 0/1/2/3/4/5 means UART0/1/2/3/4/5
 * @return none
 * UARTx initialization. Baudrate is 9600, stop bit is 2, data width is 8, parity is none
 */
void uartInit(uint32_t id, uint32_t baud, uint32_t rxie, uint32_t txie, uint32_t errie){
  SYSCTL->SYS_PS = 0x82;
  if(id < 4)
    SYSCTL->MOD0_EN |= (1 << (8 + id));
  else
    SYSCTL->MOD0_EN |= (1 << (2 + id));

  setBaud(id, baud);
  SYSCTL->MOD1_EN |= (0xA5800000 | (1 << 5));   /* enable GPIO clk */

  if(id < 4)
  {
    if(id == 2)
      GPIO->PCA0 &= (~(1 << 29));
    GPIO->PCA0 |= (1 << (25 + id));	              /* UARTi PAD */
    *(uint32_t *)(&(UART0->CTRL) + id * (UART1 - UART0)) = (1 << 0)  |   /* uart enable */
                                                           (0 << 4)  |   /* stop bit: 1 */
                                                           (3 << 5)  |   /* data bit: 8 */
                                                           (rxie << 2) | /* rx ie */
                                                           (txie << 1) | /* tx ie */
                                                           (errie << 3); /* err ie */
  }
  else
  {
    if(id == 4)
      GPIO->PCA1 |= (0xf << 0);	              /* UARTi PAD */
    else
      GPIO->PCB |= (0x3 << 12);
    *(uint32_t *)(&(UART4->CTRL) + (id - 4) * (UART5 - UART4)) = (1 << 0)  |   /* uart enable */
                                                           (0 << 4)  |   /* stop bit: 1 */
                                                           (3 << 5)  |   /* data bit: 8 */
                                                           (rxie << 2) | /* rx ie */
                                                           (txie << 1) | /* tx ie */
                                                           (errie << 3); /* err ie */
  }
}

/**
 * @brief send string from UARTx port
 * @param uint32_t id: 0/1/2/3 means UART0/1/2/3
 *        uint8_t *str:string to be sent
 * @return none
 * Send string from UARTx port
 */
void sendStr(uint32_t id, uint8_t *str){
  while(*str != 0){
    if(id < 4)
    {
      while(*(uint32_t *)(&(UART0->STA) + id * (UART1 - UART0)) & 0x80)
        ;
      *(uint32_t *)(&(UART0->TXD) + id * (UART1 - UART0)) = *str++;
    }
    else
    {
      while(*(uint32_t *)(&(UART4->STA) + (id - 4)* (UART5 - UART4)) & 0x80)
        ;
      *(uint32_t *)(&(UART4->TXD) + (id - 4) * (UART5 - UART4)) = *str++;
    }
  }
}

/**
 * @brief send character from UARTx port
 * @param uint32_t id: 0/1/2/3 means UART0/1/2/3
 *        uint8_t c: character to be sent
 * @return none
 * Send one character from UARTx port
 */
void sendChar(uint32_t id, uint8_t c){
  if(id < 4)
  {
    while(*(uint32_t *)(&(UART0->STA) + id * (UART1 - UART0)) & 0x80)
      ;
    *(uint32_t *)(&(UART0->TXD) + id * (UART1 - UART0)) = c;
  }
  else
  {
    while(*(uint32_t *)(&(UART4->STA) + (id - 4) * (UART5 - UART4)) & 0x80)
      ;
    *(uint32_t *)(&(UART4->TXD) + (id - 4) * (UART5 - UART4)) = c;
  }
}

/**
 * @brief exit testcase successful
 * @param none
 * @return none
 * Exit testcase successful, write success result(0x0) into special address
 */
void exitPass(){
  //MSG("TESTING PASS!\n");
  //*(uint32_t *)(RESULT_ADDR) = 0x0;
  //*(uint32_t *)(RESULT_ADDR - 4) = 0x0;  //trigger for data watch point
  
  /* light half LED(P3.0~P3.7) */
// 	SYSCTL->MOD1_EN |= (0xA5800000u | (1u << 5));
// 	GPIO->PCA0 &= (~0xf0u);
// 	GPIO->PMA  &= (~(0x3cu << 0));
// 	GPIO->PA |= (0x3cu << 0);
	
  while(1)
		WdtFeed();
}

/**
 * @brief exit testcase with error code
 * @param uint32_t res: error code
 * @return none
 * Exit testcase with error code, write error code(res) into special address
 */
void exitFail(uint32_t res){
  uint32_t i;
  //MSG("TESTING FAIL(0x%x)\n", res);
  //*(uint32_t *)(RESULT_ADDR) = res;
  //*(uint32_t *)(RESULT_ADDR - 4) = 0x0;  //trigger for data watch point
  
  /* light All LED(P3.0~P3.7) */
  SYSCTL->MOD1_EN |= (0xA5800000u | (1u << 5));
	GPIO->PCA0 &= (~0xf0u);
	GPIO->PMA  &= (~(0x3cu << 0));
	GPIO->PA &= (~(0x3cu << 0));
	
  while(1){												
    WdtFeed();		
    for(i = 0; i < 100000; i++) ;
    GPIO->PA |= (0x3cu << 0);		
    WdtFeed();		
    for(i = 0; i < 100000; i++) ;
    GPIO->PA &= (~(0x3cu << 0));
  }
}

void uDelay(uint32_t time)
{
	if(time == 0)
		return;
	
	SystemCoreClockUpdate();
  SysTick->CTRL = 0x4;
  SysTick->VAL  = 0x0;
	if(SystemCoreClock == 32768)
		SysTick->LOAD = SystemCoreClock / 1000 * time;
	else
    SysTick->LOAD = SystemCoreClock / 1000000 * time;
  
  SysTick->CTRL = 0x5;
  while(!(SysTick->CTRL & 0x10000))
  {
    ;
  }
	SysTick->CTRL = 0x4;
	WDT->EN = 0xbb;
}

void mDelay(uint32_t ms)
{
	uint32_t i;
	for(i = 0; i < ms; i++)
	{
		uDelay(1000);
	}
}

// void WdtInit(void){
// 	SYSCTL->SYS_PS = 0x82;
//   SYSCTL->MOD1_EN |= (1 << 9);
//   WDT->EN = 0xbb;
// }

void WdtInit(void){
	SYSCTL->SYS_PS = 0x82;
  SYSCTL->MOD1_EN |= (1 << 9);
	WDT->PASS=0x70;
	WDT->PASS=0x7e;
	WDT->CTRL = 0x6E;
  WDT->EN = 0xbb;
}

void WdtFeed(void)
{
	//while(WDT->EN&0x100)
	//	;
	WDT->EN = 0xbb;
}

void mod_clk(MOD_CLK_ID mod, MOD_CLK_OP op)
{
	SYSCTL->SYS_PS = 0x82;
	
	if(op == MOD_OFF)
	{
		if(mod < 32)
		{
			SYSCTL->MOD0_EN &= (~(1 << mod));
		}
		else
		{
			SYSCTL->MOD1_EN &= (~(1 << (mod - 32)));
		}
	}
	else
	{
		if(mod < 32)
		{
			SYSCTL->MOD0_EN |= (1 << mod);
		}
		else
		{
			SYSCTL->MOD1_EN |= (1 << (mod - 32));
		}
	}
}
/*
void tcInit(uint32_t cycles)
{
  mod_clk(MOD_TC0, MOD_ON);
  
  NVIC_EnableIRQ(TC0_IRQn);
  __enable_irq();
  TC0->DN = cycles;
  TC0->IE = 1;
  TC0->CTRL = (0 << 4) | 1;  //loop trigger
}

uint32_t tc0_flag = 0;
void TC0_HANDLER(void)
{
  TC0->STA = 1;
	tc0_flag = 1;
}
*/
