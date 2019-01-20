/**************************************************************************//**
 * @file     system_RN821x.c
 * @brief    CMSIS Cortex-M0 Device Peripheral Access Layer Source File
 *           for the RN821x CM0 Device
 * @version  V1.00
 * @date     14. APR 2011
 *
 * @note
 * Copyright (C) REnergy Micro-Electronics Co.,LTD.
 *
 *
 ******************************************************************************/


#include <stdint.h>
#include "rn821x.h"


/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/
    
/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define __HSI             (1843200UL)
#define __XTAL            (1843200UL)    /* Oscillator frequency             */
#define __SYS_OSC_CLK     (    ___HSI)    /* Main oscillator frequency        */

#define __SYSTEM_CLOCK    (1*__XTAL)

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
/* ToDo: initialize SystemCoreClock with the system core clock frequency value
         achieved after system intitialization.
         This means system core clock frequency after call to SystemInit()    */
uint32_t SystemCoreClock = __SYSTEM_CLOCK;  /*!< System Clock Frequency (Core Clock)*/


/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
/* ToDo: add code to calculate the system frequency based upon the current
         register settings.
         This function can be used to retrieve the system core clock frequeny
         after user changed register sittings.                                */
  SystemCoreClock = __SYSTEM_CLOCK;
  switch ((SYSCTL->OSC_CTRL1 >> 8) & 0x7)
  {
    case 0:
      SystemCoreClock = 7372800;
      break;
    case 1:
      SystemCoreClock = 3686400;
      break;
    case 2:
      SystemCoreClock = 1843200;
		  break;
    case 3:
      SystemCoreClock = 32768;
      break;
		default:
			SystemCoreClock = 1843200;
      break;
  }
}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void)
{
/* ToDo: add code to initialize the system
         do not use global variables because this function is called before
         reaching pre-main. RW section maybe overwritten afterwards.          */  
  //SystemCoreClock = __SYSTEM_CLOCK;
  
  return;	//chengaofei add
}
