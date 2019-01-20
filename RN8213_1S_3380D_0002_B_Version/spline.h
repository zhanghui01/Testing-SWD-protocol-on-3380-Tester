/********************************************************************************************
 *    sline0
 *    [F1_L]     [F1_H]     [F2_L]       [F2_H]     [F3_L]     [F3_H]    [F4_L]      [F4_H]
 *    [ANA_ADC]  [ANA_CLK1] [ANA_CLK2]   [ANA_LDO]  [ANA_BGR]  [ANA_PSW] [RES]       [RES]
 *    [TPS_CTRL] [ADC_TEST] [CACHE_CTRL] [RES0]     [RES1]     [RES10]   [CKS_L]     [CKS_H]
*********************************************************************************************/
#ifndef spline_H
#define spline_H

//no patch functions
#define FT_times    0x0
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
#define WAFER_CHIPID            0x08c00026
#define WAFER_SERIAL_1_ADR      0x08c00028
#define WAFER_SERIAL_2_ADR      0x08c0002a
#define WAFER_NUM_ADR           0x08c0002c
#define WAFER_X_ADR             0x08c0002e
#define WAFER_Y_ADR             0x08c00030
#define WAFER_RESULT_ADR        0x08c00032
#define SOFT_CHIPID_NEW_ADR     0x08c00026

#define FT_PASS                 0x5678
/***************************************************************************************************************
 *    Special line1:
 *    [CP0_L] [CP0_H] [PASS_L] [PASS_H] [RTC_MODE] [RTC_DOTA0] [RTC_ALPHAL] [RTC_ALPHAH] [RTC_XT0] [RTC_BETA]
 *    [WDT_CTRL] [EMAP] [WDT_HALT] [WDT_STBY] [RTC_TADJ] [RTC_ZT]...[RES]... [CKS_L] [CHS_H]
 *
 *
***************************************************************************************************************/

//no code protection
#define CP0         0x0
//no password
#define PASSWORD    0x0

#define FLASH_BASE_REMAP3     (0x18000000)                              /*!< (FLASH     ) Base Address */
#define EEPROM_BASE_REMAP3    (0x08000000)                              /*!< (EEPROM    ) Base Address */
#define SRAM_BASE_REMAP3      (0x10000000)                              /*!< (SRAM      ) Base Address */
#define ROM_BASE_REMAP3       (0x00000000)                              /*!< (ROM       ) Base Address */

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

#define EEPROM_SP0_BASE        (0x08c08000)
#define EEPROM_SP1_BASE        (0x08c08040)
#define EEPROM_SP0_OFFSET      (0x00c08000)
#define EEPROM_CMD_OFFSET      (0x00800000)                              /*EEPROM Command Register Offset Address */
#define EEPROM_PGA_OFFSET      (0x00800004)                              /*EEPROM Page Address Register Offset Address */
#define EEPROM_STA_OFFSET      (0x00800008)                              /*EEPROM Status Register Offset Address */
#define EEPROM_TBC0_OFFSET     (0x0080000C)                              /*EEPROM Calibration Register Offset Address */
#define EEPROM_TBC1_OFFSET     (0x00800010)                              /*EEPROM Wait Register Offset Address */
#define EEPROM_PROT_OFFSET     (0x00800014)                              /*EEPROM Port Register Offset Address */
#define EEPROM_TW_OFFSET       (0x00800018)                              /*EEPROM Wait time Register Offset Address */
#define EEPROM_TUS_OFFSET      (0x00800020)                              /*EEPROM us time Register Offset Address */
#define EEPROM_TMS_OFFSET      (0x00800024)                              /*EEPROM ms time Register Offset Address */
#define EEPROM_TP_OFFSET       (0x00800028)                              /*EEPROM Program time Register Offset Address */
#define EEPROM_TE_OFFSET       (0x0080002c)                              /*EEPROM Erase time Register Offset Address */

#define FLASH_CMD_OFFSET      (0x00800000)                              /*Flash Command Register Offset Address */
#define FLASH_PGA_OFFSET      (0x00800004)                              /*Flash Page Address Register Offset Address */
#define FLASH_STA_OFFSET      (0x00800008)                              /*Flash Status Register Offset Address */
#define FLASH_TBC0_OFFSET     (0x0080000C)                              /*Flash Calibration Register Offset Address */
#define FLASH_TBC1_OFFSET     (0x00800010)                              /*Flash Wait Register Offset Address */
#define FLASH_PROT_OFFSET     (0x00800014)                              /*Flash Port Register Offset Address */
#define FLASH_TW_OFFSET       (0x00800018)                              /*Flash Wait time Register Offset Address */
#define FLASH_CC_CTRL_OFFSET  (0x0080001c)                              /*Flash Cache Register Offset Address */
#define FLASH_TUS_OFFSET      (0x00800020)                              /*Flash us time Register Offset Address */
#define FLASH_TMS_OFFSET      (0x00800024)                              /*Flash ms time Register Offset Address */
#define FLASH_TP_OFFSET       (0x00800028)                              /*Flash Program time Register Offset Address */
#define FLASH_TE_OFFSET       (0x0080002c)                              /*Flash Erase time Register Offset Address */


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

