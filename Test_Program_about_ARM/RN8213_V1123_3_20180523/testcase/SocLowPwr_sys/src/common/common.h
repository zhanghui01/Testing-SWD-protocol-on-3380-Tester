#ifndef _COMMON_H
#define _COMMON_H

#include <rn821x.h>

typedef enum {MOD_SYS_TC = 0, MOD_EEPROM = 2, MOD_CP = 3, MOD_TC0 = 4, MOD_TC1 = 5, MOD_UART4 = 6, \
              MOD_UART5 = 7, MOD_UART0 = 8, MOD_UART1 = 9, MOD_UART2 = 10, MOD_UART3 = 11, MOD_IR = 12,\
              MOD_7816 = 13, MOD_IIC = 14, MOD_SPI = 15, MOD_DMA0 = 32, MOD_DMA1 = 33, MOD_DMA = 36, \
              MOD_GPIO = 37, MOD_LCD = 38, MOD_EMU = 39, MOD_NVM = 40, MOD_WDT = 41, MOD_RTC = 42, MOD_SAR = 43} MOD_CLK_ID;
typedef enum  {MOD_OFF = 0, MOD_ON = 1} MOD_CLK_OP;

// #define DEBUG
#ifdef DEBUG
  #include <stdio.h>
  #define MSG(args...) printf(##args)
#else
  #define MSG(format, args...)
#endif

/* modify uart 0/1/2/3/4/5 according to Hardware */
#define UART_ID 0

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
void mDelay(uint32_t ms);
void WdtInit(void);
void WdtFeed(void);

void mod_clk(MOD_CLK_ID mod, MOD_CLK_OP op);

/* address for saving testcase result */
#define RESULT_ADDR 0x10001800
void exitPass(void);
void exitFail(uint32_t res);

extern  uint32_t  tc0_flag;
void tcInit(uint32_t cycles);

#if 0
/*__attribute__((always_inline)) */void systickOn(){
  SysTick->LOAD = 0xffffff;
  SysTick->VAL = 0x0;
  SysTick->CTRL = 0x5;
  return;
}

/*__attribute__((always_inline)) */void systickOff(){
  SysTick->CTRL = 0x0;
  return;
}

/*__attribute__((always_inline)) */uint32_t systickRead(){
  return SysTick->VAL;
}
#endif


//P23/22 as timer GPIO
#define TRIG_INIT()  SYSCTL->SYS_PS = 0x82; \
                     SYSCTL->MOD1_EN |= (1 << 5);\
                     GPIO->PCA0 &= (~(1 << 26));\
                     GPIO->PMA &= (~(3 << 18));\
                     GPIO->PA &= (~(3 << 18))
	
#define TRIG_HIGH()  GPIO->PA |= (3 << 18)
#define TRIG_LOW()   GPIO->PA &= (~(3 << 18))


#define  GPIO_INIT  SYSCTL->SYS_PS = 0x82;\
                    SYSCTL->MOD1_EN |= (1 << 5);\
                    GPIO->PCA0 &= (~((1 << 26) | (1 << 28)) | (1 << 0) | (1 << 2));\
                    GPIO->PMA &= (~((0x3 << 18) | (0x3 << 22) | (0x3 << 0)));\
                    GPIO->PA &= (~((3 << 18) | (3 << 22) | (0x3 << 0)))

#define  P22_HIGH   GPIO->PA |= (1 << 18)
#define  P22_LOW    GPIO->PA &= (~(1 << 18))

#define  P23_HIGH   GPIO->PA |= (1 << 19)
#define  P23_LOW    GPIO->PA &= (~(1 << 19))
#define  P23_TOGGLE GPIO->PA = (GPIO->PA & (~(1 << 19))) | ((~(GPIO->PA & (1 << 19))) & (1 << 19))

#define  P26_HIGH   GPIO->PA |= (1 << 22)
#define  P26_LOW    GPIO->PA &= (~(1 << 22))

#define  P27_HIGH   GPIO->PA |= (1 << 23)
#define  P27_LOW    GPIO->PA &= (~(1 << 23))

#define  P00_HIGH   GPIO->PA |= (1 << 0)
#define  P00_LOW    GPIO->PA &= (~(1 << 0))
#define  P00_TOGGLE GPIO->PA = (GPIO->PA & (~(1 << 0))) | ((~(GPIO->PA & (1 << 0))) & (1 << 0))

#define  P01_HIGH   GPIO->PA |= (1 << 1)
#define  P01_LOW    GPIO->PA &= (~(1 << 1))
#define  P01_TOGGLE GPIO->PA = (GPIO->PA & (~(1 << 1))) | ((~(GPIO->PA & (1 << 1))) & (1 << 1))


#endif
