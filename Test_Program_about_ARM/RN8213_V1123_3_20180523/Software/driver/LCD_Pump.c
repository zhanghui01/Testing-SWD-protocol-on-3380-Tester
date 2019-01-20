 #include "drv.h"

//=================  baislvl=31  ==========  8% 20170413
#define LCD31_VA_MAX 1.35  //1.25			//1.7
#define LCD31_VA_MIN 1.15			        //1.0

#define LCD31_VB_MAX 2.70  //2.50			//3.0
#define LCD31_VB_MIN 2.30			        //2.0

#define LCD31_VC_MAX 4.05	 //	3.75	  //4.5
#define LCD31_VC_MIN 3.45	 		        //3.0

#define LCD31_VD_MAX 5.40	 //5.00		  //6.0
#define LCD31_VD_MIN 4.60			        //4.5

//=================  biaslvl=21  ==========  8%  20170413
#define LCD21_VA_MAX 1.134  //1.05			//1.7
#define LCD21_VA_MIN 0.966			        //1.0

#define LCD21_VB_MAX 2.268  //2.10			//3.0
#define LCD21_VB_MIN 1.932			        //2.0

#define LCD21_VC_MAX 3.402	 //	3.15	  //4.5
#define LCD21_VC_MIN 2.898	 		        //3.0

#define LCD21_VD_MAX 4.536	 //4.21		  //6.0
#define LCD21_VD_MIN 3.864			        //4.5

float lcd_va, lcd_vb, lcd_vc, lcd_vd;

uint32_t LCDPump_QC(void)
{
#if 1
  uint32_t i ,cnt ;
//	swd_connect(1);
//	SocPumpEna();		  // having being initial at main()
 	usDelay(350000);//100000 X,200000 OK,
  adcInit();
//==========testing  biaslvl=31==========================
	lcd_va = adcGetValue(ADC_LCDVA);
	lcd_vb = adcGetValue(ADC_LCDVB);
	lcd_vc = adcGetValue(ADC_LCDVC);
  lcd_vd = adcGetValue(ADC_LCDVD); 
	lcd_va = adcGetValue(ADC_LCDVA);
	lcd_vb = adcGetValue(ADC_LCDVB);
	lcd_vc = adcGetValue(ADC_LCDVC);
  lcd_vd = adcGetValue(ADC_LCDVD);  // the first data is error ,ignore.
  lcd_va = 0 ;
	lcd_vb = 0 ;
	lcd_vc = 0 ;
	lcd_vd = 0 ;
	cnt = 5 ;
	for( i=0 ;i<cnt ;i++)
	{
	   lcd_va = lcd_va + adcGetValue(ADC_LCDVA);
	   lcd_vb = lcd_vb + adcGetValue(ADC_LCDVB);
	   lcd_vc = lcd_vc + adcGetValue(ADC_LCDVC);
	   lcd_vd = lcd_vd + adcGetValue(ADC_LCDVD);
	}
	lcd_va = lcd_va / cnt ;
	lcd_vb = lcd_vb / cnt ;
	lcd_vc = lcd_vc / cnt ;
	lcd_vd = lcd_vd / cnt ;
	
	if((lcd_va < LCD31_VA_MIN) ||
		(lcd_va > LCD31_VA_MAX) ||
		(lcd_vb < LCD31_VB_MIN) ||
		(lcd_vb > LCD31_VB_MAX) ||
		(lcd_vc < LCD31_VC_MIN) ||
		(lcd_vc > LCD31_VC_MAX) ||
		(lcd_vd < LCD31_VD_MIN) ||
		(lcd_vd > LCD31_VD_MAX))
	{
	    DBG_ERR("lcd va is %f\n", lcd_va);
		DBG_ERR("lcd vb is %f\n", lcd_vb);
		DBG_ERR("lcd vc is %f\n", lcd_vc);
		DBG_ERR("lcd vd is %f\n", lcd_vd);
	  return 1;
	}
	
	swd_connect(1);	
	SocPump_21Ena();		  // having being initial at main()
 	usDelay(400000);//100000 X,200000 OK,
	
	//==========testing  biaslvl=21==========================

	lcd_va = adcGetValue(ADC_LCDVA);
	lcd_vb = adcGetValue(ADC_LCDVB);
	lcd_vc = adcGetValue(ADC_LCDVC);
  lcd_vd = adcGetValue(ADC_LCDVD); 
	lcd_va = adcGetValue(ADC_LCDVA);
	lcd_vb = adcGetValue(ADC_LCDVB);
	lcd_vc = adcGetValue(ADC_LCDVC);
  lcd_vd = adcGetValue(ADC_LCDVD);  // the first data is error ,ignore.
  lcd_va = 0 ;
	lcd_vb = 0 ;
	lcd_vc = 0 ;
	lcd_vd = 0 ;
	for( i=0 ;i<cnt ;i++)
	{
	   lcd_va = lcd_va + adcGetValue(ADC_LCDVA);
	   lcd_vb = lcd_vb + adcGetValue(ADC_LCDVB);
	   lcd_vc = lcd_vc + adcGetValue(ADC_LCDVC);
	   lcd_vd = lcd_vd + adcGetValue(ADC_LCDVD);
	}
	lcd_va = lcd_va / cnt ;
	lcd_vb = lcd_vb / cnt ;
	lcd_vc = lcd_vc / cnt ;
	lcd_vd = lcd_vd / cnt ;
	
	if((lcd_va < LCD21_VA_MIN) ||
		(lcd_va > LCD21_VA_MAX) ||
		(lcd_vb < LCD21_VB_MIN) ||
		(lcd_vb > LCD21_VB_MAX) ||
		(lcd_vc < LCD21_VC_MIN) ||
		(lcd_vc > LCD21_VC_MAX) ||
		(lcd_vd < LCD21_VD_MIN) ||
		(lcd_vd > LCD21_VD_MAX))
	{
	  DBG_ERR("lcd va is %f\n", lcd_va);
		DBG_ERR("lcd vb is %f\n", lcd_vb);
		DBG_ERR("lcd vc is %f\n", lcd_vc);
		DBG_ERR("lcd vd is %f\n", lcd_vd);
	  return 1;
	}
	
	
	
	adcExit() ;
#endif

	return 0;
}

