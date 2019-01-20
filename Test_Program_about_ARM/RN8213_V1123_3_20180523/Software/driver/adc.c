#include "drv.h"

#ifdef   SIZE1
#define  KA   1.0//0.737731044
#define  KB   1.0//0.918325769
#define  KC   1.0//0.846675529
#define  KD   1.0//0.935084312
#endif


#ifdef  SIZE2
#define  KA   1.0
#define  KB   1.0
#define  KC   1.0
#define  KD   1.0
#endif
/**
 * 3. ADC measure:
 *       AD0[2](P0.25) : LCDVD.   Value = 3 * Vadc
 *       AD0[3](P0.26) : LCDVC.   Value = 2 * Vadc
 *       AD0[6](P0.3)  : LCDVB.   Value = 1 * Vadc
 *       AD0[7](P0.2)  : LCDVA.   Value = Vadc
 *       AD0[5](P1.31) : VSWO.    Value = 2 * Vadc
 */
void adcInit(void)
{
  LPC_SC->PCONP |= (1 << 12);           /* module clock */
  LPC_ADC->ADCR |= (1 << 21);           /* power-on module */
  LPC_SC->PCLKSEL0 &= (~(3 << 24));     /* apb clock(0:CCLK/4, 1: CCLK, 2: CCLK/2, 3: CCLK/8)*/
  LPC_ADC->ADCR = LPC_ADC->ADCR & (~(0xff << 8)) | (2 << 8); /* ADC sample clock = apb_clk/(t + 1) = 8.3Mhz, should be less than 13MHz*/
  LPC_PINCON->PINSEL0 |= ((0x2 << 4) | (0x2 << 6));    /* multi-function */
  LPC_PINCON->PINSEL1 |= ((0x1 << 18) | (0x1 << 20));
  LPC_PINCON->PINSEL3 |= (0x3u << 30);
}

void adcExit(void)
{
  LPC_PINCON->PINSEL0 &= (~(0x3<<4)) ;  // P0.2/ADC0.7 as GPIO
	LPC_PINCON->PINSEL0 &= (~(0x3<<6)) ;  // P0.3/ADC0.6 as GPIO
	LPC_PINCON->PINSEL1 &= (~(0x3<<18)) ; // P0.25/ADC0.2 as GPIO
	LPC_PINCON->PINSEL1 &= (~(0x3<<20)) ; // P0.26/ADC0.3 as GPIO
	LPC_PINCON->PINSEL3 &= (~(0x3<<20)) ; // P1.31/ADC0.5 as GPIO	
}

#define ADC_VREF (3.281*1.0)

float adcGetValue(uint8_t ch)
{
  float    res = 0;
  uint32_t i;
  
	LPC_ADC->ADCR = LPC_ADC->ADCR & (~(0xff << 0)) | (1 << ch);             /* select adc channel */
  LPC_ADC->ADCR |= (1 << 24);             /* start ADC conversion */
  while(!(LPC_ADC->ADSTAT & (1 << ch)))
  {
    ;
  }

//   usDelay(3000);
		
  for(i = 0; i < 10; i++)
  {
    LPC_ADC->ADCR = LPC_ADC->ADCR & (~(0xff << 0)) | (1 << ch);             /* select adc channel */
    LPC_ADC->ADCR |= (1 << 24);             /* start ADC conversion */
    while(!(LPC_ADC->ADSTAT & (1 << ch)))
    {
      ;
    }
    usDelay(100);
    switch (ch)
    {
      case ADC_VSWO:
        res +=  (ADC_VREF * 2 * ((LPC_ADC->ADDR5 >> 4) & 0xfff) / 0xfff);
        break;
      case ADC_LCDVA:
        res +=  (KA * ADC_VREF * ((LPC_ADC->ADDR7 >> 4) & 0xfff) / 0xfff);
        break;
      case ADC_LCDVB:
        res +=  (KB * ADC_VREF * ((LPC_ADC->ADDR6 >> 4) & 0xfff) / 0xfff);
        break;
      case ADC_LCDVC:
        res +=  (KC * ADC_VREF * 2 * ((LPC_ADC->ADDR3 >> 4) & 0xfff) / 0xfff);
        break;
      case ADC_LCDVD:
        res +=  (KD * ADC_VREF * 3 * ((LPC_ADC->ADDR2 >> 4) & 0xfff) / 0xfff);
        break;
      default:
        res =  0x0;
        break;
    }
  }
  
  return (res / 10.0);
}


#include "rn821x.h"

