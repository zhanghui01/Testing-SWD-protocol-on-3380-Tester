#ifndef _COMMON_H
#define _COMMON_H

#include <RN821x.h>

#define DEBUG
#ifdef DEBUG
  #include <stdio.h>
  #define MSG(args...) printf(##args)
#else
  #define MSG(format, args...)
#endif

/* modify uart 0/1/2/3/4/5 according to Hardware */
#define UART_ID 0
#define RA9103A0

#define ClrBit(reg, bit)           ((reg) &= (~(1 << (bit))))
#define SetBit(reg, bit)           ((reg) |= (1 << (bit)))

#define SetValue(reg, start, depth, value) (reg) = (((reg) << (32 - (start))) >> (32 - (start))) | (((reg) >> ((start) + (depth))) << ((start) + (depth))) | ((value) << (start))

uint32_t getSysclk(void);
extern uint32_t sysclk_loops;
uint32_t sysclkLoop(void);

void uartInit(uint32_t id, uint32_t baud, uint32_t rxie, uint32_t txie, uint32_t errie);
uint32_t setBaud(uint32_t id, uint32_t value);
void sendStr(uint32_t id, uint8_t *str);
void sendChar(uint32_t id, uint8_t c);

void uDelay(uint32_t time);
void swDelay_soft(uint32_t ulTime) ;
void WdtInit(uint8_t irq_en);
void WdtFeed(void);
void WDT_OFF(void) ;

/* address for saving testcase result */
#define RESULT_ADDR 0x10001800
void exitPass(void);
void exitFail(uint32_t res);

__attribute__((always_inline)) void systickOn(){
  SysTick->LOAD = 0xffffff;
  SysTick->VAL = 0x0;
  SysTick->CTRL = 0x5;
  return;
}

__attribute__((always_inline)) void systickOff(){
  SysTick->CTRL = 0x0;
  return;
}

__attribute__((always_inline)) uint32_t systickRead(){
  return SysTick->VAL;
}

#endif
