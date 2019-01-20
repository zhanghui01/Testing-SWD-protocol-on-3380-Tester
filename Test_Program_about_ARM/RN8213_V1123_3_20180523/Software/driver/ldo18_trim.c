#include "drv.h"
uint32_t  trim_ANALDO = 0x0;

static uint32_t  ldo18_trim_write(uint32_t trim)
{
  uint32_t temp; 
  temp = 0x75;
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PS)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->ANA_LDO)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
	//trim write
 if( powswh_bin == 1 )
		{
			temp = 0xa5580000 + ((((temp & (~(0x1f << 7))) | ((trim & 0x1f) << 7))) & 0xffff);
			trim_ANALDO = (temp & 0xffff);
			if(swd_mem_wr32((unsigned long)(&(SYSCTL->ANA_LDO)),(unsigned long *)&temp,1) != SWD_RESP_OK)
			{
				DBG_ERR("Write SOC ANA_LDO fail\n");
				return SWD_RESP_PROTOCAL_ERROR;
			}
				DBG_MSG("PWRSW_BIN1 trim_ANALDO= %x,SPL0[10]=%x \n",trim_ANALDO,(( ANALDO_B & 0xf06f)  | (trim_ANALDO & 0x0f90) ));
		}
   else if( powswh_bin == 2 )
		{
			temp = 0xa5580000 + ((((temp & (~(0x1f << 7))) | ((trim & 0x1f) << 7))) & 0xffef);
			trim_ANALDO = (temp & 0xffff);
			if(swd_mem_wr32((unsigned long)(&(SYSCTL->ANA_LDO)),(unsigned long *)&temp,1) != SWD_RESP_OK)
			{
				DBG_ERR("Write SOC ANA_LDO fail\n");
				return SWD_RESP_PROTOCAL_ERROR;
			}
				DBG_MSG("PWRSW_BIN2 trim_ANALDO= %x,SPL0[10]=%x \n",trim_ANALDO,(( ANALDO_B & 0xf06f)  | (trim_ANALDO & 0x0f90) ));
		}
  
  return SWD_RESP_OK;
}

uint32_t  trim_ldo18 = 0x10;
uint32_t  ldo18_trim(void)
{
  float    vldo18;
  
  swd_connect(0);
//  ADS1243Init();  //164.364ms
  adsInit();

  adsInitvdd(CHAN_RA2);
  
//  	for(i=0;i<5;i++)
//	{
//		mem_wr_reg32(MADC_AD_START,0x1);	
//		swDelay(50);    //Delay 300ms for stable.
//	  ad_data[i] = adsAcquire(); 		
//		
//		if(ad_data[i] == 0xffffffff)
//		{
//			result = FAIL;
//		  return result;	
//		}
//		
//		while(!(mem_rd_reg32(MADC_AD_STAT) & 0x1));
//		mem_wr_reg32(MADC_AD_STAT,0x1);		
//	}
//	
//	ad_aver = average(ad_data,5);
  
  
  
  
  while(1)
  {
    vldo18 = ((float)(ADS1243GetValue(CHAN_RA2)) * ADS_VERF) / ((float)(0xffffff));
  
    trim_ldo18 = (signed)trim_ldo18 + (signed)((1.8 - vldo18) / 0.005);
    if(trim_ldo18 & 0x80000000)
    {
      trim_ldo18 = 0;
    }
    else if(trim_ldo18 > 0x1f)
    {
      trim_ldo18 = 0x1f;
    }
    while(ldo18_trim_write(trim_ldo18) != SWD_RESP_OK)
    {
      ;
    }
    
    vldo18 = ((float)(ADS1243GetValue(CHAN_RA2)) * ADS_VERF) / ((float)(0xffffff));
    
    if(((vldo18 > 1.8) && ((vldo18 - 1.8) < 0.01)) ||
       ((vldo18 < 1.8) && ((1.8 - vldo18) < 0.01)) ||
       (vldo18 == 1.8))
    {
      break;
    }
    
    if((trim_ldo18 == 0) || (trim_ldo18 == 0x1f))
    {
			if((vldo18 > 1.89) ||(vldo18 < 1.71) )
			{
			 exitFail(1) ;
			}
      DBG_ERR("ldo18 trim = %d, VLDO18 = %f\n", trim_ldo18, vldo18);
      break;
    }
    
  }
  
  return 0;
}

