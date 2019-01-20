#define _ADS_ACQUIRE_GLOBALS

#include "ads_acquire.h"


void adsInit(void)
{
	spiInit();
}

/*********************************************************************************************************
** Function name:       errorCal
** Descriptions:        
** input parameters:    delay duration
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
uint32_t errorCal(uint32_t * pdata,uint32_t aver,uint32_t num) 
{
    uint32_t result = 0x0;
	  uint32_t i;

    for(i=0;i<num;i++)
    {
      if(aver > *(pdata + i))
      {
        result += (aver - *(pdata+i));
      }
      else
      {
        result += (*(pdata+i) - aver);
      }
    }
    
    return result;
}




uint32_t adsDataRead(void)
{
  uint32_t dout = 0x0;
  uint32_t temp = 0x0;

  while(SPIDRDYN == 0x0);  //Wait for new data upate.
	while(SPIDRDYN != 0x0);   //Wait for data update complete.

  SPICS_SET0;
  spiSendByte(0x01);      //CMD
  
  temp = spiReceByte();
  dout = (temp << 16);
  temp = spiReceByte();
  dout |= (temp << 8);
  temp = spiReceByte();
  dout |= temp;
    
  SPICS_SET1;

  return dout;
}


uint8_t adsRegRead(uint8_t addr)
{
  uint8_t dout;

  SPICS_SET0;
  spiSendByte(addr | 0x10);      //Address
  spiSendByte(0x0);              //Num
  swDelay(1);
  dout = spiReceByte();
  SPICS_SET1;

  return dout;
}

void adsRegWrite(uint8_t addr,uint8_t data)
{
  SPICS_SET0;
  spiSendByte(addr | 0x50);      //Address
  spiSendByte(0x0);              //Num
  spiSendByte(data);             //Data
  SPICS_SET1;
}


void adsOffsetCal(void)
{
	DIRINPUT_SET0;
	
	//Reset ADS1243.
	SPICS_SET0;
  spiSendByte(ADS_RESET);  
  SPICS_SET1;		
	
// 	/***********************Self Calibration***********************/
// 	//PGA = 1
//   adsRegWrite(ADS_SETUP,0x0);
//   //AIN4
//   adsRegWrite(ADS_MUX,0x7 | CHAN_ADJUST);
//   //Data Format(6): Bipolar;
//   //SPEED(5):Fosc/128
//   //BUFEN(4):Buffer On 
//   //MSB(3).
//   //RANGE(2): -Vref ~ +Vref
//   //DR(1:0):  15Hz
//   adsRegWrite(ADS_ACR,0x40);
//   //Offset DAC
//   adsRegWrite(ADS_ODAC,0x00);
//   //DIR. Set as IN
//   adsRegWrite(ADS_DIR,0xff);
//   //IOCON. Set as Analog
//   adsRegWrite(ADS_IOCON,0x00); 
// 	
// 	swDelay(100);
// 	
// 	SPICS_SET0;
//   spiSendByte(ADS_SELFCAL);  
//   SPICS_SET1;	
// 	
// 	swDelay(100);
// 	
// 	while(0x80 & (adsRegRead(ADS_ACR)));

/***********************Self Gain Calibration***********************/
	//PGA = 1
  adsRegWrite(ADS_SETUP,0x0);
  //AIN4
  adsRegWrite(ADS_MUX,0x0);
  //Data Format(6): Bipolar;
  //SPEED(5):Fosc/128
  //BUFEN(4):Buffer On 
  //MSB(3).
  //RANGE(2): -Vref ~ +Vref
  //DR(1:0):  15Hz
  adsRegWrite(ADS_ACR,0x40);
  //Offset DAC
  adsRegWrite(ADS_ODAC,0x00);
  //DIR. Set as IN
  adsRegWrite(ADS_DIR,0xff);
  //IOCON. Set as Analog
  adsRegWrite(ADS_IOCON,0x00); 
	
	swDelay(100);
	
	SPICS_SET0;
  spiSendByte(ADS_SELFGCAL);  
  SPICS_SET1;	
	
	swDelay(100);
	
	while(0x80 & (adsRegRead(ADS_ACR)));
	
