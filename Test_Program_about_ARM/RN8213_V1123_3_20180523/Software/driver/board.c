#include "drv.h"
#define  KEY_FILTE_CNT  200
// P1.23 = 0 ,P1.24 = 1
/*--------------------------*/
/*       FLASH²é¿Õ          */
/*--------------------------*/
uint32_t empty_check_fl(void)
{
	uint32_t i,data;
	uint32_t em_flag=0;
	unsigned long td, fl_addr;
	unsigned long remap;
	
	remap = swd_mem_rd32(0x40034018, &td, 1);  //remap
  td = td & 0x7;
  fl_addr = (td == 0) ? 0x0 : (td == 1) ? 0x08000000 : (td == 2) ? 0x10000000 : (td == 3) ? 0x18000000 : 0x0;
	
	for(i = 0; i < 256; i++)
	{
		data = 1;
		if(swd_mem_rd32(fl_addr + 4*i,(unsigned long *)&data,1) != SWD_RESP_OK){
			DBG_ERR("FLASH Read fail\n");
		}
		
		if(data != 0x0)
		{
			DBG_ERR("FLASH ADDR£º%x, !=  %x!   \n",(fl_addr + 4*i),data);
			exitFail(1) ;
		}
	}
  return em_flag;	
}
/*--------------------------*/
/*       EEPROM²é¿Õ          */
/*--------------------------*/
uint32_t empty_check_ee(void)
{
	uint32_t i,data;
	uint32_t em_flag=0;	
	unsigned long td, ee_addr;
	unsigned long remap;
	
	remap = swd_mem_rd32(0x40034018, &td, 1);  //remap
  td = td & 0x7;
  ee_addr = (td == 1) ? 0x0 : 0x08000000;
	
	for(i = 0; i < 256; i++)
	{
		data = 1;
		if(swd_mem_rd32(ee_addr + 4*i,(unsigned long *)&data,1) != SWD_RESP_OK){
			DBG_ERR("EEPROM Read fail\n");
		}
		
		if(data != 0x0)
		{
			DBG_ERR("EEPROM ADDR£º%x, !=  %x!   \n",(ee_addr + 4*i),data);
			exitFail(1) ;
		}
	}
  return em_flag;	
}

void exitFail(uint32_t err_num)
{
	BUS_ON ;
	DIR1_SET1 ;
	LPC_PINCON->PINSEL3 &= (0x3<<14) ;  // P1.23 ,P1.24 as GPIO
	LPC_GPIO1->FIOMASK  &= (0x3<<23) ;  // P1.23 ,P1.24 can be read and written
	LPC_GPIO1->FIODIR   |= (0x3<<23) ;  // P1.23 ,P1.24 as output
	if( err_num == TEMPER_ERR )	
	{
	  LPC_GPIO1->FIOCLR   |= (1<<23) ;  // P1.23 = 0 
	  LPC_GPIO1->FIOCLR   |= (1<<24) ;  // P1.24 = 0
	}
	else
	{
	  LPC_GPIO1->FIOCLR   |= (1<<23) ;  // P1.23 = 0 
	  LPC_GPIO1->FIOCLR   |= (1<<24) ;  // P1.24 = 0
	}
	__set_swd_sio_as_in() ;
	
	BUS_OFF;
	while(1) ;
}


// P1.23 = 1 ,P1.24 = 1
void exitPass(void)
{
//	SocLowPwr_FT() ;
	__set_swd_sio_as_in() ;
	BUS_ON ;  // 
	DIR1_SET1 ;  // LPC1759 PIN as output
	LPC_PINCON->PINSEL3 &= (0x3<<14) ;  // P1.23 ,P1.24 as GPIO
	LPC_GPIO1->FIOMASK  &= (0x3<<23) ;  // P1.23 ,P1.24 can be read and written
	LPC_GPIO1->FIODIR   |= (0x3<<23) ;  // P1.23 ,P1.24 as output	
	LPC_GPIO1->FIOSET   |= (1<<23) ;  // P1.23 = 1 
	LPC_GPIO1->FIOSET   |= (1<<24) ;  // P1.24 = 1 
//	__set_swd_sio_as_in() ;
//	BUS_OFF ;
	//while(1) ;
}
//==========exit_UID_check_PASS==============
void exit_UID_check_PASS(void)
{
	BUS_ON ;  // 
	DIR1_SET1 ;  // LPC1759 PIN as output
	LPC_PINCON->PINSEL3 &= (0x3<<14) ;  // P1.23 ,P1.24 as GPIO
	LPC_GPIO1->FIOMASK  &= (0x3<<23) ;  // P1.23 ,P1.24 can be read and written
	LPC_GPIO1->FIODIR   |= (0x3<<23) ;  // P1.23 ,P1.24 as output	
	LPC_GPIO1->FIOSET   |= (1<<23) ;  // P1.23 = 1 
	LPC_GPIO1->FIOCLR   |= (1<<24) ;  // P1.24 = 0   
	//__set_swd_sio_as_in() ;
	//BUS_OFF ;
	//while(1) ;
}