uint32_t LCD3V3(void)
{
#if 1
  uint32_t i ,cnt, data;
// 	  uint32_t * addr ;
//	swd_connect(1);
//	SocPumpEna();		  // having being initial at main()
	usDelay(300000);//100000 X,200000 OK,
 	SocLcdIoCfg();    //ADD LCD port configuration to avoid COM/SEG opened
	
	//=====================
// 	addr= (uint32_t *)(&(LCD->BUF[0]));0x40048000
// 	addr= (uint32_t *)(&(*(uint32_t *)0x40048020));
	data = 0xffffffff;
// 	data = 0x00000000;
	for( i=0;i<10 ;i++)
	{	
			if(swd_mem_wr32((unsigned long)(&(*(uint32_t *)0x40048020)+i),(unsigned long *)&data,1) != SWD_RESP_OK)
			{
				DBG_ERR("Write SOC LCDBUF fail\n");
				return SWD_RESP_PROTOCAL_ERROR;
			}	
	}	
	
	
	
  adcInit();
//==========testing  biaslvl=31==========================
	lcd_va = adcGetValue(ADC_LCDVA);
	lcd_vb = adcGetValue(ADC_LCDVB);
	lcd_vc = adcGetValue(ADC_LCDVC);
  lcd_vd = adcGetValue(ADC_LCDVD); 
	lcd_va = adcGetValue(ADC_LCDVA);
	lcd_vb = adcGetValue(ADC_LCDVB);
	lcd_vc = adcGetValue(ADC_LCDVC);
  lcd_vd = adcGetValue(ADC_LCDVD);  // the first data is error ,ignore.
  lcd_va = 0 ;
	lcd_vb = 0 ;
	lcd_vc = 0 ;
	lcd_vd = 0 ;
	cnt = 5 ;
	for( i=0 ;i<cnt ;i++)
	{
	   lcd_va = lcd_va + adcGetValue(ADC_LCDVA);
	   lcd_vb = lcd_vb + adcGetValue(ADC_LCDVB);
	   lcd_vc = lcd_vc + adcGetValue(ADC_LCDVC);
	   lcd_vd = lcd_vd + adcGetValue(ADC_LCDVD);
	}
	lcd_va = lcd_va / cnt ;
	lcd_vb = lcd_vb / cnt ;
	lcd_vc = lcd_vc / cnt ;
	lcd_vd = lcd_vd / cnt ;
	
	if((lcd_va < LCD31_VA_MIN) ||
		(lcd_va > LCD31_VA_MAX) ||
		(lcd_vb < LCD31_VB_MIN) ||
		(lcd_vb > LCD31_VB_MAX) ||
		(lcd_vc < LCD31_VC_MIN) ||
		(lcd_vc > LCD31_VC_MAX) ||
		(lcd_vd < LCD31_VD_MIN) ||
		(lcd_vd > LCD31_VD_MAX))
	{
	    DBG_ERR("lcd va is %f\n", lcd_va);
		DBG_ERR("lcd vb is %f\n", lcd_vb);
		DBG_ERR("lcd vc is %f\n", lcd_vc);
		DBG_ERR("lcd vd is %f\n", lcd_vd);
	  return FAIL;
	}
	adcExit() ;
#endif

	return PASS;//not-funtion case
}



