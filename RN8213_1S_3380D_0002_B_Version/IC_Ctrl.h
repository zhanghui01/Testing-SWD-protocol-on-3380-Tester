#ifndef IC_Ctrl_H
#define IC_Ctrl_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <io.h>
#include <time.h>

#define DUT_COUNT 4
#define PRINT     1
#define POWER_VOL 5

#define DP_IDCODE           0x0BB11477
#define Chip_ID             0x00008215
#define CP_UID              0x1234
#define A_Version           0x0020EA00
#define D_Version           0x0200EA00
#define GF3208_CMD_FF       0xC4
#define GF3208_CMD_SLEEP    0xC8
#define GF3208_CMD_NAV      0xCA
#define GF3208_CMD_TMR_TRG  0xA1
#define GF3208_CMD_ADC_TST  0xA2



typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char  u8;


extern long int   Active_Site;
extern int LDO18_trim_flag[DUT_COUNT] = {0};     //0 mean no need trim, 1 need trim,for DVDD PIN
extern int vih_trim_flag[DUT_COUNT]   = {0};     //0 mean no need trim, 1 need trim,for power swicth vih value



void global_var_Reset(void);
void POWER_ON(double volt);
void POWER_OFF(void);
void RESET_SWD(void);
void swd_rate(double rate);
u32  write_word(u8 Head_data,u32 reg_data);
void capture_word(u8 Head_data,u32* get_data);
void Compare_word(u8 Head_data,u32 reg_data);
void Judge_A_Or_D_Version(u32* data);
#endif