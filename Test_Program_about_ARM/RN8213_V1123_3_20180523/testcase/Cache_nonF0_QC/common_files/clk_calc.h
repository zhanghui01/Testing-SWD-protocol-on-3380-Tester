#ifndef __CLK_CALC_H
#define __CLK_CALC_H

#include "ra9103.h"

typedef enum {CALC_RCH = 0, CALC_RCL = 1} CALC_CLK;

float  clk_freq(CALC_CLK clk);

#endif
