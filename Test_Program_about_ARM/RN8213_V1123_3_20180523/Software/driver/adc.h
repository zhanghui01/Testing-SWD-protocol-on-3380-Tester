#ifndef __ADC_H
#define __ADC_H

typedef enum
{
  ADC_VSWO = 5,
  ADC_LCDVA = 7,
  ADC_LCDVB = 6,
  ADC_LCDVC = 3,
  ADC_LCDVD = 2
}
ADC_CH;

// #define  ADC_VREF   3.204  //for #1 board
// #define  ADC_VREF   3.209  //for #2 board
void    adcInit(void);
float   adcGetValue(ADC_CH ch);
void    adcExit(void);
uint32_t SocPump_21Ena(void);
uint32_t SocPump_31Ena(void);
uint32_t Normal_pwr_init (void);
#ifdef   CGF_2015_05_23
uint32_t SocPumpDis(void)  ;
#endif
uint32_t LCDPump_QC(void);
uint32_t SocLcdIoCfg( void );
#endif
