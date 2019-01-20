#define _SWD_DRV_GLOBALS

// #include "swd_drv.h"
#include "drv.h"

void mem_wr_reg32(unsigned long addr,unsigned long data)
{
  unsigned long ack;
	uint8_t i ;
  
	for( i=0 ;i<SWD_REG_MAX_RETRY ;i++ )
	{
    ack = swd_mem_wr32(addr,&data,1); 
		if( ack == SWD_RESP_OK )
			break ;
	}
  
  if (ack != SWD_RESP_OK) 
  	{
    //printf("Write 0x%x to Add0x%8x Failed!\n",data,addr);
 	 	}
}

void mem_wr_reg8(unsigned long addr,uint8_t data)
{
  unsigned long ack;
	uint8_t i ;
  
	for( i=0 ;i<SWD_REG_MAX_RETRY ;i++ )
	{
    ack = swd_mem_wr8(addr,&data,1); 
		if( ack == SWD_RESP_OK )
			break ;
	}
  
  if (ack != SWD_RESP_OK) 
  	{
    //printf("Write 0x%x to Add0x%8x Failed!\n",data,addr);
  	}
}

unsigned long mem_rd_reg32(unsigned long addr)
{
  unsigned long td;
  unsigned long ack;
	uint8_t i ;
  
	for( i=0 ;i<SWD_REG_MAX_RETRY ;i++ )
	{
    ack = swd_mem_rd32(addr,&td,1); 
		if( ack == SWD_RESP_OK )
			break ;
	}
	
  if (ack != SWD_RESP_OK) 
  	{
    //printf("Read Add0x%8x Failed!\n",addr);
  	}
  
  return td;
}

uint8_t mem_rd_reg8(unsigned long addr)
{
  uint8_t td;
  unsigned long ack;
	uint8_t i ;
	
  for( i=0 ;i<SWD_REG_MAX_RETRY ;i++ )
	{
    ack = swd_mem_rd8(addr,&td,1); 
		if( ack == SWD_RESP_OK )
			break ;
	}
  
  if (ack != SWD_RESP_OK) 
  	{
    	//printf("Read Add0x%8x Failed!\n",addr);
  	}
  
  return td;
}

void ra9103_initail(void)
{
  uint32_t data ;

  mem_wr_reg32(SYS_SYS_PS,0x82);
	
	swd_mem_rd32((unsigned long)(&(SYSCTL->SYS_PD)),(unsigned long *)&data,1);
	data &= (~(0x1 << 6));   //enable I1/I2/U channel
  mem_wr_reg32(SYS_SYS_PD,data);    //Enable BGR.
  
  //Enable MADC/RTC/GPIO/LCD Clock
//  mem_wr_reg32(SYS_MOD1_EN,0xc60);
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("read SOC MOD1_EN fail\n");
  } 
  data = data | 0xc60 ;
  mem_wr_reg32(SYS_MOD1_EN,data);

  mem_wr_reg32(0x40048000,0x0);
	
	//Initail GPIO
// 	mem_wr_reg32(GPIO_PMA,0xffffffff);  //All as input.
//   mem_wr_reg32(GPIO_PCA0,0x20000000); //All as GPIO except SWD.
// 	mem_wr_reg32(GPIO_PCA1,0x20000000); //All as GPIO except SWD.
// 	mem_wr_reg32(GPIO_PIEA,0xffffffff); //All input is diable.
// 	
// 	mem_wr_reg32(GPIO_PMB,0xffffffff);  //All as input.
//   mem_wr_reg32(GPIO_PCB,0x00000000);  //All as GPIO.
// 	mem_wr_reg32(GPIO_PIEB,0xffffffff); //All input is diable.	
// 	
// 	mem_wr_reg32(GPIO_PMC,0xffffffff);  //All as input.
//   mem_wr_reg32(GPIO_PCC,0x00000000);  //All as GPIO.
// 	mem_wr_reg32(GPIO_PIEC,0xffffffff); //All input is diable.	
	
// 	mem_wr_reg32(GPIO_PMA,0xC0300000);  //All as output except SWD and HOSC
//   mem_wr_reg32(GPIO_PCA0,0x20000000); //All as GPIO except SWD.
// 	mem_wr_reg32(GPIO_PCA1,0xa000); //All as GPIO except SWD.
// 	mem_wr_reg32(GPIO_PA,0xffffffff);   //All output is high.
// 	mem_wr_reg32(GPIO_PIEA,0xffffffff); //All input is diable.
// 	
// 	mem_wr_reg32(GPIO_PMB,0x00000000);  //All as output.
//   mem_wr_reg32(GPIO_PCB,0x00000000);  //All as GPIO.
// 	mem_wr_reg32(GPIO_PB ,0xffffffff);  //All output is high.
// 	mem_wr_reg32(GPIO_PIEB,0xffffffff); //All input is diable.	
// 	
// 	mem_wr_reg32(GPIO_PMC,0x00000000);  //All as output.
//   mem_wr_reg32(GPIO_PCC,0x00000000);  //All as GPIO.
// 	mem_wr_reg32(GPIO_PC, 0xffffffff);  //All output is high.	
// 	mem_wr_reg32(GPIO_PIEC,0xffffffff); //All input is diable.
	
	mem_wr_reg32(GPIO_PMA,0x00000000);  //All as output except SWD and HOSC
  mem_wr_reg32(GPIO_PCA0,0x20000000); //All as GPIO except SWD.
	mem_wr_reg32(GPIO_PCA1,0x0000); //All as GPIO except SWD.
	mem_wr_reg32(GPIO_PA,0x0);   //All output is high.
	mem_wr_reg32(GPIO_PIEA,0xffffffff); //All input is diable.
	
	mem_wr_reg32(GPIO_PMB,0x00000000);  //All as output.
  mem_wr_reg32(GPIO_PCB,0xffff0000);  //All as GPIO.
	mem_wr_reg32(GPIO_PB ,0x0);  //All output is high.
	mem_wr_reg32(GPIO_PIEB,0xffffffff); //All input is diable.	
	
	mem_wr_reg32(GPIO_PMC,0x00000000);  //All as output.
  mem_wr_reg32(GPIO_PCC,0x00000000);  //All as GPIO.
	mem_wr_reg32(GPIO_PC, 0xffffffff);  //All output is high.	
	mem_wr_reg32(GPIO_PIEC,0xffffffff); //All input is diable.

/*
	mem_wr_reg32(0x40048000,0x1000);
  
	mem_wr_reg32(GPIO_PCB,0x0);
	
  mem_wr_reg32(GPIO_PA,0x0);
  mem_wr_reg32(GPIO_PMA,0x0);
  mem_wr_reg32(GPIO_PB,0x0);
  mem_wr_reg32(GPIO_PMB,0x0);
  mem_wr_reg32(GPIO_PC,0x0);
  mem_wr_reg32(GPIO_PMC,0x0);
  mem_wr_reg32(GPIO_PD,0x0);
  mem_wr_reg32(GPIO_PMD,0x0);    
*/
}


const unsigned char __swd_line_reset_sequence[] = {
    0xFF, 0xFF, 0xFF, 0xFF
  , 0xFF, 0xFF, 0xFF, 0xFF
  , 0x9E, 0xE7
  , 0xFF, 0xFF, 0xFF, 0xFF
  , 0xFF, 0xFF, 0xFF, 0xFF
  , 0x00, 0x00
};
const unsigned long __swd_line_reset_sequence_bit_size = (sizeof(__swd_line_reset_sequence) / sizeof(unsigned char) << 3);

s_swd_dp_reg_set swd_dp_reg_set;

void swd_drv_init(void) {  
  if(swd_num == SWD_PARALLEL) 
  {
    __swd_p_init();
  }
  
  if(swd_num == SWD_SERIAL) 
  {
    LPC_GPIO1->FIODIR  |= (1 << 28);
    LPC_GPIO1->FIODIR  |= (1 << 29);    
    LPC_GPIO2->FIODIR  |= (1 << 8);  
  
    DIROUTPUT_SET1;
// 	DIRINPUT_SET1;//cgf test @20160913
    TR0_EN_SET0;    

    __swd_s_init();
  }  
}

void swd_tx(const unsigned char * pBuf,unsigned long bit_len) {
  unsigned long i = 0;

  if(swd_num == SWD_PARALLEL)
  {
    __swd_p_idle();
    
    for (i = 0; i < bit_len; i ++) {
	  	
      if (((pBuf[i >> 3] >> (i & 7)) & 1) != 0) {
	    __set_swd_pio_to_high();
      } else {
	    __set_swd_pio_to_low();
	  }
	  __set_swd_pclk_to_high();
	  SWD_DELAY;
	  __set_swd_pclk_to_low();
    }
    
    __swd_p_exit();   
  }
  
  if(swd_num == SWD_SERIAL)
  {
    __swd_s_idle();
    
    for (i = 0; i < bit_len; i ++) {
	  	
      if (((pBuf[i >> 3] >> (i & 7)) & 1) != 0) {
	    __set_swd_sio_to_high();
      } else {
	    __set_swd_sio_to_low();
	  }
	  __set_swd_sclk_to_high();
	  SWD_DELAY;
	  __set_swd_sclk_to_low();
    }
    
    __swd_s_exit();   
  }  
  
}

#define __set_swd_pcycle(a)   {\
	                  __set_swd_pio(a);\
                     __set_swd_pclk_to_high();\
						SWD_DELAY;\
						__set_swd_pclk_to_low();\
            }
#define __set_swd_scycle(a)   {\
	                  __set_swd_sio(a);\
                     __set_swd_sclk_to_high();\
						SWD_DELAY;\
						__set_swd_sclk_to_low();\
            }            

void swd_tx8(const unsigned char * pBuf) {
  if(swd_num == SWD_PARALLEL) 
  {
    __swd_p_idle();
    
    __set_swd_pcycle(((*pBuf >> 0)& 0x1));
    __set_swd_pcycle(((*pBuf >> 1) & 0x1));
    __set_swd_pcycle(((*pBuf >> 2) & 0x1));
    __set_swd_pcycle(((*pBuf >> 3) & 0x1));
    __set_swd_pcycle(((*pBuf >> 4) & 0x1));
    __set_swd_pcycle(((*pBuf >> 5) & 0x1));
    __set_swd_pcycle(((*pBuf >> 6) & 0x1));
    __set_swd_pcycle(((*pBuf >> 7) & 0x1));
	  
    __swd_p_exit();
  }
  
  if(swd_num == SWD_SERIAL) 
  {
    __swd_s_idle();
    
    __set_swd_scycle(((*pBuf >> 0)& 0x1));
    __set_swd_scycle(((*pBuf >> 1) & 0x1));
    __set_swd_scycle(((*pBuf >> 2) & 0x1));
    __set_swd_scycle(((*pBuf >> 3) & 0x1));
    __set_swd_scycle(((*pBuf >> 4) & 0x1));
    __set_swd_scycle(((*pBuf >> 5) & 0x1));
    __set_swd_scycle(((*pBuf >> 6) & 0x1));
    __set_swd_scycle(((*pBuf >> 7) & 0x1));
	  
    __swd_s_exit();
  }  
}

