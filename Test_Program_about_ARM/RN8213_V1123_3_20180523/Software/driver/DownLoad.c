#include "drv.h"

uint32_t DownLoad(void)
{
	uint32_t loops = 0, sysimg_size, anaimg_size ;
	unsigned long ack;		

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
        return 0 ;
			}
		  else
		  {
		    DBG_ERR("RTC_QC download fail!") ;
		    return 1 ;
		  }
		}		
	  /**********	download testcase **********/		
	}	
	
	return 0 ;
}









