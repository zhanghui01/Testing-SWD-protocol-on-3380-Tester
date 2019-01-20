#define _RA0_ROUTINE_GLOBALS

#include "ra0_routine.h"
#include "drv.h"

uint32_t sumGen32(uint32_t* p_data,uint32_t length)
{
  uint32_t sum;
  uint32_t i;
  
  sum = 0x0;
  
  for(i=0;i<length;i++)
  {
    sum = sum + *(p_data+i);   
  }
  
  return sum;
}

/*********************************************************************************************************
** Function name:       average
** Descriptions:        mercily software delay
** input parameters:    delay duration
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
uint32_t average(uint32_t * pdata,uint32_t num) 
{
    uint32_t i;
    uint32_t result = 0x0;

    for(i=0;i<num;i++)
    {
      result += *(pdata+i);
    }
    
    result = result / num;
    
    return result;
}

/*********************************************************************************************************
** Function name:       swDelay for 1 ms
** Descriptions:        Software Delay
** input parameters:    None
** output parameters:   None
** Returned value:      None
*********************************************************************************************************/
void swDelay(uint16_t delay_time)
{
  uint16_t cur_time;
  
  cur_time = sys_stamp ? sys_stamp : (sys_stamp - 1);

  while(1)
  {
  if(stampThrough(cur_time) >= delay_time)
    {
      return;
    }
  }
}

uint32_t max2min (uint32_t * pdata,uint32_t num) 
{
    uint32_t i;
	  uint32_t max;
	  uint32_t min;
	  uint32_t max2min;

    max = *(pdata+0);
	  min = *(pdata+0);
	
	  for(i=0;i<num;i++)
    {
      if(*(pdata+i) > max)
			{
				max = *(pdata+i);
			}
      if(*(pdata+i) < min)
			{
				min = *(pdata+i);
			}			
    }
		
		max2min = max - min;

    return max2min;
}

uint8_t swdInitial(uint8_t swd_id)
{
	//Swd Initialize
	swd_num = swd_id;
	
	swd_drv_init();
	if (swd_connect(0) == SWD_RESP_OK)
	{
    ra9103_initail();
	}			
	else
	{
		return FAIL;
	}
	
  if(swd_id == SWD_SERIAL)
	{
    mem_wr_reg32(SYS_SYS_PS,0x82);
		//mem_wr_reg32(SYS_SYS_PD,0x38);      //BGR is on.
	  mem_wr_reg32(SYS_SYS_CTRL,0x3);		 
		mem_wr_reg32(SYS_SYS_MODE,0xe);	 // switch to RCH
			
		if((mem_rd_reg32(SYS_OSC_CTRL1)&0x20) != 0x20)
		{
			return FAIL;
		}				
			
		mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_TW_OFFSET) ,TW_3M);
		mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_TUS_OFFSET) ,TUS_3M);		
		mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_TMS_OFFSET) ,EEPROM_TMS_PAGE);		
		mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_TP_OFFSET) ,EEPROM_TP);		
		mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_TE_OFFSET) ,EEPROM_TE);		
	
		mem_wr_reg32((FLASH_BASE_REMAP3+FLASH_TW_OFFSET) ,TW_3M);
		mem_wr_reg32((FLASH_BASE_REMAP3+FLASH_TUS_OFFSET) ,TUS_3M);		
		mem_wr_reg32((FLASH_BASE_REMAP3+FLASH_TMS_OFFSET) ,FLASH_TMS_PAGE);		
		mem_wr_reg32((FLASH_BASE_REMAP3+FLASH_TP_OFFSET) ,FLASH_TP);		
		mem_wr_reg32((FLASH_BASE_REMAP3+FLASH_TE_OFFSET) ,FLASH_TE);	
	}				
	else
	{
	  if((mem_rd_reg32(SYS_OSC_CTRL1)&0x70) != 0x40)
	  {
		  return FAIL;
	  }		
		
	  mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_TW_OFFSET) ,TW_7M);
	  mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_TUS_OFFSET) ,TUS_7M);		
    mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_TMS_OFFSET) ,EEPROM_TMS_PAGE);		
    mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_TP_OFFSET) ,EEPROM_TP);		
    mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_TE_OFFSET) ,EEPROM_TE);		
	
	  mem_wr_reg32((FLASH_BASE_REMAP3+FLASH_TW_OFFSET) ,TW_7M);
	  mem_wr_reg32((FLASH_BASE_REMAP3+FLASH_TUS_OFFSET) ,TUS_7M);		
    mem_wr_reg32((FLASH_BASE_REMAP3+FLASH_TMS_OFFSET) ,FLASH_TMS_PAGE);		
    mem_wr_reg32((FLASH_BASE_REMAP3+FLASH_TP_OFFSET) ,FLASH_TP);		
    mem_wr_reg32((FLASH_BASE_REMAP3+FLASH_TE_OFFSET) ,FLASH_TE);			
	}	
	
	mem_wr_reg32(SYS_SYS_PS,0x75);
	mem_wr_reg32(SYS_SYS_CTRL,0x13);
	
	return SUCCESS;
}

