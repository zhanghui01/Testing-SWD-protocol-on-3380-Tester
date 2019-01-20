 #include "drv.h"

uint32_t SAR_QC(void)
{
  uint32_t  data ;
	
	swd_connect(1);
	
  data = 0x82;
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PS)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
	
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->SYS_PD)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC SYS_PD fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  data &= (~(1 << 6)) ;       //BGR 
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PD)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC SYS_PD fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->SYS_PD)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC SYS_PD fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  DBG_MSG("SYS_PD:0x%x\n", data);	
  
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  data |= (1 << 11) ;       //SAR apb 
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC MOD1_EN fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  DBG_MSG("mod1_en:0x%x\n", data);	
		
	
	usDelay(50000);  //50ms
	
  data = (0xe << 7) | (1 << 0);
  if(swd_mem_wr32((unsigned long)(&(MADC->AD_CTRL)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC AD_CTRL fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
	
  data = 1;
  if(swd_mem_wr32((unsigned long)(&(MADC->AD_STAT)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC AD_STAT fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
	
  data = 1;
  if(swd_mem_wr32((unsigned long)(&(MADC->AD_START)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC AD_START fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
	
	while(1)
	{
		if(swd_mem_rd32((unsigned long)(&(MADC->AD_STAT)),(unsigned long *)&data,1) != SWD_RESP_OK){
      DBG_ERR("Write SOC PS fail\n");
      return SWD_RESP_PROTOCAL_ERROR;
    }
		if(data & 0x1)
		{
			break;
		}
	}
	
	if(swd_mem_rd32((unsigned long)(&(MADC->AD_DATA)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
	
	if((data < VBAT_MIN) || (data > VBAT_MAX))
	{
		return 1;//FUNTION  CASE
// 		return FAIL;
	}
	
		return 0;//FUNTION  CASE
// 		return PASS;
}
