#include "drv.h"

uint8_t ROM_FT( void )
{
  uint32_t  *rom_addr;
  uint32_t  remap, i, sum ;	
	
  remap = (SYSCTL->MAP_CTRL & 0x3);
	
	rom_addr = (uint32_t *)((remap == 3) ? 0x0 : 0x18000000);
	sum = 0 ;
	for(i = 0; i < 2048; i++)
	{
		sum += rom_addr[i];
	}
	
	if( (sum != ROM_CHKSUM_A1) && (sum != ROM_CHKSUM_A2) && (sum != rom_chksum_D0) )
	{
	  return FAIL ;
	}
		
  return SUCCESS ;	
	
	
}