void ra9103ADCInit(void)
{
	mem_wr_reg32(SYS_SYS_PS,0x75);
	mem_wr_reg32(SYS_ANA_BGR,0xa5280000|ANABGR);
	mem_wr_reg32(SYS_SYS_PS,0x82);
	mem_wr_reg32(SYS_SYS_PD,0x38);      //BGR is on.
//	mem_wr_reg32(SYS_MOD1_EN,0xc20);   //SAR,RTC,GPIO
  mem_wr_reg32(SYS_MOD1_EN,0xca0);   //SAR,RTC,GPIO,EMU
  mem_wr_reg32(GPIO_PCA0,(mem_rd_reg32(GPIO_PCA0) | 0x5));
	mem_wr_reg32(MADC_AD_TEST,0xa00);
  mem_wr_reg32(MADC_AD_TEST,0xa2f);   //BIASMODE=0x2,DEMEN=0x1,IBIAS=0x1,TEST=0x3.
	mem_wr_reg32(MADC_AD_CTRL,0xf98);    //PGA=2. Channel=VIN0.   
}

uint32_t ra0TempAdjust(uint16_t std_temper)
{
  uint8_t i,j;
  uint32_t ad_data[5]; 
	uint32_t ad_aver;
//  uint32_t din_aver;
//	uint32_t temper;
	uint32_t result;
	uint32_t tpsin[5];
	uint32_t tpsin_avg;
	uint32_t tpsin_max2min;
// 	uint32_t eeprom_tadj;
//	int tadj_maxmin;
	
// 	eeprom_tadj = mem_rd_reg32(SPL1_ADDR+0x1c) & 0xffff;
	
	mem_wr_reg32(SYS_SYS_PS,0x75);
	mem_wr_reg32(SYS_ANA_BGR,0xa5280000|ANABGR);
	if((mem_rd_reg32(SYS_ANA_BGR)!=ANABGR))
	{
		DBG_ERR("bgr=%x\n", mem_rd_reg32(SYS_ANA_BGR));
		result = FAIL;
		return result;			
	}
	mem_wr_reg32(SYS_SYS_PS,0x82);
	
	mem_wr_reg32(SYS_MOD1_EN,0xc20);   //SAR,RTC,GPIO
	//mem_wr_reg32(SYS_SYS_PD,0x3f);      //BGR is on.
  //mem_wr_reg32(SYS_MOD1_EN,0xca0);   //SAR,RTC,GPIO,EMU
  mem_wr_reg32(GPIO_PCA0,(mem_rd_reg32(GPIO_PCA0) | 0x5));
	mem_wr_reg32(MADC_AD_TEST,0xa00);
  mem_wr_reg32(MADC_AD_TEST,0xa2f);   //BIASMODE=0x2,DEMEN=0x1,IBIAS=0x1,TEST=0x3.
	mem_wr_reg32(MADC_AD_CTRL,0xf98);    //PGA=2. Channel=VIN0.
  swDelay(200);
	adsInit();
	
	if(mem_rd_reg32(MADC_AD_TEST) != 0xa2f)
	{
	  DBG_ERR("ADTEST verify err!\n");
		return 0xffffffff;
	}
	
	if(mem_rd_reg32(MADC_AD_CTRL) != 0xf98)
	{
	  DBG_ERR("ADCTRL verify err!\n");
		return 0xffffffff;
	}
    adsAcquireInit(CHAN_RA1);	
	
	//Measure VBE(P00,P01) When IBIAS = 16uA.
	for(i=0;i<5;i++)
	{
		mem_wr_reg32(MADC_AD_START,0x1);	
		swDelay(50);    //Delay 300ms for stable.
	  ad_data[i] = adsAcquire(); 		
		
		if(ad_data[i] == 0xffffffff)
		{
			DBG_ERR("ads1243 get wrong vbe!\n");
			result = FAIL;
		  return result;		
		}
		
		while(!(mem_rd_reg32(MADC_AD_STAT) & 0x1));
		mem_wr_reg32(MADC_AD_STAT,0x1);		
	}
	
	ad_aver = average(ad_data,5);
	DBG_MSG("\tad_aver=%d(%f),", ad_aver,((float)ad_aver)/240.76-278.0);  //cgf @20170810
	ad_aver = (ad_aver * 8) / 240.76;            //Precision is 0.125 degree.	
	ad_aver = ad_aver + CHIP_TEMPERTRIM_OFFSET;

	if(((ad_aver - (RTCZT * 8) - TEMPER_OFFSET) > (std_temper * 2 + MACHINE_TEMP_DIFF)) || ((ad_aver - (RTCZT * 8) - TEMPER_OFFSET) < (std_temper * 2 - MACHINE_TEMP_DIFF)))   //Error is larger than +-1 degree.
	{ 
		DBG_ERR("Tvbe=%f, Tstandard=%f\n", (float)(ad_aver - (RTCZT * 8) - TEMPER_OFFSET)/8.0, (float)std_temper / 4.0);
		result = FAIL;
		return result;
	}		

	if(mem_rd_reg32(MADC_AD_TEST)!=0xa2f)
	{
		DBG_ERR("test=%x\n", mem_rd_reg32(MADC_AD_TEST));
		result = FAIL;
		return result;			
	}	
	
	mem_wr_reg32(GPIO_PCA0,(mem_rd_reg32(GPIO_PCA0) & 0xfffffff8));
	
  mem_wr_reg32(RTC_CTRL,0x100);
	
  while(mem_rd_reg32(RTC_CTRL) & 0x200);		
  mem_wr_reg32(RTC_CTRL,0x1c9);   //TSE = 0B01
  while(mem_rd_reg32(RTC_CTRL) & 0x200);	
  
	mem_wr_reg32(MADC_AD_TEST,0xa00);
  mem_wr_reg32(MADC_AD_TEST,(0xa08 | (2<<4)));	//16uA		

 	if(mem_rd_reg32(RTC_CTRL) != 0x1c9)
	{
	  DBG_ERR("RTC_CTRL verify err!\n");
		return 0xffffffff;
	}
	if(mem_rd_reg32(MADC_AD_TEST) != (0xa08 | (2<<4)))
	{
	  DBG_ERR("AD_TEST verify err!\n");
		return 0xffffffff;
	}
	
		for(j=0;j<2;j++)  //skip the first measurement cgf@20170810
  {
    mem_wr_reg32(RTC_TEMP,0x00);
		swDelay(10);
    while(mem_rd_reg32(RTC_CTRL) & 0x600);		   
  }	
	
  for(j=0;j<5;j++)
  {
    mem_wr_reg32(RTC_TEMP,0x00);
		swDelay(50);
    while(mem_rd_reg32(RTC_CTRL) & 0x600);				
            
    tpsin[j] = mem_rd_reg32(RTC_TPSIN);    
  }	
	
	tpsin_avg = average(tpsin,5);
	tpsin_max2min = max2min(tpsin,5);
	DBG_MSG("tpsin=%d, diff=%2d,", tpsin_avg, tpsin_max2min);  //cgf @20170810
// 	DBG_MSG("(%d,%d,%d,%d,%d),", tpsin[0],tpsin[1],tpsin[2],tpsin[3],tpsin[4]);  //cgf @20170811 	
	if(tpsin_max2min > 15)
	{
		DBG_ERR("tpsin not stable!!(%d)\n", tpsin_max2min);
		tpsin_avg = 0xffffffff;
		result = FAIL;
		return result;			
	}
	
  if(((ad_aver * 8192) / (tpsin_avg + 1021 * 4)) & 0x1)
	{
		tadj = (ad_aver * 4096) / (tpsin_avg + 1021 * 4) + 1;
	}
	else
	{
		tadj = (ad_aver * 4096) / (tpsin_avg + 1021 * 4);
	}
		//==============
		DBG_MSG("tadj=%x,", tadj);  //cgf @20170810
				if((tadj > 0x6b0) ||(tadj < 0x650) )//tadj:[0x650,0x6b0]
			{
				DBG_ERR("tadj out of range!!(%d)\n", tadj);
			 exitFail(1) ;
			}
	mem_wr_reg32(RTC_PS,0x8E);
	mem_wr_reg32(RTC_TADJ,tadj);
	mem_wr_reg32(RTC_ZT,RTCZT);

    while(mem_rd_reg32(RTC_CTRL) & 0x200);
	mem_wr_reg32(RTC_TEMP,0x00);
    swDelay(80);
    while(mem_rd_reg32(RTC_CTRL) & 0x600);

	ad_aver = mem_rd_reg32(RTC_TEMP);
	DBG_MSG("RT_TEMP=%3d(%f %f)\n", ad_aver, ((float)ad_aver)/4.0, (float)std_temper/4.0);   //cgf @20170810
	ad_aver = ad_aver * 2;
 	if(((ad_aver - TEMPER_OFFSET) > (std_temper * 2 + MACHINE_TEMP_DIFF)) || ((ad_aver - TEMPER_OFFSET) < (std_temper * 2 - MACHINE_TEMP_DIFF)))   //Error is larger than +-1 degree.
	{ 
		DBG_ERR("Tstandard=%f, Trtc=%f\n", (float)std_temper/4.0, (float)(ad_aver-TEMPER_OFFSET)/8.0);
		result = FAIL;
		return result;
	}	
	
	mem_wr_reg32(SYS_SYS_PS,0x75);
	if((mem_rd_reg32(SYS_ANA_BGR)!=ANABGR) || (mem_rd_reg32(MADC_AD_TEST)!=0xa28))
	{
		DBG_ERR("bgr=%x,test=%d\n", mem_rd_reg32(SYS_ANA_BGR),mem_rd_reg32(MADC_AD_TEST));
		result = FAIL;
		return result;		
	}
	
	if(temperLinePro() == FAIL)
	{
		DBG_ERR("spline prog fail\n");
		result = FAIL;
		return result;
	}
	else
	{
		result = SUCCESS;
	}
	
  return result;
}


