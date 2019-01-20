#ifndef __BOARD_H
#define __BOARD_H

extern uint32_t  powswh_bin;
//signals from machine:
//  01 : clear nvm command
//  10 : start full function test
//  11 : start low power test only
#define START_TEST    	0x3
#define PORSW_BIN1  		0x1
#define PORSW_BIN2  		0x2
#define START_LCD3V3  	0x3
#define START_FUNTION 	0x1

#define START_LP      	0x2
#define START_DISLP   	0x1//0x1
#define START_CLRNVM  	0x3

#define START_SAR  			0x2	//no use
#define START_SP0     	0x2//0x1
// void swDelay(uint32_t i);
void usDelay(uint32_t us);
void msDelay(uint32_t ms);
void exitFail(uint32_t err_num) ;
void exitPass(void) ;
void Ini_Test(void) ;

void finishTest(void);

uint8_t wait_start(void);
uint8_t wait_LCD3V3(void);
uint8_t wait_SAR(void);
uint8_t wait_FUNTION(void);
uint8_t wait_clrmip(void);

void exit_UID_check_PASS(void);
void exit_SP0_Pass(void);
void exit_LCD3V3_Pass(void);
void exit_SAR_Pass(void);
void exit_FUNTION_Pass(void);

uint32_t empty_check_fl(void);
uint32_t empty_check_ee(void);
uint8_t wait_SP0(void);
uint8_t wait_DISLowpwr(void);
#endif
