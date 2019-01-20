#ifndef __ANALOG_TESTCASE__H
#define __ANALOG_TESTCASE_H

#include "swd_atf.h"

const s_atf_img analog_atf_tbl[]={ 
//   {2, 0, 0, "EMU_FT", 2000, (unsigned char *)EMU_FT},
	{2, 0, 0, "LCDPump_QC", 2000, (unsigned char *)LCDPump_QC},
  {2, 0, 0, "tempProcess", 3000, (unsigned char *)tempProcess},
  {2, 0, 0, "Voltage_QC", 3000, (unsigned char *)Voltage_QC},
	{2, 0, 0, "SAR_QC", 3000, (unsigned char *)SAR_QC},
};

#endif