uint32_t splineUpdateCheck(void)
{
	mem_wr_reg32(SYS_SYS_PS,0x75);
	
	//Special line0
	if(mem_rd_reg32(SYS_ANA_ADC) != ANAADC)
	{
		return FAIL;	
	}	
	if(mem_rd_reg32(SYS_ANA_CLK1) != ANACLK1)
	{
		return FAIL;	
	}	
	if(mem_rd_reg32(SYS_ANA_CLK2) != ANACLK2)
	{
		return FAIL;	
	}	
	if(mem_rd_reg32(SYS_ANA_LDO) != ANALDO)
	{
		return FAIL;	
	}	
	//if(mem_rd_reg32(SYS_ANA_BGR) != ANABGR)
	if((mem_rd_reg32(SYS_ANA_BGR) != ANABGR)&&(mem_rd_reg32(SYS_ANA_BGR) != 0x6158))//兼容RN8211的ANA_BGR，（A版20160729前ANA_BGR为0x6158，之后改为0x615a，）
	{
		return FAIL;	
	}		
	if(mem_rd_reg32(SYS_ANA_PSW) != ANAPSW)
	{
		return FAIL;	
	}		
	if(mem_rd_reg32(MADC_TPS_CTRL) != TPSCTRL)
	{
		return FAIL;	
	}		
  if(mem_rd_reg32(MADC_AD_TEST) != ADCTEST)
	{
		return FAIL;	
	}	

	//Special line1
	if(mem_rd_reg32(RTC_TCPMD) != RTCMODE)
	{
		return FAIL;	
	}	
	if(mem_rd_reg32(RTC_DOTA0) != RTCDOTA0)
	{
		return FAIL;	
	}		
	if(mem_rd_reg32(RTC_ALPHAL) != RTCALPHAL)
	{
		return FAIL;	
	}			
	if(mem_rd_reg32(RTC_ALPHAH) != RTCALPHAH)
	{
		return FAIL;	
	}			
	if(mem_rd_reg32(RTC_XT0) != RTCXT0)
	{
		return FAIL;	
	}			
	if(mem_rd_reg32(RTC_BETA) != RTCBETA)
	{
		return FAIL;	
	}			
	if(mem_rd_reg32(RTC_ZT) != RTCZT)
	{
		return FAIL;	
	}			
	
	return SUCCESS;
	
}

