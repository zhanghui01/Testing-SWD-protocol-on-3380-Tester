#include <common.h>
#include <sysctrl.h>

/**
 * @brief get system clock
 * @param none
 * @return uint32_t freq: 32768/1843200/3686400/7372800
 * get system clock. LC mode only support 32.768Khz, RC mode only support 1.8432Mhz, HC mode support 1.8432/3.6864/7.3728Mhz)
 */
uint32_t getSysclk(void){
    uint32_t  freq;
#ifdef RA9103AM0
	  uint32_t  val = (SYSCTL->OSC_CTRL >> 8) & 0x3;
	  uint32_t  mode = (SYSCTL->OSC_CTRL >> 4) & 0x7;
#endif
#ifdef RA9103A0
    uint32_t  val = (SYSCTL->OSC_CTRL1 >> 8) & 0x7;
	  uint32_t  mode = (SYSCTL->OSC_CTRL1 >> 4) & 0x7;
#endif
    if(val == 0x3)
      freq = 32768;
    else if(val == 0x2)
		{
			if(mode == 0x2)
        freq = 1843200 - 307200;
			  //freq = 1843200;
			else
				freq = 1843200;
		}
    else if(val == 0x0)
	    freq = 7372800;
    else if(val == 0x1)
	    freq = 3686400;
	  else if(val == 0x4)
	    freq = 14745600;
	  else
	    freq = 29491200;
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
	SysModeChg(sysclk_loops/4, sysclk_loops%4); 
  sysclk_loops++;
  return 0;
}

/**
 * @brief set UARTx baudrate
 * @param uint32_t id: 0/1/2/3 means UART0/1/2/3/4/5
 *        uint32_t value: UARTx baudrate
 * @return uint32_t status: 0 means successful, 1 means fail
 * Set UARTx baudrate
 */
uint32_t setBaud(uint32_t id, uint32_t value){
  uint32_t div, status = 0 ,f ;
	f = getSysclk() ;
	if( f != 32768 )
    div = f /(value*16) - 1;
	else
		div = 9 ;
  if((div <= 0) || (id > 5)){
    status = 1;
    *(uint32_t *)(&(UART0->BAUD) + id * (&(UART1->BAUD) - &(UART0->BAUD))) = 0;
  }else if(id < 4)
    *(uint32_t *)(&(UART0->BAUD) + id * (&(UART1->BAUD) - &(UART0->BAUD))) = div;
  else
    *(uint32_t *)(&(UART4->BAUD) + (id - 4) * (&(UART5->BAUD) - &(UART4->BAUD))) = div;
  
  return status;
}

/**
 * @brief UARTx initialization
 * @param uint32_t id: 0/1/2/3/4/5 means UART0/1/2/3/4/5
 * @return none
 * UARTx initialization. Baudrate is 9600, stop bit is 2, data width is 8, parity is none
 */