//=================   bialvl=31    ========================
uint32_t SocPump_31Ena(void)
{
  uint32_t  data ;
  
  data = 0x82;
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PS)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  data |= (1 << 6) ;       //LCD apb 
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC MOD1_EN fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
//   DBG_MSG("mod1_en:0x%x\n", data);	
		
  data = 1;
  if(swd_mem_wr32((unsigned long)(&(LCD->BLINK)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC LCD_BLINK fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
	
  data = 0x2a ;
  if(swd_mem_wr32((unsigned long)(&(LCD->CLKDIV)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC LCD_CLKDIV fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
	data = 1;
  if(swd_mem_wr32((unsigned long)(&(LCD->PS)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC LCD_PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }

	if(swd_mem_rd32((unsigned long)(&(LCD->CTRL)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
	
	if(data != 0)
	{
		DBG_ERR("lcd ctrl is:0x%x, should be 0\n", data);
	}
	
  data = (1 << 0) | (5<<1) | (1 << 4) | (0x1f << 5);
  if(swd_mem_wr32((unsigned long)(&(LCD->CTRL)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC LCD_CTRL fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }			
	
	return SWD_RESP_OK;
}

//=================   bialvl=21   ========================
uint32_t SocPump_21Ena(void)
{
  uint32_t  data ;
  
  data = 0x82;
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PS)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  data |= (1 << 6) ;       //LCD apb 
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC MOD1_EN fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
//   DBG_MSG("mod1_en:0x%x\n", data);	
		
  data = 1;
  if(swd_mem_wr32((unsigned long)(&(LCD->BLINK)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC LCD_BLINK fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
	
  data = 0x2a ;
  if(swd_mem_wr32((unsigned long)(&(LCD->CLKDIV)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC LCD_CLKDIV fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
	data = 1;
  if(swd_mem_wr32((unsigned long)(&(LCD->PS)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC LCD_PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }

	if(swd_mem_rd32((unsigned long)(&(LCD->CTRL)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
	
	if(data != 0)
	{
		DBG_ERR("lcd ctrl is:0x%x, should be 0\n", data);
	}
	
  data = (1 << 0) | (5<<1) | (1 << 4) | (0x15 << 5);
  if(swd_mem_wr32((unsigned long)(&(LCD->CTRL)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC LCD_CTRL fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }			
	
	return SWD_RESP_OK;
}

//

//====================Normal pwr ============================
uint32_t Normal_pwr_init (void)
{
	 uint32_t  data ;
	 uint32_t  temp ;
 // WdtInit(1) ;	
 // NVIC_EnableIRQ(WDT_IRQn); 
 // uDelay(10000) ;
	
//	SYSCTL->SYS_PS = 0x82 ;
//	SYSCTL->MOD1_EN |= (1<<5) ;  // GPIO enable
	
	 data = 0x82;
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PS)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  data |= (1 << 5) ;       //GPIO enable 
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC MOD1_EN fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
	 if(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  DBG_MSG("mod1_en:0x%x\n", data);	
	
/*	
	// PA ,input ,don't pull up 
  GPIO->PMA = 0xffffffff ;  // input mode 
  GPIO->PCA0 = 0x20000000 ;  // GPIO mode
  GPIO->PCA1 = 0x0 ;  // GPIO mode         =0x20
	GPIO->PUA  = 0x0 ;  // pull up disable   =0x300000
  GPIO->PIEA = 0xffffffff ;  // enable input
  
  // PB ,input ,don't pull up
  GPIO->PMB = 0xffffffff ;  // input mode
  GPIO->PCB = 0x0 ;  // GPIO mode
	GPIO->PUB = 0x0 ;  // disable pull up or pull down
  GPIO->PIEB = 0xffffffff ;  // enable input
  
  // PC ,input ,don't pull up
  GPIO->PMC = 0xffffffff ;
  GPIO->PCC = 0x0        ;  // GPIO mode
	GPIO->PUC = 0x0        ;  // disable pull up or pull down
  GPIO->PIEC = 0xffffffff ;  // enable input
 */
 
  // sysclk = 3.6864MHz ,PLL
//  SYSCTL->OSC_CTRL1 = 0x4 ;  // div = 2 ,and PLL enable
//  while( !(SYSCTL->OSC_CTRL1 & (1<<7)) ) 
//    ;
//  uDelay(1000) ;
//  SYSCTL->SYS_MODE = 0xd ;  // switch to PLL
//  while( !(SYSCTL->OSC_CTRL1 & (1<<6)) )
//    ;
//  SYSCTL->OSC_CTRL1 |= (1<<1) ;  // disable RCH 
//	SYSCTL->OSC_CTRL1 |= (1<<1) ;  // disable RCH 


 // =============sysclk = 3.6864MHz ,PLL
temp = 0x4 ;  // div = 2 ,and PLL enable 
  while(swd_mem_wr32((unsigned long)(&(SYSCTL->OSC_CTRL1)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC OSC_CTRL1 fail\n");
  }
while(1)
  {
    while(swd_mem_rd32((unsigned long)(&(SYSCTL->OSC_CTRL1)),(unsigned long *)&temp,1) != SWD_RESP_OK)
    {
      DBG_ERR("Read SOC OSC_CTRL1 fail\n");
    }
    
    if(temp & (1<<7))
    {
      break;
    }
  }	
	
	temp = 0xd ;  // switch to PLL 
  while(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_MODE)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC SYS_MODE fail\n");
  }
while(1)
  {
    while(swd_mem_rd32((unsigned long)(&(SYSCTL->OSC_CTRL1)),(unsigned long *)&temp,1) != SWD_RESP_OK)
    {
      DBG_ERR("Read SOC OSC_CTRL1 fail\n");
    }
    
    if(temp & (1<<6))
    {
      break;
    }
  }	
	
if(swd_mem_rd32((unsigned long)(&(SYSCTL->OSC_CTRL1)),(unsigned long *)&data,1) != SWD_RESP_OK)
	{
    DBG_ERR("Write SOC OSC_CTRL1 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  data  |= (1<<1) ;  // disable RCH 
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->OSC_CTRL1)),(unsigned long *)&data,1) != SWD_RESP_OK)
	{
    DBG_ERR("Write SOC OSC_CTRL1 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
	 if(swd_mem_rd32((unsigned long)(&(SYSCTL->OSC_CTRL1)),(unsigned long *)&data,1) != SWD_RESP_OK)
	{
    DBG_ERR("Write SOC OSC_CTRL1 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  DBG_MSG("OSC_CTRL1:0x%x\n", data);		
	
	
  //======= ADC enable
// 	SYSCTL->SYS_PD &= (~(0x7<<0)) ;
if(swd_mem_rd32((unsigned long)(&(SYSCTL->SYS_PD)),(unsigned long *)&data,1) != SWD_RESP_OK)
	{
    DBG_ERR("Write SOC SYS_PD fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  data  &= (~(0x7<<0)) ;// ADC enable
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PD)),(unsigned long *)&data,1) != SWD_RESP_OK)
	{
    DBG_ERR("Write SOC SYS_PD fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
	 if(swd_mem_rd32((unsigned long)(&(SYSCTL->SYS_PD)),(unsigned long *)&data,1) != SWD_RESP_OK)
	{
    DBG_ERR("Write SOC SYS_PD fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  DBG_MSG("SYS_PD:0x%x\n", data);		
// 	while(1) 
// 		;	
return SUCCESS;	
}

//
uint32_t SocLcdIoCfg( void )
{
  uint32_t  data ;

  data = 0x82;
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PS)),(unsigned long *)&data,1) != SWD_RESP_OK)
	{
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK)
	{
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  data |= (1 << 5) ;       // GPIO apb 
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK)
	{
    DBG_ERR("Write SOC MOD1_EN fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK)
	{
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  DBG_MSG("mod1_en:0x%x\n", data);		

	// SOC PCB as LCD Port
  if(swd_mem_rd32((unsigned long)(&(GPIO->PCB)),(unsigned long *)&data,1) != SWD_RESP_OK)
	{
    DBG_ERR("read SOC PCB fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }  
	data |= (0xffffu<<16) ;
  if(swd_mem_wr32((unsigned long)(&(GPIO->PCB)),(unsigned long *)&data,1) != SWD_RESP_OK)
	{
    DBG_ERR("Write SOC PCB fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }	
	
	// SOC PCC as LCD Port
  if(swd_mem_rd32((unsigned long)(&(GPIO->PCC)),(unsigned long *)&data,1) != SWD_RESP_OK)
	{
    DBG_ERR("read SOC PCC fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }  
	data |= (0xfffffu<<0) ;
  if(swd_mem_wr32((unsigned long)(&(GPIO->PCC)),(unsigned long *)&data,1) != SWD_RESP_OK)
	{
    DBG_ERR("Write SOC PCC fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }	
	
	// SOC PCE as LCD Port
  if(swd_mem_rd32((unsigned long)(&(GPIO->PCE)),(unsigned long *)&data,1) != SWD_RESP_OK)
	{
    DBG_ERR("read SOC PCE fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }  
	data |= (0xf<<0) ;
  if(swd_mem_wr32((unsigned long)(&(GPIO->PCE)),(unsigned long *)&data,1) != SWD_RESP_OK)
	{
    DBG_ERR("Write SOC PCE fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }	
	return SWD_RESP_OK;
}

//
#ifdef   CGF_2015_05_23
uint32_t SocPumpDis(void)
{
  uint32_t  data ;
  
  data = 0x82;
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PS)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  data |= (1 << 6) ;       //LCD apb 
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC MOD1_EN fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD1_EN)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  DBG_MSG("mod1_en:0x%x\n", data);	
		
						 
	if(swd_mem_rd32((unsigned long)(&(LCD->CTRL)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  data = 0;
  if(swd_mem_wr32((unsigned long)(&(LCD->CTRL)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC LCD_CTRL fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
	if(swd_mem_rd32((unsigned long)(&(LCD->CTRL)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
  data = 0;
  if(swd_mem_wr32((unsigned long)(&(LCD->CTRL)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC LCD_CTRL fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
	if(swd_mem_rd32((unsigned long)(&(LCD->CTRL)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  data = 0;
  if(swd_mem_wr32((unsigned long)(&(LCD->BLINK)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC LCD_BLINK fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
	
  data = 0x2a ;
  if(swd_mem_wr32((unsigned long)(&(LCD->CLKDIV)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC LCD_CLKDIV fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
	data = 1;
  if(swd_mem_wr32((unsigned long)(&(LCD->PS)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC LCD_PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }

	if(swd_mem_rd32((unsigned long)(&(LCD->CTRL)),(unsigned long *)&data,1) != SWD_RESP_OK){
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
	
	if(data != 0)
	{
		DBG_ERR("lcd ctrl is:0x%x, should be 0\n", data);
	}
	
	
	return SWD_RESP_OK;
}
#endif
#if 0

#define LCD_VA_MAX 1.5
#define LCD_VA_MIN 1.0

#define LCD_VB_MAX 3.0
#define LCD_VB_MIN 2.0

#define LCD_VC_MAX 4.5
#define LCD_VC_MIN 3.0

#define LCD_VD_MAX 6.0
#define LCD_VD_MIN 4.5
float lcd_va, lcd_vb, lcd_vc, lcd_vd;
uint32_t LCDPump_QC(void)
{
  uint32_t i ,cnt ;
#ifdef  CGF_2015_05_23
  swd_connect(0);
#else
  swd_connect(1);
#endif
	SocPumpEna();
	SocLcdIoCfg();    //ADD LCD port configuration to avoid COM/SEG opened
	usDelay(300000);
  adcInit();

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
	cnt = 50 ;
	for( i=0 ;i<cnt ;i++)
	{
	   lcd_va = lcd_va + adcGetValue(ADC_LCDVA);
	   lcd_vb = lcd_vb + adcGetValue(ADC_LCDVB);
	   lcd_vc = lcd_vc + adcGetValue(ADC_LCDVC);
	   lcd_vd = lcd_vd + adcGetValue(ADC_LCDVD);
	}
	lcd_va = lcd_va / cnt * LCDVA_A + LCDVAA ;
	lcd_vb = lcd_vb / cnt * LCDVB_B + LCDVBB ;
	lcd_vc = lcd_vc / cnt * LCDVC_C + LCDVCC ;
	lcd_vd = lcd_vd / cnt * LCDVD_D + LCDVDD ;
	
	if((lcd_va < LCD_VA_MIN) ||
		(lcd_va > LCD_VA_MAX) ||
		(lcd_vb < LCD_VB_MIN) ||
		(lcd_vb > LCD_VB_MAX) ||
		(lcd_vc < LCD_VC_MIN) ||
		(lcd_vc > LCD_VC_MAX) ||
		(lcd_vd < LCD_VD_MIN) ||
		(lcd_vd > LCD_VD_MAX))
	{
	  DBG_ERR("lcd va is %f\n", lcd_va);
		DBG_ERR("lcd vb is %f\n", lcd_vb);
		DBG_ERR("lcd vc is %f\n", lcd_vc);
		DBG_ERR("lcd vd is %f\n", lcd_vd);
	  return 1;
	}
	adcExit() ;

	return 0;
}

#endif