uint32_t resetStd(void)
{
  uint32_t result;

	//Swd Initialize
	swd_drv_init();
	if (swd_connect(0) == SWD_RESP_OK)
	{
    //ra9103_initail();
	}
	else
	{
			swd_drv_init();
			if (swd_connect(0) == SWD_RESP_OK)
			{
					//ra9103_initail();
			}
			else
			{
					swd_drv_init();
					if (swd_connect(0) == SWD_RESP_OK)
					{
							//ra9103_initail();
					}
					else
					{
							result = FAIL;
							return result;
					}
			}

	}

  result = splineUpdateCheck();
  return result;	
}


void stdInitial(void)
{
	std_connect_flag = resetStd();
}

uint16_t stdTempAcquire(void)
{
	uint32_t i;
	uint32_t initial_time;
  uint32_t temper[10];
  uint32_t aver;		
	uint32_t temper_max2min;
	uint32_t temper_avg;
	
	if(std_connect_flag == FAIL)
	{
		return 0xffff;
	}
	
	mem_wr_reg32(RTC_CTRL,0x100);
	initial_time = sys_stamp ? sys_stamp : (sys_stamp - 1);
  while(mem_rd_reg32(RTC_CTRL) & 0x200)
	{
		if (stampThrough(initial_time) >= 2) return 0xffff;
  }			
	
	mem_wr_reg32(RTC_CTRL,0x1c9);   //TSE = 0B01
	initial_time = sys_stamp ? sys_stamp : (sys_stamp - 1);
  while(mem_rd_reg32(RTC_CTRL) & 0x200)
	{
		if (stampThrough(initial_time) >= 2) return 0xffff;
  }			
	
  for(i=0;i<3;i++)
  {
    mem_wr_reg32(RTC_TEMP,0x00);
		swDelay(80);
	  initial_time = sys_stamp ? sys_stamp : (sys_stamp - 1);
    while(mem_rd_reg32(RTC_CTRL) & 0x600)
	  {
		  if (stampThrough(initial_time) >= 2) return 0xffff;
    }						
              
    temper[i] = mem_rd_reg32(RTC_TEMP);
  }
	
  aver = max2min(temper,3);
  temper_max2min = aver;			
	
  aver = average(temper,3);
  temper_avg = aver;
	
	if(temper_max2min > 4)
	{
		return 0xffff;
	}
	else
	{
		return (uint16_t)temper_avg;
	}
}