//===========================SP0 PASS  ========================
// P1.24 = 1,P1.23 = 0,
void exit_SP0_Pass(void)
{
	BUS_ON ;  // 
	DIR1_SET1 ;  // LPC1759 PIN as output
	LPC_PINCON->PINSEL3 &= (0x3<<14) ;  // P1.23 ,P1.24 as GPIO
	LPC_GPIO1->FIOMASK  &= (0x3<<23) ;  // P1.23 ,P1.24 can be read and written
	LPC_GPIO1->FIODIR   |= (0x3<<23) ;  // P1.23 ,P1.24 as output	
	LPC_GPIO1->FIOCLR   |= (1<<23) ;  // P1.23 = 0 
	LPC_GPIO1->FIOSET   |= (1<<24) ;  // P1.24 = 1 
	//__set_swd_sio_as_in() ;
	//BUS_OFF ;
	//while(1) ;
}

//===========================LCD3V3 PASS  ========================
// P1.24 = 1,P1.23 = 1,
void exit_LCD3V3_Pass(void)
{
	BUS_ON ;  // 
	DIR1_SET1 ;  // LPC1759 PIN as output
	LPC_PINCON->PINSEL3 &= (0x3<<14) ;  // P1.23 ,P1.24 as GPIO
	LPC_GPIO1->FIOMASK  &= (0x3<<23) ;  // P1.23 ,P1.24 can be read and written
	LPC_GPIO1->FIODIR   |= (0x3<<23) ;  // P1.23 ,P1.24 as output	
	LPC_GPIO1->FIOSET   |= (1<<23) ;  // P1.23 = 1 
	LPC_GPIO1->FIOSET   |= (1<<24) ;  // P1.24 = 1 
	//__set_swd_sio_as_in() ;
	//BUS_OFF ;
	//while(1) ;
}

//===========================FUNTION PASS  ========================
// P1.24 = 0,P1.23 = 1,
void exit_FUNTION_Pass(void)
{
	BUS_ON ;  // 
	DIR1_SET1 ;  // LPC1759 PIN as output
	LPC_PINCON->PINSEL3 &= (0x3<<14) ;  // P1.23 ,P1.24 as GPIO
	LPC_GPIO1->FIOMASK  &= (0x3<<23) ;  // P1.23 ,P1.24 can be read and written
	LPC_GPIO1->FIODIR   |= (0x3<<23) ;  // P1.23 ,P1.24 as output	
	LPC_GPIO1->FIOSET   |= (1<<23) ;  // P1.23 = 1 
	LPC_GPIO1->FIOCLR   |= (1<<24) ;  // P1.24 = 0
	//__set_swd_sio_as_in() ;
	//BUS_OFF ;
	//while(1) ;
}

//===========================SAR PASS  ========================
// P1.24 = 1,P1.23 = 0,
void exit_SAR_Pass(void)
{
	BUS_ON ;  // 
	DIR1_SET1 ;  // LPC1759 PIN as output
	LPC_PINCON->PINSEL3 &= (0x3<<14) ;  // P1.23 ,P1.24 as GPIO
	LPC_GPIO1->FIOMASK  &= (0x3<<23) ;  // P1.23 ,P1.24 can be read and written
	LPC_GPIO1->FIODIR   |= (0x3<<23) ;  // P1.23 ,P1.24 as output	
	LPC_GPIO1->FIOSET   |= (1<<23) ;  // P1.23 = 1 
	LPC_GPIO1->FIOCLR   |= (1<<24) ;  // P1.24 = 0  
	//__set_swd_sio_as_in() ;
	//BUS_OFF ;
	//while(1) ;
}

