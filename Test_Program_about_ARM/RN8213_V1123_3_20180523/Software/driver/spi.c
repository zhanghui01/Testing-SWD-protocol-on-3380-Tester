#define _SPI_GLOBALS

// #include "spi.h"
#include "drv.h"

/*
 *   SCK     P0.15
 *   MOSI    P0.18
 *   MISO0   P0.17
 *   SPICS   P0.16
 */
 // Negedge for ADS1243.
void spiInit(void)
{
  //DIR1    ->  P1_29      //Ouput . High
  //DIR2    ->  P1_28      //Input . Low
  //TR0_EN  ->  P2_8
  
  LPC_GPIO0->FIODIR  |= (1 << 16);  
  LPC_GPIO1->FIODIR  |= (1 << 28);
  LPC_GPIO1->FIODIR  |= (1 << 29);    
  LPC_GPIO2->FIODIR  |= (1 << 8);  
  
  SPICS_SET1;
  DIROUTPUT_SET1;
  DIRINPUT_SET0;
  TR0_EN_SET0;
  
  LPC_SC->PCONP   |= (1 << 21);                                    /* ��SSP0��Դ*/    

  LPC_SC->PCLKSEL0 &= (~(3 << 10));                                /* clk = cclk / 4 (25Mhz)*/
  LPC_PINCON->PINSEL0 |= (0x1u << 31);                              //SCK,P0_15
  LPC_PINCON->PINSEL1 |= (0x1u << 5);                               //MOSI,P0_18 
  
	LPC_PINCON->PINSEL1 |= (0x1 << 3);                               //MISO0,P0_17    
  LPC_PINCON->PINSEL3 &= ~(0x3 << 14);   		

  /*
  ** ��ʼ��SSP��ͨѶ��ʽ,�������ݳ���Ϊ8bit,֡��ʽΪSPI,SCK Ϊ����Ч,
  ** ������SCK �ĵ�һ��ʱ���ز���,����λ���ʡ� 
  */
  LPC_SSP0->CR0  = (0x00 << 8) |                                   /* SCR  ����SPIλ����           */
                   (0x01 << 7) |                                   /* CPHA ʱ�������λ            *///NEGEDGE
                   (0x00 << 6) |                                   /* CPOL ʱ���������            */
                   (0x00 << 4) |                                   /* FRF  ֡��ʽ 00=SPI��01=SSI,  */
                                                                   /* 10=Microwire��11=����        */
                   (0x07 << 0);                                    /* DSS  ���ݳ���,0000-0010=���� */
                                                                   /* 0011=4λ��0111=8λ��1111=16λ*/

  LPC_SSP0->CR1  = (0x00 << 3) |                                   /* SOD  �ӻ�������ܣ�0=����    */
                   (0x00 << 2) |                                   /* MS   ����ѡ��0=����        */
                   (0x01 << 1) |                                   /* SSE  SSPʹ�ܣ�1=����SSP��    */
                                                                   /* �����豸ͨ��                 */
                   (0x00 << 0);                                    /* LBM  ��дģʽ                */
                  

  LPC_SSP0->CPSR = 254;                                          /* SPI Clock = Pclk / cnt = 25Mhz / 255 = 98khz*/
}


uint8_t spiSendByte(uint8_t data)
{
  LPC_SSP0->DR = data;
  while(!(LPC_SSP0->SR & (1 << 0)))      //Wait Transmit FIFO Empty.
  {
    ;
  }
  while(LPC_SSP0->SR & (1 << 4))         //Wait Not Busy
  {
    ;
  }
  return LPC_SSP0->DR;
}

uint8_t spiReceByte(void)
{
  LPC_SSP0->DR = 0x0;
  while(!(LPC_SSP0->SR & (1 << 2)))
  {
    ;
  }
  return LPC_SSP0->DR;
}

void spiExit(void)
{
// 	DIRINPUT_SET1;//cgf test @20160913
}
