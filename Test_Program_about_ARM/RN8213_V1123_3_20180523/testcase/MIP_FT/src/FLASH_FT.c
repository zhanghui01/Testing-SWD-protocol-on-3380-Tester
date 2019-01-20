#include "drv.h"

#ifdef FL_PAGE_SIZE
  #undef FL_PAGE_SIZE
  #define  FL_PAGE_SIZE     128
#endif

#define  FL_CHIP_SIZE     (192 * 1024)


uint8_t FLASH_FT(void)
{ 
  uint32_t  i;
  uint8_t   data[FL_PAGE_SIZE] ;
	
	for(i = 0; i < FL_PAGE_SIZE; i++)
  {
    data[i] = i; 
  }
    
  /* 1. FLASH CHIP ERASE */
  fl_chip_erase();
  if(fl_chip_check_empty())
  {
		return FAIL ;
  }
  
  /* 2. FLASH CHIP PROG */
  fl_chip_prog(data);
  if(fl_chip_check_data(data))
  {
		fl_chip_erase();
		return FAIL ;
  }
	
  fl_chip_erase();  
	return SUCCESS ;

}



uint32_t fl_chip_check_empty(void)
{
  uint32_t i;
	uint32_t fl_addr;
	
	fl_addr = SYSCTL->MAP_CTRL & 0x3;
	fl_addr = (fl_addr == 0x0) ? 0x0 : (fl_addr == 0x1) ? 0x08000000 : (fl_addr == 0x2) ? 0x10000000 : 0x18000000;
	
  for(i = 0; i < FL_CHIP_SIZE / 4; i++)
  {
		WdtFeed();
    if(*(uint32_t *)(fl_addr + 4 * i) != 0x0)
    {
      return 1;
    }
  }
  
  return 0;
}

uint32_t fl_chip_check_data(uint8_t *data)
{
  uint32_t i, j ;
	uint8_t  flag ;
	uint32_t fl_addr;
	
	fl_addr = SYSCTL->MAP_CTRL & 0x3;
	fl_addr = (fl_addr == 0x0) ? 0x0 : (fl_addr == 0x1) ? 0x08000000 : (fl_addr == 0x2) ? 0x10000000 : 0x18000000;
	flag = 0 ;
  
  for(i = 0; i < (FL_CHIP_SIZE / FL_PAGE_SIZE); i++)
  {
    WdtFeed();
    
    for(j = 0; j < FL_PAGE_SIZE; j++)
    {
      if(*(uint8_t *)(fl_addr + i * FL_PAGE_SIZE + j) != data[j])
      {
				flag = 1 ;
				return flag ;
      }
    }
  }
  
  return flag;
}

uint32_t fl_page_check_empty(uint32_t pg)
{
  uint32_t i;
	uint32_t fl_addr;
	
	fl_addr = SYSCTL->MAP_CTRL & 0x3;
	fl_addr = (fl_addr == 0x0) ? 0x0 : (fl_addr == 0x1) ? 0x08000000 : (fl_addr == 0x2) ? 0x10000000 : 0x18000000;
	
  for(i = 0; i < FL_PAGE_SIZE; i++)
  {
    if(*(uint8_t *)(fl_addr + pg * FL_PAGE_SIZE + i) != 0x0)
    {
      return 1;
    }
  }
  
  WdtFeed();
  
  return 0;
}

uint32_t fl_page_check_data(uint32_t pg, uint8_t *data)
{
  uint32_t i;
	uint32_t fl_addr;
	
	fl_addr = SYSCTL->MAP_CTRL & 0x3;
	fl_addr = (fl_addr == 0x0) ? 0x0 : (fl_addr == 0x1) ? 0x08000000 : (fl_addr == 0x2) ? 0x10000000 : 0x18000000;
  
  for(i = 0; i < FL_PAGE_SIZE; i++)
  {
    if(*(uint8_t *)(fl_addr + pg * FL_PAGE_SIZE + i) != data[i])
    {
      return 1;
    }
  }
  
  WdtFeed();
  
  return 0;
}
