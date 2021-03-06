#include "drv.h"

void FT_Result( uint32_t result ) ;


int32_t main(void)
{
	uint8_t  result ;
  uint32_t * sram_res;
  uint32_t remap ;

  SYSCTL->SYS_PS = 0x82;
  SYSCTL->OSC_CTRL2 = 0x2;
  SysModeChg(3, 0);
	
  remap = (SYSCTL->MAP_CTRL & 0x3);
	sram_res = (uint32_t *)(((remap == 2) ? 0x0 : 0x10000000) + RES_OFFSET);
	*sram_res = TEST_START;
			
	result = ROM_FT() ;
	if( result != SUCCESS )
	{
		FT_Result(TEST_FAIL) ;
	}
		
// 	result = FLASH_FT() ;
//   if( result != SUCCESS )
// 	{
//     FT_Result(TEST_FAIL) ;
// 	}
	
// 	result = EEPROM_FT() ;
//   if( result != SUCCESS )
// 	{
//     FT_Result(TEST_FAIL) ;
// 	}
	
	FT_Result(TEST_PASS) ;	
	while(1)
	  WdtFeed() ;		
}

void FT_Result( uint32_t result )
{
  uint32_t  * sram_res ;
  uint32_t  remap ;
  
  remap    = (SYSCTL->MAP_CTRL & 0x3) ;
	sram_res = (uint32_t *)(((remap == 2) ? 0x0 : 0x10000000) + RES_OFFSET) ;
	
  *sram_res = result;
	if( result == TEST_FAIL )
	{
		while(1)
			WdtFeed() ;	
	}
	
}
















