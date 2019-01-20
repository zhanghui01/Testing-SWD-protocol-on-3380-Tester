#ifndef  __UART_CMD_H
#define  __UART_CMD_H

#include "ra9103.h"

uint32_t ascii2hex(uint8_t *str, uint32_t *hex);

#define  UART_MAX_CHAR  10
typedef struct{
	uint8_t  cmd[UART_MAX_CHAR+1];
	uint8_t  para1[UART_MAX_CHAR+1];
	uint8_t  para2[UART_MAX_CHAR+1];
	uint8_t  cmd_done;
}UART_CMD_T;
extern UART_CMD_T  uart_s;

#endif