// P1.23 = 0 ,P1.24 = 0
void Ini_Test(void)
{
	BUS_ON ;  // 
	DIR1_SET1 ;  // LPC1759 PIN as output
	LPC_PINCON->PINSEL3 &= (0x3<<14) ;  // P1.23 ,P1.24 as GPIO
	LPC_GPIO1->FIOMASK  &= (0x3<<23) ;  // P1.23 ,P1.24 can be read and written
	LPC_GPIO1->FIODIR   |= (0x3<<23) ;  // P1.23 ,P1.24 as output	
	LPC_GPIO1->FIOCLR   |= (1<<23) ;  // P1.23 = 0 
	LPC_GPIO1->FIOCLR   |= (1<<24) ;  // P1.24 = 0
}

//00 : start
//11 : function pass
//10 : test finish
void finishTest(void)
{
    __set_swd_sio_as_in() ;
    BUS_ON ;  // 
	DIR1_SET1 ;  // LPC1759 PIN as output
	LPC_PINCON->PINSEL3 &= (0x3<<14) ;  // P1.23 ,P1.24 as GPIO
	LPC_GPIO1->FIOMASK  &= (0x3<<23) ;  // P1.23 ,P1.24 can be read and written
	LPC_GPIO1->FIODIR   |= (0x3<<23) ;  // P1.23 ,P1.24 as output	
	LPC_GPIO1->FIOSET   |= (1<<23) ;  // P1.23 = 1 
	LPC_GPIO1->FIOSET   |= (1<<24) ;  // P1.24 = 1 
}

void debugTest(void)
{
    __set_swd_sio_as_in() ;
    BUS_ON ;  // 
	DIR1_SET1 ;  // LPC1759 PIN as output
	LPC_PINCON->PINSEL3 &= (0x3<<14) ;  // P1.23 ,P1.24 as GPIO
	LPC_GPIO1->FIOMASK  &= (0x3<<23) ;  // P1.23 ,P1.24 can be read and written
	LPC_GPIO1->FIODIR   |= (0x3<<23) ;  // P1.23 ,P1.24 as output	
	LPC_GPIO1->FIOSET   |= (1<<23) ;  // P1.23 = 0 
	LPC_GPIO1->FIOCLR   |= (1<<24) ;  // P1.24 = 1
}


/********************************************

Start_Test:when LPC1759 receive P0.10 = 0 , P0.11 = 1 ,start FT

*********************************************/
uint8_t wait_clrmip(void)
{
	uint32_t j;
	uint32_t data ;
	
	LPC_PINCON->PINSEL0 &= (0xf<<20) ;  // P0.10 ,P0.11 as GPIO
	LPC_GPIO0->FIOMASK  &= (0x3<<10) ;  // P0.10 ,P0.11 can be read and written
	LPC_GPIO0->FIODIR   &= (0x3<<10) ;  // P0.10 ,P0.11 as input
	LPC_PINCON->PINMODE0 = 0x00300000;   //P0.10 PULL DOWN, P0.11 PULL UP.

// 	DIR2_SET1 ;
	DIR2_CLR0 ;  // LPC1759 receive test start signal
	BUS_ON ;  // 164245 enable
	
	j = 0 ;
	while(1)
	{
		data = LPC_GPIO0->FIOPIN >> 10 ;
		if( (data & 0x3) == START_CLRNVM )  // check 10 times
		{
			j++ ;
			if( j>KEY_FILTE_CNT )
			  break ;
		}
		else
			j = 0 ;
	}
//  debugTest();
	swd_drv_init() ;
	while(swd_connect(1) != SWD_RESP_OK)
    {
      ;
    }
    clearMip();
	
	if(empty_check_fl())
		{
			DBG_ERR("Flash Is Not Empty!\n");
			exitFail(1);
		}
		DBG_MSG("Flash check success£¡\n");
		if(empty_check_ee())
	  { 
			DBG_ERR("EEPROM Is Not Empty!\n");
			exitFail(1);
	  }
		DBG_MSG("EEPR check success£¡\n");
		
	return PASS ;

}
//================wait test start =================
uint8_t wait_start(void)
{
	uint32_t cnt1, cnt2, cnt3;
	uint32_t data ;
	
	LPC_PINCON->PINSEL0 &= (0xf<<20) ;  // P0.10 ,P0.11 as GPIO
	LPC_GPIO0->FIOMASK  &= (0x3<<10) ;  // P0.10 ,P0.11 can be read and written
	LPC_GPIO0->FIODIR   &= (0x3<<10) ;  // P0.10 ,P0.11 as input
	LPC_PINCON->PINMODE0 = 0x00300000;   //P0.10 PULL DOWN, P0.11 PULL UP.

// 	DIR2_SET1 ;
	DIR2_CLR0 ;  // LPC1759 receive test start signal
	BUS_ON ;  // 164245 enable
	
	cnt1 = 0 ;
  cnt2 = 0;
	cnt3 = 0;
	while(1)
	{
		data = LPC_GPIO0->FIOPIN >> 10 ;
		if((data & 0x3) == START_TEST)  // check 200 times
		{
			cnt1++ ;
			if(cnt1 > KEY_FILTE_CNT)
			  break ;
		}
		else
		{
			cnt1 = 0 ;
		}
		
		if((data & 0x3) == START_LP)  // check 200 times
		{
			cnt2++ ;
			if(cnt2 > KEY_FILTE_CNT)
			  break ;
		}
		else
		{
			cnt2 = 0 ;
		}
		
		if((data & 0x3) == START_DISLP)  // check 200 times
		{
			cnt3++ ;
			if(cnt3 > KEY_FILTE_CNT)
			  break ;
		}
		else
		{
			cnt3 = 0 ;
		}
		
	}
	
	if(cnt1 > KEY_FILTE_CNT)
	{
	  return  START_TEST;
	}
		
	if(cnt2 > KEY_FILTE_CNT)
	{
	  return  START_LP;
	}
		
	if(cnt3 > KEY_FILTE_CNT)
	{
	  return  START_DISLP;
	}
		return START_TEST;
}

