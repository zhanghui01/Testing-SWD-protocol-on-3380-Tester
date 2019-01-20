#include "sar.h"
#include "common.h"

float  sar_get(uint32_t ch)
{
  uint32_t  i, pga = 3, data;
  
  SYSCTL->SYS_PS = 0x82;
  SYSCTL->SYS_PD &= (~(1 << 6));
  
  mod_clk(MOD_SAR, MOD_ON);
  
  if(ch > 1)
  {
    mod_clk(MOD_GPIO, MOD_ON);
    if(ch < 4)
    {
      GPIO->PCA0 &= (~(0x3 << (2 * (ch - 2))));
    }
    else
    {
      GPIO->PCA0 &= (~(1 << ch));
    }
  }
  
  while(1)
  {
    MADC->SAR_CTRL = (0xE << 7) | (pga << 3) | (ch << 0);
    while(MADC->SAR_STAT & (1 << 1))
    {
      ;
    }
    MADC->SAR_STAT = 1;
    MADC->SAR_START = 1;
    while(!(MADC->SAR_STAT & 1))
    {
      ;
    }
    data = MADC->SAR_DATA;
    if(data > 0x3f0)
    {
      if(pga == 0)
      {
        break;
      }
      pga--;
    }
    else
    {
      break;
    }
  }
  
  return ((float)data) / (0.5 * ((float)(pga+1)));
}
