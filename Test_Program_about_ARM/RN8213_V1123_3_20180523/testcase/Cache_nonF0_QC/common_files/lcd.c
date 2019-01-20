#include "lcd.h"
#include "common.h"

#define NHTN12095


#ifdef  NHTN12095   //3-phase, 8*32 LCD
/*
数据格式：8COM，数据寄存器存放的是
  {SEG[i] COM7, SEG[i] COM6, SEG[i] COM5, SEG[i] COM4, SEG[i] COM3, SEG[i] COM2, SEG[i] COM1, SEG[i] COM0}
	P6:seg[7:0]  P7:seg[15:8]  P8:seg[23:16]  P9:seg[31:24]  P10:seg[35:32]
8*17屏数据为buf[17]

硬件连接的SEG顺序为：
lcd：0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
pad：71,70,67,66,97,96,95,94,93,92,91,90,87,86,85,84
seg: 9  8  7  6  31 30 29 28 27 26 25 24 23 22 21 20
                                                            
     
lcd：16 17 18 19 20  21 22 23 24 25 26 27 28 29 30 31    
pad：63,62,61,60,64, 72,73,74,75,76,77,80,81,82,83,65
seg: 3  2  1  0  4   10 11 12 13 14 15 16 17 18 19 5
     0     1      2     3     4     5     6     7               
*/
const uint8_t DIG_BUF[][2] = {{3,2},{1,0},{4,10},{11,12},{13,14},{15,16},{17,18},{19,5}};
const uint8_t DIG[] = {
  ((0xd << 0) | (0x7 << 4))  //"0"
 ,((0x0 << 0) | (0x6 << 4))  //"1"
 ,((0xe << 0) | (0x3 << 4))  //"2"
 ,((0xa << 0) | (0x7 << 4))  //"3"
 ,((0x3 << 0) | (0x6 << 4))  //"4"
 ,((0xb << 0) | (0x5 << 4))  //"5"
 ,((0xf << 0) | (0x5 << 4))  //"6"
 ,((0x0 << 0) | (0x7 << 4))  //"7"
 ,((0xd << 0) | (0x7 << 4))  //"8"
 ,((0xb << 0) | (0x7 << 4))  //"9"
};

#else               //single-pahse, 8*17 LCD
/*
硬件连接的SEG顺序为：
lcd：0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16
pad：20,21,22,23,0, 9, 26,10,11,27,12,28,29,13,14,15,16
     8     7     6     0/5   4     3     2     1       
*/
const uint8_t DIG_BUF[][2] = {{26,10},{14,15},{29,13},{12,28},{11,27},{26,10},{0,9},{22,23},{20,21}};

const uint8_t DIG[] = {
  ((0x7 << 0) | (0x5 << 4))
 ,((0x0 << 0) | (0x5 << 4))
 ,((0xd << 0) | (0x3 << 4))
 ,((0x9 << 0) | (0x7 << 4))
 ,((0x2 << 0) | (0x7 << 4))
 ,((0xb << 0) | (0x6 << 4))
 ,((0xf << 0) | (0x6 << 4))
 ,((0x1 << 0) | (0x5 << 4))
 ,((0xf << 0) | (0x7 << 4))
 ,((0xb << 0) | (0x7 << 4))
};
#endif

void set_digital(uint8_t n, uint8_t d, uint8_t p)
{
  if((n > 8) || (d > 9))
    return;

#ifndef  NHTN12095
  if(n == 0)
  {
    LCD->BUF[DIG_BUF[n][1]] |= (DIG[d] & 0xf);
    LCD->BUF[DIG_BUF[n][0]] |= ((DIG[d] & 0xf0) >> 4);
  }
  else
#endif
  {
    LCD->BUF[DIG_BUF[n][1]] |= ((DIG[d] & 0xf) << 4);
    LCD->BUF[DIG_BUF[n][0]] |= (DIG[d] & 0xf0);
  }
  
}

void lcd_pad_init(void)
{
	mod_clk(MOD_GPIO, MOD_ON);
	GPIO->PCB |= (0xffffu << 16);
	GPIO->PCC |= (0xffffu << 0);
	GPIO->PCE = 0xf;	
}

void lcd_init(void)
{
	lcd_pad_init();
	
	mod_clk(MOD_LCD, MOD_ON);
	LCD->CLKDIV = 0x1f;        //64Hz
	LCD->BLINK = 1;
	LCD->CTRL = (0x23 << 5) |  // Vd = 4 * Vref*(1+BIASLVL[4:0]/63), Vd = 5v, Vref = 0.8
	            (1 << 4) |     // 1/4 bias
	            (5 << 1) |     // 1/8 duty
	            (1 << 0);  
  lcd_full_buf();
}

void lcd_clr_buf(void)
{
	uint32_t i;
	
	for(i = 0; i < 40; i++)
	{
		LCD->BUF[i] = 0x0;
	}
}

void lcd_full_buf(void)
{
	uint32_t i;
	
	for(i = 0; i < 40; i++)
	{
		LCD->BUF[i] = 0xff;
	}
}

void lcd_half_buf(void)
{
	uint32_t i;
	
	for(i = 0; i < 40; i++)
	{
		LCD->BUF[i] = 0x55;
	}
}
