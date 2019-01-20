#ifndef __EEPROM_FT_H
#define __EEPROM_FT_H

uint32_t   ee_chip_check_empty( void ) ;
uint32_t   ee_chip_check_data( uint8_t *data ) ;
uint32_t   ee_page_check_empty( uint32_t pg )  ;
uint32_t   ee_page_check_data( uint32_t pg, uint8_t *data ) ;
uint8_t    EEPROM_FT( void ) ;

#endif










