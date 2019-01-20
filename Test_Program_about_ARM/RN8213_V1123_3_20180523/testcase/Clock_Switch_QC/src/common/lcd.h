#ifndef __LCD_H
#define __LCD_H

#include "rn821x.h"

void set_digital(uint8_t n, uint8_t d, uint8_t p);
void lcd_init(void);
void lcd_clr_buf(void);
void lcd_half_buf(void);
void lcd_full_buf(void);

#endif