/***********************Self Offset Calibration***********************/	
	//PGA = 16
  adsRegWrite(ADS_SETUP,0x4);
  //AIN4
  adsRegWrite(ADS_MUX,0x0);
  //Data Format(6): Bipolar;
  //SPEED(5):Fosc/128
  //BUFEN(4):Buffer On 
  //MSB(3).
  //RANGE(2): -Vref ~ +Vref
  //DR(1:0):  15Hz
  adsRegWrite(ADS_ACR,0x50);
  //Offset DAC
  adsRegWrite(ADS_ODAC,0x00);
  //DIR. Set as IN
  adsRegWrite(ADS_DIR,0xff);
  //IOCON. Set as Analog
  adsRegWrite(ADS_IOCON,0x00); 
	
	SPICS_SET0;
  spiSendByte(ADS_SELFOCAL);     
  SPICS_SET1;	
	
	swDelay(100);
	while(0x80 & (adsRegRead(ADS_ACR)));	

/***********************System Offset Calibration***********************/
/*	//PGA = 1
  adsRegWrite(ADS_SETUP,0x0);
  //AIN4
  adsRegWrite(ADS_MUX,0x7 | CHAN_ADJUST);
  //Data Format(6): Bipolar;
  //SPEED(5):Fosc/128
  //BUFEN(4):Buffer On 
  //MSB(3).
  //RANGE(2): -Vref ~ +Vref
  //DR(1:0):  15Hz
  adsRegWrite(ADS_ACR,0x40);
  //Offset DAC
  adsRegWrite(ADS_ODAC,0x00);
  //DIR. Set as IN
  adsRegWrite(ADS_DIR,0xff);
  //IOCON. Set as Analog
  adsRegWrite(ADS_IOCON,0x00); 
	
	swDelay(100);
	
	SPICS_SET0;
  spiSendByte(ADS_SYSOCAL);  
  SPICS_SET1;	
	
	swDelay(100);
	
	while(0x80 & (adsRegRead(ADS_ACR)));
	
	fsr[0] = adsRegRead(ADS_FSR0);
	fsr[1] = adsRegRead(ADS_FSR1);
	fsr[2] = adsRegRead(ADS_FSR2);	

	ads_fsr = (fsr[2] << 16)  | (fsr[1] << 8) | fsr[0];
*/
/***********************System Gain Calibration***********************/
/*	
	//PGA = 1
  adsRegWrite(ADS_SETUP,0x0);
  //AIN4
  adsRegWrite(ADS_MUX,0x7 | CHAN_RA3_REF);
  //Data Format(6): Bipolar;
  //SPEED(5):Fosc/128
  //BUFEN(4):Buffer On 
  //MSB(3).
  //RANGE(2): -Vref ~ +Vref
  //DR(1:0):  15Hz
  adsRegWrite(ADS_ACR,0x50);
  //Offset DAC
  adsRegWrite(ADS_ODAC,0x00);
  //DIR. Set as IN
  adsRegWrite(ADS_DIR,0xff);
  //IOCON. Set as Analog
  adsRegWrite(ADS_IOCON,0x00); 
	
	SPICS_SET0;
  spiSendByte(ADS_SYSGCAL);     
  SPICS_SET1;	
	
	swDelay(100);
	while(0x80 & (adsRegRead(ADS_ACR)));	
	
	fsr[0] = adsRegRead(ADS_FSR0);
	fsr[1] = adsRegRead(ADS_FSR1);
	fsr[2] = adsRegRead(ADS_FSR2);

	ads_fsr = (fsr[2] << 16)  | (fsr[1] << 8) | fsr[0];
*/

  //Send DSYNC
	SPICS_SET0;
  spiSendByte(ADS_DSYNC);     
  SPICS_SET1;	
  
// 	DIRINPUT_SET1;//cgf test @20160913
}

