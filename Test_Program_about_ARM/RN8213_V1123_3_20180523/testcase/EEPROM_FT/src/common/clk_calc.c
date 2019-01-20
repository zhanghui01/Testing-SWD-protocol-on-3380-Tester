#include "clk_calc.h"
#include "common.h"

// float  clk_freq(CALC_CLK clk)
// {
//   uint32_t  ref_cnt;
//   
//   SYSCTL->SYS_PS = 0x82;
//   if(clk == CALC_RCH)
//   {
//     SYSCTL->OSC_CTRL1 &= (~(1 << 1));
//   }
//   
//   ref_cnt = 32768;
//   while(1)
//   {
//     SYSCTL->TRIM_START = (3 << 24);
//     SYSCTL->TRIM_CFG1 = (clk << 26) | 
//                         (0 << 24) |    //use losc as reference clk
//                         (ref_cnt << 0);
//     SYSCTL->TRIM_START = (1 << 26);
//     while(!(SYSCTL->TRIM_START & (0x3 << 24)))
//     {
//       WdtFeed();
//     }
//     
//     if(SYSCTL->TRIM_START & (1 << 25))
//     {
//       ref_cnt /= 2;
//       if(ref_cnt == 0)
//       {
//         break;
//       }
//     }
//     else
//     {
//       break;
//     }
//   }
//   
//   return (32768.0 * (float)(SYSCTL->TRIM_START & (0xfffff))) / ((float)ref_cnt);
// }
