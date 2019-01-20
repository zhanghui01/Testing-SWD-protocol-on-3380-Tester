#include "drv.h"

#define DVDD_MAX   1.90
#define DVDD_MIN   1.63

#define AVDD_MAX   3.4
#define AVDD_MIN   3.2

#define VREF_MAX  1.290
#define VREF_MIN  1.220

#define ADC1243_VREF 2.500
#define CHAN_VREF  ((0x6 << 4) | (0x7))  // AN6,AN7


uint32_t Voltage_QC(void)
{
	float vref ;
#if 1
	swd_connect(0);
	SocAVDDEna();
  adsInit();  // ADS1243 initial
	
	vref = ((float)(ADS1243GetValue(CHAN_VREF)) * ADC1243_VREF) / ((float)(0xffffff));

	if(
		(vref < VREF_MIN) ||
		(vref > VREF_MAX))
	{
		DBG_ERR("vref is %f\n", vref);
	  return 1;
	}
#endif
	return 0;
}

uint32_t ADS1243GetValue( uint8_t CHAN )
{
	uint32_t data[5] ;
	uint32_t data_ave ;
	uint32_t i ;
	
	adsInitvdd(CHAN) ;	
	
	DIRINPUT_SET0;
	data[0] = adsDataRead();  // ignore the first data	
  for(i=0;i<5;i++)
  {
    data[i] = adsDataRead();
  }
// 	DIRINPUT_SET1;//cgf test @20160913
	
	data_ave = 0 ;
	for( i=0;i<5;i++ )
	{
		data_ave = data_ave + data[i] ;
	}
	data_ave = data_ave/5 ;
	
	return data_ave ;
	
}

uint32_t SocAVDDEna(void)
{
  uint32_t  data;
  
  data = 0x82;
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PS)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->SYS_PD)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  data &= (~(0x1 << 0));   //enable I1 channel
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PD)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PD fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
 	
	return SWD_RESP_OK;
}

uint32_t SocVrefEna(void)
{
  uint32_t  data;
  
  data = 0x82;
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PS)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }	
	
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->SYS_PD)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Read SYS_PD fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  data &= (~(1 << 6));   //enable BGR
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PD)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SYS_PD fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }		

  return SWD_RESP_OK;
}

uint32_t SocVrefDis(void)
{
  uint32_t  data;
  
  data = 0x82;
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PS)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }	
	
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->SYS_PD)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Read SYS_PD fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  data |= (1 << 6);   //disable BGR
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PD)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SYS_PD fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }		   
  return SWD_RESP_OK;
}

