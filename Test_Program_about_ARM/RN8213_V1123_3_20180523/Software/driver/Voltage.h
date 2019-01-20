#ifndef __VOLTAGE_H
#define __VOLTAGE_H

uint32_t Voltage_QC(void) ;
uint32_t ADS1243GetValue( uint8_t CHAN ) ;
uint32_t SocAVDDEna(void) ;
uint32_t SocVrefEna(void) ;
uint32_t SocVrefDis(void) ;

#endif


