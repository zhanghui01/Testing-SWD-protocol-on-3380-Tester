#include "rn821x.h"

#include <sysctrl.h>
#include "common.h"
#include "stdio.h"

#define  RES_OFFSET     0x37f8
#define  CHIPID_OFFSET  0x37fc
#define  TEMP_OFFSET    0x37f4

uint32_t i = 0;
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
    *test_flag = 0x12345a11 ;
	while(1)
	{
	  WDT->EN = 0xbb;	
	}
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

	while(1)
	{
	  WDT->EN = 0xbb;	
	}
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
	
	test_flag  = (uint32_t*)(SRAM_base + RES_OFFSET) ; 
	*test_flag = 0xa5a5a5a5 ;
}

void SYSTC_reg_wr(void);
void RTC_reg_wr(void);
void SPI_reg_wr(void);
void UART_reg_wr(void);
void WDT_reg_wr (void);
void ISO7816_reg_wr(void);
void TC_reg_wr (void);
void NVM_reg(void);
void KBI_reg_wr(void);
void INTC_reg_wr(void);
void I2C_reg_rw (void);
void EMU_reg_wr(void);
void RTC_calendar(void);
// void  RTC_HANDLER(void);
uint32_t hex2bcd (uint32_t data);
void LCD_reg_rw (void);

uint32_t chip_id;
int32_t main(void)
{    
  test_flag_init();	
  WdtInit();
	
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->OSC_CTRL2 = 0x13;
  SysModeChg(3, 0);   //PLL,29.4912M	
	
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->MOD0_EN = 0xffff;
	SYSCTL->MOD1_EN = 0xffff;
	SYSCTL->INTC_EN = 0x1ff;
	SYSCTL->KBI_EN = 0x1ff;
	
	SYSCTL->SYS_PS = 0x75;
	if((SYSCTL->DIE_VER & 0xff) == 0)
	{
		if(ISO7816->CTRL1 == 0x20ea00)
		{
		  chip_id = 0xa0;
		}
		else
		{ 
			chip_id = 0xb0;
    }
	}
	else
	{
		chip_id = SYSCTL->DIE_VER & 0xff;
	}
	SYSCTL->SYS_PS = 0x82;	
	
	RTC_calendar();
	WDT_reg_wr();
	UART_reg_wr();
	SPI_reg_wr();
	RTC_reg_wr();
	ISO7816_reg_wr();
  TC_reg_wr ();
  KBI_reg_wr();
  INTC_reg_wr();
  I2C_reg_rw ();
	if((chip_id != 0xf0) && (chip_id != 0xe0))
	{
		EMU_reg_wr();
		NVM_reg();
	}
	LCD_reg_rw();
	
	if(chip_id != 0xa0 )
	{
	  SYSTC_reg_wr();  //A
	}
	
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->MOD0_EN = 0;
	SYSCTL->MOD1_EN = 0;
	SYSCTL->INTC_EN = 0;
	SYSCTL->KBI_EN = 0;
  Pass();
}


/****************************************************************************** 
 ** @brief  test I2C registers
 ** @param  none.
 ** @return none.
 ** @
 ******************************************************************************/