/*
extern u8 a_Version_spl0[PAGE_SIZE] = {
    F1 & 0xff,        (F1 >> 8) & 0xff,        (F1 >> 16) & 0xff, (F1 >> 24) & 0xff,
    F2 & 0xff,        (F2 >> 8) & 0xff,        (F2 >> 16) & 0xff, (F2 >> 24) & 0xff,
    F3 & 0xff,        (F3 >> 8) & 0xff,        (F3 >> 16) & 0xff, (F3 >> 24) & 0xff,
    F4 & 0xff,        (F4 >> 8) & 0xff,        (F4 >> 16) & 0xff, (F4 >> 24) & 0xff,
    ANAADC & 0xff,    (ANAADC >> 8) & 0xff,    ANACLK1 & 0xff,    (ANACLK1 >> 8) & 0xff,
    ANACLK2 & 0xff,   (ANACLK2 >> 8) & 0xff,   ANALDO & 0xff,     (ANALDO >> 8) & 0xff,
    ANABGR & 0xff,    (ANABGR >> 8) & 0xff,    ANAPSW & 0xff,     (ANAPSW >> 8) & 0xff,
    0xff,             0xff,                    0xff,              0xff,
    TPSCTRL & 0xff,   (TPSCTRL >> 8) & 0xff,   ADCTEST & 0xff,    (ADCTEST >> 8) & 0xff,
    CACHECTRL & 0xff, (CACHECTRL >> 8) & 0xff, 0xff,              0xff,
    0xff,             0xff,                    0xff,              0xff,
    0xff,             0xff,                    0xff,              0xff,
    0xff,             0xff,                    0xff,              0xff,
    0xff,             0xff,                    0xff,              0xff,
    0xff,             0xff,                    0xff,              0xff,
    0xff,             0xff,                    0xff,              0xff
};

extern u8 d_Version_spl0[PAGE_SIZE] = {
      F1 & 0xff,        (F1 >> 8) & 0xff,        (F1 >> 16) & 0xff, (F1 >> 24) & 0xff,
      F2 & 0xff,        (F2 >> 8) & 0xff,        (F2 >> 16) & 0xff, (F2 >> 24) & 0xff,
      F3 & 0xff,        (F3 >> 8) & 0xff,        (F3 >> 16) & 0xff, (F3 >> 24) & 0xff,
      F4 & 0xff,        (F4 >> 8) & 0xff,        (F4 >> 16) & 0xff, (F4 >> 24) & 0xff,
      ANAADC & 0xff,    (ANAADC >> 8) & 0xff,    ANACLK1 & 0xff,    (ANACLK1 >> 8) & 0xff,
      ANACLK2_B & 0xff,   (ANACLK2_B >> 8) & 0xff,   ANALDO_B & 0xff,     (ANALDO_B >> 8) & 0xff,
      ANABGR_B & 0xff,    (ANABGR_B >> 8) & 0xff,    ANAPSW_B & 0xff,     (ANAPSW_B >> 8) & 0xff,
      0xff,             0xff,                    0xff,              0xff,
      TPSCTRL & 0xff,   (TPSCTRL >> 8) & 0xff,   ADCTEST & 0xff,    (ADCTEST >> 8) & 0xff,
      CACHECTRL & 0xff, (CACHECTRL >> 8) & 0xff, SOFT_CHIPID_B & 0xff,            (SOFT_CHIPID_B >> 8) &  0xff,
      0xff,             0xff,                    0xff,              0xff,
      0xff,             0xff,                    0xff,              0xff,
      0xff,             0xff,                    0xff,              0xff,
      0xff,             0xff,                    0xff,              0xff,
      0xff,             0xff,                    0xff,              0xff,
      0xff,             0xff,                    0xff,              0xff
};
*/




u32 trim_ldo18;
u32 trim_ANALDO;
u32 trim_rcl;

u32  ldo18_trim_write(u32 trim);
u32  LDO18_TRIM(void);
u32  rcl_trim(void);
u32  UID_adr_read(u32 address);
u32  sumGen32(u32* p_data,u32 length);
u32  sp0LinePro(void);
#endif