/********************************************

Start_Test:when LPC1759 receive P0.10 = 0 , P0.11 = 0 ,start DISLP

*********************************************/
uint8_t wait_DISLowpwr(void)
{
	uint32_t j;
	uint32_t data ;
	
  __set_swd_sclk_as_out() ;
  __set_swd_sclk_to_low() ;
  __set_swd_sio_as_out() ;
  __set_swd_sio_to_low() ;	
	

	DIR2_CLR0 ;  // LPC1759 receive test start signal
//	BUS_ON ;  // 164245 enable
	
	
	j = 0 ;
	while(1)
	{
		data = LPC_GPIO0->FIOPIN >> 10 ;
		if( (data & 0x3) == START_DISLP )  // check 10 times
		{
			j++ ;
			if( j>KEY_FILTE_CNT )
			  break ;
		}
		else
			j = 0 ;
	}
	
 	msDelay(50);
	swd_drv_init() ;

	while(swd_connect(1) != SWD_RESP_OK)
    {
      ;
    }
	Soc_DISLowPwr_FT() ;
	return PASS ;

}
/********************************************

Start_Test:when LPC1759 receive P0.10 = 0 , P0.11 = 1 ,start SP0

*********************************************/
uint32_t  powswh_bin = 2;//0
uint8_t wait_SP0(void)
{
	uint32_t cnt1, cnt2;
	uint32_t data ;
	
  __set_swd_sclk_as_out() ;
  __set_swd_sclk_to_low() ;
  __set_swd_sio_as_out() ;
  __set_swd_sio_to_low() ;	
	

	DIR2_CLR0 ;  // LPC1759 receive test start signal
//	BUS_ON ;  // 164245 enable
	cnt1 = 0 ;
  cnt2 = 0;	
	while(1)
	{
		data = LPC_GPIO0->FIOPIN >> 10 ;
		if((data & 0x3) == PORSW_BIN1)  // check 200 times
		{
			cnt1++ ;
			if(cnt1 > KEY_FILTE_CNT)
			  break ;
		}
		else
		{
			cnt1 = 0 ;
		}
		
		if((data & 0x3) == PORSW_BIN2)  // check 200 times
		{
			cnt2++ ;
			if(cnt2 > KEY_FILTE_CNT)
			  break ;
		}
		else
		{
			cnt2 = 0 ;
		}
	}
	
if(cnt1 > KEY_FILTE_CNT)
	{
	  powswh_bin = 1 ;
	}
		
	if(cnt2 > KEY_FILTE_CNT)
	{
	  powswh_bin = 2 ;
	}

	return PASS ;

}

