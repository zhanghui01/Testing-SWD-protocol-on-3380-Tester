#include <uti\cpin.h>
#include <uti\cpingrp.h>
#include <uti\ctimemain.h>
#include <uti\clevel.h>
#include <uti\clcdlevel.h>
#include <uti\alpgconst.h>
#include <uti\plnlib.h>
#include <uti\stntype.h>
/* PIN Declare */
CPIN STNTYPE VCC;
CPIN STNTYPE SWDCLK;
CPIN STNTYPE SWDIO;
CPIN STNTYPE AVDD;
CPIN STNTYPE VREF;
CPIN STNTYPE VBAT;
CPIN STNTYPE VSWO;
CPIN STNTYPE DVDD;

/*PINGROUP Declare*/
CPINGRP STNTYPE MLDPS_ALLPINS;
CPINGRP STNTYPE IO_ALLPINS;
CPINGRP STNTYPE SWD_PINS;
CPINGRP STNTYPE LDO_PINS;

/*LEVEL SET in pln*/
CLEVEL STNTYPE rst_lvl;
CLEVEL STNTYPE OS_lvl;
CLEVEL STNTYPE UID_lvl;
CLEVEL STNTYPE LDO_POR_lvl;
CLEVEL STNTYPE PSW_lvl;
CLEVEL STNTYPE CLK_lvl;
CLEVEL STNTYPE LCD_lvl;
CLEVEL STNTYPE SAR_ADC_lvl;
CLEVEL STNTYPE COMP_BOR_lvl;
CLEVEL STNTYPE WNVR_lvl;
CLEVEL STNTYPE SCAN_lvl;
CLEVEL STNTYPE SRAM_lvl;
CLEVEL STNTYPE POWER_lvl;
CLEVEL STNTYPE GPIO_lvl;
CLEVEL STNTYPE EMU_lvl;
CLEVEL STNTYPE RTC_lvl;