void adsAcquireInit(uint32_t channel)
{
//   uint32_t temp;
	
	DIRINPUT_SET0;
	
	//AIN0 - RA1_P00
  //AIN1 - RA1_P01
  //AIN2 - RA2_P00
  //AIN3 - RA2_P01
  //AIN4 - RA3_P01
  //AIN5 - RA3_P01
  //AIN6 - RA4_P01
  //AIN7 - RA4_P01
  
  //PGA = 1
	//adsRegWrite(ADS_SETUP,0x0);  //PGA = 1
  adsRegWrite(ADS_SETUP,0x4);  //PGA = 16
  //AIN4
  adsRegWrite(ADS_MUX,channel);
  //adsRegWrite(ADS_MUX,0x1);
	//Data Format(6): Bipolar;
  //SPEED(5):Fosc/128
  //BUFEN(4):Buffer On 
  //MSB(3).
  //RANGE(2): -Vref ~ +Vref
  //DR(1:0):  15Hz
  //adsRegWrite(ADS_ACR,0x40);
  adsRegWrite(ADS_ACR,0x50);
	//Offset DAC
  adsRegWrite(ADS_ODAC,0x00);
  //DIR. Set as IN
  adsRegWrite(ADS_DIR,0xff);
  //IOCON. Set as Analog
  adsRegWrite(ADS_IOCON,0x00);
	
// 	temp = adsRegRead(ADS_MUX);
// 	if(temp != 0x54)
// 	{
// 		while(1);
// 	}
	
	while(SPIDRDYN == 0x1);
	adsDataRead();
	while(SPIDRDYN == 0x1);
	adsDataRead();
	while(SPIDRDYN == 0x1);
	
// 	DIRINPUT_SET1;//cgf test @20160913
}

void adsInitvdd(uint32_t channel)
{
//   uint32_t temp;
	
	DIRINPUT_SET0;
	
	//AIN0 - RA1_P00
  //AIN1 - RA1_P01
  //AIN2 - RA2_P00
  //AIN3 - RA2_P01
  //AIN4 - RA3_P01
  //AIN5 - RA3_P01
  //AIN6 - RA4_P01
  //AIN7 - RA4_P01
  
  //PGA = 1
	//adsRegWrite(ADS_SETUP,0x0);  //PGA = 1
  adsRegWrite(ADS_SETUP,0x0);  //PGA = 16
  //AIN4
  adsRegWrite(ADS_MUX,channel);
  //adsRegWrite(ADS_MUX,0x1);
	//Data Format(6): Bipolar;
  //SPEED(5):Fosc/128
  //BUFEN(4):Buffer On 
  //MSB(3).
  //RANGE(2): -Vref ~ +Vref
  //DR(1:0):  15Hz
  //adsRegWrite(ADS_ACR,0x40);
  adsRegWrite(ADS_ACR,0x50);
	//Offset DAC
  adsRegWrite(ADS_ODAC,0x00);
  //DIR. Set as IN
  adsRegWrite(ADS_DIR,0xff);
  //IOCON. Set as Analog
  adsRegWrite(ADS_IOCON,0x00);
	
// 	temp = adsRegRead(ADS_MUX);
// 	if(temp != 0x54)
// 	{
// 		while(1);
// 	}
	
	while(SPIDRDYN == 0x1);
	adsDataRead();
	while(SPIDRDYN == 0x1);
	adsDataRead();
	while(SPIDRDYN == 0x1);
	
// 	DIRINPUT_SET1;//cgf test @20160913
}


uint32_t adsAcquire(void)
{
  uint32_t   dout[ACQUIRE_TIMES];
  uint32_t   i;
  uint32_t   aver;
  uint32_t   error;
	
	float  vref = ADS_VERF;
	float temp;
	
	float dout_uv[ACQUIRE_TIMES];

	DIRINPUT_SET0;

	DIRINPUT_SET0;
	dout[0] = adsDataRead();  // ignore the first data
	
  for(i=0;i<ACQUIRE_TIMES;i++)
  {
    dout[i] = adsDataRead();
  }
	
// 	DIRINPUT_SET1;//cgf test @20160913
	
	for(i=0;i<ACQUIRE_TIMES;i++)
	{
		dout_uv[i] = (float)((float)dout[i] * vref);
		temp = ( (float)1000000 / (float)(16777216 -1));
		dout_uv[i] = (dout_uv[i] * temp); 		
	}
	
	aver = average(dout,ACQUIRE_TIMES);
  
  error = errorCal(dout,aver,ACQUIRE_TIMES);
  
  temp = 	((float)((float)aver * vref)) * ( (float)1000000 / (float)(16777216 -1));
	temp = (temp * ((float)ADS_RATIO))/ 16;
	
  //if(error > (ACQUIRE_TIMES * 512))        //256LSB Error = 0.0763mV
  if(error > (ACQUIRE_TIMES * 700))        //256LSB Error = 0.0763mV
  {
    return 0xffffffff;
  }
  else
  {
    return (uint32_t)temp;
  }
}