/********************************************

Start_Test:when LPC1759 receive P0.10 = 1 , P0.11 = 1 ,start LCD3V3

*********************************************/
uint8_t wait_LCD3V3(void)
{
	uint32_t j;
	uint32_t data ;
	
  __set_swd_sclk_as_out() ;
  __set_swd_sclk_to_low() ;
  __set_swd_sio_as_out() ;
  __set_swd_sio_to_low() ;	
	

	DIR2_CLR0 ;  // LPC1759 receive test start signal
//	BUS_ON ;  // 164245 enable
	
	
	j = 0 ;
	while(1)
	{
		data = LPC_GPIO0->FIOPIN >> 10 ;
		if( (data & 0x3) == START_LCD3V3 )  // check 10 times
		{
			j++ ;
			if( j>KEY_FILTE_CNT )
			  break ;
		}
		else
			j = 0 ;
	}
	
// 	swd_drv_init() ;

	return PASS ;

}

/********************************************

wait_FUNTION:when LPC1759 receive P0.10 = 1 , P0.11 = 1 ,start FUNTION

*********************************************/
uint8_t wait_FUNTION(void)
{
	uint32_t j;
	uint32_t data ;
	
  __set_swd_sclk_as_out() ;
  __set_swd_sclk_to_low() ;
  __set_swd_sio_as_out() ;
  __set_swd_sio_to_low() ;	
	

	DIR2_CLR0 ;  // LPC1759 receive test start signal
//	BUS_ON ;  // 164245 enable
	
	
	j = 0 ;
	while(1)
	{
		data = LPC_GPIO0->FIOPIN >> 10 ;
		if( (data & 0x3) == START_FUNTION )  // check 10 times
		{
			j++ ;
			if( j>KEY_FILTE_CNT )
			  break ;
		}
		else
			j = 0 ;
	}
	
	swd_drv_init() ;

	return PASS ;
}

/********************************************

wait_SAR:when LPC1759 receive P0.10 = 1 , P0.11 = 0 ,start SAR

*********************************************/
uint8_t wait_SAR(void)
{
	uint32_t j;
	uint32_t data ;
	
  __set_swd_sclk_as_out() ;
  __set_swd_sclk_to_low() ;
  __set_swd_sio_as_out() ;
  __set_swd_sio_to_low() ;	
	

	DIR2_CLR0 ;  // LPC1759 receive test start signal
//	BUS_ON ;  // 164245 enable
	
	
	j = 0 ;
	while(1)
	{
		data = LPC_GPIO0->FIOPIN >> 10 ;
		if( (data & 0x3) == START_SAR )  // check 10 times
		{
			j++ ;
			if( j>KEY_FILTE_CNT )
			  break ;
		}
		else
			j = 0 ;
	}
	
	swd_drv_init() ;

	return PASS ;
}


/********************************************

Time delay

*********************************************/
#if 1
void usDelay(uint32_t us)
{
  uint32_t    i, loops;
  
	if(us == 0)
		return;
	
  SystemCoreClockUpdate();
    
  //Max freq:100Mhz(10ns), max systick length:167ms
  loops = (us / 167000);
  for(i = 0; i < loops; i++)
  {
    SysTick->LOAD = 0xffffff;   
    SysTick->CTRL = 0x4;
    SysTick->VAL  = 0x0;
	  SysTick->LOAD = 0xffffff;//SystemCoreClock / 1000000 * us;
    
    SysTick->CTRL = 0x5;
    while(!(SysTick->CTRL & 0x10000))
    {
      ;
    }
    SysTick->CTRL = 4;
  }
  
  loops = us - (loops * 167000);
  if(loops != 0)
  {
    SysTick->LOAD = 0xffffff;   
    SysTick->CTRL = 0x4;
    SysTick->VAL  = 0x0;
	  SysTick->LOAD = SystemCoreClock / 1000000 * loops;
    
    SysTick->CTRL = 0x5;
    while(!(SysTick->CTRL & 0x10000))
    {
      ;
    }
    SysTick->CTRL = 4;
  }
}

void msDelay(uint32_t ms)
{
  uint32_t  i, loops;
  
  loops = (ms *10);
  for(i = 0; i < loops; i++)
  {
    usDelay(100);
  }
//   usDelay(1);
}

// void swDelay(uint32_t i)
// {
//   while(i--)
//   {
//     ;
//   }
// }
#endif