void swd_tx32(const unsigned long * pBuf) {
  if(swd_num == SWD_PARALLEL) 
  {
    __swd_p_idle();
    
    __set_swd_pcycle(((*pBuf >> 0) & 0x1));
    __set_swd_pcycle(((*pBuf >> 1) & 0x1));
    __set_swd_pcycle(((*pBuf >> 2) & 0x1));
    __set_swd_pcycle(((*pBuf >> 3) & 0x1));
    __set_swd_pcycle(((*pBuf >> 4) & 0x1));
    __set_swd_pcycle(((*pBuf >> 5) & 0x1));
    __set_swd_pcycle(((*pBuf >> 6) & 0x1));
    __set_swd_pcycle(((*pBuf >> 7) & 0x1));
    __set_swd_pcycle(((*pBuf >> 8) & 0x1));
    __set_swd_pcycle(((*pBuf >> 9) & 0x1));
    __set_swd_pcycle(((*pBuf >> 10) & 0x1));
    __set_swd_pcycle(((*pBuf >> 11) & 0x1));
    __set_swd_pcycle(((*pBuf >> 12) & 0x1));
    __set_swd_pcycle(((*pBuf >> 13) & 0x1));
    __set_swd_pcycle(((*pBuf >> 14) & 0x1));
    __set_swd_pcycle(((*pBuf >> 15) & 0x1));
    __set_swd_pcycle(((*pBuf >> 16) & 0x1));
    __set_swd_pcycle(((*pBuf >> 17) & 0x1));
    __set_swd_pcycle(((*pBuf >> 18) & 0x1));
    __set_swd_pcycle(((*pBuf >> 19) & 0x1));
    __set_swd_pcycle(((*pBuf >> 20) & 0x1));
    __set_swd_pcycle(((*pBuf >> 21) & 0x1));
    __set_swd_pcycle(((*pBuf >> 22) & 0x1));
    __set_swd_pcycle(((*pBuf >> 23) & 0x1));
    __set_swd_pcycle(((*pBuf >> 24) & 0x1));
    __set_swd_pcycle(((*pBuf >> 25) & 0x1));
    __set_swd_pcycle(((*pBuf >> 26) & 0x1));
    __set_swd_pcycle(((*pBuf >> 27) & 0x1));
    __set_swd_pcycle(((*pBuf >> 28) & 0x1));
    __set_swd_pcycle(((*pBuf >> 29) & 0x1));
    __set_swd_pcycle(((*pBuf >> 30) & 0x1));
    __set_swd_pcycle(((*pBuf >> 31) & 0x1));
	  
    __swd_p_exit();   
  }
  
  if(swd_num == SWD_SERIAL) 
  {
    __swd_s_idle();
    
    __set_swd_scycle(((*pBuf >> 0) & 0x1));
    __set_swd_scycle(((*pBuf >> 1) & 0x1));
    __set_swd_scycle(((*pBuf >> 2) & 0x1));
    __set_swd_scycle(((*pBuf >> 3) & 0x1));
    __set_swd_scycle(((*pBuf >> 4) & 0x1));
    __set_swd_scycle(((*pBuf >> 5) & 0x1));
    __set_swd_scycle(((*pBuf >> 6) & 0x1));
    __set_swd_scycle(((*pBuf >> 7) & 0x1));
    __set_swd_scycle(((*pBuf >> 8) & 0x1));
    __set_swd_scycle(((*pBuf >> 9) & 0x1));
    __set_swd_scycle(((*pBuf >> 10) & 0x1));
    __set_swd_scycle(((*pBuf >> 11) & 0x1));
    __set_swd_scycle(((*pBuf >> 12) & 0x1));
    __set_swd_scycle(((*pBuf >> 13) & 0x1));
    __set_swd_scycle(((*pBuf >> 14) & 0x1));
    __set_swd_scycle(((*pBuf >> 15) & 0x1));
    __set_swd_scycle(((*pBuf >> 16) & 0x1));
    __set_swd_scycle(((*pBuf >> 17) & 0x1));
    __set_swd_scycle(((*pBuf >> 18) & 0x1));
    __set_swd_scycle(((*pBuf >> 19) & 0x1));
    __set_swd_scycle(((*pBuf >> 20) & 0x1));
    __set_swd_scycle(((*pBuf >> 21) & 0x1));
    __set_swd_scycle(((*pBuf >> 22) & 0x1));
    __set_swd_scycle(((*pBuf >> 23) & 0x1));
    __set_swd_scycle(((*pBuf >> 24) & 0x1));
    __set_swd_scycle(((*pBuf >> 25) & 0x1));
    __set_swd_scycle(((*pBuf >> 26) & 0x1));
    __set_swd_scycle(((*pBuf >> 27) & 0x1));
    __set_swd_scycle(((*pBuf >> 28) & 0x1));
    __set_swd_scycle(((*pBuf >> 29) & 0x1));
    __set_swd_scycle(((*pBuf >> 30) & 0x1));
    __set_swd_scycle(((*pBuf >> 31) & 0x1));
	  
    __swd_s_exit();   
  }  
}

void swd_rx(unsigned char * pBuf,unsigned long bit_len) {
  unsigned long i = 0;

  if(swd_num == SWD_PARALLEL) 
  {
    __swd_p_idle();
    __set_swd_pio_as_in();  
    __set_swd_pclk_to_low();
    for (i = 0; i < bit_len; i ++) {
	  __set_swd_pclk_to_high();
	  __nop();__nop();__nop();__nop();
      __set_swd_pclk_to_low();
	  if ((i & 7) == 0) {
	    pBuf[i >> 3] = 0;
	  }
	  pBuf[i >> 3] |= (__get_swd_pio() & 1) << (i & 7);
	  __nop();__nop();__nop();__nop();
    }
    __swd_p_exit();   
  }
  
  if(swd_num == SWD_SERIAL) 
  {
    __swd_s_idle();
    __set_swd_sio_as_in();  
    __set_swd_sclk_to_low();
    for (i = 0; i < bit_len; i ++) {
	  __set_swd_sclk_to_high();
	  __nop();__nop();__nop();__nop();
      __set_swd_sclk_to_low();
	  if ((i & 7) == 0) {
	    pBuf[i >> 3] = 0;
	  }
	  pBuf[i >> 3] |= (__get_swd_sio() & 1) << (i & 7);
	  __nop();__nop();__nop();__nop();
    }
    __swd_s_exit();   
  }  
} 
  
#define __get_swd_pcycle(a, b)   {\
	                  __set_swd_pclk_to_high();\
	                  SWD_DELAY;\
	                  __set_swd_pclk_to_low();\
                     a |= (__get_swd_pio() << b);\
            }
#define __get_swd_scycle(a, b)   {\
	                  __set_swd_sclk_to_high();\
	                  SWD_DELAY;\
	                  __set_swd_sclk_to_low();\
                     a |= (__get_swd_sio() << b);\
            }            
  
void swd_rx8(unsigned char * pBuf) {
  
  if(swd_num == SWD_PARALLEL)
  {
    *pBuf = 0;
    __swd_p_idle();	
    __set_swd_pio_as_in();
    
    __get_swd_pcycle(*pBuf, 0);
    __get_swd_pcycle(*pBuf, 1);
    __get_swd_pcycle(*pBuf, 2);
    __get_swd_pcycle(*pBuf, 3);
    __get_swd_pcycle(*pBuf, 4);
    __get_swd_pcycle(*pBuf, 5);
    __get_swd_pcycle(*pBuf, 6);
    __get_swd_pcycle(*pBuf, 7);	
    
    __swd_p_exit();    
  }
  
  if(swd_num == SWD_SERIAL)
  {
    *pBuf = 0;
    __swd_s_idle();	
    __set_swd_sio_as_in();
    
    __get_swd_scycle(*pBuf, 0);
    __get_swd_scycle(*pBuf, 1);
    __get_swd_scycle(*pBuf, 2);
    __get_swd_scycle(*pBuf, 3);
    __get_swd_scycle(*pBuf, 4);
    __get_swd_scycle(*pBuf, 5);
    __get_swd_scycle(*pBuf, 6);
    __get_swd_scycle(*pBuf, 7);	
    
    __swd_s_exit();    
  }  
}

void swd_rx32(unsigned long * pBuf) {
  
  if(swd_num == SWD_PARALLEL)
  {
    *pBuf = 0;
    __swd_p_idle();	
    __set_swd_pio_as_in();
    
    __get_swd_pcycle(*pBuf, 0);
    __get_swd_pcycle(*pBuf, 1);
    __get_swd_pcycle(*pBuf, 2);
    __get_swd_pcycle(*pBuf, 3);
    __get_swd_pcycle(*pBuf, 4);
    __get_swd_pcycle(*pBuf, 5);
    __get_swd_pcycle(*pBuf, 6);
    __get_swd_pcycle(*pBuf, 7);	
    __get_swd_pcycle(*pBuf, 8);	
    __get_swd_pcycle(*pBuf, 9);	
    __get_swd_pcycle(*pBuf, 10);
    __get_swd_pcycle(*pBuf, 11);
    __get_swd_pcycle(*pBuf, 12);
    __get_swd_pcycle(*pBuf, 13);
    __get_swd_pcycle(*pBuf, 14);
    __get_swd_pcycle(*pBuf, 15);
    __get_swd_pcycle(*pBuf, 16);
    __get_swd_pcycle(*pBuf, 17);	
    __get_swd_pcycle(*pBuf, 18);	
    __get_swd_pcycle(*pBuf, 19);	
    __get_swd_pcycle(*pBuf, 20);
    __get_swd_pcycle(*pBuf, 21);
    __get_swd_pcycle(*pBuf, 22);
    __get_swd_pcycle(*pBuf, 23);
    __get_swd_pcycle(*pBuf, 24);
    __get_swd_pcycle(*pBuf, 25);
    __get_swd_pcycle(*pBuf, 26);
    __get_swd_pcycle(*pBuf, 27);	
    __get_swd_pcycle(*pBuf, 28);	
    __get_swd_pcycle(*pBuf, 29);	
    __get_swd_pcycle(*pBuf, 30);
    __get_swd_pcycle(*pBuf, 31);
	  
    __swd_p_exit();  
  }
  
  if(swd_num == SWD_SERIAL)
  {
    *pBuf = 0;
    __swd_s_idle();	
    __set_swd_sio_as_in();
    
    __get_swd_scycle(*pBuf, 0);
    __get_swd_scycle(*pBuf, 1);
    __get_swd_scycle(*pBuf, 2);
    __get_swd_scycle(*pBuf, 3);
    __get_swd_scycle(*pBuf, 4);
    __get_swd_scycle(*pBuf, 5);
    __get_swd_scycle(*pBuf, 6);
    __get_swd_scycle(*pBuf, 7);	
    __get_swd_scycle(*pBuf, 8);	
    __get_swd_scycle(*pBuf, 9);	
    __get_swd_scycle(*pBuf, 10);
    __get_swd_scycle(*pBuf, 11);
    __get_swd_scycle(*pBuf, 12);
    __get_swd_scycle(*pBuf, 13);
    __get_swd_scycle(*pBuf, 14);
    __get_swd_scycle(*pBuf, 15);
    __get_swd_scycle(*pBuf, 16);
    __get_swd_scycle(*pBuf, 17);	
    __get_swd_scycle(*pBuf, 18);	
    __get_swd_scycle(*pBuf, 19);	
    __get_swd_scycle(*pBuf, 20);
    __get_swd_scycle(*pBuf, 21);
    __get_swd_scycle(*pBuf, 22);
    __get_swd_scycle(*pBuf, 23);
    __get_swd_scycle(*pBuf, 24);
    __get_swd_scycle(*pBuf, 25);
    __get_swd_scycle(*pBuf, 26);
    __get_swd_scycle(*pBuf, 27);	
    __get_swd_scycle(*pBuf, 28);	
    __get_swd_scycle(*pBuf, 29);	
    __get_swd_scycle(*pBuf, 30);
    __get_swd_scycle(*pBuf, 31);
	  
    __swd_s_exit();  
  }  
}

void          swd_nop(unsigned long bit_len) {
  unsigned long i = 0;
  
  if(swd_num == SWD_PARALLEL)
  {
    __set_swd_pio_as_in();
    
    for (i = 0; i < bit_len; i ++) {
    __set_swd_pclk_to_low();
	  __nop();__nop();__nop();__nop();
	  __set_swd_pclk_to_high();
    }
    __set_swd_pclk_to_low();     
  }
  if(swd_num == SWD_SERIAL)
  {
    __set_swd_pio_as_in();
    for (i = 0; i < bit_len; i ++) {
    __set_swd_sclk_to_low();
	  __nop();__nop();__nop();__nop();
	  __set_swd_sclk_to_high();
    }
    __set_swd_sclk_to_low();      
  } 
}

unsigned long swd_parity_cal(unsigned long data,unsigned long bit_len,unsigned long start_pos) {
  unsigned int parity = 0;
  unsigned int i = 0;

  if(bit_len == 32)
	{
		parity = data ^ (data>>16);
		parity = (parity>>8) ^ parity;
		parity = (parity>>4) ^ parity;
		parity = (parity>>2) ^ parity;
		parity = (parity>>1) ^ parity;
		parity = parity & 0x1;
	}
	else
	{
		for (i = start_pos; i < bit_len; i ++) {
			if (((data >> i) & 1) != 0) {
				parity = 1 - parity;
			}
		}
	}

  return parity;
}

unsigned long swd_pkt_req(unsigned char pkt) {
  unsigned long ack = SWD_RESP_PROTOCAL_ERROR;

  pkt = ((swd_parity_cal(pkt,5,1) & 1) << 5) | (pkt & 0x1F) | 0x80;
  //swd_tx(&pkt,8);
  swd_tx8(&pkt);
  swd_rx((unsigned char *)&ack,3);

  return (ack & 0x7);
}

