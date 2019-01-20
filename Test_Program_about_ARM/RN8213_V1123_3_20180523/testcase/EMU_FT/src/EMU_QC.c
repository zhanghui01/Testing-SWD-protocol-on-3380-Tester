#include "rn821x.h"
#include "common.h"
#include "sysctrl.h"
#include <stdio.h>
#include "nvm.h"

#define A_Ch
#define B_Ch

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
    *test_flag = 0x12345675 ;
//    printf("TEST Fail!\n");
	while(1);
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
//    printf("TEST Ok!\n");
	while(1)
		WdtFeed();
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
void WDT_HANDLER(void)
{    
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
void WDT_OFF (void) 
{
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
void swDelay(uint32_t ulTime) 
{
  uint32_t i;

  i = 0;
  while(ulTime--) 
  {
    for (i = 0; i < 5000; i++)
      ;
  }
}

 /*********************************************************************************************************
** Function name:       power_test
** Descriptions:        power_test
** input parameters:    none
**三路信号均用信号源输入200mVrms的信号
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
//void power_test2(void){	
//
//  if( (EMU->PowerPA < 530000000) || (EMU->PowerPA > 670000000))
//  {	  	
//      MSG("Err, PowerPA %d\n",EMU->PowerPA);
//	  Fail();
//  }
//  if( (EMU->PowerPB < 530000000) || (EMU->PowerPB > 670000000))
//  {				  	
//      MSG("Err, PowerPB %d\n",EMU->PowerPB);
//	  Fail();
//  }	 
//  if( (EMU->PowerQA < 28000000) || (EMU->PowerQA > 37000000))
//  {				 	
//      MSG("Err, PowerQA %d\n",EMU->PowerQA);	 
//	  Fail();
//  }		 
//  if( (EMU->PowerQB < 28000000) || (EMU->PowerQB > 37000000))
//  {				   	
//      MSG("Err, PowerQB %d\n",EMU->PowerQB);	  
//	  Fail();
//  }	  	 
//  if( (EMU->PowerSA < 530000000) || (EMU->PowerSA > 670000000))
//  {				  		
//      MSG("Err, PowerSA %d\n",EMU->PowerSA);	 
//	  Fail();
//  }			   	  	 
//  if( (EMU->PowerSB < 530000000) || (EMU->PowerSB > 670000000))
//  {				   	
//      MSG("Err, PowerSB %d\n",EMU->PowerSB);	 
//	  Fail();
//  }	  		   	  	 
//  if( (EMU->PowerPA2 < 530000000) || (EMU->PowerPA2 > 670000000))
//  {				  	 	
//      MSG("Err, PowerPA2 %d\n",EMU->PowerPA2); 	  
//	  Fail();
//  }	     		   	  	 
//  if( (EMU->PowerPB2 < 530000000) || (EMU->PowerPB2 > 670000000))
//  {				   	
//      MSG("Err, PowerPB2 %d\n",EMU->PowerPB2);  	
//	  Fail();
//  }	      		   	  	  
//
//}

/*********************************************************************************************************
** Function name:       Urms_test
** Descriptions:        Urms_test
** input parameters:    num
** output parameters:   none		  
**三路信号均用信号源输入200mVrms的信号
** Returned value:      none
*********************************************************************************************************/
//void rms_test2(void){	
//
//  if( (EMU->RmsIA < (2750000*1)) || (EMU->RmsIA > (3550000*1)))
//  {				  		
//      MSG("Err, RmsIA %d\n",EMU->RmsIA);	 
//	  Fail();
//  }	      		   	  	 
//  if( (EMU->RmsIB < (2750000*1)) || (EMU->RmsIB > (3550000*1)))
//  {				  		
//      MSG("Err, RmsIB %d\n",EMU->RmsIB);	
//	  Fail();
//  }	        		   	  	 
//  if( (EMU->RmsU < (2750000*1)) || (EMU->RmsU > (3550000*1)))
//  {				  		
//      MSG("Err, RmsU %d\n",EMU->RmsU);	 
//	  Fail();
//  }	
//}


 /*********************************************************************************************************
** Function name:       reg_wr1
** Descriptions:        reg_wr1
** input parameters:    none
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
#ifdef A_Ch
void reg_wr1(void)
{	 

  sendBuf[ 0]=0x55555555 ;
  sendBuf[ 1]=0xaaaaaaaa ;
  sendBuf[ 2]=0x0        ;
  sendBuf[ 3]=0xffffffff ;

	EMU->SPCMD = 0xe5;
    
  recvPtr = 0;
  i = 0;    
  while(recvPtr < 4)
  {      
    EMU->EMUCON = sendBuf[i];
    recvBuf[recvPtr] = EMU->EMUCON;
    if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x001fffff))
    {
    	
    }
    else 
    {			
      MSG("No.%d EMUCON  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x001fffff)); 	
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
    if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x00ffffff))
    {
    	
    }
    else 
    {			 
      MSG("No.%d EMUCON2  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x00ffffff));	 
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
    if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff))
    {
    	
    }
    else 
    {					
      MSG("No.%d HFConst  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	
	    Fail();
    }
  }
  
  recvPtr = 0;
  i = 0;    
  while(recvPtr < 4)
  {      
    EMU->PStart = sendBuf[i];
    recvBuf[recvPtr] = EMU->PStart;
    if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff))
    {
    	
    }
    else 
    {				 	
      MSG("No.%d PStart  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));
	    Fail();
    }
  }
  
  recvPtr = 0;
  i = 0;    
  while(recvPtr < 4)
  {      
    EMU->QStart = sendBuf[i];
    recvBuf[recvPtr] = EMU->QStart;
    if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff))
    {
    	
    }
    else 
    {				 	
      MSG("No.%d QStart  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff)); 
	    Fail();
    }
  }
  
  recvPtr = 0;
  i = 0;    
  while(recvPtr < 4)
  {      
    EMU->GPQA = sendBuf[i];
    recvBuf[recvPtr] = EMU->GPQA;
    if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff))
    {
    	
    }
    else 
    {					
      MSG("No.%d GPQA  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	 
	   Fail();
    }
  }
  
  recvPtr = 0;
  i = 0;    
  while(recvPtr < 4)
  {      
    EMU->GPQB = sendBuf[i];
    recvBuf[recvPtr] = EMU->GPQB;
    if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff))
    {
    	
    }
    else 
    {				 	
      MSG("No.%d GPQB  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	 
	    Fail();
    }
  }
  
  recvPtr = 0;
  i = 0;    
  while(recvPtr < 4)
  {      
    EMU->PhsA = sendBuf[i];
    recvBuf[recvPtr] = EMU->PhsA;
    if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x000001ff))
    {
    	
    }
    else 
    {				  	
      MSG("No.%d PhsA  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x000001ff));	
	    Fail();
    }
  }
  
  recvPtr = 0;
  i = 0;    
  while(recvPtr < 4)
  {      
    EMU->PhsB = sendBuf[i];
    recvBuf[recvPtr] = EMU->PhsB;
    if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x000001ff))
    {
    	
    }
    else 
    {				 	
      MSG("No.%d PhsB  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x000001ff));	
	    Fail();
    }
  }
  
  recvPtr = 0;
  i = 0;    
  while(recvPtr < 4)
  {      
    EMU->QPhsCal = sendBuf[i];
    recvBuf[recvPtr] = EMU->QPhsCal;
    if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff))
    {
    	
    }
    else 
    {				  	
      MSG("No.%d QPhsCal  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	 
	    Fail();
    }
  }
  
  recvPtr = 0;
  i = 0;    
  while(recvPtr < 4)
  {      
    EMU->APOSA = sendBuf[i];
    recvBuf[recvPtr] = EMU->APOSA;
    if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff))
    {
    	
    }
    else 
    {				   	
      MSG("No.%d APOSA  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));
	    Fail();
    }
  }     


  recvPtr = 0;
  i = 0;    
  while(recvPtr < 4)
  {      
    EMU->APOSB = sendBuf[i];
    recvBuf[recvPtr] = EMU->APOSB;
    if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff))
    {
    	
    }
    else 
    {						
      MSG("No.%d APOSB  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));
	  Fail();
    }
  }
  
  recvPtr = 0;
  i = 0;    
  while(recvPtr < 4)
  {      
    EMU->RPOSA = sendBuf[i];
    recvBuf[recvPtr] = EMU->RPOSA;
    if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff))
    {
    	
    }
    else 
    {					  	
      MSG("No.%d RPOSA  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));
	    Fail();
    }
  }
  
  recvPtr = 0;
  i = 0;    
  while(recvPtr < 4)
  {      
    EMU->RPOSB = sendBuf[i];
    recvBuf[recvPtr] = EMU->RPOSB;
    if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff))
    {
    
    }
    else 
    {					  	
      MSG("No.%d RPOSB  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));
	    Fail();
    }
  }
  
  recvPtr = 0;
  i = 0;    
  while(recvPtr < 4)
  {      
    EMU->IARMSOS = sendBuf[i];
    recvBuf[recvPtr] = EMU->IARMSOS;
    if(recvBuf[recvPtr++] == (sendBuf[i++] & 0x0000ffff))
    {
    	
    }
    else 
    {					   	
      MSG("No.%d IARMSOS  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	 
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
      MSG("No.%d IBRMSOS  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));  
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
      MSG("No.%d URMSOS  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	
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
      MSG("No.%d IAGAIN  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));   
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
      MSG("No.%d IBGAIN  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));  	
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
      MSG("No.%d UGAIN  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));
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
      MSG("No.%d IADCOS  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x00ffffff));  	   
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
      MSG("No.%d IBDCOS  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x00ffffff));   	
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
      MSG("No.%d UDCOS  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x00ffffff));	  	
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
      MSG("No.%d UADD  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x00ffffff));	 	   
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
      MSG("No.%d USAG  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	 	  
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
        MSG("No.%d IAPEAK  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff)); 	   
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
        MSG("No.%d IBPEAK  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));   
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
        MSG("No.%d UPEAK  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	   	
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
        MSG("No.%d D2FP  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],sendBuf[i-1]);	   			
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
        MSG("No.%d IE  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x00ffffff));  	
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
        MSG("No.%d DMA_en  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x00ffffff));  	 
	    Fail();
      }
    }	

}
#endif
 /*********************************************************************************************************
** Function name:       reg_default
** Descriptions:        reg_default
** input parameters:    none
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
#ifdef B_Ch
void reg_wr2(void){		
	
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
      if( (recvBuf[recvPtr] == (sendBuf[i] & 0x0000ffff)) || (recvBuf[recvPtr] == ((sendBuf[i] & 0x0000ffff) + 1))){
	    recvPtr++ ;
		i++ ;
      }
	  else
      {				 	
        MSG("No.%d PF2cnt  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));  	 
	    Fail();
      }
    }    

    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->QF2cnt = sendBuf[i];
      recvBuf[recvPtr] = EMU->QF2cnt;
      if( (recvBuf[recvPtr] == (sendBuf[i] & 0x0000ffff)) || (recvBuf[recvPtr] == ((sendBuf[i] & 0x0000ffff) + 1))){
	    recvPtr++ ;
		i++ ;
      }
	  else 
      {				  	
        MSG("No.%d QF2cnt  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));  	  
	    Fail();
      }
    }
 
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->SF2cnt = sendBuf[i];
      recvBuf[recvPtr] = EMU->SF2cnt;
      if( (recvBuf[recvPtr] == (sendBuf[i] & 0x0000ffff)) || (recvBuf[recvPtr] == ((sendBuf[i] & 0x0000ffff) + 1))){
	    recvPtr++ ;
		i++ ;
      }
	  else 
      {					   	
        MSG("No.%d SF2cnt  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));  	  
	    Fail();
      }
    }
	
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->PFcnt = sendBuf[i] & 0x0000ffff;
      recvBuf[recvPtr] = EMU->PFcnt;
      if( (recvBuf[recvPtr] == (sendBuf[i] & 0x0000ffff)) || (recvBuf[recvPtr] == ((sendBuf[i] & 0x0000ffff) + 1))){
	    recvPtr++ ;
		i++ ;
      }
	  else 
      {					   	
        MSG("No.%d PFcnt  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	 
	    Fail();
      }
    }    

    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->QFcnt = sendBuf[i];
      recvBuf[recvPtr] = EMU->QFcnt;
      if( (recvBuf[recvPtr] == (sendBuf[i] & 0x0000ffff)) || (recvBuf[recvPtr] == ((sendBuf[i] & 0x0000ffff) + 1))){
	    recvPtr++ ;
		i++ ;
      }
	  else 
      {					   	
        MSG("No.%d QFcnt  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	  
	    Fail();
      }
    }
 
    recvPtr = 0;
    i = 0;    
    while(recvPtr < 4)
    {      
      EMU->SFcnt = sendBuf[i];
      recvBuf[recvPtr] = EMU->SFcnt;
      if( (recvBuf[recvPtr] == (sendBuf[i] & 0x0000ffff)) || (recvBuf[recvPtr] == ((sendBuf[i] & 0x0000ffff) + 1))){
	    recvPtr++ ;
		i++ ;
      }
	  else 
      {					   	
        MSG("No.%d SFcnt  is 0x%02x(should be 0x%02x).\n",i-1,recvBuf[i-1],(sendBuf[i-1] & 0x0000ffff));	
	    Fail();
      }
    }
}
#endif
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
	NVIC_EnableIRQ(WDT_IRQn);
	test_flag_init();
	WDT_OFF();
  SYSCTL->SYS_PS = 0x82 ;		
  SysModeChg(2, 1);				
//	uartInit( 0 ,9600 ,0 ,0 ,0 ) ; 

	SYSCTL->MOD1_EN |= 0xa0;  // EMU and GPIO clk		    
	GPIO->PCB = 0xc3 << 8 ;  // P50/P51/P56/P57 as SF/QF/PF/ZXOUT	
	SYSCTL->ADC_CTRL = 0x0;		//16倍增益						   
	SYSCTL->SYS_PD = 0x0;  	 	
  EMU->SPCMD = 0xe5;	   						  
  EMU->PhsA = 0x100;
  EMU->PhsB = 0x100;
	
  //功率测试,两套功率寄存器						 				  
  EMU->GPQA	= 0x7fff;	
  EMU->GPQB	= 0x7fff;	 
  EMU->IAGAIN = 0x7fff;
  EMU->IBGAIN = 0x7fff;
  EMU->UGAIN = 0x7fff;	 
  EMU->IADCOS = 0x1fffff;	
  EMU->IBDCOS = 0x1fffff;	 
  EMU->UDCOS = 0x1fffff;  	
	uDelay(50000) ;

//寄存器读写	
#ifdef A_Ch
  reg_wr1() ;
#endif
  uDelay(1000) ;
#ifdef B_Ch
  reg_wr2() ;	 	   
#endif
	
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->SYS_PD = 0x3f;  

	Pass();
}

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
