#include "drv.h"
#include "testcase.h"
#include "analog_testcase.h"
#include "string.h"
uint32_t num ;
uint8_t test_all(void)
{
  uint32_t loops = 0, sysimg_size, anaimg_size ;
  unsigned long ack;	
  uint32_t data ;	  
  uint32_t result ;
	
  sysimg_size = sizeof(sys_atf_tbl) / sizeof(s_atf_img);
  anaimg_size = sizeof(analog_atf_tbl) / sizeof(s_atf_img);

  DBG_WARNNING("there are %d testcase need to be run\n", sysimg_size + anaimg_size);
//  num = 0x55555555 ;
  FLUSH ;
  for(loops = 0; loops < (sysimg_size + anaimg_size); loops++) // all testcase test
  {    
			if(loops==13) 
			{
			  EMU_CLK_Ini();	
				EMU_Fun_Ini();
				EMU_GPIO_Ini() ;	
			}	
			
    if(loops < anaimg_size)   //analog testcase test
    {
      if(SocChipCheck(analog_atf_tbl[loops].descr))
      {
        continue;
      }
      FLUSH ;
//       DBG_MSG("get the %dth testcase(%s)\n", loops, analog_atf_tbl[loops].descr);
    
      if( ((uint32_t (*)(void))(analog_atf_tbl[loops].pImg))() )
      {
        DBG_ERR("image %s execute fail\n", analog_atf_tbl[loops].descr);

        return FAIL ;
      }
      else
      {
       DBG_MSG("execute pass!the %dth testcase(%s)\n", loops, analog_atf_tbl[loops].descr);
      }					         
    } // end of analog testcase test
    else  //digital testcase test
    {		 
      if(SocChipCheck(sys_atf_tbl[loops - anaimg_size].descr))
      {
       continue;
      }
	if(loops<=12) 
	{
			
      if(swd_connect(1) != SWD_RESP_OK)
      {
         swDelay(100);
         swd_connect(1);
      }  
//EMU 20161214
	}
	else
	{
        if(swd_connect(0) != SWD_RESP_OK)
      {
         swDelay(100);
         swd_connect(0);
      }		
	}
		
      if(swd_debug_halt() != SWD_RESP_DEBUG_STATE)
      {
        DBG_ERR("SOC halt fail\n");
        exitFail(1) ;
      }
      FLUSH ;     
      /////////////////
      data = 0x82;
      if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PS)),(unsigned long *)&data,1) != SWD_RESP_OK){
        DBG_ERR("Write SOC PS fail\n");
        return SWD_RESP_PROTOCAL_ERROR;
      }
      data = 0x3;
      if(swd_mem_wr32((unsigned long)(&(SYSCTL->MAP_CTRL)),(unsigned long *)&data,1) != SWD_RESP_OK){
        DBG_ERR("Write SOC MAP_CTRL fail\n");
        return SWD_RESP_PROTOCAL_ERROR;
      }
      if(swd_mem_rd32((unsigned long)(0x100037f8),(unsigned long *)&data,1) != SWD_RESP_OK){
        DBG_ERR("Write FLAG fail\n");
        return SWD_RESP_PROTOCAL_ERROR;
      }
      data = 0xffffffff;
      if(swd_mem_wr32((unsigned long)(0x100037f8),(unsigned long *)&data,1) != SWD_RESP_OK){
        DBG_ERR("Write FLAG fail\n");
        return SWD_RESP_PROTOCAL_ERROR;
      }
      
      /////////////////
      if(atf_img_run((ps_atf_img)(&sys_atf_tbl[loops - anaimg_size])) == SWD_RESP_OK)
      {
				if(loops==7)
				{
					usDelay(420000);//0.8s fail£»0.85s pass
				}
        ack = atf_img_monitor((ps_atf_img)(&sys_atf_tbl[loops - anaimg_size]));	
        if(ack != TC_OK)
        {
          DBG_ERR("image %s execute fail(%x)\n", sys_atf_tbl[loops - anaimg_size].descr, ack);

          return FAIL ;

        }
        else
        {
         DBG_MSG("execute pass!the %dth testcase(%s)\n", loops, sys_atf_tbl[loops - anaimg_size].descr);
        }
      }
      else
      {
        DBG_ERR("image %s download fail\n", sys_atf_tbl[loops - anaimg_size].descr);
        DBG_WARNNING("\nPress KEY6 to continue next testcase.\n") ;
        DBG_WARNNING("Or Press KEY4 to run the faulse case again.\n") ;
        

        return FAIL ;

      }
    } // end of digital testcase test				
  } // end of all testcase test	

	//=================EMU test=========
	result = EMU_Power_Test2() ;
  if( result != 0 )
	{
		DBG_ERR("EMU fail\n");
  return FAIL ;
	}
// clear EEPROM and FLASH 
  clearMip();	
  FLUSH ;
//  num = 0xffffffff ;	
// download software to DUT
//if( DownLoad() != 0 ) 
//  return FAIL ;
  FLUSH ;
  return PASS ;	
}


