#include "drv.h"

// #define EMU_DBG

/**********************************************************************************
*
*
**********************************************************************************/
uint32_t EMU_CLK_Ini(void)
{
  uint32_t  data;
  
  data = 0x82;
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PS)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }	

  // switch to PLL and PLL div 2
  data = 0xe ;  // switch to RCH 
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_MODE)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC SYS_MODE fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  } 

  if(swd_mem_rd32((unsigned long)(&(SYSCTL->OSC_CTRL1)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("read SOC OSC_CTRL1 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  data &= ~((1<<0)) ;  // PLL enalbe
  data = (data & (~(3<<2))) | (1<<2) ;  // PLL div 2
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->OSC_CTRL1)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC OSC_CTRL1 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  } 
  
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->OSC_CTRL2)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("read SOC OSC_CTRL2 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  data &= (~(1<<4)) ;  // PLL selection for sysclk
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->OSC_CTRL2)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC OSC_CTRL2 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  } 
  
  while(1)
  {
   if(swd_mem_rd32((unsigned long)(&(SYSCTL->OSC_CTRL1)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("read SOC OSC_CTRL1 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
   } 
   if( data & (1<<7) )  // if PLL lock
     break ;   	
  }
  data = 0xd ;  // switch to PLL 
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_MODE)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC SYS_MODE fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  } 
  
  return 0 ; 
}

