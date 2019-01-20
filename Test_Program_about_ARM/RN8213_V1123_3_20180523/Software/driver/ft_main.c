#include <lpc17xx.h>
#include <stdio.h>

#include "drv.h"

#define version 20131022
int32_t main(void)
{ 
  uint8_t result ;
  uartInit();		 //cgf @20170810
  Ini_Test() ;  // P1.23 = 0 ,P1.24 = 0
  /****************************************************/
  //Rst std temperatrue 
  swd_num = SWD_PARALLEL;  // CHECK SWD
	DBG_MSG("============\n");
  swd_drv_init() ;
	DBG_MSG("============\n");
  swd_connect(0);
	DBG_MSG("============\n");
  swd_debug_reset();
	DBG_MSG("============\n");
  // enable LCD_Pump and VREF ,AVDD
  swd_num = SWD_SERIAL ;  // select DUT SWD	
  swd_drv_init() ;
	DBG_MSG("============\n");
	DBG_MSG("swd_connect(1) BEAGIN");
  swd_connect(1);
	DBG_MSG("swd_connect(1) END");
	DBG_MSG("=====swd_debug_halt=======\n");
  if(swd_debug_halt() != SWD_RESP_DEBUG_STATE)
  {
    DBG_ERR("SOC halt fail\n");
    exitFail(1) ;
  } 
	DBG_MSG("======UID_CHECK======\n");
	UID_check();
	DBG_MSG("======UID_END======\n");
// 	Normal_pwr_init();
// SocPump_31Ena() ;	 // enable LCD_Pump  
 DBG_MSG("======Enable AVDD PIN======\n");
 SocAVDDEna() ;   // enable VREF ,AVDD 
 DBG_MSG("======Enable AVDD PIN_END======\n");
 DBG_MSG("======Enable Vref PIN======\n");
 SocVrefEna() ;  // enable VREF
	DBG_MSG("======Enable Vref PIN_END======\n");
//  adsInit();
  __set_swd_sio_as_in() ;

/****************************************************/
  result = wait_start() ;  // P0.10 =0 ,P0.11 = 1 ,then start test  
// 	swd_connect(1);
 SocVrefDis() ;  // VREF test over ,turn off
  swd_num = SWD_SERIAL ;  // select DUT SWD	
  swd_drv_init() ;
  swd_connect(0);
  if(swd_debug_halt() != SWD_RESP_DEBUG_STATE)
  {
    DBG_ERR("SOC halt fail\n");
    exitFail(1) ;
  }
// 	SocPumpEna() ;	 // enable LCD_Pump  
  FLUSH_INI ;
  __set_swd_sio_as_in() ;
//Timer3 Initial
  timer3Init();	 
//========================================================================  	
//>>>>>>>>>>>>>>>>>>>>>>    1. function test:	0x3 <<<<<<<<<<<<<<<<<<<<<<<<
//======================================================================== 
  if(result == START_TEST)       
  {
	DBG_MSG("================PATCH RN8213_V112.3  updata 20180523 !3#S2=======================\n"); 
// 		SocnorPwr_FT();    //normal pwr
//==================1.1     SP0    ====================
		wait_SP0();  //    wait PORSW_BIN1=0x1;	PORSW_BIN2=0x2;
		DBG_MSG("SPL0 start! powswh_bin=%d \n",powswh_bin);

		result = sp0LinePro() ;
    if( result == FAIL )
		{
			DBG_ERR("sp0LinePro fail\n");
      exitFail(1) ;
		}
		DBG_MSG("SPL0 pass !\n");
		SocPump_31Ena() ;	 // enable LCD_Pump 
		exit_SP0_Pass() ;	//  output=10
		
//==================1.2   LCD3V3   ====================		
 		wait_LCD3V3();  //    wait 11	
		if(LCD3V3() == FAIL)
		{
			exitFail(1) ;
		}	

		exit_LCD3V3_Pass() ;	//  output=11
		
		
//==================1.3  FUNTION   ====================		
		wait_FUNTION();  //    wait 01
		if( test_all() == FAIL)
		{
			exitFail(1) ;
		}	
		SocLowPwr_sram_FT() ;
		DBG_MSG("funtion pass and SRAM0 loaded!\n");		
		exit_FUNTION_Pass() ;//  output=01
		
		}
//========================================================================  	
//>>>>>>>>>>>>>>>>>     2. low power test:		0x02    <<<<<<<<<<<<<<<<<<<<
//======================================================================== 
  else if(result == START_LP)
// 	 if(result == START_LP)
  {
    SocLowPwr_FT() ;			 //lowpwr case;
		DBG_MSG(" FLASH loaded!\n");	
  }
//========================================================================  	
//>>>>>>>>>>>>>>>>   3. display lowpwr test:		0x01    <<<<<<<<<<<<<<<<<<
//======================================================================== 
	else if(result == START_DISLP)
	{
    Soc_DISLowPwr_FT() ;   //display lowpwr case;
		DBG_MSG(" DISLP loaded!\n");	
	}
   else 
  {
    exitFail(1);
  } 
  //3.   wait_clrmip
	
  wait_clrmip(); 
		//write FT_UID
	if(FT_UID_write() == FAIL)
		{
			exitFail(1) ;
		}
		
	//=============check again ==============
		if(empty_check_fl())
		{
			DBG_ERR("Flash Is Not Empty!\n");
			exitFail(1);
		}
		DBG_MSG("Flash check success again£¡\n");
		if(empty_check_ee())
	  { 
			DBG_ERR("EEPROM Is Not Empty!\n");
			exitFail(1);
	  }
		DBG_MSG("EEPR check success again£¡\n");		
  finishTest();
  
  //BUS_OFF ;
  while(1)
    ;
}