void uartInit(uint32_t id, uint32_t baud, uint32_t rxie, uint32_t txie, uint32_t errie){
#ifdef RA9103AM0
  SYSCTL->MOD0_EN |= (0xA5890000 | (1 << (8 + id)));
#endif
#ifdef RA9103A0
  SYSCTL->SYS_PS = 0x82;
  if(id < 4)
    SYSCTL->MOD0_EN |= (1 << (8 + id));
  else
    SYSCTL->MOD0_EN |= (1 << (2 + id));
#endif
  setBaud(id, baud);
  SYSCTL->MOD1_EN |= (0xA5800000 | (1 << 5));   /* enable GPIO clk */
#ifdef RA9103AM0
  GPIO->PCA0 |= (1 << (25 + id));	              /* UARTi PAD */
  *(uint32_t *)(&(UART0->CTRL) + id * (UART1 - UART0)) = (1 << 0)  |   /* uart enable */
                                                         (0 << 4)  |   /* stop bit: 1 */
                                                         (3 << 5)  |   /* data bit: 8 */
                                                         (rxie << 2) | /* rx ie */
                                                         (txie << 1) | /* tx ie */
                                                         (errie << 3); /* err ie */
#endif
#ifdef RA9103A0
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
#endif
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
#ifdef RA9103AM0
    while(*(uint32_t *)(&(UART0->STA) + id * (UART1 - UART0)) & 0x80)
      ;
    *(uint32_t *)(&(UART0->TXD) + id * (UART1 - UART0)) = *str++;
#endif
#ifdef RA9103A0
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
#endif
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
#ifdef RA9103AM0
    while(*(uint32_t *)(&(UART0->STA) + id * (UART1 - UART0)) & 0x80)
      ;
    *(uint32_t *)(&(UART0->TXD) + id * (UART1 - UART0)) = c;
#endif    
#ifdef RA9103A0
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
#endif
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
#if 1	
  SYSCTL->MOD1_EN |= ( 0xA5800000 
                        |(1 << 5));  /* GPIO clk enable */
  GPIO->PCA1 &= (~(0xfff << 0)); /* gpio mode */
  GPIO->PMA  &= (~(0x3fu << 24)); /* output */
  GPIO->PA   &= (~(0x3fu << 24));
#endif
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
  SYSCTL->MOD1_EN |= ( 0xA5800000 
                        |(1 << 5));  /* GPIO clk enable */
  GPIO->PCA1 &= (~(0xfff << 0)); /* gpio mode */
  GPIO->PMA  &= (~(0x3fu << 24)); /* output */
  GPIO->PA |= (0x3fu << 24);

  while(1){												
    WdtFeed();		
    for(i = 0; i < 100000; i++) ;
    GPIO->PA = GPIO->PA & (~(0x3fu << 24)) | ((~(GPIO->PA & (0x3fu << 24))) & (0x3fu << 24)) ;
  }
}

void uDelay(uint32_t time)
{
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
}
static void wdtWait(void)
{
	while(WDT->EN & (1 << 8))
	{
		;
	}
}
void WdtInit(uint8_t irq_en){
// 	SYSCTL->SYS_PS = 0x82;
//   SYSCTL->MOD1_EN |= (1 << 9);
//   //Close WDT
//   WDT->PASS = 0x70;
//   WDT->PASS = 0x7e;
// 	if(irq_en)
// 	{
//     WDT->CTRL  = 0xee;      /* 8s */
// 	}
// 	else
// 	{
// 		WDT->CTRL  = 0x6e;
// 	}
// 	
//   while(WDT->EN&0x100);
//   //WDT->PASS = 0x00;
// 	
// 	if(irq_en)
// 	{
// 	  __enable_irq();
//     NVIC_EnableIRQ(WDT_IRQn);
// 	}
	wdtWait();
	WDT->PASS = 0x70;
	wdtWait();
	WDT->PASS = 0x7e;
	wdtWait();
	WDT->HALT = 0x7e5a;
	wdtWait();
	WDT->STBY = 0xa538;
}


// void 	WDT_HANDLER(void)
// {	
// 	if( wdt_test == 0x55 )
// 	{
//     wdt_int_flag = 1;
//     wdt_cnt = TC0->CNT;
//     TC0->CTRL = 0;
// 	}
//   WDT->EN   = 0xbb;    
//   while(WDT->EN & 0x100)
// 		;
//     
// }

void    WDT_HANDLER  (void)
{    
   WDT->EN = 0xbb;
	 while(WDT->EN & 0x100)
	 {
		 ;
	 }
	 //SYSCTL->MOD1_EN &= (~(1 << 9));
} 


void WDT_OFF(void) 
{
	NVIC_EnableIRQ(WDT_IRQn);
	SYSCTL->MOD1_EN	|= 1<<9;
	while (WDT->EN & 0x100);
	WDT->PASS = 0x70;
	WDT->PASS = 0x7e;
	WDT->CTRL = 0xee;
	while (WDT->EN & 0x100);
}

/*********************************************************************************************************
** Function name:       swDelay_soft
** Descriptions:        mercily software delay
** input parameters:    delay duration
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
void swDelay_soft(uint32_t ulTime) 
{
    uint32_t i;

    i = 0;
    while (ulTime--)
     {
        for (i = 0; i < 5000; i++)
          ;
     }
}


void WdtFeed(void)
{
	//while(WDT->EN&0x100)
	//	;
	
	WDT->EN = 0xbb;
}