/*********************************************************
**
**  clearMip() ,clear FLASH and EEPROM data
**
***********************************************************/
void clearMip(void)
{
  uint32_t td;
  
  if(swd_connect(1) != SWD_RESP_OK)
  {
    DBG_ERR("swd connect fail\n");
  }
  
  //flash chip erase
  td = 0xfeed0000;
  if(swd_mem_wr32(0x18800014, (unsigned long *)(&td), 1) != SWD_RESP_OK) //protect
  {
    DBG_ERR("write flash_prot fail\n");
  }
  
  td = 1; //tus
  if(swd_mem_wr32(0x18800020, (unsigned long *)(&td), 1) != SWD_RESP_OK)
  {
    DBG_ERR("write flash_tus fail\n");
  }
  
  td = 895; //tms
  if(swd_mem_wr32(0x18800024, (unsigned long *)(&td), 1) != SWD_RESP_OK)
  {
    DBG_ERR("write flash_tms fail\n");
  }
  
  td = 6; //te
  if(swd_mem_wr32(0x1880002c, (unsigned long *)(&td), 1) != SWD_RESP_OK)
  {
    DBG_ERR("write flash_te fail\n");
  }
  
  td = 0xfeed0009;//cmd=chip erase
  if(swd_mem_wr32(0x18800000, (unsigned long *)(&td), 1) != SWD_RESP_OK)
  {
    DBG_ERR("write flash_cmd fail\n");
  }
  usDelay(10000);
  
  //eeprom chip erase
  td = 0xfeed0000;
  if(swd_mem_wr32(0x08800014, (unsigned long *)(&td), 1) != SWD_RESP_OK) //protect
  {
    DBG_ERR("write eeprom_protect fail\n");
  }
  
  td = 1; //tus
  if(swd_mem_wr32(0x08800020, (unsigned long *)(&td), 1) != SWD_RESP_OK)
  {
    DBG_ERR("write eeprom_tus fail\n");
  }
  
  td = 860; //tms
  if(swd_mem_wr32(0x08800024, (unsigned long *)(&td), 1) != SWD_RESP_OK)
  {
    DBG_ERR("write eeprom_tms fail\n");
  }
  
  td = 2; //te
  if(swd_mem_wr32(0x0880002c, (unsigned long *)(&td), 1) != SWD_RESP_OK)
  {
    DBG_ERR("write eeprom_te fail\n");
  }
  
  td = 0xfeed0009;//cmd=chip erase
  if(swd_mem_wr32(0x08800000, (unsigned long *)(&td), 1) != SWD_RESP_OK)
  {
    DBG_ERR("write eeprom_cmd fail\n");
  }
  usDelay(5000);
}


/*********************************************************
**
**  SocChipCheck() ,ignore testcase which is listed
**
***********************************************************/
//return 1 means testcase should be skipped
#define SOC_CHIP_ID 0x8213
uint8_t SocChipCheck(uint8_t * tc)
{
  uint8_t *rn8211_skip[] = {"Voltage_QC","SofeWare", "SocLowPwr_sram","SocLowPwr_sys","Soc_DISLowPwr_sys"/*,"EMU_FT","GPIO_FT","MIP_FT"*/};
  uint8_t *rn8213_skip[] = {"Voltage_QC","SofeWare", "SocLowPwr_sram","SocLowPwr_sys","Soc_DISLowPwr_sys"};
  uint8_t *rn8215_skip[] = {"LCDPump_QC","Voltage_QC","SofeWare", "SocLowPwr_sram","SocLowPwr_sys","Soc_DISLowPwr_sys"};
  uint8_t *rn8312_skip[] = {"LCDPump_QC","Voltage_QC","SofeWare", "SocLowPwr_sram","SocLowPwr_sys","Soc_DISLowPwr_sys"};
  uint8_t *rn8316_skip[] = {"LCDPump_QC","Voltage_QC","SofeWare", "SocLowPwr_sram","SocLowPwr_sys","Soc_DISLowPwr_sys"};
  uint32_t i ;  
  uint32_t rn8211_skip_len = sizeof(rn8211_skip)/ sizeof(uint8_t *) ;
  uint32_t rn8213_skip_len = sizeof(rn8213_skip)/ sizeof(uint8_t *) ;
  uint32_t rn8215_skip_len = sizeof(rn8215_skip)/ sizeof(uint8_t *) ;
  uint32_t rn8312_skip_len = sizeof(rn8312_skip)/ sizeof(uint8_t *);
  uint32_t rn8316_skip_len = sizeof(rn8316_skip)/ sizeof(uint8_t *);
  uint8_t res = 0;

  switch (SOC_CHIP_ID)
  {
    case 0x8211:
      for(i = 0; i < rn8211_skip_len; i++)
      {
        if(strcmp((const char *)tc, (const char *)(rn8211_skip[i])) == 0)
          res = 1;
      }
      break;
    case 0x8213:
      for(i = 0; i < rn8213_skip_len; i++)
      {
        if(strcmp((const char *)tc, (const char *)(rn8213_skip[i])) == 0)
          res = 1;
      }
      break;
    case 0x8215:
      for(i = 0; i < rn8215_skip_len; i++)
      {
        if(strcmp((const char *)tc, (const char *)(rn8215_skip[i])) == 0)
          res = 1;
      }
      break;
    case 0x8312:
      for(i = 0; i < rn8312_skip_len; i++)
      {
        if(strcmp((const char *)tc, (const char *)(rn8312_skip[i])) == 0)
          res = 1;
      }
      break;
    case 0x8316:
      for(i = 0; i < rn8316_skip_len; i++)
      {
        if(strcmp((const char *)tc, (const char *)(rn8316_skip[i])) == 0)
          res = 1;
      }
      break;
    default:
      break;
  }

  return res;
}

