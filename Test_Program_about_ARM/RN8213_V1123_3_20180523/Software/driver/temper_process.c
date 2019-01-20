
#include "ads_acquire.h"
#include "spline.h"
#include "spi.h"
#include "ra0_routine.h"
#include "drv.h"

/*********************************************************************************************************
** Function name:       tempMeasure
** Descriptions:        Temper Measure
** input parameters:    None
** output parameters:   None
** Returned value:      None
*********************************************************************************************************/
//uint32_t tempProcess(void)
uint32_t tempProcess(void)
{
  uint16_t std_temper;
//  uint32_t result ;
	
	//Timer3 Initial
  timer3Init();	
	
	swd_num = SWD_PARALLEL;  // CHECK SWD
	
  stdInitial();
 
	std_temper = stdTempAcquire();
	if(std_temper == 0xffff )
	{
	std_temper = 100;
	}
// *    SWDCLK:  P1.25
// *    SWDIO :  P1.26
//  __set_swd_pio_as_in();
//  __set_swd_pclk_as_in();	
	
	swd_num = SWD_SERIAL;
	
  if(swdInitial(SWD_SERIAL) == FAIL)  // DUT SWD
	{
		return 1;
	}
	
#if 1	
  if(ra0TempAdjust(std_temper) == FAIL)
  {
    exitFail(TEMPER_ERR) ;
    return 1;
  }
#endif
	
  return 0;
}
