#include "uart_cmd.h"

UART_CMD_T  uart_s;

uint8_t * rx_addr = uart_s.cmd;
uint8_t rec = 0;
uint8_t idx = 0;
void UART0_HANDLER(void)
{
	uint8_t  data;
	
	if(UART0->STA & (1 << 0))
	{
		UART0->STA = 1 << 0;
		data = UART0->RXD;
		
		if(data == ' ')
		{
			if(rec != data)
			{
				rx_addr[idx] = 0x0;
				idx = 0;
				if(rx_addr == uart_s.para1)
				{
					rx_addr = uart_s.para2;
				}
				else
				{
					rx_addr = uart_s.para1;
				}
			}
		}
		else if((data == '\r') || (data == 0xa) || (data == 0xd))
		{
			if((rec != '\r') && (rec != 0xa) && (rec != 0xd))
			{
			  uart_s.cmd_done = 1;
			  rx_addr[idx] = 0x0;
			  idx = 0;
			  rx_addr = uart_s.cmd;
			}
		}
		else
		{
			if(idx > (UART_MAX_CHAR - 1))
			{
				idx = 0;
			}		
			rx_addr[idx] = data;
			idx++;
		}
		rec = data;
	}
}


uint32_t ascii2hex(uint8_t *str, uint32_t *hex)
{
	uint32_t i;
	
	*hex = 0;
	for(i = 0; (str[i] != 0) || (i > 8); i++)
	{
		if((str[i] > 0x2f) && (str[i] < 0x3a))
		{
			*hex = ((*hex) << 4) | (str[i] - '0');
		}
		else if((str[i] > 0x60) && (str[i] < 0x67))
		{
			*hex = ((*hex) << 4) | (str[i] - 'a' + 0xa);
		}
		else if((str[i] > 0x40) && (str[i] < 0x47))
		{
			*hex = ((*hex) << 4) | (str[i] - 'A' + 0xa);
		}
		else
		{
			i = 9;
		}
	}
	
	if(i > 8)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
