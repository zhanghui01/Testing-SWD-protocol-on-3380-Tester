#include "drv.h"

uint32_t  trim_rcl = 0x9;
uint32_t  rcl_trim(void)
{
  uint32_t temp;
  uint32_t hosc_cnt = 327670*3;  //30ms
    
  swd_connect(1);
  
  temp = 0x82;
  while(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PS)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC PS fail\n");
  }
  
  while(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Rd MOD1 fail\n");
  }
  temp |= (1 << 5);  //enable GPIO
  while(swd_mem_wr32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write MOD1 fail\n");
  }

  while(swd_mem_rd32((unsigned long)(&(GPIO->PCA1)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Rd PCA1 fail\n");
  }
  temp |= (0xf << 12);  //HOSC PAD configuration
  while(swd_mem_wr32((unsigned long)(&(GPIO->PCA1)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write PCA1 fail\n");
  }
  
  while(swd_mem_rd32((unsigned long)(&(SYSCTL->OSC_CTRL2)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Rd PCA1 fail\n");
  }
  temp &= (~(0x1 << 3));  //Enable HOSC
  while(swd_mem_wr32((unsigned long)(&(SYSCTL->OSC_CTRL2)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write PCA1 fail\n");
  }

  temp = (1 << 24) | (1 << 25);  //clear status
  while(swd_mem_wr32((unsigned long)(&(SYSCTL->TRIM_START)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC TRIM_START fail\n");
  }
  
  temp = (hosc_cnt  << 0) | (1 << 24) | (1 << 26);  //ref clk=hosc, calc clk=rcl
  while(swd_mem_wr32((unsigned long)(&(SYSCTL->TRIM_CFG1)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC TRIM_START fail\n");
  }
  
  temp = (1 << 24) | (1 << 25) | (1 << 26);
  while(swd_mem_wr32((unsigned long)(&(SYSCTL->TRIM_START)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC TRIM_START fail\n");
  }
  
  while(1)
  {
    while(swd_mem_rd32((unsigned long)(&(SYSCTL->TRIM_START)),(unsigned long *)&temp,1) != SWD_RESP_OK)
    {
      DBG_ERR("Read SOC TRIM_START fail\n");
    }
    
    if(temp & (1 << 24))
    {
      break;
    }
  }
  
 
  trim_rcl =((float)(temp & 0xfffff) * 29491200.0 / (float)((float)(hosc_cnt+1))) ;  //31K
			if((trim_rcl > 37200) ||(trim_rcl < 24800) )//RCL:[24.8,37.2]KHz
			{
			 exitFail(1) ;
			}
			
  trim_rcl = ( trim_rcl * 8) / 32768;
			if((trim_rcl > 9) ||(trim_rcl < 6) )//TRIM_RCL:[6,9]
			{
			 exitFail(1) ;
			}
			
	return 0;
}