/**********************************************************************************
*
*
**********************************************************************************/
uint32_t EMU_GPIO_Ini(void)
{   
  uint32_t  data;
  
  data = 0x82;
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PS)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }	
      
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("read SOC MOD1_EN fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }  
  data |= 0xa0 ;  // enable EMU and GPIO
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC MOD1_EN fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  } 
  data = (0x3c<<8) ;  // P50/P51/P56/P57 as SF/PF/QF/ZXOUT
  if(swd_mem_wr32((unsigned long)(&(GPIO->PCB)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write PCB fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }  

  return 0 ;	
}


/**********************************************************************************
*
*
**********************************************************************************/
uint32_t EMU_Fun_Ini(void)
{
  uint32_t  data;
  
  data = 0x82;
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PS)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  } 
  
  // enable EMU and GPIO
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("read SOC MOD1_EN fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }  
  data |= 0xa0 ;  
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC MOD1_EN fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }  	

  // ADC 16x Gaim 
  data = 0x03 ;  
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->ADC_CTRL)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC ADC_CTRL fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  } 
  // chA ,chB ,chC enable
  data = 0x0 ;
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PD)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC SYS_PD fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }  
  
  // EMU register
  data = 0xe5 ;
  if(swd_mem_wr32((unsigned long)(&(EMU->SPCMD)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write EMU SPCMD fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  data = 0x100 ;
  if(swd_mem_wr32((unsigned long)(&(EMU->PhsA)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write EMU PhsA fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  data = 0x100 ;
  if(swd_mem_wr32((unsigned long)(&(EMU->PhsB)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write EMU PhsB fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  } 
//   data = 0x7fff ;//X2
	data = 0x0 ;//X1 @liwb 20171031
  if(swd_mem_wr32((unsigned long)(&(EMU->GPQA)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write EMU GPQA fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
 //   data = 0x7fff ;//X2
	data = 0x0 ;//X1 @liwb 20171031
  if(swd_mem_wr32((unsigned long)(&(EMU->GPQB)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write EMU GPQB fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }  
 //   data = 0x7fff ;//X2
	data = 0x0 ;//X1 @liwb 20171031
  if(swd_mem_wr32((unsigned long)(&(EMU->IAGAIN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write EMU IAGAIN fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  //   data = 0x7fff ;//X2
	data = 0x0 ;//X1 @liwb 20171031
  if(swd_mem_wr32((unsigned long)(&(EMU->IBGAIN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write EMU IBGAIN fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  } 
  //   data = 0x7fff ;//X2
	data = 0x0 ;//X1 @liwb 20171031
  if(swd_mem_wr32((unsigned long)(&(EMU->UGAIN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write EMU UGAIN fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }  
//   data = 0x1fffff ;
		data = 0x0 ;//X1 @liwb 20171031
  if(swd_mem_wr32((unsigned long)(&(EMU->IADCOS)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write EMU IADCOS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }  
//   data = 0x1fffff ;
		data = 0x0 ;//X1 @liwb 20171031
  if(swd_mem_wr32((unsigned long)(&(EMU->IBDCOS)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write EMU IBDCOS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }  
//   data = 0x1fffff ;
		data = 0x0 ;//X1 @liwb 20171031
  if(swd_mem_wr32((unsigned long)(&(EMU->UDCOS)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write EMU UDCOS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }      
  	
  return 0 ;
}


/**********************************************************************************
*
*
**********************************************************************************/
#define Ufmax    9600
#define Ufmin    9216
#define UMAX    85614		//84500
#define UMIN    77460  	//76500
#define daumax  1.777		//17.44//1.09*16
#define daumin  1.608		//15.84//0.99*16
#define dbumax  0.211		//2.10
#define dbumin  0.191		//1.90
// IA value
#define IAMIN   131078//1248000
#define IAMAX   144875//1408000//By liwb PGA=16
#define PAMIN		323542//5920000
#define PAMAX   357599//7200000
#define SAMIN		326166//5920000
#define SAMAX   360499//7200000
#define PA2MIN	322861//5920000
#define PA2MAX  356847//7200000 
 
// IB value
#define IBMIN   15551//152000
#define IBMAX   17188//174000
#define PBMIN	  38591//152000
#define PBMAX   42653//174000
#define SBMIN	  38695//700000
#define SBMAX   42653//870000
#define PB2MIN	38613//152000
#define PB2MAX  42678//174000
uint32_t mt ;
uint32_t EMU_Power_Test2(void)
{
  uint32_t IA ,IB ,U ,Ufeq;
  uint32_t PA ,SA ,PA2 ;
	uint32_t PB ,SB ,PB2 ;
  float dau ,dbu ;

  msDelay(1400) ;//600 fail
  // RMSIA
  if(swd_mem_rd32((unsigned long)(&(EMU->RmsIA)),(unsigned long *)&IA,1) != SWD_RESP_OK){
    DBG_ERR("read EMU RmsIA fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
		DBG_WARNNING("IA= %d \n", IA);
#ifndef EMU_DBG 
  if( (IA<IAMIN)  || (IA>IAMAX) )
  {
     return 1 ;
  }

#endif	
	
  // RMSIB
  if(swd_mem_rd32((unsigned long)(&(EMU->RmsIB)),(unsigned long *)&IB,1) != SWD_RESP_OK){
    DBG_ERR("read EMU RmsIB fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
	DBG_WARNNING("IB= %d \n", IB);
#ifndef EMU_DBG 	
  if( (IB<IBMIN)  || (IB>IBMAX) )
  {
     return 1 ;
  }
	
#endif	
	
  // RMSU
  if(swd_mem_rd32((unsigned long)(&(EMU->RmsU)),(unsigned long *)&U,1) != SWD_RESP_OK){
    DBG_ERR("read EMU RmsU fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
		DBG_WARNNING("U= %d \n", U);
#ifndef EMU_DBG 
  if( (U<UMIN)  || (U>UMAX) )
  {
     return 1 ;
  }
#endif
  dau = (float)((float)IA/(float)U) ; 
	DBG_WARNNING("dau= %f \n", dau);
#ifndef EMU_DBG   
  if( (dau < daumin) || (dau > daumax))
  {	  	
    return 1 ;
  }
#endif
  dbu = (float)((float)IB/(float)U) ; 
	DBG_WARNNING("dbu= %f \n", dbu);
#ifndef EMU_DBG   
  if( (dbu < dbumin) || (dbu > dbumax))
  {	  	
    return 1 ;
  }
#endif
  
  // PowerPA test
  if(swd_mem_rd32((unsigned long)(&(EMU->PowerPA)),(unsigned long *)&PA,1) != SWD_RESP_OK){
    DBG_ERR("read EMU PowerPA fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  } 
	DBG_WARNNING("PA= %d \n", PA);
#ifndef EMU_DBG  
  if( (PA < PAMIN) || (PA > PAMAX))
  {	  	
    return 1 ;
  }
#endif
  
 // PowerPB test
 if(swd_mem_rd32((unsigned long)(&(EMU->PowerPB)),(unsigned long *)&PB,1) != SWD_RESP_OK){
   DBG_ERR("read EMU PowerPB fail\n");
   return SWD_RESP_PROTOCAL_ERROR;
 }
 	DBG_WARNNING("PB= %d \n", PB);
#ifndef EMU_DBG
 if( (PB < PBMIN) || (PB > PBMAX))
 {	  	
   return 1 ;
 }
#endif
  
  // PowerSA test
 if(swd_mem_rd32((unsigned long)(&(EMU->PowerSA)),(unsigned long *)&SA,1) != SWD_RESP_OK){
   DBG_ERR("read EMU PowerSA fail\n");
   return SWD_RESP_PROTOCAL_ERROR;
 } 
		DBG_WARNNING("SA= %d \n", SA);
#ifndef EMU_DBG  
  if( (SA < SAMIN) || (SA > SAMAX))
  {	  	
    return 1 ;
  }
#endif
  
 // PowerSB test
 if(swd_mem_rd32((unsigned long)(&(EMU->PowerSB)),(unsigned long *)&SB,1) != SWD_RESP_OK){
   DBG_ERR("read EMU PowerSB fail\n");
   return SWD_RESP_PROTOCAL_ERROR;
 } 
 		DBG_WARNNING("SB= %d \n", SB);
#ifndef EMU_DBG
 if( (SB < SBMIN) || (SB > SBMAX))
 {	  	
   return 1 ;
 }
#endif
  
  // PowerPA2 test
  if(swd_mem_rd32((unsigned long)(&(EMU->PowerPA2)),(unsigned long *)&PA2,1) != SWD_RESP_OK){
    DBG_ERR("read EMU PowerPA2 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
	 	DBG_WARNNING("PA2= %d \n", PA2);
#ifndef EMU_DBG  
  if( (PA2 < PA2MIN) || (PA2 > PA2MAX))
  {	  	
    return 1 ;
  }
#endif
  
 // PowerPB2 test
 if(swd_mem_rd32((unsigned long)(&(EMU->PowerPB2)),(unsigned long *)&PB2,1) != SWD_RESP_OK){
   DBG_ERR("read EMU PowerPB2 fail\n");
   return SWD_RESP_PROTOCAL_ERROR;
 }
  DBG_WARNNING("PB2= %d \n", PB2);
#ifndef EMU_DBG 
 if( (PB2 < PB2MIN) || (PB2 > PB2MAX))
 {	  	
   return 1 ;
 }
#endif
  
   // Ufeq test
  if(swd_mem_rd32((unsigned long)(&(EMU->FreqU)),(unsigned long *)&Ufeq,1) != SWD_RESP_OK){
    DBG_ERR("read EMU PowerPA fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  } 
#ifndef EMU_DBG  
	DBG_WARNNING("Ufeq= %d \n", Ufeq);
  if( (Ufeq < Ufmin) || (Ufeq > Ufmax))
  {	  	
    return 1 ;
  }

#endif
 
  return 0 ;	
}

uint32_t EMU_FT_Ana(void)
{
  uint32_t result ;

  EMU_CLK_Ini()	;	
  EMU_GPIO_Ini() ; 
  EMU_Fun_Ini()  ;
  result = EMU_Power_Test2() ;
  if( result != 0 )
    return 1 ;

  return 0 ;

}