/*********************************************************
**
**  download() ,download software to FLASH
**
***********************************************************/
uint32_t DownLoad(void)
{
  uint32_t loops = 0, sysimg_size, anaimg_size ;	

  sysimg_size = sizeof(sys_atf_tbl) / sizeof(s_atf_img);
  anaimg_size = sizeof(analog_atf_tbl) / sizeof(s_atf_img);	

  for(loops = 0; loops < (sysimg_size + anaimg_size); loops++) // all testcase test
  {
    /**********	download testcase **********/
    if(strcmp((const char *)(sys_atf_tbl[loops].descr), (const char *)("SofeWare")) == 0)
    {
      if(atf_img_run((ps_atf_img)(&sys_atf_tbl[loops])) == SWD_RESP_OK)
      {
        DBG_MSG("download PASS!") ;
        swd_connect(1);
        return 0 ;
      }
      else
      {
        DBG_ERR("RTC_QC download fail!") ;
        swd_connect(1);
        return 1 ;
      }
    }
    /**********	download testcase **********/		
  }	
  swd_connect(1);
  return 0 ;
}

/*********************************************************
**
**  SocLowPwr_sram() ,test SOC low power
**
***********************************************************/
uint32_t SocLowPwr_sram_FT(void)
{
	uint32_t  j ;
	uint32_t  sysimg_size ;

  if(swd_connect(1) != SWD_RESP_OK)
	{
		DBG_ERR("swd connect fail\n");
		return 1 ;
	}

	
	/**********	download testcase **********/
  sysimg_size = sizeof(sys_atf_tbl) / sizeof(s_atf_img);
  for(j = 0; j < sysimg_size; j++ )
	{
		if(strcmp((const char *)(sys_atf_tbl[j].descr), (const char *)("SocLowPwr_sram")) == 0)
		{
			if(atf_img_run((ps_atf_img)(&sys_atf_tbl[j])) == SWD_RESP_OK)
				break ;
			else
			{
				DBG_ERR("SocLowPwr_sram download fail!") ;
				return 0 ;
			}
		}
		
	}			
	/**********	download testcase **********/
	
	return 0;
}


/*********************************************************
**
**  SocLowPwr_sys() ,test SOC low power
**
***********************************************************/
uint32_t SocLowPwr_FT(void)
{
  uint32_t  j ;
  uint32_t  sysimg_size ;

  if(swd_connect(1) != SWD_RESP_OK)
  {
    DBG_ERR("swd connect fail\n");
    return 1 ;
  }

  //msDelay(10000) ;
  
  /**********	download testcase **********/
  sysimg_size = sizeof(sys_atf_tbl) / sizeof(s_atf_img);
  for(j = 0; j < sysimg_size; j++ )
  {
    if(strcmp((const char *)(sys_atf_tbl[j].descr), (const char *)("SocLowPwr_sys")) == 0)
    {
      if(atf_img_run((ps_atf_img)(&sys_atf_tbl[j])) == SWD_RESP_OK)
        break ;
      else
      {
        DBG_ERR("SocLowPwr_sys download fail!") ;
        return 0 ;
      }
    }
  }
  /**********	download testcase **********/
  
  return 0;
}

/*********************************************************
**
**  Soc_DISLowPwr_sys() ,test SOC display low power
**
***********************************************************/
uint32_t Soc_DISLowPwr_FT(void)
{
  uint32_t  j ;
  uint32_t  sysimg_size ;

  if(swd_connect(1) != SWD_RESP_OK)
  {
    DBG_ERR("swd connect fail\n");
    return 1 ;
  }

  //msDelay(10000) ;
  
  /**********	download testcase **********/
  sysimg_size = sizeof(sys_atf_tbl) / sizeof(s_atf_img);
  for(j = 0; j < sysimg_size; j++ )
  {
    if(strcmp((const char *)(sys_atf_tbl[j].descr), (const char *)("Soc_DISLowPwr_sys")) == 0)
    {
      if(atf_img_run((ps_atf_img)(&sys_atf_tbl[j])) == SWD_RESP_OK)
        break ;
      else
      {
        DBG_ERR("Soc_DISLowPwr_sys download fail!") ;
        return 0 ;
      }
    }
  }
  /**********	download testcase **********/
  
  return 0;
}