volatile uint32_t lcd_defaut[] = {0x0,0x0,0x0,0x0,0xccc,0xb81,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
volatile uint32_t lcd_mask[] = {0x1dbf,0x0,0xff,0x3ffffff,0x1fff,0x1fff,0x0,0x0,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
                                0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffff};
void LCD_reg_rw (void) 
{   
  volatile uint32_t *p_addr = (uint32_t *)(LCD);
	uint32_t  i;
	uint32_t  data;
	
	if(chip_id == 0xe0) 
	{
		lcd_defaut[5]=0xb34;
		lcd_mask[0]=0x1fff;
	}
	if((chip_id == 0xa0))
	{
		lcd_mask[0]=0x3dbf;
	}
	if((chip_id == 0xf0))
	{
		lcd_mask[5]=0xffff;
	}
  /* Register reset value */	
	for(i=0;i<sizeof(LCD_TypeDef) / sizeof(uint32_t);i++)
	{
		if(((chip_id == 0xa0) || (chip_id == 0xb0)) && ((i==5) || (i==6)))
		{
      continue;
    }
		if(*(uint32_t *)((uint32_t)p_addr + 4*i) != lcd_defaut[i]) 
		{
			Fail();
		}
	}
	
	/* Register WR 0xffffffff value */
	data= 0xffffffff;
  for(i = 0; i < sizeof(LCD_TypeDef) / sizeof(uint32_t); i++)
  {  
		if(((chip_id == 0xa0) || (chip_id == 0xb0)) && ((i==5)))
		{
      continue;
    }
		if((i==1) || (i==6) || (i==7))
		{
      continue;
    }
		*(uint32_t *)((uint32_t)p_addr + 4*i) = data;  
		if((*(uint32_t *)((uint32_t)p_addr + 4*i)) != (data & lcd_mask[i]))
		{					
			Fail();
		}
		if(i==0)
		{
			LCD->CTRL = 0x0;	
		}
  }

}

/****************************************************************************** 
 ** @brief  test EMU registers
 ** @param  none.
 ** @return none.
 ** @
 ******************************************************************************/
volatile uint32_t emu_default[] = {0x1c0007,0x0,0x1000,0x60,0x120,0x0,0x0,0x0,0x0,
                                   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                                   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};       
volatile uint32_t emu_mask[] = {0xffffff,0xffffff,0xffff,0xffff,0xffff,0xffff,
	                              0xffff,0x1ff,0x1ff,0xffff,0xffff,0xffff,
	                              0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	                              0xffff,0xffff,0xffffff,0xffffff,0xffffff,
	                              0xffffff,0xffff,0xffff,0xffff,0xffff,0xffffffff};
void EMU_reg_wr(void){	 
  volatile uint32_t i,data;
  volatile uint32_t *p_addr = (uint32_t *)(EMU);
 
  /* Register reset value */	
	for(i=0;i<sizeof(EMU_TypeDef) / sizeof(uint32_t);i++)
	{
		if(*(uint32_t *)((uint32_t)p_addr + 4*i) != emu_default[i]) 
		{
			Fail();
		}	
		if(i==0x1c)  //0x70H,d2fp
		{
		  break;	
		}	
	}	
	
	EMU->SPCMD = 0xe5; 	
	if((chip_id == 0xa0) || (chip_id == 0xb0))
  {
	  emu_mask[0] = 0x1fffff; 	
	}	
	
  for(i = 0; i < sizeof(EMU_TypeDef) / sizeof(uint32_t); i++)
  { 
		if(i==1)
			data = 0xfffffff8;
		else
	    data = 0xffffffff;
		
		*(uint32_t *)((uint32_t)p_addr + 4*i) = data;  
		if((*(uint32_t *)((uint32_t)p_addr + 4*i)) != (data & emu_mask[i]))
		{					
			Fail();
		}
		if(i==0x1c)  //0x70H,d2fp
		{
		  break;	
		}	
  }
	
}

/****************************************************************************** 
 ** @brief  test I2C registers
 ** @param  none.
 ** @return none.
 ** @
 ******************************************************************************/
volatile uint32_t i2c_defaut[] = {0x0,0x0,0x0,0x0,0x0};
volatile uint32_t i2c_mask[] = {0x3f,0x7,0x0,0xff,0xff};
void I2C_reg_rw (void) 
{   
  volatile uint32_t *p_addr = (uint32_t *)(I2C);
	uint32_t  i;
	uint32_t  data;
	
  /* Register reset value */	
	for(i=0;i<sizeof(I2C_TypeDef) / sizeof(uint32_t);i++)
	{
		if(*(uint32_t *)((uint32_t)p_addr + 4*i) != i2c_defaut[i]) 
		{
			Fail();
		}
	}
	
	/* Register WR 0xffffffff value */
	data= 0xffffffff;
  for(i = 0; i < sizeof(I2C_TypeDef) / sizeof(uint32_t); i++)
  {  
		*(uint32_t *)((uint32_t)p_addr + 4*i) = data;  
		if((*(uint32_t *)((uint32_t)p_addr + 4*i)) != (data & i2c_mask[i]))
		{					
			Fail();
		}
  }

}

/****************************************************************************** 
 ** @brief  test ISO7816 registers
 ** @param  none.
 ** @return none.
 ** @
 ******************************************************************************/ 
volatile uint32_t iso7816_defaut[] = {0xea00,0x200ea00,0x0,0x17401,0x17401,0x600,0x600,0x0,0x0};
volatile uint32_t iso7816_mask[] = {0xfffffff,0xffffffff,0xf,0x3fffff,0x3fffff,0x0,0x0,0x0,0x0};
void ISO7816_reg_wr(void)
{	
	uint32_t i ,data;
	uint32_t *p_addr = (uint32_t *)(ISO7816);	  
	
  /* Register reset value */	
	if(chip_id == 0xa0) 
	{
    iso7816_defaut[1] = 0x20ea00 ;
	}	
	for(i=0;i<9;i++)
	{
		if(*(uint32_t *)((uint32_t)p_addr + 4*i) != iso7816_defaut[i]) 
		{
			Fail();
		}
	}

	/* Register WR 0xffffffff value */
	data = 0xffffffff;
	for(i=0;i<5;i++)
	{
	  *(uint32_t *)((uint32_t)p_addr + 4*i) = data;	
		if(*(uint32_t *)((uint32_t)p_addr + 4*i) != (iso7816_mask[i] & data)) 
		{
			Fail();
		}
	}
			                                           	
}

/****************************************************************************** 
 ** @brief  test INTC registers
 ** @param  none.
 ** @return none.
 ** @
 ******************************************************************************/
volatile uint32_t intc_defaut[] = {0x0,0x0,0x0,0x0};
volatile uint32_t intc_mask[] = {0xff,0xffff,0xff,0x00};
void INTC_reg_wr(void)
{
  uint32_t data,j;
  uint32_t *p_addr = (uint32_t *)(INTC);	

	/* Register reset value */
	for(j = 0; j < sizeof(INTC_TypeDef) / sizeof(uint32_t); j++)
	{			
		if(*(uint32_t *)((uint32_t)p_addr + 4*i) != intc_defaut[i]) 
		{
			Fail();
		}
	}
	
	data = 0xffffffff;
  for(j = 0; j < sizeof(INTC_TypeDef) / sizeof(uint32_t); j++)
  {   	
		*(uint32_t *)((uint32_t)p_addr + 4*j) = data;		
		if(*(uint32_t *)((uint32_t)p_addr + 4*j) != (data & intc_mask[j]))
		{								  
			Fail() ;
		}
  }	

}

/****************************************************************************** 
 ** @brief  test KBI registers
 ** @param  none.
 ** @return none.
 ** @
 ******************************************************************************/
volatile uint32_t kbi_defaut[] = {0x0,0x0,0x0,0x0};
volatile uint32_t kbi_mask[] = {0xff,0xff,0x00,0xff};
void KBI_reg_wr(void)
{
  uint32_t data,j;
  uint32_t *p_addr = (uint32_t *)(KBI);	

	/* Register reset value */
	for(j = 0; j < sizeof(KBI_TypeDef) / sizeof(uint32_t); j++)
	{			
		if(*(uint32_t *)((uint32_t)p_addr + 4*i) != kbi_defaut[i]) 
		{
			Fail();
		}
	}
	
	data = 0xffffffff;
  for(j = 0; j < sizeof(KBI_TypeDef) / sizeof(uint32_t); j++)
  {   	
		*(uint32_t *)((uint32_t)p_addr + 4*j) = data;		
		if(*(uint32_t *)((uint32_t)p_addr + 4*j) != (data & kbi_mask[j]))
		{								  
			Fail() ;
		}
  }	
  
}

/****************************************************************************** 
 ** @brief  test NVM registers
 ** @param  none.
 ** @return none.
 ** @
 ******************************************************************************/
volatile uint32_t nvm_defaut[] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
volatile uint32_t nvm_mask[] = {0x3f,0x0,0x3f,0xffffff,0xffffff,0x0,0x0,0xf,0xffff,0xffffff,0xffffff};
void NVM_reg(void)
{ 
  uint32_t data,j;
  uint32_t *p_addr = (uint32_t *)(NVM);	
  
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->MOD1_EN &= (~(0x1<<12));
	SYSCTL->OSC_CTRL1 &= (~(0x1<<1));

	/* Register reset value */
	for(j = 0; j < sizeof(NVM_TypeDef) / sizeof(uint32_t); j++)
	{			
		if(*(uint32_t *)((uint32_t)p_addr + 4*i) != nvm_defaut[i]) 
		{
			Fail();
		}
	}
	
  for(j = 0; j < sizeof(NVM_TypeDef) / sizeof(uint32_t); j++)
  {   
		if(chip_id == 0xa0) 
		{	  
      nvm_mask[0] = 0xf;
			nvm_mask[2] = 0xf;
      if((j==8) || (j==9) || (j==10))	
      continue;				
		}
		if(j==2)
			data = 0xfffffffe;
		else
			data = 0xffffffff;
		
		*(uint32_t *)((uint32_t)p_addr + 4*j) = data;		
		if(*(uint32_t *)((uint32_t)p_addr + 4*j) != (data & nvm_mask[j]))
		{								  
			Fail() ;
		}
  }

}

/****************************************************************************** 
 ** @brief  test TC registers
 ** @param  none.
 ** @return none.
 ** @
 ******************************************************************************/
volatile uint32_t tc_defaut[] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
volatile uint32_t tc_mask[] = {0,0xffff,0x0,0xffffffff,0x0,0xffffffff,0xffffffff,
                               0x00ff7f03,0x1f1ffffd,0x3fffffff,0x3fffffff,0xf,0x0};
void TC_reg_wr (void)
{	
  uint32_t data,j,addr,id;
  uint32_t *p_addr = (uint32_t *)(TC0);
  
	/* Register reset value */
	for(id=0;id<2;id++)
	{
		addr = (0x4000 * id);	
		data = 0xffffffff;
		for(j = 0; j < sizeof(TC_TypeDef) / sizeof(uint32_t); j++)
		{
			if((i==2) || (i==4))
				continue;
				
			if(*(uint32_t *)((uint32_t)p_addr + addr + 4*i) != tc_defaut[i]) 
			{
				Fail();
			}
	  }
  }
	
	for(id=0;id<2;id++)
	{
		addr = (0x4000 * id);	
		data = 0xffffffff;
		for(j = 0; j < sizeof(TC_TypeDef) / sizeof(uint32_t); j++)
		{
			*(uint32_t *)((uint32_t)p_addr + addr + 4*j) = data;
			if(*(uint32_t *)((uint32_t)p_addr + addr + 4*j) != (data & tc_mask[j]))
			{							  
				Fail() ;
			}
		}
  }
	
}

/****************************************************************************** 
 ** @brief  test WDT registers
 ** @param  none.
 ** @return none.
 ** @
 ******************************************************************************/
volatile uint32_t wdt_defaut[] = {0x55,0x68,0x0,0,0,0x7e5a,0xa538};
volatile uint32_t wdt_mask[] = {0xff,0xee,0xff,0,0,0xffff,0xffff};
void WDT_reg_wr (void) 
{
	uint32_t i ,data;
	uint32_t *p_addr = (uint32_t *)(WDT);	  
	
	WDT->PASS = 0x70;
	WDT->PASS = 0x7e;
	
  /* Register reset value */
// 	for(i=0;i<7;i++)
// 	{
// 		if((i==0) || (i==2) || (i==3) || (i==4))
// 			continue;
// 			
// 		if(*(uint32_t *)((uint32_t)p_addr + 4*i) != wdt_defaut[i]) 
// 		{
// 			Fail();
// 		}
// 	}

	/* Register WR 0xffffffff value */
	data = 0xffffffff;
	for(i=0;i<7;i++)
	{
		if((i==0) || (i==2) || (i==3) || (i==4))
			continue;
		while(WDT->EN & (1<<8));
		*(uint32_t *)((uint32_t)p_addr + 4*i) = data;	
		while(WDT->EN & (1<<8));
		if(*(uint32_t *)((uint32_t)p_addr + 4*i) != (wdt_mask[i] & data)) 
		{
			Fail();
		}
	}
	WDT->HALT = 0x7e5a;
	WDT->STBY = 0xa538;
	WDT->EN = 0xbb;
	
}

/****************************************************************************** 
 ** @brief  test UART registers
 ** @param  none.
 ** @return none.
 ** @
 ******************************************************************************/
volatile uint32_t uart_defaut[] = {0,0,0x100,0,0,0};
volatile uint32_t uart_mask[] = {0x7fff,0xfff,0,0,0,0x3fff};
void UART_reg_wr(void)
{
	uint32_t i ,data,addr;
	uint8_t id;
	uint32_t *p_addr = (uint32_t *)(UART0);
	  
  /* Register reset value */
	for(id=0;id<8;id++)
	{
		if(id==4)
			id = id+2;
		
		addr = (0x4000 * id);
		for(i=0;i<6;i++)
		{
			if(*(uint32_t *)((uint32_t)p_addr + 4*i + addr) != uart_defaut[i]) 
			{
				Fail();
			}
		}
  }
	
	/* Register WR 0xffffffff value */
	data = 0xffffffff;
	if(chip_id == 0xa0)
	{
		uart_mask[0] = 0x1fff;
	}
	for(id=0;id<8;id++)
	{
		if(id==4)
			id = id+2;
		
		addr = (0x4000 * id);
		for(i=0;i<(sizeof(UART_TypeDef) / sizeof(uint32_t));i++)
		{
			if((i==2) || (i==3) || (i==4))
				continue;
			if((chip_id == 0xa0) && (i==5))
				continue;
			
			*(uint32_t *)((uint32_t)p_addr + 4*i + addr) = data;
			if((*(uint32_t *)((uint32_t)p_addr + 4*i + addr)) != (uart_mask[i] & data)) 
			{
				Fail();
			}
		}
  }
	
}

/****************************************************************************** 
 ** @brief  test SPI registers
 ** @param  none.
 ** @return none.
 ** @
 ******************************************************************************/
volatile uint32_t spi_defaut[] = {0x002,0x0,0x0,0x0};
volatile uint32_t spi_mask[] = {0x3fffff,0x1f,0xffffffff,0xffffffff};
void SPI_reg_wr(void)
{
	uint32_t i ,data;
	uint32_t *p_addr = (uint32_t *)(SPI);	  
		
  /* Register reset value */
	for(i=0;i<4;i++)
	{
		if(*(uint32_t *)((uint32_t)p_addr + 4*i) != spi_defaut[i]) 
		{
			Fail();
		}
	}

	/* Register WR 0xffffffff value */
	data = 0xffffffff;
	for(i=0;i<4;i++)
	{
		*(uint32_t *)((uint32_t)p_addr + 4*i) = data;	
		if((i==1) || (i==3))
		{
			if(*(uint32_t *)((uint32_t)p_addr + 4*i) != 0) 
			{
				Fail();
			}
	  }		
		else
		{
			if(*(uint32_t *)((uint32_t)p_addr + 4*i) != (spi_mask[i] & data)) 
			{
				Fail();
			}
	  }
	}

}

/****************************************************************************** 
 ** @brief  test RTC registers
 ** @param  none.
 ** @return none.
 ** @
 ******************************************************************************/
void RTC_reg_wr(void)
{
 uint32_t reg_value = 0;
 uint32_t data;
																	
 /* Register ALL Write 0xfff Test */ 
	data = 0xfff;
	RTC->CTRL = 0x100;  //pready is work?
	while(RTC->CTRL & 0x200);
	RTC->CTRL = data;
	while(RTC->CTRL & 0x600);
	reg_value = RTC->CTRL & 0x1ff;
	if ((reg_value) != 0x1ff) {
		 Fail();      
	}            

	RTC->PS = data;
	while(RTC->CTRL & 0x200);
	reg_value = RTC->PS;
	if ((reg_value) != 0xff) {
		 Fail();      
	}  
			
	RTC->CTRL = 0x100;
	while(RTC->CTRL & 0x200);
	RTC->PS = 0x8e;       
	while(RTC->CTRL & 0x200);

	RTC->CNT1 = data;
	while(RTC->CTRL & 0x200);
	reg_value = RTC->CNT1 & 0xff;
	if ((reg_value) != 0xff) {
		 Fail();      
	}
	
	RTC->CNT2 = data;
	while(RTC->CTRL & 0x200);
	reg_value = RTC->CNT2 & 0xff;
	if ((reg_value) != 0xff) {
		 Fail();      
	}    
	
	RTC->IE = data;
	while(RTC->CTRL & 0x200);
	reg_value = RTC->IE & 0x1ff;
	if ((reg_value) != 0x1ff) {
		 Fail();      
	}

	RTC->IF = data;
	while(RTC->CTRL & 0x200);
	reg_value = RTC->IF;
	if ((reg_value) != 0x00) {
		 Fail();      
	}      

	RTC->CTRL = 0x180;
	while(RTC->CTRL & 0x200);
	while(RTC->CTRL & 0x400);
			
	RTC->TEMP = data;
	while(RTC->CTRL & 0x200);
	while(RTC->CTRL & 0x400);
	RTC->TEMP = data;
	while(RTC->CTRL & 0x200);
	while(RTC->CTRL & 0x400);
	reg_value = RTC->TEMP;
	if ((reg_value) != 0x3ff) {
		 Fail();      
	}    
	while(RTC->CTRL & 0x400);     //Wait Calculate Complete
	
	RTC->CTRL = 0x100;
	while(RTC->CTRL & 0x200);
	
	RTC->TCPMD = data;
	while(RTC->CTRL & 0x200);
	reg_value = RTC->TCPMD;
	if ((reg_value) != 0x1) {
		 Fail();      
	}     

	RTC->DOTA0 = data;
	while(RTC->CTRL & 0x200);
	reg_value = RTC->DOTA0;
	if ((reg_value) != 0xfff) {
		 Fail();      
	}     
	
	RTC->ALPHAL = data;
	while(RTC->CTRL & 0x200);
	reg_value = RTC->ALPHAL;
	if ((reg_value) != 0x7ff) {;
		 Fail();      
	} 
	
	RTC->ALPHAH = data;
	while(RTC->CTRL & 0x200);
	reg_value = RTC->ALPHAH;
	if ((reg_value) != 0x7ff) {
		 Fail();      
	} 
	
	RTC->XT0 = data;
	while(RTC->CTRL & 0x200);
	reg_value = RTC->XT0;
	if ((reg_value) != 0x7f) {
		 Fail();      
	} 
	
	RTC->TADJ = data;
	while(RTC->CTRL & 0x200);
	reg_value = RTC->TADJ;
	if ((reg_value) != 0x7ff) {
		 Fail();      
	}            
	
	RTC->ZT = data;
	while(RTC->CTRL & 0x200);
	reg_value = RTC->ZT & 0x1ff;
	if ((reg_value) != 0x1ff) {
		 Fail();      
	}     
	
	RTC->TEMPOS = data;
	while(RTC->CTRL & 0x200);
	reg_value = RTC->TEMPOS;
	if ((reg_value) != 0x3f) {
		 Fail();      
	}     
 
}


/****************************************************************************** 
 ** @brief  test SYSTC registers
 ** @param  none.
 ** @return none.
 ** @
 ******************************************************************************/
volatile uint32_t systc_mask[] = {0xd,0xffffffff,0xffffffff};
void SYSTC_reg_wr(void)
{   
	uint32_t i,data;
  uint32_t *p_addr = (uint32_t *)(SYS_TC);


  /* Register default value	 */
	SYS_TC->LOAD=0;  //bootloader ?LOAD??0xffffffff
  for(i = 0; i < sizeof(SYS_TC_TypeDef) / sizeof(uint32_t); i++)
  {
    if(*(p_addr+i) != 0)
    {
      Fail();
    }
  }	

	/* Register write */
	SYS_TC->LOAD=0xffffffff;
	for(i = 0; i < (sizeof(SYS_TC_TypeDef) / sizeof(uint32_t)-1); i++)
  {		
  	data = 0xffffffff;
		*(p_addr + i) = data;
    if(*(p_addr+i) != (data & systc_mask[i]))
    {
      Fail();
    }
		data =~data;
		*(p_addr + i) = data;
    if(*(p_addr+i) != (data & systc_mask[i]))
    {
      Fail();
    }
  }
}


/*********************************************************************************************************
** Function name:       hex2bcd
** Descriptions:        Hex To BCD
** input parameters:    Hex data (0-99)
** output parameters:   BCD data
** Returned value:      none
*********************************************************************************************************/
uint32_t hex2bcd (uint32_t data) {
  uint32_t result;
  
  if(data>=90)
  {
    result = (9<<4) | (data-90);
  }
  if((data>=80) && (data<90))
  {
    result = (8<<4) | (data-80);
  }
  if((data>=70) && (data<80))
  {
    result = (7<<4) | (data-70);
  }
  if((data>=60) && (data<70))
  {
    result = (6<<4) | (data-60);
  }
  if((data>=50) && (data<60))
  {
    result = (5<<4) | (data-50);
  }
  if((data>=40) && (data<50))
  {
    result = (4<<4) | (data-40);
  }
  if((data>=30) && (data<40))
  {
    result = (3<<4) | (data-30);
  }
  if((data>=20) && (data<30))
  {
    result = (2<<4) | (data-20);
  }
  if((data>=10) && (data<20))
  {
    result = (1<<4) | (data-10);
  }  
  if(data<10)
  {
    result = data;
  } 
  
  return result;         
}

/*********************************************************************************************************
** Function name:       RTC_IRQHandler
** Descriptions:        RTC Interrupt Manager
** input parameters:    none
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
uint32_t intc_flag = 0;
// void  RTC_HANDLER(void){ 
//     
//     if((RTC->IF & 0x8) != 0x8)
//     {
//         ;
//     }
//         
//     if((RTC->IF & 0x8) == 0x8)           
//     {
//         intc_flag = 0x01;
//     }     
//        
//     RTC->IF = 0xff;        //Clear Pending Bit  
//     while(RTC->CTRL & 0x200);
// } 

void RTC_calendar(void)
{
	uint32_t temp;
	
  //Enable RTC INT
//   __enable_irq();
//   NVIC_EnableIRQ(RTC_IRQn);  
  SYSCTL->SYS_PS = 0x75; 	
	if((chip_id == 0xe0) || (chip_id == 0xf0))
	{
		MADC->ANA_TIEHL = 0xa << 12;
		MADC->ANA_TIEHL = 0xa << 12;  //tie release
		MADC->ANA_TEST = (0xa << 13) | (MADC->ANA_TEST | (1 << 7));
		MADC->ANA_TEST = (0xa << 13) | (MADC->ANA_TEST | (1 << 7));
	}
	else
	{
    SYSCTL->TEST_CTRL = 0xa5730308;
	}

  *test_flag = 0x11111111 ;
  RTC->PS = 0x8e;
  while(RTC->CTRL & 0x200);
  RTC->CTRL = 0x100;
  while(RTC->CTRL & 0x200);
  RTC->IE = 0xf8;
  while(RTC->CTRL & 0x200);
	
  RTC->LCNT = 0x1;
  while(RTC->CTRL & 0x200);  
  
  RTC->YR = 0x0;
  while(RTC->CTRL & 0x200);  
  RTC->MO = 0x1;
  while(RTC->CTRL & 0x200);  
  RTC->DW = 0x0;
  while(RTC->CTRL & 0x200);  
  RTC->DT = 0x1;
  while(RTC->CTRL & 0x200);   
  RTC->HR = 0x0;
  while(RTC->CTRL & 0x200);
  RTC->MN = 0x0;
  while(RTC->CTRL & 0x200);
  RTC->SC = 0x0; 
  while(RTC->CTRL & 0x200);  

  for(i=0;i<60;i++)            //Second Test
  {
		WDT->EN = 0xbb;
    while(RTC->CTRL & 0x200);
    
		temp = hex2bcd(i);
		
    RTC->SC = temp;
    while(RTC->CTRL & 0x200);
    RTC->MN = temp;
    while(RTC->CTRL & 0x200);  		
    RTC->HR = temp;
    while(RTC->CTRL & 0x200);    	
    RTC->DT = temp;
    while(RTC->CTRL & 0x200);
    RTC->MO = temp;
    while(RTC->CTRL & 0x200); 		
    RTC->YR = temp;
    while(RTC->CTRL & 0x200); 		
    
		if(chip_id == 0xa0)
		{
			RTC->LCNT = 0x3fff;
			while(RTC->CTRL & 0x200);
			mDelay(10);
			RTC->LCNT = 0x7fff;
			while(RTC->CTRL & 0x200);    
		}
		else
		{
			RTC->LCNT = 0x3f;
			while(RTC->CTRL & 0x200);
			mDelay(10);
			RTC->LCNT = 0x7f;
			while(RTC->CTRL & 0x200); 			
		}
    if(i<60)
		{
		  temp = (i == 59) ? 0 : ( hex2bcd(i+1));
    
      while (RTC->SC != temp)
      {
        ;
      }
      while (RTC->MN != temp)
      {
        ;
      }
		}

    if(i<24)
		{
		  temp = (i == 23) ? 0 : ( hex2bcd(i+1));
    
      while (RTC->HR != temp)
      {
        ;
      }
		}
		
		if((i>0) && (i<30))
		{
      temp = (i == 31) ? 1 : ( hex2bcd(i+1));
    
      while (RTC->DT != temp)
      {
        ;
      } 			
		}
		
		if((i>0) && (i<13))
		{
		  temp = (i == 12) ? 1 : ( hex2bcd(i+1));
			while (RTC->MO != temp)
      {
        ;
      }
		}		

    temp = (i == 99) ? 0 : ( hex2bcd(i+1));
    
    while (RTC->YR != temp)
    {
      ;
    } 		
		
 }
	
  SYSCTL->SYS_PS = 0x75;    
  SYSCTL->TEST_CTRL = 0xa5730000;	
	if((chip_id == 0xe0) || (chip_id == 0xf0))
	{
		temp = MADC->ANA_TEST & (~(1 << 7));
		MADC->ANA_TEST = (0xa << 13) | temp;
		MADC->ANA_TEST = (0xa << 13) | temp;
	}
	else
	{
    SYSCTL->TEST_CTRL = 0xa573000;
	}
	SYSCTL->SYS_PS = 0x82; 
  RTC->IE = 0x0;
	
  //Auto-com Test
  RTC->CTRL = 0x100;
  while(RTC->CTRL & 0x200);
  RTC->CTRL = 0x8;
  while(RTC->CTRL & 0x200);

  RTC->PS = 0x8e;
  RTC->DOTA0 = 0x5;
  RTC->ALPHAL = 0x3ee;
  RTC->ALPHAH = 0x4cf;
  RTC->XT0 = 0x64;
  RTC->TADJ = 0x687;
  RTC->BETA = 0xff;
  RTC->ZT = 0x116;	

  RTC->CTRL = 0x100;
  while(RTC->CTRL & 0x200);
  RTC->CTRL = 0x188;
  while(RTC->CTRL & 0x200);
  RTC->CTRL = 0x188;
  while(RTC->CTRL & 0x600);	
	mDelay(1);
  RTC->TEMP = 0x80;
  while(RTC->CTRL & 0x600);
  mDelay(100); //100ms

  if((RTC->DOTAT != 0xffcb) || (RTC->FPTR != 0x7fe8))
	{
		Fail();
	}	
}
