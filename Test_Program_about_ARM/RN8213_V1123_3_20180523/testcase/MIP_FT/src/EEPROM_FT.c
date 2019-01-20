#include "drv.h"

uint8_t EEPROM_FT( void )
{ 
  uint32_t  i;
  uint8_t   data[EE_PAGE_SIZE] ;
	
	for(i = 0; i < EE_PAGE_SIZE; i++)
  {
    data[i] = i; 
  }
    
  /* 1. EEPROM CHIP ERASE */
  ee_chip_erase();
  if(ee_chip_check_empty())
  {
		return FAIL ;
  }
  
  /* 2. EEPROM CHIP PROG */
  ee_chip_prog(data);
  if(ee_chip_check_data(data))
  {
		ee_chip_erase();
		return FAIL ;
  }
   
  ee_chip_erase();	
  return SUCCESS ;    

}


uint32_t ee_chip_check_empty(void)
{
  uint32_t i;
	uint32_t ee_addr;
	
	ee_addr = ((SYSCTL->MAP_CTRL & 0x3) == 0x1) ? 0x0 : 0x08000000;
	
  for(i = 0; i < EE_CHIP_SIZE / 4; i++)
  {
		WdtFeed();
    if(*(uint32_t *)(ee_addr + 4 * i) != 0x0)
    {
      return 1;
    }
  }
  
  return 0;
}

uint32_t ee_chip_check_data(uint8_t *data)
{
  uint32_t i, j ;
	uint8_t  flag ;
	uint32_t ee_addr;
	
	ee_addr = ((SYSCTL->MAP_CTRL & 0x3) == 0x1) ? 0x0 : 0x08000000;
	flag = 0 ;
  
  for(i = 0; i < (EE_CHIP_SIZE / EE_PAGE_SIZE); i++)
  {
    WdtFeed();
    
    for(j = 0; j < EE_PAGE_SIZE; j++)
    {
      if(*(uint8_t *)(ee_addr + i * EE_PAGE_SIZE + j) != data[j])
      {
				flag = 1 ;
				return flag ;
      }
    }
  }
  
  return flag;
}

uint32_t ee_page_check_empty(uint32_t pg)
{
  uint32_t i;
	uint32_t ee_addr;
	
	ee_addr = ((SYSCTL->MAP_CTRL & 0x3) == 0x1) ? 0x0 : 0x08000000;
	
  for(i = 0; i < EE_PAGE_SIZE; i++)
  {
    if(*(uint8_t *)(ee_addr + pg * EE_PAGE_SIZE + i) != 0x0)
    {
      return 1;
    }
  }
  
  WdtFeed();
  
  return 0;
}

uint32_t ee_page_check_data(uint32_t pg, uint8_t *data)
{
  uint32_t i;
	uint32_t ee_addr;
	
	ee_addr = ((SYSCTL->MAP_CTRL & 0x3) == 0x1) ? 0x0 : 0x08000000;
  
  for(i = 0; i < EE_PAGE_SIZE; i++)
  {
    if(*(uint8_t *)(ee_addr + pg * EE_PAGE_SIZE + i) != data[i])
    {
      return 1;
    }
  }
  
  WdtFeed();
  
  return 0;
}
