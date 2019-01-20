#include <rn821x.h>
#include <common.h>

#define UART_CTRL ((UART_ID < 4) ? (*(uint32_t *)((uint32_t)(&(UART0->CTRL)) + UART_ID * 0x4000)) : (*(uint32_t *)((uint32_t)(&(UART4->CTRL)) + (UART_ID - 4) * 0x4000)))
#define UART_BAUD ((UART_ID < 4) ? (*(uint32_t *)((uint32_t)(&(UART0->BAUD)) + UART_ID * 0x4000)) : (*(uint32_t *)((uint32_t)(&(UART4->BAUD)) + (UART_ID - 4) * 0x4000)))
#define UART_STA  ((UART_ID < 4) ? (*(uint32_t *)((uint32_t)(&(UART0->STA)) + UART_ID * 0x4000)) : (*(uint32_t *)((uint32_t)(&(UART4->STA)) + (UART_ID - 4) * 0x4000)))
#define UART_TXD  ((UART_ID < 4) ? ((uint32_t *)((uint32_t)(&(UART0->TXD)) + UART_ID * 0x4000)) : ((uint32_t *)((uint32_t)(&(UART4->TXD)) + (UART_ID - 4) * 0x4000)))
#define UART_RXD  ((UART_ID < 4) ? (*(uint32_t *)((uint32_t)(&(UART0->RXD)) + UART_ID * 0x4000)) : (*(uint32_t *)((uint32_t)(&(UART4->RXD)) + (UART_ID - 4) * 0x4000)))

/* ra9103 uart send character */
void sendchar(uint8_t c){
	uint32_t * u_txd;
	u_txd = UART_TXD;
	
	if (c == '\n'){
    while (UART_STA & 0x80)
      ;
    *u_txd = 0x0D;                          /* output CR */
  }
  
	while(UART_STA & 0x80)
	  ;
	*u_txd = c;
  while(UART_STA & 0x80)
	  ;
}

/* ra9103 uart get character */
int getkey(void){
	while(!(UART_STA & 0x1))
		;
	return UART_RXD;
}