#ifndef CGF
void swd_fault_clear(void) {
#else
unsigned long swd_fault_clear(void) {
#endif
  unsigned long ack = 0;
  unsigned long cns = 0;

#ifndef CGF
  while(1) {
#else
  {
#endif
    ack = swd_pkt_req(SWD_PKT_REQ_WRITE_ABORT);
    swd_rx((unsigned char *)(&ack),2);  // turn around
    ack = 0x1e;
#ifndef CGF
    swd_tx((unsigned char *)(&ack),32);
#else
    swd_tx32(&ack);
#endif
    ack = 0;
    swd_tx((unsigned char *)(&ack),4);
#ifdef CGF
    swd_pkt_kickoff();
    swd_pkt_kickoff();
    swd_pkt_kickoff();
    swd_pkt_kickoff();
    swd_pkt_kickoff();
    swd_pkt_kickoff();
#endif
    ack = swd_pkt_req(SWD_PKT_REQ_READ_CTLSTA);
    swd_rx((unsigned char *)(&cns),32);
    swd_rx((unsigned char *)(&ack),2);
    ack &= 1;
    swd_pkt_kickoff();
    if (((cns & 0xb2) != 0) && (swd_parity_cal(cns,32,0) != ack))
#ifndef CGF
      return;
#else
      return SWD_RESP_OK;
    else
	  return SWD_RESP_FAULT;
#endif
#ifndef CGF
    swd_pkt_kickoff();
    swd_pkt_kickoff();
    swd_pkt_kickoff();
    swd_pkt_kickoff();
    swd_pkt_kickoff();
    swd_pkt_kickoff();
#endif
  }
}

unsigned long __swd_dp_wr_i(unsigned long pkt_req,unsigned long wdata) {
  unsigned long parity_tx = swd_parity_cal(wdata,32,0);
  unsigned long ack = SWD_RESP_PROTOCAL_ERROR;
  unsigned long ignore = 0;
	unsigned char header = 0;

  ack = swd_pkt_req(pkt_req);
  if (ack == SWD_RESP_OK) {
		
    swd_rx((unsigned char *)(&ignore),2);
#ifndef CGF
    swd_tx((unsigned char *)(&wdata),32);
#else
    swd_tx32(&wdata);
#endif

    swd_tx((unsigned char *)(&parity_tx),4);
	/************zhanghui*************/
	header = ((swd_parity_cal(pkt_req,5,1) & 1) << 5) | (pkt_req & 0x1F) | 0x80;
	DBG_MSG("0x%02X,0x%08X\n",header,wdata);
//	if((header & 0x04) > 0)
//	{
//		DBG_MSG("This is Read data,Header data is:0x%02X,Read data is:0x%08X\n",header,wdata);
//	}else{
//		DBG_MSG("This is Write data,Header data is:0x%02X,Write data is:0x%08X\n",header,wdata);
//	}
	/*******************************/
		
		
  }

  swd_pkt_kickoff();

  return ack;
}

#ifdef CGF
unsigned long __swd_dp_wr_i_mem(unsigned long pkt_req,unsigned long wdata) {
  unsigned long parity_tx = swd_parity_cal(wdata,32,0);
  unsigned long ack = SWD_RESP_PROTOCAL_ERROR;
  unsigned long ignore = 0;
	unsigned char header = 0;

  ack = swd_pkt_req(pkt_req);
  if (ack == SWD_RESP_OK) {
		
    swd_rx((unsigned char *)(&ignore),2);
#ifndef CGF
    swd_tx((unsigned char *)(&wdata),32);
#else
    swd_tx32(&wdata);
#endif

    swd_tx((unsigned char *)(&parity_tx),4);
	/************zhanghui*************/
	header = ((swd_parity_cal(pkt_req,5,1) & 1) << 5) | (pkt_req & 0x1F) | 0x80;
	DBG_MSG("0x%02X,0x%08X\n",header,wdata);
//	if((header & 0x04) > 0)
//	{
//		DBG_MSG("This is Read data,Header data is:0x%02X,Read data is:0x%08X\n",header,wdata);
//	}else{
//		DBG_MSG("This is Write data,Header data is:0x%02X,Write data is:0x%08X\n",header,wdata);
//	}
	/*******************************/
  }

  //swd_pkt_kickoff();

  return ack;
}
#endif

unsigned long __swd_dp_rd_i(unsigned long pkt_req,unsigned long * pRdata) {
  unsigned long parity_id = 0;
  unsigned long parity_rx = 0;
  unsigned long id = 0;
  unsigned long ack = SWD_RESP_PROTOCAL_ERROR;
	unsigned char header = 0;

  ack = swd_pkt_req(pkt_req);
  if (ack == SWD_RESP_OK) {
    //swd_rx((unsigned char *)(&id),32);
    swd_rx32(&id);
    swd_rx((unsigned char *)(&parity_rx),2);
    parity_id = swd_parity_cal(id,32,0);
	parity_rx &= 1;
	/************zhanghui*************/
	header = ((swd_parity_cal(pkt_req,5,1) & 1) << 5) | (pkt_req & 0x1F) | 0x80;
	DBG_MSG("0x%02X,0x%08X\n",header,id);
//	if((header & 0x04) > 0)
//	{
//		DBG_MSG("This is Read data,Header data is:0x%02X,Read data is:0x%08X\n",header,id);
//	}else{
//		DBG_MSG("This is Write data,Header data is:0x%02X,Write data is:0x%08X\n",header,id);
//	}
	/*******************************/
	if (parity_rx != parity_id) {
	  ack = SWD_RESP_WAIT;
	}
    else
	  * pRdata = id;
  }

  swd_pkt_kickoff();

  return ack;
}

#ifdef CGF
unsigned long __swd_dp_rd_i_mem(unsigned long pkt_req,unsigned long * pRdata) {
  unsigned long parity_id = 0;
  unsigned long parity_rx = 0;
  unsigned long id = 0;
  unsigned long ack = SWD_RESP_PROTOCAL_ERROR;
	unsigned char header = 0;

  ack = swd_pkt_req(pkt_req);
  if (ack == SWD_RESP_OK) {
#ifndef CGF
    swd_rx((unsigned char *)(&id),32);
#else
    swd_rx32(&id);
#endif
    swd_rx((unsigned char *)(&parity_rx),2);
    parity_id = swd_parity_cal(id,32,0);
	parity_rx &= 1;
		/************zhanghui*************/
	header = ((swd_parity_cal(pkt_req,5,1) & 1) << 5) | (pkt_req & 0x1F) | 0x80;
	DBG_MSG("0x%02X,0x%08X\n",header,id);
//	if((header & 0x04) > 0)
//	{
//		DBG_MSG("This is Read data,Header data is:0x%02X,Read data is:0x%08X\n",header,id);
//	}else{
//		DBG_MSG("This is Write data,Header data is:0x%02X,Write data is:0x%08X\n",header,id);
//	}
	/*******************************/
	if (parity_rx != parity_id) {
	  ack = SWD_RESP_WAIT;
	}
    else
	  * pRdata = id;
  }

  return ack;
}
#endif

unsigned long swd_dp_wr(unsigned long pkt_req,unsigned long wdata) {
  unsigned long parity_tx = swd_parity_cal(wdata,32,0);
  unsigned long ack = SWD_RESP_PROTOCAL_ERROR;
  unsigned long ignore = 0;

  do {
    ack = __swd_dp_wr_i(pkt_req,wdata);
	ignore ++;
	if (ack == SWD_RESP_FAULT) {
#ifndef CGF
		swd_fault_clear();
#else
	  ack = swd_fault_clear();
#endif
	}
	if (ack == SWD_RESP_WAIT) {
	  delay(10000);
	}
  } while((ack != SWD_RESP_OK) && (ignore < SWD_BUS_MAX_RETRY));

  return ack;
}

#ifdef CGF
unsigned long swd_dp_wr_mem(unsigned long pkt_req,unsigned long wdata) {
  unsigned long parity_tx = swd_parity_cal(wdata,32,0);
  unsigned long ack = SWD_RESP_PROTOCAL_ERROR;
  unsigned long ignore = 0;

  do {
    ack = __swd_dp_wr_i_mem(pkt_req,wdata);
	ignore ++;
	if (ack == SWD_RESP_FAULT) {
#ifndef CGF
		swd_fault_clear();
#else
	  ack = swd_fault_clear();
#endif
	}
	if (ack == SWD_RESP_WAIT) {
	  delay(10000);
	}
  } while((ack != SWD_RESP_OK) && (ignore < SWD_BUS_MAX_RETRY));

  return ack;
}

#endif

unsigned long swd_dp_rd(unsigned long pkt_req,unsigned long * pRdata) {
  unsigned long id = 0;
  unsigned long ack = SWD_RESP_PROTOCAL_ERROR;

  do {
    ack = __swd_dp_rd_i(pkt_req,pRdata);
	if (ack == SWD_RESP_FAULT) {
#ifndef CGF
		swd_fault_clear();
#else
	  ack = swd_fault_clear();
#endif
	}
	id ++;
	if (ack == SWD_RESP_WAIT) {
	  delay(10000);
	}
  } while((ack != SWD_RESP_OK) && (id < SWD_BUS_MAX_RETRY));

  return ack;
}

#ifdef CGF
unsigned long swd_dp_rd_mem(unsigned long pkt_req,unsigned long * pRdata) {
  unsigned long id = 0;
  unsigned long ack = SWD_RESP_PROTOCAL_ERROR;

  do {
    ack = __swd_dp_rd_i_mem(pkt_req,pRdata);
	if (ack == SWD_RESP_FAULT) {
#ifndef CGF
		swd_fault_clear();
#else
	  ack = swd_fault_clear();
#endif
	}
	id ++;
	if (ack == SWD_RESP_WAIT) {
	  delay(10000);
	}
  } while((ack != SWD_RESP_OK) && (id < SWD_BUS_MAX_RETRY));

  return ack;
}
#endif

unsigned long swd_dp_ctrl_n_stat_get(unsigned long * pCns) {
//  if (swd_dp_select_set(SWD_PKT_REQ_CTRL_SEL) == SWD_RESP_OK) {
     return swd_dp_rd(SWD_PKT_REQ_READ_CTLSTA,pCns);
//  }

//  return SWD_RESP_PROTOCAL_ERROR;
}

unsigned long swd_dp_ctrl_n_stat_set(unsigned long cns) {
//  if (swd_dp_select_set(SWD_PKT_REQ_CTRL_SEL) == SWD_RESP_OK) {
     return swd_dp_wr(SWD_PKT_REQ_WRITE_CTLSTA,cns);
//  }

//  return SWD_RESP_PROTOCAL_ERROR;
}

void          swd_pkt_kickoff(void) {
  unsigned long ko = 0;
#ifndef CGF
  swd_tx((unsigned char *)(&ko),8);
#else
  swd_tx8((unsigned char *)(&ko));
#endif
}

unsigned long swd_line_reset(unsigned long * pID) {
  unsigned long ack = SWD_RESP_PROTOCAL_ERROR;
  unsigned long retry = 0;
  do {
    swd_tx(__swd_line_reset_sequence, __swd_line_reset_sequence_bit_size);
    ack = swd_dp_id_get(pID);
	retry ++;
  } while((ack != SWD_RESP_OK) && (retry < SWD_BUS_MAX_RETRY));

  return ack;
}

void          swd_ctrl_n_stat_prn(unsigned long cns) {
    //DBG("CTRL/STAT is :\n");
	//DBG("  ORUNDETECT %s\n",((cns & 1) != 0) ? "enabled" : "disabled");
	//DBG("  STICKYORUN %s\n",((cns & 2) != 0) ? "found" : "not found");
	//DBG("  TRNMODE is %s\n", (((cns >> 2) & 3) == 0) ? "Normal" : ((((cns >> 2) & 3) == 1) ? "Push-Verify" : ((((cns >> 2) & 3) == 2) ? "Push-Compare" : "Unknown")));
	//DBG("  STICKYCMP %s\n",((cns & 0x10) != 0) ? "enabled" : "disabled");
	//DBG("  STICKYERR %s\n",((cns & 0x20) != 0) ? "found" : "not found");
	//DBG("  READOK %s\n",((cns & 0x40) != 0) ? "found" : "not found");
	//DBG("  WDATAERR %s\n",((cns & 0x80) != 0) ? "found" : "not found");
	//DBG("  MASKLANE is 0x%x\n",(cns >> 8) & 0xf);
	//DBG("  TRNCNT is %d\n",(cns >> 12) & 0xFFF);
	//DBG("  CDBGRSTREQ %s\n",((cns & 0x4000000) != 0) ? "enabled" : "disabled");
	//DBG("  CDBGRSTACK %s\n",((cns & 0x8000000) != 0) ? "found" : "not found");
	//DBG("  CDBGPWRUPREQ %s\n",((cns & 0x10000000) != 0) ? "enabled" : "disabled");
	//DBG("  CDBGPWRUPACK %s\n",((cns & 0x20000000) != 0) ? "found" : "not found");
	//DBG("  CSYSPWRUPREQ %s\n",((cns & 0x40000000) != 0) ? "enabled" : "disabled");
	//DBG("  CSYSPWRUPACK %s\n",((cns & 0x80000000) != 0) ? "found" : "not found");
}

static unsigned int loop = 0;

extern void usDelay(uint32_t us);
unsigned long swd_connect(unsigned char rst)
{
  unsigned long id = 0;
  unsigned long ack = SWD_RESP_PROTOCAL_ERROR;
#ifdef  CGF_2015_05_23	
			  	
  ack = swd_line_reset(&id);
  if (ack != SWD_RESP_OK) {
    //DBG_ERR("Fail to reset SoC DAP!\n");
	return ack;
  }
#endif
  
  if(rst)
  {
      swd_debug_reset();
      usDelay(10000);
  }
	
  ack = swd_line_reset(&id);
  if (ack != SWD_RESP_OK) {
    //DBG_ERR("Fail to reset SoC DAP!\n");
	return ack;
  }
  //DBG("OK to reset SoC DAP ...\n");

  if (id != SWD_DPID_RN8209) {
    //DBG_ERR("Invalid DAP IDCODE found(fnd:0x%08x,exp:0x%08x)!\n",id,SWD_DPID_RN8209);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to recognize SoC IDCODE as 0x%08x ...\n",id);

  if (
          (swd_dp_abort_set(0x1E) != SWD_RESP_OK) 
	   || (swd_dp_ctrl_n_stat_set(0x50000000) != SWD_RESP_OK) 
	   || (swd_dp_ctrl_n_stat_get(&id) != SWD_RESP_OK) 
	   || (((id & (~0x40)) != 0xF0000000))
	 ) {
    //DBG_ERR("Fail to power up SoC DAP and put it into idle ...\n");
	return SWD_RESP_PROTOCAL_ERROR;
  }

  //DBG("OK to power up SoC DAP and put it into idle ...\n");

  if ((swd_mem_rd32(SOC_CHIP_ID_ADDR,&id,1) != SWD_RESP_OK) || ((id != 0x8211) && (id != 0x8213) && (id != 0x8215) && (id != 0x8219) && (id != 0x8312) && (id != 0x8316))) {
    //DBG_ERR("Fail to identify SoC as RN8219 (fnd:0x%08x,exp:0x%08x) ...\n",id,SOC_CHIP_ID_VALUE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to identify SoC as RN8219 ...\n");

  ack = swd_debug_halt();
  if(ack != SWD_RESP_DEBUG_STATE)
  {
      //DBG_ERR("not in debug state(0x%x)\n", ack);
	   return ack;
  }

  if(rst)
  {
	id = 0x82;
	ack = swd_mem_wr32(0x40034030, &id, 1);
	ack = swd_mem_rd32(0x40034014, &id, 1);
    if((ack != SWD_RESP_OK) || ((id & 0x100) != 0x100))
    {
        //DBG_ERR("read SYS_RST err(0x%x)\n", ack);
	    return SWD_RESP_PROTOCAL_ERROR;
    }
    ack = swd_mem_wr32(0x40034014, &id, 1);
	swd_mem_rd32(0x40034014, &id, 1);
    if(ack != SWD_RESP_OK)
    {
        //DBG_ERR("write SYS_RST err(0x%x)\n", ack);
	    return ack;
    }
  }
  return SWD_RESP_OK;
}

#ifdef  CGF_2015_05_23	  
unsigned long soc_detect(void)
{			
  unsigned long data;

  //SYSCTL read  
  if (swd_mem_rd32((unsigned long)&SYSCTL->OSC_CTRL1,&data,1) != SWD_RESP_OK)
  {
	return 1;
  }			 
  if (swd_mem_rd32((unsigned long)&SYSCTL->SYS_MODE,&data,1) != SWD_RESP_OK)
  {
	return 1;
  }		
  if (swd_mem_rd32((unsigned long)&SYSCTL->OSC_CTRL2,&data,1) != SWD_RESP_OK)
  {
	return 1;
  }	
  if (swd_mem_rd32((unsigned long)&SYSCTL->MAP_CTRL,&data,1) != SWD_RESP_OK)
  {
	return 1;
  }	 
  if (swd_mem_rd32((unsigned long)&SYSCTL->SYS_PD,&data,1) != SWD_RESP_OK)
  {
	return 1;
  }

  return 0;
}
#endif
unsigned long swd_dap_detect(void) {
  unsigned long id = 0;
  unsigned long ack = SWD_RESP_PROTOCAL_ERROR;

  //DBG("Detecting SoC DAP (loop : %d) ...\n",loop);
  ack = swd_line_reset(&id);
  if (ack != SWD_RESP_OK) {
    //DBG_ERR("Fail to reset SoC DAP!\n");
	return ack;
  }
  //DBG("OK to reset SoC DAP ...\n");

  if (id != SWD_DPID_RN8209) {
    //DBG_ERR("Invalid DAP IDCODE found(fnd:0x%08x,exp:0x%08x)!\n",id,SWD_DPID_RN8209);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to recognize SoC IDCODE as 0x%08x ...\n",id);

  ack = swd_dp_wr(SWD_PKT_REQ_WRITE_SELECT, 0x1); //select WCR
  if(ack != SWD_RESP_OK){
		//DBG_ERR("write SELECT reg fail\n");
 	   return SWD_RESP_PROTOCAL_ERROR;
  	}
	
  ack = swd_dp_rd(SWD_PKT_REQ_READ_WCR, &id);
  if(ack != SWD_RESP_OK){
		//DBG_ERR("get WCR reg fail(0x%x)\n", id);
 	   return SWD_RESP_PROTOCAL_ERROR;
  	}
	//DBG("WCR is 0x%x\n", id);

  ack = swd_dp_wr(SWD_PKT_REQ_WRITE_SELECT, 0x0); //select CTRL
  if(ack != SWD_RESP_OK){
		//DBG_ERR("write SELECT reg fail\n");
 	   return SWD_RESP_PROTOCAL_ERROR;
  	}
	
  if (
          (swd_dp_abort_set(0x1E) != SWD_RESP_OK) 
	   || (swd_dp_ctrl_n_stat_set(0x50000000) != SWD_RESP_OK) 
	   || (swd_dp_ctrl_n_stat_get(&id) != SWD_RESP_OK) 
	   || (((id & (~0x40)) != 0xF0000000))
	 ) {
    //DBG_ERR("Fail to power up SoC DAP and put it into idle ...\n");
	return SWD_RESP_PROTOCAL_ERROR;
  }

  //DBG("OK to power up SoC DAP and put it into idle ...\n");
  
  //======================================
  // Check AP configuration information
  //======================================
  if (swd_apacc_init(0,15) != SWD_RESP_OK) {
    //DBG_ERR("Fail to access SoC AP ...\n");
	return SWD_RESP_PROTOCAL_ERROR;
  }

  ack = swd_ap_rd(1,&id);
  if (ack != SWD_RESP_OK) {
    //DBG_ERR("Failure found in SoC AP for endian error (step1) ...\n");
	return SWD_RESP_PROTOCAL_ERROR;
  }

  ack = swd_dp_rd(SWD_PKT_REQ_READ_RDBUFF,&id);	 
  if (ack != SWD_RESP_OK) {
    //DBG_ERR("Failure found in SoC AP for endian error (step2) ...\n");
	return SWD_RESP_PROTOCAL_ERROR;
  }

  if (id != SWD_AP_REG_VALUE_CFG) {
    //DBG_ERR("Failure found in SoC AP for endian error (fnd:0x%08x,exp:0x%08x) ...\n",id,SWD_AP_REG_VALUE_CFG);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to verify SoC endian ...\n");

  ack = swd_ap_rd(2,&id);
  if (ack != SWD_RESP_OK)  {
    //DBG_ERR("Invalid core ROM Table base found (step1) ...\n");
	return SWD_RESP_PROTOCAL_ERROR;
  }

  ack = swd_dp_rd(SWD_PKT_REQ_READ_RDBUFF,&id);
  if (ack != SWD_RESP_OK)  {
    //DBG_ERR("Invalid core ROM Table base found (step2) ...\n");
	return SWD_RESP_PROTOCAL_ERROR;
  }

  if (id != SWD_AP_REG_VALUE_BASE) {
    //DBG_ERR("Invalid core ROM Table base found (fnd:0x%08x,exp:0x%08x) ...\n",id,SWD_AP_REG_VALUE_BASE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to verify SoC ROM Table base ...\n");

  ack = swd_ap_rd(3,&id);
  if (ack != SWD_RESP_OK) {
    //DBG_ERR("Invalid SoC AP identify code (step1) ...\n");
	return SWD_RESP_PROTOCAL_ERROR;
  }

  ack = swd_dp_rd(SWD_PKT_REQ_READ_RDBUFF,&id); 
  if (ack != SWD_RESP_OK) {
    //DBG_ERR("Invalid SoC AP identify code (step2) ...\n");
	return SWD_RESP_PROTOCAL_ERROR;
  }

  if (id != SWD_AP_REG_VALUE_IDR) {
    //DBG_ERR("Invalid SoC AP identify code (fnd:0x%08x,exp:0x%08x) ...\n",id,SWD_AP_REG_VALUE_IDR);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to verify SoC AP identify code ...\n");
  //======================================
  // End of Check AP configuration information
  //======================================
  
  //======================================
  // Check ROM-TABLE configuration information
  //======================================
#ifndef CGF
  if ((swd_mem_rd32(SOC_CHIP_ID_ADDR,&id,1) != SWD_RESP_OK) || (id != SOC_CHIP_ID_VALUE)) {
#else
  if ((swd_mem_rd32(SOC_CHIP_ID_ADDR,&id,1) != SWD_RESP_OK) || ((id != 0x8211) && (id != 0x8213) && (id != 0x8215) && (id != 0x8219) && (id != 0x8312) && (id != 0x8316))) {
#endif
    //DBG_ERR("Fail to identify SoC as RN8219 (fnd:0x%08x,exp:0x%08x) ...\n",id,SOC_CHIP_ID_VALUE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to identify SoC as RN8219 ...\n");

  if ((swd_mem_rd32(SOC_ROM_TABLE_ENTRY0_ADDR,&id,1) != SWD_RESP_OK) || (id != SOC_ROM_TABLE_ENTRY0_VALUE)) {
    //DBG_ERR("Fail to recognize SoC RT Entry0 (fnd:0x%08x,exp:0x%08x) ...\n",id,SOC_ROM_TABLE_ENTRY0_VALUE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to recognize SoC RT Entry0 ...\n");

  if ((swd_mem_rd32(SOC_ROM_TABLE_ENTRY1_ADDR,&id,1) != SWD_RESP_OK) || (id != SOC_ROM_TABLE_ENTRY1_VALUE)) {
    //DBG_ERR("Fail to recognize SoC RT Entry1 (fnd:0x%08x,exp:0x%08x) ...\n",id,SOC_ROM_TABLE_ENTRY1_VALUE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to recognize SoC RT Entry1 ...\n");

  if ((swd_mem_rd32(SOC_ROM_TABLE_ENTRY2_ADDR,&id,1) != SWD_RESP_OK) || (id != SOC_ROM_TABLE_ENTRY2_VALUE)) {
    //DBG_ERR("Fail to recognize SoC RT Entry2 (fnd:0x%08x,exp:0x%08x) ...\n",id,SOC_ROM_TABLE_ENTRY2_VALUE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to recognize SoC RT Entry2 ...\n");

  if ((swd_mem_rd32(SOC_ROM_TABLE_ENTRY3_ADDR,&id,1) != SWD_RESP_OK) || (id != SOC_ROM_TABLE_ENTRY3_VALUE)) {
    //DBG_ERR("Fail to recognize SoC RT Entry3 (fnd:0x%08x,exp:0x%08x) ...\n",id,SOC_ROM_TABLE_ENTRY3_VALUE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to recognize SoC RT Entry3 ...\n");

  if ((swd_mem_rd32(SOC_ROM_TABLE_PID0_ADDR,&id,1) != SWD_RESP_OK) || (id != SOC_ROM_TABLE_PID0_VALUE)) {
    //DBG_ERR("Fail to recognize SoC RT PID0 (fnd:0x%08x,exp:0x%08x) ...\n",id,SOC_ROM_TABLE_PID0_VALUE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to recognize SoC RT PID0 ...\n");

  if ((swd_mem_rd32(SOC_ROM_TABLE_PID1_ADDR,&id,1) != SWD_RESP_OK) || (id != SOC_ROM_TABLE_PID1_VALUE)) {
    //DBG_ERR("Fail to recognize SoC RT PID1 (fnd:0x%08x,exp:0x%08x) ...\n",id,SOC_ROM_TABLE_PID1_VALUE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to recognize SoC RT PID1 ...\n");

  if ((swd_mem_rd32(SOC_ROM_TABLE_PID2_ADDR,&id,1) != SWD_RESP_OK) || (id != SOC_ROM_TABLE_PID2_VALUE)) {
    //DBG_ERR("Fail to recognize SoC RT PID2 (fnd:0x%08x,exp:0x%08x) ...\n",id,SOC_ROM_TABLE_PID2_VALUE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to recognize SoC RT PID2 ...\n");

  if ((swd_mem_rd32(SOC_ROM_TABLE_PID3_ADDR,&id,1) != SWD_RESP_OK) || (id != SOC_ROM_TABLE_PID3_VALUE)) {
    //DBG_ERR("Fail to recognize SoC RT PID3 (fnd:0x%08x,exp:0x%08x) ...\n",id,SOC_ROM_TABLE_PID3_VALUE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to recognize SoC RT PID3 ...\n");

  if ((swd_mem_rd32(SOC_ROM_TABLE_PID4_ADDR,&id,1) != SWD_RESP_OK) || (id != SOC_ROM_TABLE_PID4_VALUE)) {
    //DBG_ERR("Fail to recognize SoC RT PID4 (fnd:0x%08x,exp:0x%08x) ...\n",id,SOC_ROM_TABLE_PID4_VALUE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to recognize SoC RT PID4 ...\n");

  if ((swd_mem_rd32(SOC_ROM_TABLE_PID5_ADDR,&id,1) != SWD_RESP_OK) || (id != SOC_ROM_TABLE_PID5_VALUE)) {
    //DBG_ERR("Fail to recognize SoC RT PID5 (fnd:0x%08x,exp:0x%08x) ...\n",id,SOC_ROM_TABLE_PID5_VALUE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to recognize SoC RT PID5 ...\n");

  if ((swd_mem_rd32(SOC_ROM_TABLE_PID6_ADDR,&id,1) != SWD_RESP_OK) || (id != SOC_ROM_TABLE_PID6_VALUE)) {
    //DBG_ERR("Fail to recognize SoC RT PID6 (fnd:0x%08x,exp:0x%08x) ...\n",id,SOC_ROM_TABLE_PID6_VALUE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to recognize SoC RT PID6 ...\n");

  if ((swd_mem_rd32(SOC_ROM_TABLE_PID7_ADDR,&id,1) != SWD_RESP_OK) || (id != SOC_ROM_TABLE_PID7_VALUE)) {
    //DBG_ERR("Fail to recognize SoC RT PID7 (fnd:0x%08x,exp:0x%08x) ...\n",id,SOC_ROM_TABLE_PID7_VALUE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to recognize SoC RT PID7 ...\n");

  if ((swd_mem_rd32(SOC_ROM_TABLE_CID0_ADDR,&id,1) != SWD_RESP_OK) || (id != SOC_ROM_TABLE_CID0_VALUE)) {
    //DBG_ERR("Fail to recognize SoC RT CID0 (fnd:0x%08x,exp:0x%08x) ...\n",id,SOC_ROM_TABLE_CID0_VALUE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to recognize SoC RT CID0 ...\n");

  if ((swd_mem_rd32(SOC_ROM_TABLE_CID1_ADDR,&id,1) != SWD_RESP_OK) || (id != SOC_ROM_TABLE_CID1_VALUE)) {
    //DBG_ERR("Fail to recognize SoC RT CID1 (fnd:0x%08x,exp:0x%08x) ...\n",id,SOC_ROM_TABLE_CID1_VALUE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to recognize SoC RT CID1 ...\n");

  if ((swd_mem_rd32(SOC_ROM_TABLE_CID2_ADDR,&id,1) != SWD_RESP_OK) || (id != SOC_ROM_TABLE_CID2_VALUE)) {
    //DBG_ERR("Fail to recognize SoC RT CID2 (fnd:0x%08x,exp:0x%08x) ...\n",id,SOC_ROM_TABLE_CID2_VALUE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to recognize SoC RT CID2 ...\n");

  if ((swd_mem_rd32(SOC_ROM_TABLE_CID3_ADDR,&id,1) != SWD_RESP_OK) || (id != SOC_ROM_TABLE_CID3_VALUE)) {
    //DBG_ERR("Fail to recognize SoC RT CID3 (fnd:0x%08x,exp:0x%08x) ...\n",id,SOC_ROM_TABLE_CID3_VALUE);
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to recognize SoC RT CID3 ...\n");

  //DBG("OK to recognize SoC process core as Cortex-M0 ...\n");

  ack = swd_dwt_numcomp_get(&id);
  if ((ack != SWD_RESP_OK) || (id != 2)) {
    //DBG_ERR("Fail to locate 2 DWT in SoC ...\n");
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to locate all 2 DWT in SoC ...\n");

  ack = swd_bpu_numcode_get(&id);
  if ((ack != SWD_RESP_OK) || (id != 4)) {
    //DBG_ERR("Fail to locate 4 BPU in SoC ...\n");
	return SWD_RESP_PROTOCAL_ERROR;
  }
  //DBG("OK to locate all 4 BPU in SoC ...\n");
  //======================================
  // End of Check ROM-TABLE configuration information
  //======================================

  //DBG("Checking SoC states : \n");
	ack = swd_debug_halt();
  if (ack == SWD_RESP_DEBUG_STATE) {
    //DBG("  in DEBUG state \n");
  } else if (ack == SWD_RESP_NOT_DEBUG_STATE) {
    //DBG("  not in DEBUG state \n");
  } else {
    //DBG_ERR("  unknown DEBUG state \n");
  }
  
	
	ack = swd_debug_state_chk();
  if (ack == SWD_RESP_DEBUG_STATE) {
    //DBG("  in DEBUG state \n");
  } else if (ack == SWD_RESP_NOT_DEBUG_STATE) {
    //DBG("  not in DEBUG state \n");
  } else {
    //DBG_ERR("  unknown DEBUG state \n");
  }

  ack = swd_sleep_chk();
  if (ack == SWD_RESP_SLEEP) {
    //DBG("  in SLEEP state \n");
  } else if (ack == SWD_RESP_NOT_SLEEP) {
    //DBG("  not in SLEEP state \n");
  } else {
    //DBG_ERR("  unknown SLEEP state \n");
  }

  ack = swd_lockup_chk();
  if (ack == SWD_RESP_LOCKUP) {
    //DBG("  in LOCKUP state \n");
  } else if (ack == SWD_RESP_NOT_LOCKUP) {
    //DBG("  not in LOCKUP state \n");
  } else {
    //DBG_ERR("  unknown LOCKUP state \n");
  }

  ack = swd_retire_st_chk();
  if (ack == SWD_RESP_RETIRE_ST) {
    //DBG("  at least one instruction has completed since last access \n");
  } else if (ack == SWD_RESP_NOT_RETIRE_ST) {
    //DBG("  no instruction has completed since last access \n");
  } else {
    //DBG_ERR("  unknown instruction has completed since last access\n");
  }

  ack = swd_reset_st_chk();
  if (ack == SWD_RESP_RESET_ST) {
    //DBG("  at least one reset since last access \n");
  } else if (ack == SWD_RESP_NOT_RESET_ST) {
    //DBG("  no reset since last access \n");
  } else {
    //DBG_ERR("  unknown reset since last access\n");
  }

  ack = swd_halted_chk();
  if (ack == SWD_RESP_HALTED) {
    //DBG("  in halted state \n");
  } else if (ack == SWD_RESP_NOT_HALTED) {
    //DBG("  not in halted state \n");
  } else {
    //DBG_ERR("  unknown halted state\n");
  }

  ack = swd_watch_chk();
  if (ack == SWD_RESP_WATCH) {
    //DBG("  watch event found \n");
  } else if (ack == SWD_RESP_NOT_WATCH) {
    //DBG("  no watch event found \n");
  } else {
    //DBG_ERR("  unknown watch event\n");
  }

  ack = swd_break_chk();
  if (ack == SWD_RESP_BREAK) {
    //DBG("  break event found \n");
  } else if (ack == SWD_RESP_NOT_BREAK) {
    //DBG("  no break event found \n");
  } else {
    //DBG_ERR("  unknown break event\n");
  }

  ack = swd_vector_capture_chk();
  if (ack == SWD_RESP_VECTOR_CAPTURE) {
    //DBG("  vectore capture event found \n");
  } else if (ack == SWD_RESP_NOT_VECTOR_CAPTURE) {
    //DBG("  no vectore capture event found \n");
  } else {
    //DBG_ERR("  unknown vectore capture event\n");
  }

  ack = swd_ext_dbg_req_chk();
  if (ack == SWD_RESP_EXT_DBG_REQ) {
    //DBG("  external debug request event found \n");
  } else if (ack == SWD_RESP_NOT_EXT_DBG_REQ) {
    //DBG("  no external debug request event found \n");
  } else {
    //DBG_ERR("  unknown external debug request event\n");
  }

  ack = swd_hardfault_vc_en_chk();
  if (ack == SWD_RESP_HARDFAULT_VC_EN) {
    //DBG("  hardfault vectore capture enabled\n");
  } else if (ack == SWD_RESP_NOT_HARDFAULT_VC_EN) {
    //DBG("  hardfault vectore capture disabled\n");
  } else {
    //DBG_ERR("  unknown hardfault vectore capture state\n");
  }

  ack = swd_reset_vc_en_chk();
  if (ack == SWD_RESP_RESET_VC_EN) {
    //DBG("  reset vectore capture enabled\n");
  } else if (ack == SWD_RESP_NOT_RESET_VC_EN) {
    //DBG("  reset vectore capture disabled\n");
  } else {
    //DBG_ERR("  unknown reset vectore capture state\n");
  }

  ack = swd_dwt_en_chk();
  if (ack == SWD_RESP_DWT_EN) {
    //DBG("  dwt enabled\n");
  } else if (ack == SWD_RESP_NOT_DWT_EN) {
    //DBG("  dwt disabled\n");
  } else {
    //DBG_ERR("  unknown dwt state\n");
  }

  ack = swd_bpu_en_chk();
  if (ack == SWD_RESP_BPU_EN) {
    //DBG("  bpu enabled\n");
  } else if (ack == SWD_RESP_NOT_BPU_EN) {
    //DBG("  bpu disabled\n");
  } else {
    //DBG_ERR("  unknown bpu state\n");
  }

#if 0
// Watch & Break demo
  {
    s_swd_dwt dwt0,dwt1;
	ack = swd_mem_rd32(CM0_DBG_DWT_REG_CTRL_ADDR,&id,1);
	//tc_printf("ctrl of dwt is 0x%08x\n",id);
	ack = swd_dwt_get(0,&dwt0);
	ack = swd_dwt_get(1,&dwt1);
	//tc_printf("0x%08x,0x%08x,0x%08x\n",dwt0.comp,dwt0.mask,dwt0.func);
	//tc_printf("0x%08x,0x%08x,0x%08x\n",dwt1.comp,dwt1.mask,dwt1.func);
	dwt0.comp = 0x55aa55aa;dwt0.mask=0x5;dwt0.func=5;
	dwt1.comp = 0x08000000;dwt1.mask=0xa;dwt1.func=4;
	ack = swd_dwt_set(0,&dwt0);
	ack = swd_dwt_set(1,&dwt1);
	dwt0.comp = 0;dwt0.mask=0;dwt0.func=0;
	dwt1.comp = 0;dwt1.mask=0;dwt1.func=0;
	ack = swd_dwt_get(0,&dwt0);
	ack = swd_dwt_get(1,&dwt1);
	//tc_printf("0x%08x,0x%08x,0x%08x\n",dwt0.comp,dwt0.mask,dwt0.func);
	//tc_printf("0x%08x,0x%08x,0x%08x\n",dwt1.comp,dwt1.mask,dwt1.func);
	dwt0.comp = 0x55aa55aa;dwt0.mask=0x5;dwt1.func=5;
	dwt1.comp = 0x08000000;dwt1.mask=0xa;dwt1.func=4;
	ack = swd_dwt_set(0,&dwt1);
	ack = swd_dwt_set(1,&dwt0);
	dwt0.comp = 0;dwt0.mask=0;dwt0.func=0;
	dwt1.comp = 0;dwt1.mask=0;dwt1.func=0;
	ack = swd_dwt_get(0,&dwt0);
	ack = swd_dwt_get(1,&dwt1);
	//tc_printf("0x%08x,0x%08x,0x%08x\n",dwt0.comp,dwt0.mask,dwt0.func);
	//tc_printf("0x%08x,0x%08x,0x%08x\n",dwt1.comp,dwt1.mask,dwt1.func);
	ack = swd_dwt_eia_sample_get(&id);
	//tc_printf("EIA sample is 0x%08x\n",id);
	ack = swd_dwt_eia_sample_get(&id);
	//tc_printf("EIA sample is 0x%08x\n",id);
    ack = swd_mem_rd32(CM0_DBG_BPU_REG_CTRL_ADDR,&id,1);
	//tc_printf("ctrl of bpu is 0x%08x\n",id);
    ack = swd_bpu_get(0,&id);
    //tc_printf("0x%08x\n",id);
	id = 0x5a5a5a5a;
    ack = swd_bpu_set(0,id);
	id = 0;
    ack = swd_bpu_get(0,&id);
    //tc_printf("0x%08x\n",id);
    ack = swd_bpu_get(1,&id);
    //tc_printf("0x%08x\n",id);
	id = 0xa5a5a5a5;
    ack = swd_bpu_set(1,id);
	id = 0;
    ack = swd_bpu_get(1,&id);
    //tc_printf("0x%08x\n",id);
    ack = swd_bpu_get(2,&id);
    //tc_printf("0x%08x\n",id);
	id = 0x55aa55aa;
    ack = swd_bpu_set(2,id);
	id = 0;
    ack = swd_bpu_get(2,&id);
    //tc_printf("0x%08x\n",id);
    ack = swd_bpu_get(3,&id);
    //tc_printf("0x%08x\n",id);
	id = 0xaa55aa55;
    ack = swd_bpu_set(3,id);
	id = 0;
    ack = swd_bpu_get(3,&id);
    //tc_printf("0x%08x\n",id);
  }
#endif
#if 0
// following procedure is intrusive debug session 
// for demo only
  ack = swd_debug_halt();
  ack = swd_halted_chk();
  {
    unsigned long i = 0;
	for (i = 0; i < 13; i ++) {
      ack = swd_core_reg_rd(i,&id);
	  //tc_printf("R%d = 0x%08x\n",i,id);
	}
    ack = swd_core_reg_rd(i++,&id);
    //tc_printf("SP = 0x%08x\n",id);
    ack = swd_core_reg_rd(i++,&id);
    //tc_printf("LR = 0x%08x\n",id);
    ack = swd_core_reg_rd(i++,&id);
    //tc_printf("PC = 0x%08x\n",id);
    ack = swd_core_reg_rd(i++,&id);
    //tc_printf("XPSR = 0x%08x\n",id);
    ack = swd_core_reg_rd(i++,&id);
    //tc_printf("MSP = 0x%08x\n",id);
    ack = swd_core_reg_rd(i++,&id);
    //tc_printf("PSP = 0x%08x\n",id);
	i++;
    ack = swd_core_reg_rd(i++,&id);
    //tc_printf("PRIMASK = 0x%08x\n",id);

	ack = swd_mem_rd32(0,&id,1);
    //tc_printf("Setting SP as 0x%08x\n",id);
	ack = swd_core_reg_wr(CM0_DBG_REGSEL_SP,id);
	ack = swd_core_reg_rd(CM0_DBG_REGSEL_SP,&id);
    //tc_printf("SP = 0x%08x\n",id);
	ack = swd_mem_rd32(4,&id,1);
    //tc_printf("Setting PC as 0x%08x\n",id);
	ack = swd_core_reg_wr(CM0_DBG_REGSEL_PC,id);
	ack = swd_core_reg_rd(CM0_DBG_REGSEL_PC,&id);
    //tc_printf("PC = 0x%08x\n",id);
  }
  ack = swd_debug_disable();
  //tc_printf("Halting SoC ...\n");
  ack = swd_debug_halt();
  ack = swd_halted_chk();
  {
    unsigned long i = 0;
	for (i = 0; i < 13; i ++) {
      ack = swd_core_reg_rd(i,&id);
	  //tc_printf("R%d = 0x%08x\n",i,id);
	}
    ack = swd_core_reg_rd(i++,&id);
    //tc_printf("SP = 0x%08x\n",id);
    ack = swd_core_reg_rd(i++,&id);
    //tc_printf("LR = 0x%08x\n",id);
    ack = swd_core_reg_rd(i++,&id);
    //tc_printf("PC = 0x%08x\n",id);
    ack = swd_core_reg_rd(i++,&id);
    //tc_printf("XPSR = 0x%08x\n",id);
    ack = swd_core_reg_rd(i++,&id);
    //tc_printf("MSP = 0x%08x\n",id);
    ack = swd_core_reg_rd(i++,&id);
    //tc_printf("PSP = 0x%08x\n",id);
	i++;
    ack = swd_core_reg_rd(i++,&id);
    //tc_printf("PRIMASK = 0x%08x\n",id);   
  }
  ack = swd_debug_disable();
#endif

  loop ++;
  return SWD_RESP_OK;									  
}

//write DP-SELECT register, set AP select field(aps) and its bank address(rbn)
unsigned long swd_apacc_init(unsigned long aps,unsigned long rbn) {
  unsigned long ack = 0;
  unsigned long cmd = 0;

  cmd = ((aps << 24) & 0xFF000000) | ((rbn << 4) & 0xF0);
  ack = swd_dp_select_set(cmd);

  return ack;
}

//AP read:
//  1. Write DP-SELECT register to select AP and its bank address
//  2. Send CMD to initialize AP access(REG) and get read value
unsigned long swd_ap_rd(unsigned long addr,unsigned long * pRdata) {
  unsigned long ack = 0;
  unsigned long cmd = 0;
  unsigned long i   = 0;

  cmd = 7 | ((addr << 3) & 0x18);  //packet format:|start bit|APnDP|RnW|A[2:3]|Parity|Stop|
  do {
    ack = swd_dp_rd(cmd,pRdata);
	if (ack != SWD_RESP_WAIT)
	  i ++;
  } while((ack != SWD_RESP_OK) && (i < SWD_BUS_MAX_RETRY));
#ifndef CGF
  swd_pkt_kickoff();swd_pkt_kickoff();
#endif
  return ack;
}

#ifdef CGF
unsigned long swd_ap_rd_mem(unsigned long addr,unsigned long * pRdata) {
  unsigned long ack = 0;
  unsigned long cmd = 0;
  unsigned long i   = 0;

  cmd = 7 | ((addr << 3) & 0x18);  //packet format:|start bit|APnDP|RnW|A[2:3]|Parity|Stop|
  do {
    ack = swd_dp_rd_mem(cmd,pRdata);
	if (ack != SWD_RESP_WAIT)
	  i ++;
  } while((ack != SWD_RESP_OK) && (i < SWD_BUS_MAX_RETRY));
#ifndef CGF
  swd_pkt_kickoff();swd_pkt_kickoff();
#endif
  return ack;
}
#endif

unsigned long swd_ap_wr(unsigned long addr,unsigned long wdata) {
  unsigned long ack = 0;
  unsigned long i   = 0;
  unsigned long cmd = 0;

  cmd = 3 | ((addr << 3) & 0x18);
  do {
    ack = swd_dp_wr(cmd,wdata);
	if (ack != SWD_RESP_WAIT)
	  i ++;
  } while((ack != SWD_RESP_OK) && (i < SWD_BUS_MAX_RETRY));

  return ack;
}

#ifdef CGF
unsigned long swd_ap_wr_mem(unsigned long addr,unsigned long wdata) {
  unsigned long ack = 0;
  unsigned long i   = 0;
  unsigned long cmd = 0;

  cmd = 3 | ((addr << 3) & 0x18);
  do {
    ack = swd_dp_wr_mem(cmd,wdata);
	if (ack != SWD_RESP_WAIT)
	  i ++;
  } while((ack != SWD_RESP_OK) && (i < SWD_BUS_MAX_RETRY));

  return ack;
}
#endif

unsigned long swd_mem_rd(unsigned long addr,unsigned long * pBuf,unsigned long len,unsigned long size) 
{
  unsigned long ack = SWD_RESP_OK;
  unsigned long swd_ap_pkt_size = MAX_SWD_AP_PKT_SIZE;
  unsigned long i = 0;
  unsigned long td = 0;

  i = swd_ap_pkt_size - (addr & (swd_ap_pkt_size - 1));
  if (i < swd_ap_pkt_size) {
    td = i >> (size & 3);
    td = (td >= len) ? len : td;
    if (i != 0) {
      ack = swd_mem_rd_1k(addr,pBuf,td,size);
	  if (ack != SWD_RESP_OK)
	    return ack;
    }
    if (td >= len)
      return ack;
    addr += i;
    pBuf += (i >> (size & 3));
    len  -= (i >> (size & 3));
  }

  td = len & (~((swd_ap_pkt_size >> (size & 3)) - 1));
  for (i = 0; i < td; i += (swd_ap_pkt_size >> (size &3))) {
    ack = swd_mem_rd_1k(addr,pBuf,swd_ap_pkt_size >> (size & 3),size);
	if (ack != SWD_RESP_OK)
	  return ack;
	addr += swd_ap_pkt_size;
	pBuf += (swd_ap_pkt_size >> (size & 3));
	len  -= (swd_ap_pkt_size >> (size & 3));
  }

  if (len != 0) {
    ack = swd_mem_rd_1k(addr,pBuf,len & ((swd_ap_pkt_size >> (size & 3)) - 1),size);
  }

  return ack;
}

#ifdef CGF
unsigned long swd_mem_verf(unsigned long addr,unsigned long * pBuf,unsigned long len,unsigned long size) {
  unsigned long ack = SWD_RESP_OK;
  unsigned long swd_ap_pkt_size = MAX_SWD_AP_PKT_SIZE;
  unsigned long i = 0;
  unsigned long td = 0;

  i = swd_ap_pkt_size - (addr & (swd_ap_pkt_size - 1));
  if (i < swd_ap_pkt_size) {
    td = i >> (size & 3);
    td = (td >= len) ? len : td;
    if (i != 0) {
      ack = swd_mem_verf_1k(addr,pBuf,td,size);
	  if (ack != SWD_RESP_OK)
	    return ack;
    }
    if (td >= len)
      return ack;
    addr += i;
    pBuf += (i >> (size & 3));
    len  -= (i >> (size & 3));
  }

  td = len & (~((swd_ap_pkt_size >> (size & 3)) - 1));
  for (i = 0; i < td; i += (swd_ap_pkt_size >> (size &3))) {
    ack = swd_mem_verf_1k(addr,pBuf,swd_ap_pkt_size >> (size & 3),size);
	if (ack != SWD_RESP_OK)
	  return ack;
	addr += swd_ap_pkt_size;
	pBuf += (swd_ap_pkt_size >> (size & 3));
	len  -= (swd_ap_pkt_size >> (size & 3));
  }

  if (len != 0) {
    ack = swd_mem_verf_1k(addr,pBuf,len & ((swd_ap_pkt_size >> (size & 3)) - 1),size);
  }

  return ack;
}
#endif

unsigned long swd_mem_wr(unsigned long addr,unsigned long * pBuf,unsigned long len,unsigned long size) {
  unsigned long ack = SWD_RESP_OK;
  unsigned long swd_ap_pkt_size = MAX_SWD_AP_PKT_SIZE;
  unsigned long i = 0;
  unsigned long td = 0;

  i = swd_ap_pkt_size - (addr & (swd_ap_pkt_size - 1));
  if (i < swd_ap_pkt_size) {
    td = i >> (size & 3);
    td = (td >= len) ? len : td;
    if (i != 0) {
      ack = swd_mem_wr_1k(addr,pBuf,td,size);
	  if (ack != SWD_RESP_OK)
	    return ack;
    }
    if (td >= len)
      return ack;
    addr += i;
    pBuf += (i >> (size & 3));
    len  -= (i >> (size & 3));
  }

  td = len & (~((swd_ap_pkt_size >> (size & 3)) - 1));
  for (i = 0; i < td; i += (swd_ap_pkt_size >> (size & 3))) {
    ack = swd_mem_wr_1k(addr,pBuf,swd_ap_pkt_size >> (size&3),size);
	if (ack != SWD_RESP_OK)
	  return ack;
	addr += swd_ap_pkt_size;
	pBuf += (swd_ap_pkt_size >> (size & 3));
	len  -= (swd_ap_pkt_size >> (size & 3));
  }

  if (len != 0) {
    ack = swd_mem_wr_1k(addr,pBuf,len & ((swd_ap_pkt_size >> (size & 3)) - 1),size);
  }

  return ack;
}

#ifdef CGF
unsigned long swd_mem_wr_mem(unsigned long addr,unsigned long * pBuf,unsigned long len,unsigned long size) {
  unsigned long ack = SWD_RESP_OK;
  unsigned long swd_ap_pkt_size = MAX_SWD_AP_PKT_SIZE;
  unsigned long i = 0;
  unsigned long td = 0;

  i = swd_ap_pkt_size - (addr & (swd_ap_pkt_size - 1));
  if (i < swd_ap_pkt_size) {
    td = i >> (size & 3);
    td = (td >= len) ? len : td;
    if (i != 0) {
      ack = swd_mem_wr_1k_mem(addr,pBuf,td,size);
	  if (ack != SWD_RESP_OK)
	    return ack;
    }
    if (td >= len)
      return ack;
    addr += i;
    pBuf += (i >> (size & 3));
    len  -= (i >> (size & 3));
  }

  td = len & (~((swd_ap_pkt_size >> (size & 3)) - 1));
  for (i = 0; i < td; i += (swd_ap_pkt_size >> (size & 3))) {
    ack = swd_mem_wr_1k_mem(addr,pBuf,swd_ap_pkt_size >> (size&3),size);
	if (ack != SWD_RESP_OK)
	  return ack;
	addr += swd_ap_pkt_size;
	pBuf += (swd_ap_pkt_size >> (size & 3));
	len  -= (swd_ap_pkt_size >> (size & 3));
  }

  if (len != 0) {
    ack = swd_mem_wr_1k_mem(addr,pBuf,len & ((swd_ap_pkt_size >> (size & 3)) - 1),size);
  }

  return ack;
}
#endif

//Memory read:
//  1. Read AP-CSW(Write DP-Select; Read AP) check if AP is busy
//  2. Write AP-CSW to enable AP(SW debug, AP enable)
//  3. Write AP-TAR to set AP memory address
//  4. Read AP-DRW to get data from AP memory looply.
// PS: max. length of read data is: TAR ~ address[31:10] | 0x3ff (1K)
unsigned long swd_mem_rd_1k(unsigned long addr,unsigned long * pBuf,unsigned long len,unsigned long size) {
  unsigned long ack = 0;
  unsigned long i = 0;
  unsigned long td = 0;

  if (len == 0)
    return SWD_RESP_OK;
  
  if ((((addr & 3) != 0) && (size == 2)) || (((addr & 1) != 0) && (size == 1)) || (size > 2)) {
    return SWD_RESP_PROTOCAL_ERROR;
  }

  if (swd_apacc_init(0,0) != SWD_RESP_OK) {
    return SWD_RESP_PROTOCAL_ERROR;
  }

  do {
    ack = swd_ap_rd(0,&td);
	ack = swd_dp_rd(SWD_PKT_REQ_READ_RDBUFF,&td);
	if ((ack == SWD_RESP_WAIT) || (ack == SWD_RESP_FAULT))
	  i ++;
  }while(((ack != SWD_RESP_OK) || ((td & 0x80) != 0)) && (i < SWD_BUS_MAX_RETRY));
  if (i >= SWD_BUS_MAX_RETRY) {
    return SWD_RESP_PROTOCAL_ERROR;
  }

  td = 0x80000040 | (size & 0x3);
  if (len > 1) 
    td |= 0x10;

  {
    ack = swd_ap_wr(0,td);
	if (ack != SWD_RESP_OK) {
      return SWD_RESP_PROTOCAL_ERROR;
	}
    ack = swd_ap_wr(1,addr);
	if (ack != SWD_RESP_OK) {
      return SWD_RESP_PROTOCAL_ERROR;
	}
    ack = swd_ap_rd(3,&td);
	if (ack != SWD_RESP_OK) {
      return SWD_RESP_PROTOCAL_ERROR;
	}
  }
  for (i = 0; i < len; i ++) {
    ack = swd_ap_rd(3,&td);
    if (ack != SWD_RESP_OK) {
	  return SWD_RESP_PROTOCAL_ERROR;
	} else {
	  if (size == 2) {
	    pBuf[i] = td;
	  } else if (size == 1) {
	    ack =  ((((addr >> 1) + i) & 1) << 4);
	    ((unsigned short *)pBuf)[i] = (td >> ack) & 0xFFFF;
	  } else {	    
	  	ack =  ((addr + i) & 3) << 3;
	    ((unsigned char *)pBuf)[i] = (td >> ack) & 0xFF;
	  }
	}
  } 

  return SWD_RESP_OK;
//  return swd_ap_wr(0,0x80000002);
}

#ifdef CGF
unsigned long swd_mem_verf_1k(unsigned long addr,unsigned long * pBuf,unsigned long len,unsigned long size) {
  unsigned long ack = 0;
  unsigned long i = 0;
  unsigned long td = 0;

  if (len == 0)
    return SWD_RESP_OK;
  
  if ((((addr & 3) != 0) && (size == 2)) || (((addr & 1) != 0) && (size == 1)) || (size > 2)) {
    return SWD_RESP_PROTOCAL_ERROR;
  }

  if (swd_apacc_init(0,0) != SWD_RESP_OK) {
    return SWD_RESP_PROTOCAL_ERROR;
  }

  do {
    ack = swd_ap_rd(0,&td);
	ack = swd_dp_rd(SWD_PKT_REQ_READ_RDBUFF,&td);
	if ((ack == SWD_RESP_WAIT) || (ack == SWD_RESP_FAULT))
	  i ++;
  }while(((ack != SWD_RESP_OK) || ((td & 0x80) != 0)) && (i < SWD_BUS_MAX_RETRY));
  if (i >= SWD_BUS_MAX_RETRY) {
    return SWD_RESP_PROTOCAL_ERROR;
  }

  td = 0x80000040 | (size & 0x3);
  if (len > 1) 
    td |= 0x10;

  {
    ack = swd_ap_wr(0,td);
	if (ack != SWD_RESP_OK) {
      return SWD_RESP_PROTOCAL_ERROR;
	}
    ack = swd_ap_wr(1,addr);
	if (ack != SWD_RESP_OK) {
      return SWD_RESP_PROTOCAL_ERROR;
	}
    ack = swd_ap_rd(3,&td);
	if (ack != SWD_RESP_OK) {
      return SWD_RESP_PROTOCAL_ERROR;
	}
  }
  for (i = 0; i < len; i ++) {
    ack = swd_ap_rd(3, &td);
    if (ack != SWD_RESP_OK) {
	  return SWD_RESP_PROTOCAL_ERROR;
	} else {
	  if (size == 2) {
	    if(pBuf[i] != td)
	    {
	    	return SWD_RESP_NOT_MATCH;
	    }
	  } else if (size == 1) {
	    ack =  ((((addr >> 1) + i) & 1) << 4);
	    if(((unsigned short *)pBuf)[i] != ((td >> ack) & 0xFFFF))
	    {
	    	return SWD_RESP_NOT_MATCH;
	    }
	  } else {	    
	  	ack =  ((addr + i) & 3) << 3;
	    if(((unsigned char *)pBuf)[i] != ((td >> ack) & 0xFF))
	    {
	    	return SWD_RESP_NOT_MATCH;
	    }
	  }
	}
  } 

  return SWD_RESP_OK;
//  return swd_ap_wr(0,0x80000002);
}
#endif

unsigned long swd_mem_wr_1k(unsigned long addr,unsigned long * pBuf,unsigned long len,unsigned long size) {
  unsigned long ack = 0;
  unsigned long i = 0;
  unsigned long td = 0;
	
  if (len == 0)
    return SWD_RESP_OK;
  
  if ((((addr & 3) != 0) && (size == 2)) || (((addr & 1) != 0) && (size == 1)) || (size > 2)) {
    return SWD_RESP_PROTOCAL_ERROR;
  }

  if (swd_apacc_init(0,0) != SWD_RESP_OK) {
    return SWD_RESP_PROTOCAL_ERROR;
  }

  do {
    ack = swd_ap_rd(0,&td);
	ack = swd_dp_rd(SWD_PKT_REQ_READ_RDBUFF,&td);
	if ((ack == SWD_RESP_WAIT) || (ack == SWD_RESP_FAULT))
	  i ++;
  }while(((ack != SWD_RESP_OK) || ((td & 0x80) != 0)) && (i < SWD_BUS_MAX_RETRY));
  if (i >= SWD_BUS_MAX_RETRY) {
    return SWD_RESP_PROTOCAL_ERROR;
  }

  td = 0x80000040 | (size & 0x3);
  if (len > 1) 
    td |= 0x10;

  {
    ack = swd_ap_wr(0,td);
	if (ack != SWD_RESP_OK) {
      return SWD_RESP_PROTOCAL_ERROR;
	}
    ack = swd_ap_wr(1,addr);
	if (ack != SWD_RESP_OK) {
      return SWD_RESP_PROTOCAL_ERROR;
	}
  }
  for (i = 0; i < len; i ++) {
	if (size == 2) {
	  td = pBuf[i];
	} else if (size == 1) {	    
	  ack = ((((addr >> 1) + i) & 1) << 4);
	  td  = (((unsigned short *)pBuf)[i] & 0xFFFF) << ack;
	} else {	    
	  ack =  ((addr + i) & 3) << 3;
#ifndef CGF
	  td  = (((unsigned short *)pBuf)[i] & 0xFF) << ack;
#else
	  td  = ((unsigned char *)pBuf)[i];
#endif
	}
    ack = swd_ap_wr(3,td);
    if (ack != SWD_RESP_OK)
	  return SWD_RESP_PROTOCAL_ERROR;
  } 

  return SWD_RESP_OK;
}

#ifdef CGF
unsigned long swd_mem_wr_1k_mem(unsigned long addr,unsigned long * pBuf,unsigned long len,unsigned long size) {
  unsigned long ack = 0;
  unsigned long i = 0;
  unsigned long td = 0;
		
  if (len == 0)
    return SWD_RESP_OK;
  
  if ((((addr & 3) != 0) && (size == 2)) || (((addr & 1) != 0) && (size == 1)) || (size > 2)) {
    return SWD_RESP_PROTOCAL_ERROR;
  }

  if (swd_apacc_init(0,0) != SWD_RESP_OK) {
    return SWD_RESP_PROTOCAL_ERROR;
  }

  do {
    ack = swd_ap_rd(0,&td);
	ack = swd_dp_rd(SWD_PKT_REQ_READ_RDBUFF,&td);
	if ((ack == SWD_RESP_WAIT) || (ack == SWD_RESP_FAULT))
	  i ++;
  }while(((ack != SWD_RESP_OK) || ((td & 0x80) != 0)) && (i < SWD_BUS_MAX_RETRY));
  if (i >= SWD_BUS_MAX_RETRY) {
    return SWD_RESP_PROTOCAL_ERROR;
  }

  td = 0x80000040 | (size & 0x3);
  if (len > 1) 
    td |= 0x10;

  {
    ack = swd_ap_wr(0,td);
	if (ack != SWD_RESP_OK) {
      return SWD_RESP_PROTOCAL_ERROR;
	}
    ack = swd_ap_wr(1,addr);
	if (ack != SWD_RESP_OK) {
      return SWD_RESP_PROTOCAL_ERROR;
	}
  }
  for (i = 0; i < len; i ++) {
	if (size == 2) {
	  td = pBuf[i];
	} else if (size == 1) {	    
	  ack = ((((addr >> 1) + i) & 1) << 4);
	  td  = (((unsigned short *)pBuf)[i] & 0xFFFF) << ack;
	} else {	    
	  ack =  ((addr + i) & 3) << 3;
	  td  = (((unsigned short *)pBuf)[i] & 0xFF) << ack;
	}
    ack = swd_ap_wr_mem(3,td);
    if (ack != SWD_RESP_OK)
	  return SWD_RESP_PROTOCAL_ERROR;
  } 

  return SWD_RESP_OK;
}
#endif

void delay(unsigned long dly) {
  while((dly --) != 0)
    __nop();
}

unsigned long swd_core_reg_rd(unsigned long rs,unsigned long * pRdata) {
  unsigned long td = 0;
  unsigned long ack  = swd_mem_wr32(CM0_DBG_SCS_REG_DCRSR_ADDR,&rs,1);
  if (ack == SWD_RESP_OK) {
    do {
      ack = swd_mem_rd32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
	} while((ack != SWD_RESP_OK) && ((td & 0x10000) == 0));
	ack = swd_mem_rd32(CM0_DBG_SCS_REG_DCRDR_ADDR,pRdata,1);
  }

  return ack;
}

unsigned long swd_core_reg_wr(unsigned long rs,unsigned long wdata) {
  unsigned long td = 0;
  unsigned long ack  = swd_mem_wr32(CM0_DBG_SCS_REG_DCRDR_ADDR,&wdata,1);
  
  if (ack == SWD_RESP_OK) {
    td = rs | 0x10000;
    ack = swd_mem_wr32(CM0_DBG_SCS_REG_DCRSR_ADDR,&td,1);
	if (ack == SWD_RESP_OK) {
      do {
        ack = swd_mem_rd32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
	  } while((ack != SWD_RESP_OK) && ((td & 0x10000) == 0));
	}
  } 

  return ack;
}

unsigned long swd_halted_chk(void) {
  unsigned long td = 0;
  unsigned long ack  = swd_mem_rd32(CM0_DBG_SCS_REG_DFSR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    ack = ((td & 1) == 0) ? SWD_RESP_NOT_HALTED : SWD_RESP_HALTED;
  }

  return ack;
}

unsigned long swd_break_chk(void) {
  unsigned long td = 0;
  unsigned long ack  = swd_mem_rd32(CM0_DBG_SCS_REG_DFSR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    ack = ((td & 2) == 0) ? SWD_RESP_NOT_BREAK : SWD_RESP_BREAK;
  }

  return ack;
}

unsigned long swd_watch_chk(void) {
  unsigned long td = 0;
  unsigned long ack  = swd_mem_rd32(CM0_DBG_SCS_REG_DFSR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    ack = ((td & 4) == 0) ? SWD_RESP_NOT_WATCH : SWD_RESP_WATCH;
  }

  return ack;
}

unsigned long swd_vector_capture_chk(void) {
  unsigned long td = 0;
  unsigned long ack  = swd_mem_rd32(CM0_DBG_SCS_REG_DFSR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    ack = ((td & 8) == 0) ? SWD_RESP_NOT_VECTOR_CAPTURE : SWD_RESP_VECTOR_CAPTURE;
  }

  return ack;
}

unsigned long swd_ext_dbg_req_chk(void) {
  unsigned long td = 0;
  unsigned long ack = swd_mem_rd32(CM0_DBG_SCS_REG_DFSR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    ack = ((td & 16) == 0) ? SWD_RESP_NOT_EXT_DBG_REQ : SWD_RESP_EXT_DBG_REQ;
  }

  return ack;
}

unsigned long swd_svc_chk(void) {
  unsigned long td = 0;
  unsigned long ack = swd_mem_rd32(CM0_DBG_SCS_REG_SHCSR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    ack = ((td & 0x8000) == 0) ? SWD_RESP_NOT_SVC : SWD_RESP_SVC;
  }

  return ack;
}

unsigned long swd_dwt_enable(void) {
  unsigned long td = 0;
  unsigned long ack = swd_mem_rd32(CM0_DBG_SCS_REG_DEMCR_ADDR,&td,1);

  if (ack == SWD_RESP_OK) {
    td |= 0x01000000;
	ack = swd_mem_wr32(CM0_DBG_SCS_REG_DEMCR_ADDR,&td,1);
  }

  return ack;
}

unsigned long swd_hardfault_vc_enable(void) {
  unsigned long td = 0;
  unsigned long ack = swd_mem_rd32(CM0_DBG_SCS_REG_DEMCR_ADDR,&td,1);

  if (ack == SWD_RESP_OK) {
    td |= 0x00000400;
	ack = swd_mem_wr32(CM0_DBG_SCS_REG_DEMCR_ADDR,&td,1);
  }

  return ack;
}

unsigned long swd_reset_vc_enable(void) {
  unsigned long td = 0;
  unsigned long ack = swd_mem_rd32(CM0_DBG_SCS_REG_DEMCR_ADDR,&td,1);

  if (ack == SWD_RESP_OK) {
    td |= 1;
	ack = swd_mem_wr32(CM0_DBG_SCS_REG_DEMCR_ADDR,&td,1);
  }

  return ack;
}

unsigned long swd_dwt_disable(void) {
  unsigned long td = 0;
  unsigned long ack = swd_mem_rd32(CM0_DBG_SCS_REG_DEMCR_ADDR,&td,1);

  if (ack == SWD_RESP_OK) {
    td &= (~0x01000000);
	ack = swd_mem_wr32(CM0_DBG_SCS_REG_DEMCR_ADDR,&td,1);
  }

  return ack;
}

unsigned long swd_hardfault_vc_disable(void) {
  unsigned long td = 0;
  unsigned long ack = swd_mem_rd32(CM0_DBG_SCS_REG_DEMCR_ADDR,&td,1);

  if (ack == SWD_RESP_OK) {
    td &= (~0x00000400);
	ack = swd_mem_wr32(CM0_DBG_SCS_REG_DEMCR_ADDR,&td,1);
  }

  return ack;
}

unsigned long swd_reset_vc_disable(void) {
  unsigned long td = 0;
  unsigned long ack = swd_mem_rd32(CM0_DBG_SCS_REG_DEMCR_ADDR,&td,1);

  if (ack == SWD_RESP_OK) {
    td &= (~1);
	ack = swd_mem_wr32(CM0_DBG_SCS_REG_DEMCR_ADDR,&td,1);
  }

  return ack;
}

unsigned long swd_reset_vc_en_chk(void) {
  unsigned long td = 0;
  unsigned long ack  = swd_mem_rd32(CM0_DBG_SCS_REG_DEMCR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    ack = ((td & 1) == 0) ? SWD_RESP_NOT_RESET_VC_EN : SWD_RESP_RESET_VC_EN;
  }

  return ack;
}

unsigned long swd_hardfault_vc_en_chk(void) {
  unsigned long td = 0;
  unsigned long ack  = swd_mem_rd32(CM0_DBG_SCS_REG_DEMCR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    ack = ((td & 0x400) == 0) ? SWD_RESP_NOT_HARDFAULT_VC_EN : SWD_RESP_HARDFAULT_VC_EN;
  }

  return ack;
}

unsigned long swd_dwt_en_chk(void) {
  unsigned long td = 0;
  unsigned long ack  = swd_mem_rd32(CM0_DBG_SCS_REG_DEMCR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    ack = ((td & 0x1000000) == 0) ? SWD_RESP_NOT_DWT_EN : SWD_RESP_DWT_EN;
  }

  return ack;
}

unsigned long swd_debug_enable(void) {
  unsigned long td = 0;
  unsigned long ack  = swd_mem_rd32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
     
    td |= 1;
	td &= (~8);
	td &= 0xFFFF;
	td |= CM0_DBG_DHCSR_WR_KEY;
	ack  = swd_mem_wr32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
  }

  return ack;
}

unsigned long swd_debug_disable(void) {
  unsigned long td = 0;
  unsigned long ack  = swd_mem_rd32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
	td = CM0_DBG_DHCSR_WR_KEY;
	ack  = swd_mem_wr32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
  }

  return ack;
}

unsigned long swd_debug_en_chk(void) {
  unsigned long td = 0;
  unsigned long ack  = swd_mem_rd32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    ack = ((td & 1) == 0) ? SWD_RESP_NOT_DEBUG_ENABLE : SWD_RESP_DEBUG_ENABLE;
  }

  return ack;
}

unsigned long swd_debug_halt(void) {
  unsigned long td = 0;
  unsigned long ack  = 0;
  
  ack = swd_debug_en_chk();
  if (ack == SWD_RESP_NOT_DEBUG_ENABLE) {
    ack = swd_debug_enable();
	if (ack == SWD_RESP_OK) 
      ack = swd_debug_en_chk();
  }
  if (ack != SWD_RESP_DEBUG_ENABLE)
    return ack;

  ack = swd_mem_rd32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    td |= 2;
    td &= 0xFFFF;
    td |= CM0_DBG_DHCSR_WR_KEY;
     ack  = swd_mem_wr32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
  }
  if (ack == SWD_RESP_OK) {
    ack = swd_debug_state_chk();
  }  

  return ack;
}

unsigned long swd_debug_haltexit(void) {
  unsigned long td = 0;
  unsigned long ack  = 0;
  /*
  ack = swd_debug_en_chk();
  if (ack == SWD_RESP_NOT_DEBUG_ENABLE) {
    ack = swd_debug_enable();
	if (ack == SWD_RESP_OK) 
      ack = swd_debug_en_chk();
  }
  if (ack != SWD_RESP_DEBUG_ENABLE)
    return ack;
  */
  ack = swd_mem_rd32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    //td |= 2;
    td &= 0xFFFD;  //halt exit
    td |= CM0_DBG_DHCSR_WR_KEY;
     ack  = swd_mem_wr32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
  }
  if (ack == SWD_RESP_OK) {
    ack = swd_debug_state_chk();
  }  

  return ack;
}


unsigned long swd_debug_step(void) {
  unsigned long td = 0;
  unsigned long ack  = 0;
  
  ack = swd_debug_en_chk();
  if (ack == SWD_RESP_NOT_DEBUG_ENABLE)
    ack = swd_debug_enable();
  if (ack == SWD_RESP_OK) {
    ack = swd_mem_rd32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
    if (ack == SWD_RESP_OK) {
      td |= 4;
	  td &= 0xFFFF;
	  td |= CM0_DBG_DHCSR_WR_KEY;
	  ack  = swd_mem_wr32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
    }
  }  

  return ack;
}

unsigned long swd_sregrdy_chk(void) {
  unsigned long td = 0;
  unsigned long ack  = swd_mem_rd32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    ack = ((td & 0x10000) == 0) ? SWD_RESP_NOT_SREGRDY : SWD_RESP_SREGRDY;
  }

  return ack;
}

unsigned long swd_debug_state_chk(void)  {
  unsigned long td = 0;
  unsigned long ack  = swd_mem_rd32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    ack = ((td & 0x20000) == 0) ? SWD_RESP_NOT_DEBUG_STATE : SWD_RESP_DEBUG_STATE;
  }

  return ack;
}

unsigned long swd_sleep_chk(void)  {
  unsigned long td = 0;
  unsigned long ack  = swd_mem_rd32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    ack = ((td & 0x40000) == 0) ? SWD_RESP_NOT_SLEEP : SWD_RESP_SLEEP;
  }

  return ack;
}

unsigned long swd_lockup_chk(void)  {
  unsigned long td = 0;
  unsigned long ack  = swd_mem_rd32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    ack = ((td & 0x80000) == 0) ? SWD_RESP_NOT_LOCKUP : SWD_RESP_LOCKUP;
  }

  return ack;
}

unsigned long swd_retire_st_chk(void)  {
  unsigned long td = 0;
  unsigned long ack  = swd_mem_rd32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    ack = ((td & 0x1000000) == 0) ? SWD_RESP_NOT_RETIRE_ST : SWD_RESP_RETIRE_ST;
  }

  return ack;
}

unsigned long swd_reset_st_chk(void)  {
  unsigned long td = 0;
  unsigned long ack  = swd_mem_rd32(CM0_DBG_SCS_REG_DHCSR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    ack = ((td & 0x2000000) == 0) ? SWD_RESP_NOT_RESET_ST : SWD_RESP_RESET_ST;
  }

  return ack;
}

unsigned long swd_dwt_numcomp_get(unsigned long * pNumComp) {
  unsigned long td = 0;
  unsigned long ack = 0;

  ack = swd_mem_rd32(CM0_DBG_DWT_REG_CTRL_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    * pNumComp = (td >> 28) & 0xF;
  }

  return ack;
}

unsigned long swd_dwt_match_chk(void) {
  unsigned long td = 0;
  unsigned long ack = 0;

  ack = swd_mem_rd32(CM0_DBG_DWT_REG_FUNC0_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    if ((td & 0x1000000) == 0) {
	  ack = swd_mem_rd32(CM0_DBG_DWT_REG_FUNC1_ADDR,&td,1);
      if (ack == SWD_RESP_OK) {
	    ack = ((td & 0x1000000) == 0) ? SWD_RESP_NOT_DWT_MATCH : SWD_RESP_DWT_MATCH;
	  }
	} else {
	  ack = SWD_RESP_DWT_MATCH;
	}
  }

  return ack;
}

unsigned long swd_dwt_set(unsigned long cmp_idx,ps_swd_dwt pDwt) {
  unsigned long td = 0;
  unsigned long ack = 0;

  if (cmp_idx == 0) {
    ack = swd_mem_wr32(CM0_DBG_DWT_REG_FUNC0_ADDR,&td,1);
	if (ack == SWD_RESP_OK) {
      ack = swd_mem_wr32(CM0_DBG_DWT_REG_COMP0_ADDR,&(pDwt->comp),1);
	  if (ack == SWD_RESP_OK) {
        ack = swd_mem_wr32(CM0_DBG_DWT_REG_MASK0_ADDR,&(pDwt->mask),1);
  	    if (ack == SWD_RESP_OK) {
	      ack = swd_mem_wr32(CM0_DBG_DWT_REG_FUNC0_ADDR,&(pDwt->func),1);
	    }
	  }
	}
  } else {
    ack = swd_mem_wr32(CM0_DBG_DWT_REG_FUNC1_ADDR,&td,1);
	if (ack == SWD_RESP_OK) {
      ack = swd_mem_wr32(CM0_DBG_DWT_REG_COMP1_ADDR,&(pDwt->comp),1);
	  if (ack == SWD_RESP_OK) {
        ack = swd_mem_wr32(CM0_DBG_DWT_REG_MASK1_ADDR,&(pDwt->mask),1);
  	    if (ack == SWD_RESP_OK) {
	      ack = swd_mem_wr32(CM0_DBG_DWT_REG_FUNC1_ADDR,&(pDwt->func),1);
	    }
	  }
	}
  }

  return ack;
}

unsigned long swd_dwt_get(unsigned long cmp_idx,ps_swd_dwt pDwt) {
  unsigned long td = 0;
  unsigned long ack = 0;

  if (cmp_idx == 0) {
    ack = swd_mem_rd32(CM0_DBG_DWT_REG_FUNC0_ADDR,&td,1);
	if (ack == SWD_RESP_OK) {
      ack = swd_mem_rd32(CM0_DBG_DWT_REG_COMP0_ADDR,&(pDwt->comp),1);
	  if (ack == SWD_RESP_OK) {
        ack = swd_mem_rd32(CM0_DBG_DWT_REG_MASK0_ADDR,&(pDwt->mask),1);
  	    if (ack == SWD_RESP_OK) {
	      ack = swd_mem_rd32(CM0_DBG_DWT_REG_FUNC0_ADDR,&(pDwt->func),1);
	    }
	  }
	}
  } else {
    ack = swd_mem_rd32(CM0_DBG_DWT_REG_FUNC1_ADDR,&td,1);
	if (ack == SWD_RESP_OK) {
      ack = swd_mem_rd32(CM0_DBG_DWT_REG_COMP1_ADDR,&(pDwt->comp),1);
	  if (ack == SWD_RESP_OK) {
        ack = swd_mem_rd32(CM0_DBG_DWT_REG_MASK1_ADDR,&(pDwt->mask),1);
  	    if (ack == SWD_RESP_OK) {
	      ack = swd_mem_rd32(CM0_DBG_DWT_REG_FUNC1_ADDR,&(pDwt->func),1);
	    }
	  }
	}
  }

  return ack;
}

unsigned long swd_bpu_numcode_get(unsigned long * pNumCode) {
  unsigned long td = 0;
  unsigned long ack = 0;

  ack = swd_mem_rd32(CM0_DBG_BPU_REG_CTRL_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    * pNumCode = (td >> 4) & 0xF;
  }

  return ack;
}

unsigned long swd_bpu_enable(void) {
  unsigned long td = 3;
  unsigned long ack = 0;

  ack = swd_mem_wr32(CM0_DBG_BPU_REG_CTRL_ADDR,&td,1);

  return ack;
}

unsigned long swd_bpu_disable(void) {
  unsigned long td = 2;
  unsigned long ack = 0;

  ack = swd_mem_wr32(CM0_DBG_BPU_REG_CTRL_ADDR,&td,1);

  return ack;
}

unsigned long swd_bpu_en_chk(void) {
  unsigned long td = 2;
  unsigned long ack = 0;

  ack = swd_mem_rd32(CM0_DBG_BPU_REG_CTRL_ADDR,&td,1);

  if (ack == SWD_RESP_OK){
    if ((td & 1) != 0)
      ack = SWD_RESP_BPU_EN;
    else
      ack = SWD_RESP_NOT_BPU_EN;
  }

  return ack;
}

unsigned long swd_bpu_set(unsigned long cmp_idx,unsigned long cmp_addr) {
  unsigned long td = (cmp_addr & 0x1FFFFFFC) | 0xC0000001;
  unsigned long ack = 0;

  if (cmp_idx == 0) {
    ack = swd_mem_wr32(CM0_DBG_BPU_REG_COMP0_ADDR,&td,1);
  } else if (cmp_idx == 1) {
    ack = swd_mem_wr32(CM0_DBG_BPU_REG_COMP1_ADDR,&td,1);
  } else if (cmp_idx == 2) {
    ack = swd_mem_wr32(CM0_DBG_BPU_REG_COMP2_ADDR,&td,1);
  } else {
    ack = swd_mem_wr32(CM0_DBG_BPU_REG_COMP3_ADDR,&td,1);
  } 

  return ack;
}

unsigned long swd_bpu_get(unsigned long cmp_idx,unsigned long * cmp_addr) {
  unsigned long td = 0;
  unsigned long ack = 0;

  if (cmp_idx == 0) {
    ack = swd_mem_rd32(CM0_DBG_BPU_REG_COMP0_ADDR,&td,1);
  } else if (cmp_idx == 1) {
    ack = swd_mem_rd32(CM0_DBG_BPU_REG_COMP1_ADDR,&td,1);
  } else if (cmp_idx == 2) {
    ack = swd_mem_rd32(CM0_DBG_BPU_REG_COMP2_ADDR,&td,1);
  } else {
    ack = swd_mem_rd32(CM0_DBG_BPU_REG_COMP3_ADDR,&td,1);
  } 

  * cmp_addr = td;
  return ack;
}

unsigned long swd_debug_reset(void) {
  unsigned long td = 0;
  unsigned long ack = 0;

  ack = swd_mem_rd32(CM0_SCB_REG_AIRCR_ADDR,&td,1);
  if (ack == SWD_RESP_OK) {
    ack = swd_reset_st_chk();
    if ((ack == SWD_RESP_NOT_RESET_ST) || (ack == SWD_RESP_RESET_ST)) {
      td = CM0_SCB_REG_AIRCR_WR_KEY | 4;
	  ack = swd_mem_wr32(CM0_SCB_REG_AIRCR_ADDR,&td,1);
	  if (ack == SWD_RESP_OK) {
	    return swd_reset_st_chk();
	  }
	}
  }

  return ack;
}

