#include "rn821x.h"
#include "common.h"
#include "sysctrl.h"
#include <stdio.h>
#include "nvm.h"

//#define PRINT

uint32_t recvPtr = 0;
uint32_t i = 0;
uint32_t j = 0;
uint32_t reg_value = 0;
uint32_t sendBuf[4] = {0};       
uint32_t recvBuf[4] = {0};
				
uint32_t data;
uint32_t data1;
uint32_t data2;


uint32_t *test_flag;
/*********************************************************************************************************
** Function name:       Fail
** Descriptions:        Fail
** input parameters:    none
** output parameters:   none
** Returned value:      none/never return
*********************************************************************************************************/
void Fail()
{
    *test_flag = 0x12345678 ;
    printf("TEST Fail!\n");
	while(1) 
		;
}   
/*********************************************************************************************************
** Function name:       Pass
** Descriptions:        Pass
** input parameters:    none
** output parameters:   none
** Returned value:      none/never return
*********************************************************************************************************/
void Pass()
{
	*test_flag = 0x98765432 ;  
    printf("TEST Ok!\n");
	while(1)
		;
}    
/*********************************************************************************************************
** Function name:       Pass
** Descriptions:        Pass
** input parameters:    none
** output parameters:   none
** Returned value:      none/never return
*********************************************************************************************************/
void test_flag_init()
{
	uint32_t SRAM_base  ;
	
  i = (*(uint32_t*)(0x40034018)) & 0x7 ;
  switch( i )
  {
    case 0 : SRAM_base = 0x10000000 ; break ;
    case 1 : SRAM_base = 0x10000000 ; break ;
    case 2 : SRAM_base = 0x0        ; break ;
    case 3 : SRAM_base = 0x10000000 ; break ;
    default :SRAM_base = 0x10000000 ;
  }
	
	test_flag  = (uint32_t*)(SRAM_base + 0x37F8) ; 
	*test_flag = 0x11111111 ;
}
/*********************************************************************************************************
** Function name:       WDT_HANDLER
** Descriptions:        WDT Interrupt Manager
** input parameters:    none
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
void    WDT_HANDLER                             (void){ 
    
    while (WDT->EN & 0x100);
	WDT->EN    = 0xbb;
    
}
/*********************************************************************************************************
** Function name:       WDT_OFF
** Descriptions:        WDT_OFF
** input parameters:    none
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
void WDT_OFF (void) {
	NVIC_EnableIRQ(WDT_IRQn);
	SYSCTL->MOD1_EN	|= 1<<9;
	while (WDT->EN & 0x100);
	WDT->PASS = 0x70;
	WDT->PASS = 0x7e;
	WDT->CTRL = 0xff;
	while (WDT->EN & 0x100);
}

/*********************************************************************************************************
** Function name:       EMU_HANDLER
** Descriptions:        EMU_HANDLER
** input parameters:    none
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
/*
void EMU_HANDLER (void) {
	printf("0x%08x\n",EMU->IF);
    if((EMU->IF) & 0x8){
	  printf("Energy_P : 0x%08x\n",EMU->Energy_P);
	  printf("Energy_P : 0x%08x\n",EMU->Energy_P);
	  printf("IF : 0x%08x\n",EMU->IF); 
	}
	if((EMU->IF) & 0x10){
	  printf("Energy_Q : 0x%08x\n",EMU->Energy_Q);
	  printf("Energy_Q : 0x%08x\n",EMU->Energy_Q);
	  printf("IF : 0x%08x\n",EMU->IF); 
	}
	if((EMU->IF) & 0x2000){
	  printf("Energy_S : 0x%08x\n",EMU->Energy_S);
	  printf("Energy_S : 0x%08x\n",EMU->Energy_S);
	  printf("IF : 0x%08x\n",EMU->IF); 
	}
	if((EMU->IF) & 0x40000){
	  printf("Energy_P2 : 0x%08x\n",EMU->Energy_P2);
	  printf("Energy_P2 : 0x%08x\n",EMU->Energy_P2);
	  printf("IF : 0x%08x\n",EMU->IF); 
	}
	if((EMU->IF) & 0x80000){
	  printf("Energy_Q2 : 0x%08x\n",EMU->Energy_Q2);
	  printf("Energy_Q2 : 0x%08x\n",EMU->Energy_Q2);
	  printf("IF : 0x%08x\n",EMU->IF); 
	}
	if((EMU->IF) & 0x100000){
	  printf("Energy_S2 : 0x%08x\n",EMU->Energy_S2);
	  printf("Energy_S2 : 0x%08x\n",EMU->Energy_S2);
	  printf("IF : 0x%08x\n",EMU->IF); 
	}

	EMU->IF = 0xffffffff;
	GPIO->PA = (~(GPIO->PA & 0x01));
}
*/
/*********************************************************************************************************
** Function name:       swDelay
** Descriptions:        mercily software delay
** input parameters:    delay duration
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
void swDelay (uint32_t ulTime) {
    uint32_t i;

    i = 0;
    while (ulTime--) {
        for (i = 0; i < 5000; i++);
    }
}
/*********************************************************************************************************
** Function name:       swDelay
** Descriptions:        mercily software delay
** input parameters:    delay duration
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
void dupdif (uint32_t ulTime) {
    uint32_t k;
    for (k = 0; k < ulTime; k++)
	{	  	  
        while((EMU->IF) & 0x1 != 0x1);
        EMU->IF = 0x1;				
	};

}
 /*********************************************************************************************************
** Function name:       power_test
** Descriptions:        power_test
** input parameters:    none
**三路信号均用信号源输入200mVrms的信号
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
void power_test(void){	
#ifdef PRINT		   
  printf("Power test :\n");	 
#endif	
  EMU->PhsA = 0x100;
  EMU->PhsB = 0x100;
  swDelay(120);	
  if( (EMU->PowerPA < 60000000) || (EMU->PowerPA > 85000000))
  {				  	
#ifdef PRINT
      printf("Err, PowerPA %d\n",EMU->PowerPA);		
#endif	
	  Fail();
  }
  if( (EMU->PowerPB < 60000000) || (EMU->PowerPB > 85000000))
  {				  	
#ifdef PRINT
      printf("Err, PowerPB %d\n",EMU->PowerPB);	
#endif			 
	  Fail();
  }	 
  if( (EMU->PowerQA < 2000000) || (EMU->PowerQA > 5000000))
  {				 	
#ifdef PRINT 
      printf("Err, PowerQA %d\n",EMU->PowerQA);	   
#endif	
	  Fail();
  }		 
  if( (EMU->PowerQB < 2000000) || (EMU->PowerQB > 5000000))
  {				   	
#ifdef PRINT
      printf("Err, PowerQB %d\n",EMU->PowerQB);	  
#endif	
	  Fail();
  }	  	 
  if( (EMU->PowerSA < 60000000) || (EMU->PowerSA > 85000000))
  {				  		
#ifdef PRINT
      printf("Err, PowerSA %d\n",EMU->PowerSA);	  
#endif	
	  Fail();
  }			   	  	 
  if( (EMU->PowerSB < 60000000) || (EMU->PowerSB > 85000000))
  {				   	
#ifdef PRINT
      printf("Err, PowerSB %d\n",EMU->PowerSB);	   
#endif	
	  Fail();
  }	  		   	  	 
  if( (EMU->PowerPA2 < 60000000) || (EMU->PowerPA2 > 85000000))
  {				  	 	
#ifdef PRINT
      printf("Err, PowerPA2 %d\n",EMU->PowerPA2); 	  
#endif	
	  Fail();
  }	     		   	  	 
  if( (EMU->PowerPB2 < 60000000) || (EMU->PowerPB2 > 85000000))
  {				   	
#ifdef PRINT
      printf("Err, PowerPB2 %d\n",EMU->PowerPB2);  	 
#endif	
	  Fail();
  }	      		   	  	  
								  
  EMU->GPQA	= 0x7fff;	
  EMU->GPQB	= 0x7fff;	
#ifdef PRINT	    
  printf("Power test gain is 0x7fff :\n");	 
#endif	
  
  swDelay(120);		
  if( (EMU->PowerPA < (60000000*2)) || (EMU->PowerPA > (85000000*2)))
  {				 	
#ifdef PRINT 
      printf("Err, PowerPA %d\n",EMU->PowerPA);	   	
#endif	
	  Fail();
  }
  if( (EMU->PowerPB < (60000000*2)) || (EMU->PowerPB > (85000000*2)))
  {				  	
#ifdef PRINT
      printf("Err, PowerPB %d\n",EMU->PowerPB);		  
#endif	
	  Fail();
  }	 
  if( (EMU->PowerQA < (2000000*2)) || (EMU->PowerQA > (5000000*2)))
  {				  	
#ifdef PRINT
      printf("Err, PowerQA %d\n",EMU->PowerQA);	  	  
#endif	
	  Fail();
  }		 
  if( (EMU->PowerQB < (2000000*2)) || (EMU->PowerQB > (5000000*2)))
  {				  	
#ifdef PRINT
      printf("Err, PowerQB %d\n",EMU->PowerQB);	 		 
#endif	
	  Fail();
  }	  	 
  if( (EMU->PowerSA < (60000000*2)) || (EMU->PowerSA > (85000000*2)))
  {				  	
#ifdef PRINT
      printf("Err, PowerSA %d\n",EMU->PowerSA);	 	 
#endif	
	  Fail();
  }			   	  	 
  if( (EMU->PowerSB < (60000000*2)) || (EMU->PowerSB > (85000000*2)))
  {				  	
#ifdef PRINT
      printf("Err, PowerSB %d\n",EMU->PowerSB);	 	   
#endif	
	  Fail();
  }	  		   	  	 
  if( (EMU->PowerPA2 < (60000000*2)) || (EMU->PowerPA2 > (85000000*2)))
  {				  	
#ifdef PRINT
      printf("Err, PowerPA2 %d\n",EMU->PowerPA2); 		
#endif	
	  Fail();
  }	     		   	  	 
  if( (EMU->PowerPB2 < (60000000*2)) || (EMU->PowerPB2 > (85000000*2)))
  {				  	
#ifdef PRINT
      printf("Err, PowerPB2 %d\n",EMU->PowerPB2);  		
#endif	
	  Fail();
  }	      		   	  	 
								  
  EMU->GPQA	= 0x8000;	
  EMU->GPQB	= 0x8000;
#ifdef PRINT		   
  printf("Power test gain is 0x8000 :\n");	 
#endif	
  
  swDelay(120);					  	 
  if( EMU->PowerPA !=0)
  {				  	
#ifdef PRINT
      printf("Err, PowerPA %d\n",EMU->PowerPA);	 	 
#endif	
	  Fail();
  }	  	 
  if( EMU->PowerPB !=0)
  {				  	
#ifdef PRINT
      printf("Err, PowerPB %d\n",EMU->PowerPB);	   
#endif	
	  Fail();
  }	 	  	 
  if( EMU->PowerQA !=0)
  {				  	
#ifdef PRINT
      printf("Err, PowerQA %d\n",EMU->PowerQA);	 	 
#endif	
	  Fail();
  }		 	  	 
  if( EMU->PowerQB !=0)
  {				  		
#ifdef PRINT
      printf("Err, PowerQB %d\n",EMU->PowerQB);	 	  
#endif	
	  Fail();
  }	  	 	  	 
  if( EMU->PowerSA !=0)
  {				  	  	
#ifdef PRINT
      printf("Err, PowerSA %d\n",EMU->PowerSA);	 		
#endif	
	  Fail();
  }			   	  	 	  	 
  if( EMU->PowerSB !=0)
  {				  		
#ifdef PRINT
      printf("Err, PowerSB %d\n",EMU->PowerSB);	  		
#endif	
	  Fail();
  }	  		   	  	 	  	 
  if( EMU->PowerPA2 !=0)
  {				  	
#ifdef PRINT
      printf("Err, PowerPA2 %d\n",EMU->PowerPA2);		
#endif	
	  Fail();
  }	     		   	  	 	  	 
  if( EMU->PowerPB2 !=0)
  {				  	
#ifdef PRINT
      printf("Err, PowerPB2 %d\n",EMU->PowerPB2);  	 
#endif	
	  Fail();
  }	      		   	  	 	  	 
  if( EMU->SPL_PA !=0)
  {				  	
#ifdef PRINT
      printf("Err, SPL_PA %d\n",EMU->SPL_PA);  	  
#endif	
	  Fail();
  }	        		   	  	 	  	 
  if( EMU->SPL_PB !=0)
  {				  	
#ifdef PRINT
      printf("Err, SPL_PB %d\n",EMU->SPL_PB);	   
#endif	
	  Fail();
  }	 	        		   	  	 	  	 
  if( EMU->SPL_QA !=0)
  {				  	
#ifdef PRINT
      printf("Err, SPL_QA %d\n",EMU->SPL_QA);	   
#endif	
	  Fail();
  }		 	        		   	  	 
  if( EMU->SPL_QB !=0)
  {				   	
#ifdef PRINT
      printf("Err, SPL_QB %d\n",EMU->SPL_QB);  		
#endif	
	  Fail();
  }	   
								  
  EMU->GPQA	= 0x0;	
  EMU->GPQB	= 0x0;
}
/*********************************************************************************************************
** Function name:       Urms_test
** Descriptions:        Urms_test
** input parameters:    num
** output parameters:   none		  
**三路信号均用信号源输入200mVrms的信号
** Returned value:      none
*********************************************************************************************************/
void rms_test(void){	
#ifdef PRINT			
  printf("RMS test :\n");				   
#endif	
  swDelay(120);					
  if( (EMU->RmsIA < (1300000*1)) || (EMU->RmsIA > (1800000*1)))
  {				  		
#ifdef PRINT
      printf("Err, RmsIA %d\n",EMU->RmsIA);	 		
#endif	
	  Fail();
  }	      		   	  	 
  if( (EMU->RmsIB < (1300000*1)) || (EMU->RmsIB > (1800000*1)))
  {				  		
#ifdef PRINT
      printf("Err, RmsIB %d\n",EMU->RmsIB);	  		
#endif	
	  Fail();
  }	        		   	  	 
  if( (EMU->RmsU < (1300000*1)) || (EMU->RmsU > (1800000*1)))
  {				  		
#ifdef PRINT
      printf("Err, RmsU %d\n",EMU->RmsU);	 	   
#endif	
	  Fail();
  }	
					  
  EMU->IAGAIN = 0x7fff;
  EMU->IBGAIN = 0x7fff;
  EMU->UGAIN = 0x7fff;	   		
#ifdef PRINT
  printf("RMS test gain is 0x7fff :\n");	
#endif	
						 
  swDelay(120);					
  if( (EMU->RmsIA < (1300000*2)) || (EMU->RmsIA > (1800000*2)))
  {				  		
#ifdef PRINT
      printf("Err, RmsIA %d\n",EMU->RmsIA);		   
#endif	
	  Fail();
  }	      		   	  	 
  if( (EMU->RmsIB < (1300000*2)) || (EMU->RmsIB > (1800000*2)))
  {				  	 	
#ifdef PRINT
      printf("Err, RmsIB %d\n",EMU->RmsIB);	 	  
#endif	
	  Fail();
  }	        		   	  	 
  if( (EMU->RmsU < (1300000*2)) || (EMU->RmsU > (1800000*2)))
  {				  		
#ifdef PRINT
      printf("Err, RmsU %d\n",EMU->RmsU);	 	
#endif	
	  Fail();
  }	
					  								   
					  
  EMU->IAGAIN = 0x8000;
  EMU->IBGAIN = 0x8000;
  EMU->UGAIN = 0x8000;		
#ifdef PRINT   
  printf("RMS test gain is 0x8000 :\n");	
#endif	
						 
  swDelay(120);				        		   	  	 
  if( EMU->RmsIA !=0)
  {				  			
#ifdef PRINT
      printf("Err, RmsIA %d\n",EMU->RmsIA);	 	 
#endif	
	  Fail();
  }	   				        		   	  	 
  if( EMU->RmsIB !=0)
  {				  			
#ifdef PRINT
      printf("Err, RmsIB %d\n",EMU->RmsIB);	  	
#endif	
	  Fail();
  }	   				        		   	  	 
  if( EMU->RmsU !=0)
  {				  	   	
#ifdef PRINT
      printf("Err, RmsU %d\n",EMU->RmsU);		
#endif	
	  Fail();
  }	   		   

  EMU->IAGAIN = 0x0;
  EMU->IBGAIN = 0x0;
  EMU->UGAIN = 0x0;
  
  EMU->IADCOS = 0x1fffff;	
  EMU->IBDCOS = 0x1fffff;	 
  EMU->UDCOS = 0x1fffff;  
#ifdef PRINT
  printf("RMS test DCOFFSET is 0x1fffff :\n");
#endif	
  
  swDelay(120);								
  if( (EMU->RmsIA < (1300000*1)) || (EMU->RmsIA > (1800000*1)))
  {				  	   	
#ifdef PRINT
      printf("Err, RmsIA %d\n",EMU->RmsIA);	   	  
#endif	
	  Fail();
  }	      		   	  	 
  if( (EMU->RmsIB < (1300000*1)) || (EMU->RmsIB > (1800000*1)))
  {				  	 	
#ifdef PRINT
      printf("Err, RmsIB %d\n",EMU->RmsIB);	  	   
#endif	
	  Fail();
  }	        		   	  	 
  if( (EMU->RmsU < (1300000*1)) || (EMU->RmsU > (1800000*1)))
  {				  	 	
#ifdef PRINT
      printf("Err, RmsU %d\n",EMU->RmsU);  		 
#endif	
	  Fail();
  }	

  EMU->IADCOS = 0x0;	
  EMU->IBDCOS = 0x0;	 
  EMU->UDCOS = 0x0;	
}
/*********************************************************************************************************
** Function name:       ufreq_test
** Descriptions:        ufreq_test
** input parameters:    num
** output parameters:   none		  
**三路信号均用信号源输入200mVrms的信号
** Returned value:      none
*********************************************************************************************************/
void ufreq_test(void){		   
#ifdef PRINT
    printf("FreqU test :\n");		 		
#endif	
    swDelay(200);		
		
    if( (EMU->FreqU < 9100) || (EMU->FreqU > 9300))
    {				  	
#ifdef PRINT
      printf("Err, FreqU %d\n",EMU->FreqU);		
#endif	
	  Fail();
    }
}
/*********************************************************************************************************
** Function name:       cf_test
** Descriptions:        cf_test
** input parameters:    num
** output parameters:   none		  
**三路信号均用信号源输入200mVrms的信号
** Returned value:      none
*********************************************************************************************************/
void cf_test(void){		  	  
#ifdef PRINT
    printf("CF test :\n");				   
#endif	

	SYSCTL->SYS_PS = 0x82 ;				
	SYSCTL->MOD0_EN |= 1<<4;	
	TC0->DN = 0xffffffff; 	 
								  
#ifdef PRINT
    printf("Chanel A PF test :\n");			
#endif	
	EMU->HFConst = 0x700;
		  		
	TC0->CM0 = 1 | (18<<20) ;	 		 //PF
	TC0->CTRL = 0x1;
	while(!(TC0->STA & 0x2));
	TC0->STA = 0xf;				   
	while(!(TC0->STA & 0x2));
	TC0->STA = 0xf;
	while(!(TC0->STA & 0x2));
	data1 = TC0->CCD0;
	TC0->STA = 0xf;
	while(!(TC0->STA & 0x2));		 
	data2 = TC0->CCD0;
	TC0->STA = 0xf;		
	TC0->CTRL = 0x0;
#ifdef PRINT										
	  printf("PF , TC cnt is %d\n", (data2 - data1));  
#else		   
	if(((data2 - data1) < 360000) || ((data2 - data1) > 500000))
	{						
#ifdef PRINT
	  printf("PF Err , TC cnt is %d\n", (data2 - data1));  	  
#endif	
	  Fail();
	}
#endif	
#ifdef PRINT		  									
    printf("Chanel A SF test :\n");		  
#endif	
	TC0->CM0 = 1 | (16<<20) ;	 		 //SF
	TC0->CTRL = 0x1;
	while(!(TC0->STA & 0x2));
	TC0->STA = 0xf;				   
	while(!(TC0->STA & 0x2));
	TC0->STA = 0xf;
	while(!(TC0->STA & 0x2));
	data1 = TC0->CCD0;
	TC0->STA = 0xf;
	while(!(TC0->STA & 0x2));		 
	data2 = TC0->CCD0;
	TC0->STA = 0xf;		
	TC0->CTRL = 0x0;
#ifdef PRINT											   
	  printf("SF Err , TC cnt is %d\n", (data2 - data1));  
#else
	if(((data2 - data1) < 360000) || ((data2 - data1) > 500000))
	{					
#ifdef PRINT
	  printf("SF Err , TC cnt is %d\n", (data2 - data1));  		
#endif	
	  Fail();
	}
#endif
#ifdef PRINT		 						
    printf("Chanel A QF test :\n");			  
#endif	
	EMU->HFConst = 0x100;
	TC0->CM0 = 1 | (17<<20) ;	 		 //QF
	TC0->CTRL = 0x1;
	while(!(TC0->STA & 0x2));
	TC0->STA = 0xf;				   
	while(!(TC0->STA & 0x2));
	TC0->STA = 0xf;
	while(!(TC0->STA & 0x2));
	data1 = TC0->CCD0;
	TC0->STA = 0xf;
	while(!(TC0->STA & 0x2));		 
	data2 = TC0->CCD0;
	TC0->STA = 0xf;		
	TC0->CTRL = 0x0;
#ifdef	PRINT											
	  printf("QF Err , TC cnt is %d\n", (data2 - data1));  
#else
	if(((data2 - data1) < 940000) || ((data2 - data1) > 1500000))
	{					
#ifdef PRINT
	  printf("QF Err , TC cnt is %d\n", (data2 - data1));  		  
#endif	
	  Fail();
	}	 
#endif
	EMU->SPCMD = 0xa5;
#ifdef PRINT	   						
    printf("Chanel B PF test :\n");		 
#endif	
	EMU->HFConst = 0x700;
		  		
	TC0->CM0 = 1 | (18<<20) ;	 		 //PF
	TC0->CTRL = 0x1;
	while(!(TC0->STA & 0x2));
	TC0->STA = 0xf;				   
	while(!(TC0->STA & 0x2));
	TC0->STA = 0xf;
	while(!(TC0->STA & 0x2));
	data1 = TC0->CCD0;
	TC0->STA = 0xf;
	while(!(TC0->STA & 0x2));		 
	data2 = TC0->CCD0;
	TC0->STA = 0xf;		
	TC0->CTRL = 0x0;

	if(((data2 - data1) < 360000) || ((data2 - data1) > 500000))
	{			  	
#ifdef PRINT
	  printf("PF Err , TC cnt is %d\n", (data2 - data1));  	
#endif	
	  Fail();
	}
#ifdef PRINT		  					  
    printf("Chanel B SF test :\n");	  
#endif	
	TC0->CM0 = 1 | (16<<20) ;	 		 //SF
	TC0->CTRL = 0x1;
	while(!(TC0->STA & 0x2));
	TC0->STA = 0xf;				   
	while(!(TC0->STA & 0x2));
	TC0->STA = 0xf;
	while(!(TC0->STA & 0x2));
	data1 = TC0->CCD0;
	TC0->STA = 0xf;
	while(!(TC0->STA & 0x2));		 
	data2 = TC0->CCD0;
	TC0->STA = 0xf;		
	TC0->CTRL = 0x0;

	if(((data2 - data1) < 360000) || ((data2 - data1) > 500000))
	{		 	
#ifdef PRINT
	  printf("SF Err , TC cnt is %d\n", (data2 - data1));  
#endif	
	  Fail(); 
	}
#ifdef PRINT	 					   
    printf("Chanel B QF test :\n");	  
#endif	
	EMU->HFConst = 0x100;
	TC0->CM0 = 1 | (17<<20) ;	 		 //QF
	TC0->CTRL = 0x1;
	while(!(TC0->STA & 0x2));
	TC0->STA = 0xf;				   
	while(!(TC0->STA & 0x2));
	TC0->STA = 0xf;
	while(!(TC0->STA & 0x2));
	data1 = TC0->CCD0;
	TC0->STA = 0xf;
	while(!(TC0->STA & 0x2));		 
	data2 = TC0->CCD0;
	TC0->STA = 0xf;		
	TC0->CTRL = 0x0;

	if(((data2 - data1) < 940000) || ((data2 - data1) > 1500000))
	{			
#ifdef PRINT
	  printf("QF Err , TC cnt is %d\n", (data2 - data1));  	
#endif 
	  Fail();
	}	 
}
 /*********************************************************************************************************
** Function name:       reg_wr1
** Descriptions:        reg_wr1
** input parameters:    none
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
void reg_wr1(void){	 
#ifdef PRINT
    printf("EMU reg write/read TEST :\n");		
#endif	
    sendBuf[ 0] =0x55555555;sendBuf[ 1] =0xaaaaaaaa;sendBuf[ 2] =0x0;sendBuf[ 3] =0xffffffff;

	EMU->SPCMD = 0xe5;
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->EMUCON = sendBuf[i];
      recvBuf[recvPtr] = EMU->EMUCON;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x001fffff)){
      }
      else 
      {				 	
#ifdef PRINT
        printf("No.%d EMUCON  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x001fffff)); 	   
#endif	
	    Fail();
      }
    }
	EMU->EMUCON = 0x001c0007;

	recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->EMUCON2 = sendBuf[i];
      recvBuf[recvPtr] = EMU->EMUCON2;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x00ffffff)){
      }
      else 
      {			 	
#ifdef PRINT
        printf("No.%d EMUCON2  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x00ffffff));	   
#endif	
	    Fail();
      }
    }
    EMU->EMUCON2 = 0x0;

    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->HFConst = sendBuf[i];
      recvBuf[recvPtr] = EMU->HFConst;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {					
#ifdef PRINT
        printf("No.%d HFConst  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->PStart = sendBuf[i];
      recvBuf[recvPtr] = EMU->PStart;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {				 	
#ifdef PRINT
        printf("No.%d PStart  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->QStart = sendBuf[i];
      recvBuf[recvPtr] = EMU->QStart;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {				 	
#ifdef PRINT
        printf("No.%d QStart  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));   	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->GPQA = sendBuf[i];
      recvBuf[recvPtr] = EMU->GPQA;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {					
#ifdef PRINT
        printf("No.%d GPQA  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	  	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->GPQB = sendBuf[i];
      recvBuf[recvPtr] = EMU->GPQB;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {				 	
#ifdef PRINT
        printf("No.%d GPQB  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	  	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->PhsA = sendBuf[i];
      recvBuf[recvPtr] = EMU->PhsA;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x000001ff)){
      }
      else 
      {				  	
#ifdef PRINT
        printf("No.%d PhsA  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x000001ff));		   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->PhsB = sendBuf[i];
      recvBuf[recvPtr] = EMU->PhsB;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x000001ff)){
      }
      else 
      {				 	
#ifdef PRINT
        printf("No.%d PhsB  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x000001ff));		   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->QPhsCal = sendBuf[i];
      recvBuf[recvPtr] = EMU->QPhsCal;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {				  	
#ifdef PRINT
        printf("No.%d QPhsCal  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->APOSA = sendBuf[i];
      recvBuf[recvPtr] = EMU->APOSA;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {				   	
#ifdef PRINT
        printf("No.%d APOSA  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));		   
#endif	
	    Fail();
      }
    }     


    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->APOSB = sendBuf[i];
      recvBuf[recvPtr] = EMU->APOSB;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {						
#ifdef PRINT
        printf("No.%d APOSB  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	   	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->RPOSA = sendBuf[i];
      recvBuf[recvPtr] = EMU->RPOSA;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {					  	
#ifdef PRINT
        printf("No.%d RPOSA  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	   	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->RPOSB = sendBuf[i];
      recvBuf[recvPtr] = EMU->RPOSB;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {					  	
#ifdef PRINT
        printf("No.%d RPOSB  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	  	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->IARMSOS = sendBuf[i];
      recvBuf[recvPtr] = EMU->IARMSOS;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {					   	
#ifdef PRINT
        printf("No.%d IARMSOS  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->IBRMSOS = sendBuf[i];
      recvBuf[recvPtr] = EMU->IBRMSOS;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {							
#ifdef PRINT
        printf("No.%d IBRMSOS  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));  	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->URMSOS = sendBuf[i];
      recvBuf[recvPtr] = EMU->URMSOS;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {						
#ifdef PRINT
        printf("No.%d URMSOS  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->IAGAIN = sendBuf[i];
      recvBuf[recvPtr] = EMU->IAGAIN;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {						
#ifdef PRINT
        printf("No.%d IAGAIN  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));   	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->IBGAIN = sendBuf[i];
      recvBuf[recvPtr] = EMU->IBGAIN;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {							
#ifdef PRINT
        printf("No.%d IBGAIN  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));  	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->UGAIN = sendBuf[i];
      recvBuf[recvPtr] = EMU->UGAIN;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {					  	
#ifdef PRINT
        printf("No.%d UGAIN  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	  	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->IADCOS = sendBuf[i];
      recvBuf[recvPtr] = EMU->IADCOS;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x00ffffff)){
      }
      else 
      {							
#ifdef PRINT
        printf("No.%d IADCOS  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x00ffffff));  	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->IBDCOS = sendBuf[i];
      recvBuf[recvPtr] = EMU->IBDCOS;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x00ffffff)){
      }
      else 
      {						 	
#ifdef PRINT
        printf("No.%d IBDCOS  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x00ffffff));   	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->UDCOS = sendBuf[i];
      recvBuf[recvPtr] = EMU->UDCOS;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x00ffffff)){
      }
      else 
      {						   	
#ifdef PRINT
        printf("No.%d UDCOS  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x00ffffff));	  	   
#endif	
	    Fail();
      }
    }

    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->UADD = sendBuf[i];
      recvBuf[recvPtr] = EMU->UADD;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x00ffffff)){
      }
      else 
      {						 	
#ifdef PRINT
        printf("No.%d UADD  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x00ffffff));	 	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->USAG = sendBuf[i];
      recvBuf[recvPtr] = EMU->USAG;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {						 	
#ifdef PRINT
        printf("No.%d USAG  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	 	   
#endif	
	    Fail();
      }
    }

	recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->IAPEAK = sendBuf[i];
      recvBuf[recvPtr] = EMU->IAPEAK;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {						 	
#ifdef PRINT
        printf("No.%d IAPEAK  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff)); 	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->IBPEAK = sendBuf[i];
      recvBuf[recvPtr] = EMU->IBPEAK;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {						  	
#ifdef PRINT
        printf("No.%d IBPEAK  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));   	   
#endif	
	    Fail();
      }
    }
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->UPEAK = sendBuf[i];
      recvBuf[recvPtr] = EMU->UPEAK;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {						   	
#ifdef PRINT
        printf("No.%d UPEAK  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	   	   
#endif	
	    Fail();
      }
    }	    
    
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->D2FP = sendBuf[i];
      recvBuf[recvPtr] = EMU->D2FP;
      if(recvBuf[recvPtr++] == sendBuf[i++] ){
      }
      else 
      {								
#ifdef PRINT
        printf("No.%d D2FP  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],sendBuf[i-1]);	   					   
#endif	
	    Fail();
      }
    }

    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->IE = sendBuf[i];
      recvBuf[recvPtr] = EMU->IE;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x00ffffff)){
      }
      else 
      {				   	
#ifdef PRINT
        printf("No.%d IE  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x00ffffff));  		   
#endif	
	    Fail();
      }
    }

	recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->DMA_en = sendBuf[i];
      recvBuf[recvPtr] = EMU->DMA_en;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x00ffffff)){
      }
      else 
      {			  	
#ifdef PRINT
        printf("No.%d DMA_en  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x00ffffff));  	   
#endif	
	    Fail();
      }
    }	
#ifdef PRINT
  printf("EMU reg write/read ok !\n");		   
#endif	
}
 /*********************************************************************************************************
** Function name:       reg_default
** Descriptions:        reg_default
** input parameters:    none
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
void reg_wr2(void){		
#ifdef PRINT
    printf("EMU reg write/read TEST :\n");		
#endif	
    sendBuf[ 0] =0x5555;sendBuf[ 1] =0xaaaa;sendBuf[ 2] =0x3fff;sendBuf[ 3] =0x0;
	SYSCTL->SYS_PS = 0x82 ;
	SYSCTL->SYS_PD = 0x0; 
	EMU->SPCMD = 0xe5;
	EMU->HFConst = 0x0000ffff;
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->PF2cnt = sendBuf[i];
      recvBuf[recvPtr] = EMU->PF2cnt;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {				 	
#ifdef PRINT
        printf("No.%d PF2cnt  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));  	   
#endif	
	    Fail();
      }
    }    

    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->QF2cnt = sendBuf[i];
      recvBuf[recvPtr] = EMU->QF2cnt;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {				  	
#ifdef PRINT
        printf("No.%d QF2cnt  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));  	   
#endif	
	    Fail();
      }
    }
 
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->SF2cnt = sendBuf[i];
      recvBuf[recvPtr] = EMU->SF2cnt;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {					   	
#ifdef PRINT
        printf("No.%d SF2cnt  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));  	   
#endif	
	    Fail();
      }
    }
	
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->PFcnt = sendBuf[i] & 0x0000ffff;
      recvBuf[recvPtr] = EMU->PFcnt;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {					   	
#ifdef PRINT
        printf("No.%d PFcnt  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	  	   
#endif	
	    Fail();
      }
    }    

    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->QFcnt = sendBuf[i];
      recvBuf[recvPtr] = EMU->QFcnt;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {					   	
#ifdef PRINT
        printf("No.%d QFcnt  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	   	   
#endif	
	    Fail();
      }
    }
 
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->SFcnt = sendBuf[i];
      recvBuf[recvPtr] = EMU->SFcnt;
      if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff)){
      }
      else 
      {					   	
#ifdef PRINT
        printf("No.%d SFcnt  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	  	   
#endif	
	    Fail();
      }
    }
}
 /*********************************************************************************************************
** Function name:       reg_default
** Descriptions:        reg_default
** input parameters:    none
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
void reg_default(void){	   
#ifdef PRINT
    printf("EMU reg default TEST :\n");		
#endif	

    recvPtr = 0;
    i = 0;
    
    data = EMU->EMUCON;
    if(data ==  0x001c0007){
     
    }
    else 
    {						 	
#ifdef PRINT
      printf("EMUCON  is 0x%02x(should be 0x001c0007).\n",data);								   
#endif	
	    Fail();
    }
	data = EMU->EMUCON2;
    if(data ==  0x00000000){
     
    }
    else 
    {							
#ifdef PRINT
      printf("EMUCON2 is 0x%02x(should be 0x00000000).\n",data);  									  
#endif	
	    Fail();
    }
    data = EMU->HFConst;
    if(data ==  0x00001000){
     
    }
    else 
    {							
#ifdef PRINT
      printf("HFConst  is 0x%02x(should be 0x00001000).\n",data);  									  
#endif	
	    Fail();
    }
    data = EMU->PStart;
    if(data ==  0x00000060){
     
    }
    else 
    {					   	
#ifdef PRINT
      printf("PStart  is 0x%02x(should be 0x00000060).\n",data);   								   
#endif	
	    Fail();
    }
    data = EMU->QStart;
    if(data ==  0x00000120){
     
    }
    else 
    {					   	
#ifdef PRINT
      printf("QStart  is 0x%02x(should be 0x00000120).\n",data);							   
#endif	
	    Fail();
    }
    data = EMU->GPQA;
    if(data ==  0x00000000){
     
    }
    else 
    {							
#ifdef PRINT
      printf("GPQA  is 0x%02x(should be 0x00000000).\n",data);	  							  
#endif	
	    Fail();
    }
    data = EMU->GPQB;
    if(data ==  0x00000000){
     
    }
    else 
    {							
#ifdef PRINT
      printf("GPQB  is 0x%02x(should be 0x00000000).\n",data);	 						 
#endif	
	    Fail();
    }
    data = EMU->PhsA;
    if(data ==  0x00000000){
     
    }
    else 
    {					   	
#ifdef PRINT
      printf("PhsA  is 0x%02x(should be 0x00000000).\n",data);							  
#endif	
	    Fail();
    }
    data = EMU->PhsB;
    if(data ==  0x00000000){
     
    }
    else 
    {							
#ifdef PRINT
      printf("PhsB  is 0x%02x(should be 0x00000000).\n",data); 						
#endif	
	    Fail();
    }
    data = EMU->QPhsCal;
    if(data ==  0x00000000){
     
    }
    else 
    {					   	
#ifdef PRINT
      printf("QPhsCal  is 0x%02x(should be 0x00000000).\n",data);					 
#endif	
    }
    data = EMU->APOSA;
    if(data ==  0x00000000){
     
    }
    else 
    {					   	
#ifdef PRINT
      printf("APOSA  is 0x%02x(should be 0x00000000).\n",data);	 					
#endif	
	    Fail();
    }
    data = EMU->APOSB;
    if(data ==  0x00000000){
     
    }
    else 
    {					   	
#ifdef PRINT
      printf("APOSB  is 0x%02x(should be 0x00000000).\n",data);	 				   
#endif	
	    Fail();
    }
    data = EMU->RPOSA;
    if(data ==  0x00000000){
     
    }
    else 
    {							
#ifdef PRINT
      printf("RPOSA  is 0x%02x(should be 0x00000000).\n",data);	   					 
#endif	
	    Fail();
    }
    data = EMU->RPOSB;
    if(data ==  0x00000000){
     
    }
    else 
    {					   	
#ifdef PRINT
      printf("RPOSB  is 0x%02x(should be 0x00000000).\n",data);	   					  
#endif	
	    Fail();
    }
    data = EMU->IARMSOS;
    if(data ==  0x00000000){
     
    }
    else 
    {						 	
#ifdef PRINT
      printf("IARMSOS  is 0x%02x(should be 0x00000000).\n",data); 				  
#endif	
	    Fail();
    }
    data = EMU->IBRMSOS;
    if(data ==  0x00000000){
     
    }
    else 
    {						 	
#ifdef PRINT
      printf("IBRMSOS  is 0x%02x(should be 0x00000000).\n",data);				   
#endif	
	    Fail();
    }
	data = EMU->URMSOS;
    if(data ==  0x00000000){
     
    }
    else 
    {						 	
#ifdef PRINT
      printf("URMSOS  is 0x%02x(should be 0x00000000).\n",data);   				
#endif	
	    Fail();
    }	  
    data = EMU->IAGAIN;
    if(data ==  0x00000000){
     
    }
    else 
    {						 	
#ifdef PRINT
      printf("IAGAIN  is 0x%02x(should be 0x00000000).\n",data);   				  
#endif	
	    Fail();
    }
    data = EMU->IBGAIN;
    if(data ==  0x00000000){
     
    }
    else 
    {						 	
#ifdef PRINT
      printf("IBGAIN  is 0x%02x(should be 0x00000000).\n",data);   			  
#endif	
	    Fail();
    }
    data = EMU->UGAIN;
    if(data ==  0x00000000){
     
    }
    else 
    {						 	
#ifdef PRINT
      printf("UGAIN  is 0x%02x(should be 0x00000000).\n",data);	   				  
#endif	
	    Fail();
    }
    data = EMU->IADCOS;
    if(data ==  0x00000000){
     
    }
    else 
    {						 	
#ifdef PRINT
      printf("IADCOS  is 0x%02x(should be 0x00000000).\n",data); 			  
#endif	
	    Fail();
    }
    data = EMU->IBDCOS;
    if(data ==  0x00000000){
     
    }
    else 
    {						 	
#ifdef PRINT
      printf("IBDCOS  is 0x%02x(should be 0x00000000).\n",data);			   
#endif	
	    Fail();
    }
    data = EMU->UDCOS;
    if(data ==  0x00000000){
     
    }
    else 
    {						 	
#ifdef PRINT
      printf("UDCOS  is 0x%02x(should be 0x00000000).\n",data);				 
#endif	
	    Fail();
    }
    data = EMU->UADD;
    if(data ==  0x00000000){
     
    }
    else 
    {						 	
#ifdef PRINT
      printf("UADD  is 0x%02x(should be 0x00000000).\n",data);	   			  
#endif	
	    Fail();
    }
    data = EMU->USAG;
    if(data ==  0x00000000){
     
    }
    else 
    {						  	
#ifdef PRINT
      printf("USAG  is 0x%02x(should be 0x00000000).\n",data);	 			 
#endif	
	    Fail();
    } 
    data = EMU->IAPEAK;
    if(data ==  0x00000000){
     
    }
    else 
    {						  	
#ifdef PRINT
      printf("IAPEAK  is 0x%02x(should be 0x00000000).\n",data);			  
#endif	
	    Fail();
    }
    data = EMU->IBPEAK;
    if(data ==  0x00000000){
     
    }
    else 
    {						   	
#ifdef PRINT
      printf("IBPEAK  is 0x%02x(should be 0x00000000).\n",data);			   
#endif	
	    Fail();
    }
    data = EMU->UPEAK;
    if(data ==  0x00000000){
     
    }
    else 
    {								
#ifdef PRINT
      printf("UPEAK  is 0x%02x(should be 0x00000000).\n",data);					
#endif	
	    Fail();
    }
    data = EMU->D2FP;
    if(data ==  0x00000000){
     
    }
    else 
    {						   	
#ifdef PRINT
      printf("D2FP  is 0x%02x(should be 0x00000000).\n",data);	  				 
#endif	
	    Fail();
    }
    data = EMU->PF2cnt;
    if(data ==  0x00000000){
     
    }
    else 
    {						   	
#ifdef PRINT
      printf("PF2cnt  is 0x%02x(should be 0x00000000).\n",data);				 
#endif	
	    Fail();
    }
    data = EMU->QF2cnt;
    if(data ==  0x00000000){
     
    }
    else 
    {						   	
#ifdef PRINT
      printf("QF2cnt  is 0x%02x(should be 0x00000000).\n",data);   					
#endif	
	    Fail();
    }	
    data = EMU->SF2cnt;
    if(data ==  0x00000000){
     
    }
    else 
    {						   	
#ifdef PRINT
      printf("SF2cnt  is 0x%02x(should be 0x00000000).\n",data);					
#endif	
	    Fail();
    }
    data = EMU->PFcnt;
    if(data ==  0x00000000){
     
    }
    else 
    {						   	
#ifdef PRINT
      printf("PFcnt  is 0x%02x(should be 0x00000000).\n",data);					   
#endif	
	    Fail();
    }
    data = EMU->QFcnt;
    if(data ==  0x00000000){
     
    }
    else 
    {								
#ifdef PRINT
      printf("QFcnt  is 0x%02x(should be 0x00000000).\n",data);	   					  
#endif	
	    Fail();
    }	
    data = EMU->SFcnt;
    if(data ==  0x00000000){
     
    }
    else 
    {						   	
#ifdef PRINT
      printf("SFcnt  is 0x%02x(should be 0x00000000).\n",data);	  					 
#endif	
	    Fail();
    }
    data = EMU->RmsIA;
    if(data ==  0x00000000){
     
    }
    else 
    {						  	
#ifdef PRINT
      printf("RmsIA  is 0x%02x(should be 0x00000000).\n",data);		 				 
#endif	
	    Fail();
    }
    data = EMU->RmsIB;
    if(data ==  0x00000000){
     
    }
    else 
    {						 	
#ifdef PRINT
      printf("RmsIB  is 0x%02x(should be 0x00000000).\n",data);		 				
#endif	
	    Fail();
    }
    data = EMU->RmsU;
    if(data ==  0x00000000){
     
    }
    else 
    {						 	
#ifdef PRINT
      printf("RmsU  is 0x%02x(should be 0x00000000).\n",data);					  
#endif	
	    Fail();
    }
    data = EMU->FreqU;
    if(data ==  0x00002400){
     
    }
    else 
    {						 	
#ifdef PRINT
      printf("FreqU  is 0x%02x(should be 0x00002400).\n",data);					 
#endif	
	    Fail();
    }
    data = EMU->PowerPA;
    if(data ==  0x00000000){
     
    }
    else 
    {						  	
#ifdef PRINT
      printf("PowerPA  is 0x%02x(should be 0x00000000).\n",data);				 
#endif	
	    Fail();
    }
    data = EMU->PowerPB;
    if(data ==  0x00000000){
     
    }
    else 
    {						   	
#ifdef PRINT
      printf("PowerPB  is 0x%02x(should be 0x00000000).\n",data);	 			
#endif	
	    Fail();
    }
    data = EMU->PowerQA;
    if(data ==  0x00000000){
     
    }
    else 
    {						   	
#ifdef PRINT
      printf("PowerQA  is 0x%02x(should be 0x00000000).\n",data);	 		  
#endif	
	    Fail();
    } 
	data = EMU->PowerQB;
    if(data ==  0x00000000){
     
    }
    else 
    {						   	
#ifdef PRINT
      printf("PowerQB  is 0x%02x(should be 0x00000000).\n",data);	 		   
#endif	
	    Fail();
    }
	data = EMU->PowerSA;
    if(data ==  0x00000000){
     
    }
    else 
    {						   	
#ifdef PRINT
      printf("PowerSA  is 0x%02x(should be 0x00000000).\n",data);	 			
#endif	
	    Fail();
    } 
	data = EMU->PowerSB;
    if(data ==  0x00000000){
     
    }
    else 
    {						   	
#ifdef PRINT
      printf("PowerSB  is 0x%02x(should be 0x00000000).\n",data);			   
#endif	
	    Fail();
    }  
    data = EMU->Energy_P;
    if(data ==  0x00000000){
     
    }
    else 
    {								
#ifdef PRINT
      printf("Energy_P  is 0x%02x(should be 0x00000000).\n",data);	   			
#endif	
	    Fail();
    } 
    data = EMU->Energy_P2;
    if(data ==  0x00000000){
     
    }
    else 
    {						   	
#ifdef PRINT
      printf("Energy_P2  is 0x%02x(should be 0x00000000).\n",data);	   		  
#endif	
	    Fail();
    }
    data = EMU->Energy_Q;
    if(data ==  0x00000000){
     
    }
    else 
    {								
#ifdef PRINT
      printf("Energy_Q  is 0x%02x(should be 0x00000000).\n",data);	   		  
#endif	
	    Fail();
    }
    data = EMU->Energy_Q2;
    if(data ==  0x00000000){
     
    }
    else 
    {								
#ifdef PRINT
      printf("Energy_Q2  is 0x%02x(should be 0x00000000).\n",data);			 
#endif	
	    Fail();
    }
	data = EMU->Energy_S;
    if(data ==  0x00000000){
     
    }
    else 
    {							 	
#ifdef PRINT
      printf("Energy_S  is 0x%02x(should be 0x00000000).\n",data);			   
#endif	
	    Fail();
    }	   
    data = EMU->PFA;
    if(data ==  0x00000000){
     
    }
    else 
    {							 	
#ifdef PRINT
      printf("PFA  is 0x%02x(should be 0x00000000).\n",data);		 
#endif	  
	    Fail();
    }
    data = EMU->PFB;
    if(data ==  0x00000000){
     
    }
    else 
    {							 	
#ifdef PRINT
      printf("PFB  is 0x%02x(should be 0x00000000).\n",data);		
#endif	  
	    Fail();
    }
    data = EMU->ANGLEA;
    if(data ==  0x00000000){
     
    }
    else 
    {							 	
#ifdef PRINT
      printf("ANGLEA  is 0x%02x(should be 0x00000000).\n",data);
#endif		  
	    Fail();
    }
    data = EMU->ANGLEB;
    if(data ==  0x00000000){
     
    }
    else 
    {							 	
#ifdef PRINT
      printf("ANGLEB  is 0x%02x(should be 0x00000000).\n",data);	  		
#endif	
	    Fail();
    }
	data = EMU->EMUStatus;
    if(data ==  0x00e3ee78){
     
    }
    else 
    {							 	
#ifdef PRINT
      printf("EMUStatus  is 0x%02x(should be 0x00e3ee78).\n",data);	  		 
#endif	
	    Fail();
    }
    data = EMU->SPL_IA;
    if(data ==  0x00000000){
     
    }
    else 
    {							  	
#ifdef PRINT
      printf("SPL_IA  is 0x%02x(should be 0x00000000).\n",data);	  		
#endif	
	    Fail();
    }
    data = EMU->SPL_IB;
    if(data ==  0x00000000){
     
    }
    else 
    {							   	
#ifdef PRINT
      printf("SPL_IB  is 0x%02x(should be 0x00000000).\n",data);	  			 
#endif	
	    Fail();
    }
    data = EMU->SPL_U;
    if(data ==  0x00000000){
     
    }
    else 
    {							   	
#ifdef PRINT
      printf("SPL_U  is 0x%02x(should be 0x00000000).\n",data);		  		  
#endif	
	    Fail();
    }
    data = EMU->PowerPA2;
    if(data ==  0x00000000){
     
    }
    else 
    {								  	
#ifdef PRINT
      printf("PowerPA2  is 0x%02x(should be 0x00000000).\n",data);	  	  
#endif	
	    Fail();
    }
    data = EMU->PowerPB2;
    if(data ==  0x00000000){
     
    }
    else 
    {								 	
#ifdef PRINT
      printf("PowerPB2  is 0x%02x(should be 0x00000000).\n",data);	  	  
#endif	
	    Fail();
    }
    data = EMU->Energy_S2;
    if(data ==  0x00000000){
     
    }
    else 
    {								  	
#ifdef PRINT
      printf("Energy_S2  is 0x%02x(should be 0x00000000).\n",data);	 		  
#endif	
	    Fail();
    }
    data = EMU->SPL_PA;
    if(data ==  0x00000000){
     
    }
    else 
    {								 	
#ifdef PRINT
      printf("SPL_PA  is 0x%02x(should be 0x00000000).\n",data);	  		 
#endif	
	    Fail();
    }
    data = EMU->SPL_PB;
    if(data ==  0x00000000){
     
    }
    else 
    {								   	
#ifdef PRINT
      printf("SPL_PB  is 0x%02x(should be 0x00000000).\n",data);	   		  
#endif	
	    Fail();
    }
    data = EMU->SPL_QA;
    if(data ==  0x00000000){
     
    }
    else 
    {										
#ifdef PRINT
      printf("SPL_QA  is 0x%02x(should be 0x00000000).\n",data);	 	   
#endif	
	    Fail();
    }
    data = EMU->SPL_QB;
    if(data ==  0x00000000){
     
    }
    else 
    {									 	
#ifdef PRINT
      printf("SPL_QB  is 0x%02x(should be 0x00000000).\n",data);	  		 
#endif	
	    Fail();
    }
    data = EMU->IE;
    if(data ==  0x00000000){
     
    }
    else 
    {								   	
#ifdef PRINT
      printf("IE  is 0x%02x(should be 0x00000000).\n",data);		 	
#endif	
	    Fail();
    }
    data = EMU->IF;
    if(data ==  0x00000000){
     
    }
    else 
    {								   	
#ifdef PRINT
      printf("IF  is 0x%02x(should be 0x00000000).\n",data);		   
#endif	
	    Fail();
    }
	data = EMU->DMA_en;
    if(data ==  0x00000000){
     
    }
    else 
    {								   	
#ifdef PRINT
      printf("DMA_en  is 0x%02x(should be 0x00000000).\n",data);	   
#endif	
	    Fail();
    }
    data = EMU->RData;
    if(data ==  0x00000000){
     
    }
    else 
    {									 	
#ifdef PRINT
      printf("RData  is 0x%02x(should be 0x00000000).\n",data);		  	
#endif	
	    Fail();
    }
    data = EMU->WData;
    if(data ==  0x00000000){
     
    }
    else 
    {									  	
#ifdef PRINT
      printf("WData  is 0x%02x(should be 0x00000000).\n",data);		 	 
#endif	
	    Fail();
    }
    data = EMU->SPCMD;
    if(data ==  0x00000000){
     
    }
    else 
    {									  	
#ifdef PRINT
      printf("SPCMD  is 0x%02x(should be 0x00000000).\n",data);			   
#endif	
	    Fail();
    }	
#ifdef PRINT
  printf("EMU reg default ok !\n");			
#endif	
}
/*********************************************************************************************************
** Function name:       main
** Descriptions:        User Programn Entry Example
** input parameters:    none
** output parameters:   none
** Returned value:      none/never return
*********************************************************************************************************/
int main (void)
{								
	__enable_irq();
//    NVIC_EnableIRQ(EMU_IRQn);
	NVIC_EnableIRQ(WDT_IRQn);
	test_flag_init();
	WDT_OFF();
    SYSCTL->SYS_PS = 0x82 ;
	SYSCTL->MOD1_EN	|= 1<<9;		

    SysModeChg(2, 1);				
	uartInit( 0 ,9600 ,0 ,0 ,0 ) ; 

	SYSCTL->SYS_PS = 0x82 ;
	SYSCTL->SYS_RST = 0x1e0;
	SYSCTL->ADC_CTRL = 0x0;		//16倍增益

	SYSCTL->MOD1_EN |= 0xa0;	
	    
	GPIO->PCB = 0xc3 << 8 ;
//	printf("EMU test !\n");
	swDelay(50);

						  
	reg_default();
							   
	SYSCTL->SYS_PD = 0x0;  	 	
    EMU->SPCMD = 0xe5;	   
						  
    EMU->PhsA = 0x100;
    EMU->PhsB = 0x100;
//功率测试,两套功率寄存器
    power_test();
				
//有效值测试
    rms_test();		 
//频率测试
    ufreq_test();
////CF脉冲频率测试
	cf_test();
//    swDelay(1000);
//	printf("%d\n",EMU->PowerPA);
//	printf("%d\n",EMU->PowerPB);
//	printf("%d\n",EMU->PowerQA);
//	printf("%d\n",EMU->PowerQB);
//	printf("%d\n",EMU->PowerSA);
//	printf("%d\n",EMU->PowerSB);
//	printf("%d\n",EMU->RmsIA);
//	printf("%d\n",EMU->RmsIB);
//	printf("%d\n",EMU->RmsU);

//寄存器读写	
    reg_wr1();	 	   

    reg_wr2();		//输入断开之后测试
	
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->SYS_PD = 0x3f;  

	Pass();
}

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
