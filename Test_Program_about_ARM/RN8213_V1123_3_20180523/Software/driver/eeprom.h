/*
 * Special line0:
 *    [F1_L]     [F1_H]     [F2_L]       [F2_H]     [F3_L]     [F3_H]    [F4_L]      [F4_H] 
 *    [ANA_ADC]  [ANA_CLK1] [ANA_CLK2]   [ANA_LDO]  [ANA_BGR]  [ANA_PSW] [RES]       [RES]
 *    [TPS_CTRL] [ADC_TEST] [CACHE_CTRL] [RES0]     [RES1]    ............                   
 *    .........................................................[RES10]   [CKS_L]     [CKS_H]
 */
 
//no patch functions
#define F1          0x0
#define F2          0x0
#define F3          0x0
#define F4          0x0

//A°æ
//Reset Value
#define ANAADC      0x0040
#define ANACLK1     0x0420
#define ANACLK2     0x0014
#define ANALDO      0x0058
#define ANABGR      0x615a   //20160729change from 0x6158 to 0x615a(see GAOFEI's Email)
#define ANAPSW      0x0010
#define TPSCTRL     0x193b

//Set Value
#define ADCTEST     0x0028          //Bias Current = 16uA. Dem On.
#define CACHECTRL   0x0001          //Cache Enable

//B°æ
#define ANACLK2_B     0x4E14
#define ANALDO_B      0x0850
#define ANABGR_B      0x615A
#define ANAPSW_B      0x0010
#define SOFT_CHIPID_B 0X000B     //Ìí¼ÓB°æ±¾ºÅ

//UID code addr
#define WAFER_CHIPID 						0x08c00026
#define WAFER_SERIAL_1_ADR     0x08c00028
#define WAFER_SERIAL_2_ADR     0x08c0002a
#define WAFER_NUM_ADR						0x08c0002c
#define WAFER_X_ADR							0x08c0002e
#define WAFER_Y_ADR							0x08c00030
#define WAFER_RESULT_ADR				0x08c00032
#define SOFT_CHIPID_NEW_ADR			0x08c00026

#define FT_PASS    0x5678
/*
 * Special line1:
 *    [CP0_L] [CP0_H] [PASS_L] [PASS_H] [RTC_MODE] [RTC_DOTA0] [RTC_ALPHAL] [RTC_ALPHAH] [RTC_XT0] [RTC_BETA]
 *    [WDT_CTRL] [EMAP] [WDT_HALT] [WDT_STBY] [RTC_TADJ] [RTC_ZT]...[RES]... [CKS_L] [CHS_H] 
 *
 *
 */

//no code protection
#define CP0         0x0
//no password
#define PASSWORD    0x0


#define RTCMODE     0x1
#define RTCDOTA0    0x0    //Must be adjust for every IC.
//#define RTCALPHAL   1028      //round(0.031358*2048*16)
//#define RTCALPHAH   1225     //round(0.037387*2048*16) 
#define RTCALPHAL   1006      //round(0.030691*2048*16)
#define RTCALPHAH   1231     //round(0.037555*2048*16) 
#define RTCXT0      100      //round(25*4)
#define RTCBETA     0xff     //BEAT = 1

//close watch dog
#define WDTCTRL     0x68
#define EMAP        0x0      //EEPROM NOT SET AS FLASH.
#define WDTHALT    0x7e5a
#define WDTSTBY    0xa538

#define RTCTADJ     1642            //Must be adjust for every IC.
#define RTCZT       278             //Must be adjust for every block of IC.


#define SPL0_ADDR 0x08c08000
#define SPL1_ADDR 0x08c08040

#define PAGE_SIZE 0x40

#define ECMD_IDLE 0xfeed0000
#define ECMD_TBC  0xfeed0002
#define ECMD_POR  0xfeed0003
#define ECMD_HVPL 0xfeed0004
#define ECMD_LPS1 0xfeed0006
#define ECMD_LPS2 0xfeed0007
#define ECMD_CE   0xfeed0009
#define ECMD_SE   0xfeed000a
#define ECMD_PE   0xfeed000b
#define ECMD_CP   0xfeed000c
#define ECMD_OECP 0xfeed000d
#define ECMD_OESP 0xfeed000e
#define ECMD_PP   0xfeed000f

#define EE_SPL0_PGA 0x300
#define EE_SPL1_PGA 0x301

#define EEPROM_TP            1
#define EEPROM_TE            2
#define EEPROM_TMS_CHIP      932
#define EEPROM_TMS_PAGE      832

#define FLASH_TP             3
#define FLASH_TE             6
#define FLASH_TMS_CHIP       970
#define FLASH_TMS_PAGE       928

#define TW_7M  (0x2 << 16)
#define TW_3M  (0x0 << 16)
#define TW_1M  (0x0 << 16)
#define TW_32K (0x0 << 16)
#define TW_14M (0x2 << 16)
#define TW_29M (0x5 << 16)

#define TUS_7M  7
#define TUS_3M  3
#define TUS_1M  1
#define TUS_32K 1
#define TUS_14M 15
#define TUS_29M 31
