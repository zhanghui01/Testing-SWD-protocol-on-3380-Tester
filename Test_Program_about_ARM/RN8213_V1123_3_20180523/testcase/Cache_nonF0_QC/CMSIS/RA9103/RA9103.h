/**************************************************************************//**
 * @file     RA9103D0.h
 * @brief    CMSIS Cortex-M0 Core Peripheral Access Layer Header File
 *           for RA9103D0 CM0 Device
 * @version  V1.00
 * @date     28. Sep 2012
 *
 * @note
 * Copyright (C) REnergy Micro-Electronics Co.,LTD.
 *
 *
 ******************************************************************************/


#ifndef __RA9103F0_H__
#define __RA9103F0_H__

#ifdef __cplusplus
 extern "C" {
#endif 
/** @addtogroup RA9103D0_Definitions RA9103D0 Definitions
  This file defines all structures and symbols for RA9103D0:
    - registers and bitfields
    - peripheral base address
    - peripheral ID
    - Peripheral definitions
  @{
*/

/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/
/** @addtogroup RA9103D0_CMSIS Device CMSIS Definitions
  Configuration of the Cortex-M0 Processor and Core Peripherals
  @{
*/



/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */
typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                             */
  HardFault_IRQn              = -13,    /*!< 3 Cortex-M0 Hard Fault Interrupt                     */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M0 SV Call Interrupt                       */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M0 Pend SV Interrupt                       */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M0 System Tick Interrupt                   */

/******  RA9103D0 Specific Interrupt Numbers ********************************************************/
  SYSCLKCAL_IRQn               = 0,      /*!<  16 system clock calculation interrupt              */
  LVDCMP_IRQn                  = 1,      /*!<  17 Voltage compare Interrupt                       */
  //VCH_IRQn                     = 2,      /*!<  18 pwoer switch Interrupt                          */
  RTC_IRQn                     = 3,      /*!<  19 RTC Interrupt                                   */
  //EMU_IRQn                     = 4,      /*!<  19 EMU Interrupt                                   */
  SARADC_IRQn                  = 5,      /*!<  21 Mux. ADC Interrupt                              */
  UART0_IRQn                   = 6,      /*!<  22 UART0 Interrupt                                 */
  UART1_IRQn                   = 7,      /*!<  23 UART1 Interrupt                                 */
  UART2_IRQn                   = 8,      /*!<  24 UART2 Interrupt                                 */
  UART3_IRQn                   = 9,      /*!<  25 UART3 Interrupt                                 */
  SPI_IRQn                     = 10,     /*!<  26 SPI Interrupt                                   */
  I2C_IRQn                     = 11,     /*!<  27 IIC Interrupt                                   */
  ISO78160_IRQn                = 12,     /*!<  28 ISO7816 0 Interrupt                             */
  ISO78161_IRQn                = 13,     /*!<  29 ISO7816 1 Interrupt                             */
  TC0_IRQn                     = 14,     /*!<  30 Timer/Counter 0 Interrupt                       */
  TC1_IRQn                     = 15,     /*!<  31 Timer/Counter 1 Interrupt                       */
  UART4_IRQn                   = 16,     /*!<  31 UART4 Interrupt                                 */
  UART5_IRQn                   = 17,     /*!<  33 UART5 Interrupt                                 */
  WDT_IRQn                     = 18,     /*!<  34 Watch Dog Interrupt                             */
  KBI_IRQn                     = 19,     /*!<  35 Key Interrupt                                   */
  LCD_IRQn                     = 20,     /*!<  36 LCD Interrupt                                   */
  SYS_TC_IRQn                  = 21,     /*!<  39 system tick Interrupt                           */
	//DMA_IRQn                     = 22,  
  EXT0_IRQn                    = 24,     /*!<  40 Extern0 Interrupt                               */
  EXT1_IRQn                    = 25,     /*!<  41 Extern1 Interrupt                               */
  EXT2_IRQn                    = 26,     /*!<  42 Extern2 Interrupt                               */
  EXT3_IRQn                    = 27,     /*!<  43 Extern3 Interrupt                               */
  EXT4_IRQn                    = 28,     /*!<  44 Extern4 Interrupt                               */
  EXT5_IRQn                    = 29,     /*!<  45 Extern5 Interrupt                               */
  EXT6_IRQn                    = 30,     /*!<  46 Extern6 Interrupt                               */ 
  EXT7_IRQn                    = 31,     /*!<  47 Extern7 Interrupt                               */
} IRQn_Type;

/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M0 Processor and Core Peripherals */
#define __MPU_PRESENT             0         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */

#if 0
#if defined ( __CC_ARM   )
  #if defined (__TARGET_FPU_VFP)
    #define __FPU_PRESENT         1         /*!< FPU present or not                               */
  #else
    #define __FPU_PRESENT         0         /*!< FPU present or not                               */
  #endif
#else
    #define __FPU_PRESENT         0         /*!< FPU present or not                               */
#endif
#endif

/*@}*/ /* end of group RA9103D0_CMSIS */

#include "core_cm0.h"                       /* Cortex-M0 processor and core peripherals           */
#include "system_RA9103.h"                /* RA9103D0 System  include file                      */


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/
/** @addtogroup RA9103D0_Peripherals RA9103D0 Peripherals
  RA9103D0 Device Specific Peripheral registers structures
  @{
*/

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/*--------------------- GPIO --------------------------------------------------*/
typedef struct {
  __IO uint32_t PMA;               /* offset  0x0          */
  __IO uint32_t PA;                /* offset  0x4          */
  __IO uint32_t PCA0;              /* offset  0x8          */
  __IO uint32_t PCA1;              /* offset  0xc          */
  __IO uint32_t PUA;               /* offset  0x10         */
  __IO uint32_t PIMA;              /* offset  0x14         */
  __IO uint32_t PIEA;              /* offset  0x18         */
  __IO uint32_t PMB;               /* offset  0x1c         */
  __IO uint32_t PB;                /* offset  0x20         */
  __IO uint32_t PCB;               /* offset  0x24         */
  __IO uint32_t PUB;               /* offset  0x28         */
  __IO uint32_t PIMB;              /* offset  0x2c         */
  __IO uint32_t PIEB;              /* offset  0x30         */
  __IO uint32_t PMC;               /* offset  0x34         */
  __IO uint32_t PC;                /* offset  0x38         */
  __IO uint32_t PCC;               /* offset  0x3c         */
  __IO uint32_t PUC;               /* offset  0x40         */
  __IO uint32_t PIEC;              /* offset  0x44         */
  __IO uint32_t PIMC;              /* offset  0x48         */
  __IO uint32_t PCB2;              /* offset  0x4c         */
  __IO uint32_t RESERVED[4];       /* offset  0x50~0x5c    */
  __IO uint32_t PCE;               /* offset  0x60         */
  __IO uint32_t PASET;             /* offset  0x64         */
  __IO uint32_t PACLR;             /* offset  0x68         */
  __IO uint32_t PBSET;             /* offset  0x6c         */
  __IO uint32_t PBCLR;             /* offset  0x70         */
  __IO uint32_t PCSET;             /* offset  0x74         */
  __IO uint32_t PCCLR;             /* offset  0x78         */
} GPIO_TypeDef;

/*--------------------- FLASH --------------------------------------------------*/
typedef struct {
  __IO uint32_t CMD_1;               /* offset 0x00  command           */
  __IO uint32_t PGA_1;               /* offset 0x04  page address      */
  __IO uint32_t STA_1;               /* offset 0x08  status            */
  __IO uint32_t TBC0_1;              /* offset 0x0c  trim byte 0       */
  __IO uint32_t TBC1_1;              /* offset 0x10  trim byte 1       */
  __IO uint32_t PROT_1;              /* offset 0x14  sector protect bit*/  
  __IO uint32_t RESERVED1[2];        /* offset 0x18~0x1c */
  
  __IO uint32_t CMD_2;               /* offset 0x20  command           */
  __IO uint32_t PGA_2;               /* offset 0x24  page address      */
  __IO uint32_t STA_2;               /* offset 0x28  status            */
  __IO uint32_t TBC0_2;              /* offset 0x2c  trim byte 0       */
  __IO uint32_t TBC1_2;              /* offset 0x30  trim byte 1       */
  __IO uint32_t PROT_2;              /* offset 0x34  sector protect bit*/  
  __IO uint32_t RESERVED2[2];        /* offset 0x38~0x3c */
  
  __IO uint32_t TW;                  /* offset 0x40  wait cycles bit*/  
  __IO uint32_t TUS;                 /* offset 0x44  us unit */
  __IO uint32_t TMS;                 /* offset 0x48  ms unit */
  __IO uint32_t TP;                  /* offset 0x4c  program time */
  __IO uint32_t TE;                  /* offset 0x50  erase time */
  __IO uint32_t CFG;                 /* offset 0x54  flash config */
  __IO uint32_t RESERVED3[2];        /* offset 0x58~0x5c */
  
  __IO uint32_t CC_CTRL;             /* offset 0x60  cache contrl */
  __IO uint32_t CC_HIT_LO;           /* offset 0x64 cache hit counter low word */
  __IO uint32_t CC_HIT_HI;           /* offset 0x68 cache hit counter high word */
  __IO uint32_t CC_INST_LO;          /* offset 0x6c cache inst counter low word */
  __IO uint32_t CC_INST_HI;          /* offset 0x70 cache inst counter high word */
} FLASH_TypeDef;


/*--------------------- CT --------------------------------------------------*/
typedef struct {
  __IO uint32_t CTRL      ;             /* offset 0x0          CT control register            */
  __IO uint32_t IN1[6]    ;             /* offset 0x4~0x1c     CT data input register1        */
  __IO uint32_t IN2[6]    ;             /* offset 0x20~0x3c    CT data input register2        */
  __IO uint32_t RES[12]   ;             /* offset 0x0x40~0x70  CT data output register        */
} CT_TypeDef;

typedef struct {                                        
  __IO uint32_t CTRL      ;             /* offset  0x0         AES ENC control register        */
  __IO uint32_t STAT      ;             /* offset  0x4         AES ENC status register         */
  __IO uint32_t RESERVED[2];            /* offset  0x8~0xc     RESERVED address                */
  __IO uint32_t KEY[8]    ;             /* offset  0x10~0x2c    AES ENC KEY register           */
  __IO uint32_t PT[4]     ;             /* offset  0x30~3c     AES ENC Plain Text register     */
  __IO uint32_t IV[4]     ;             /* offset  0x40~4c     AES ENC Initial Vector register */
  __IO uint32_t CT[4]     ;             /* offset  0x50~5c     AES ENC Cyper Text register     */
} AES_ENC_TypeDef;  

typedef struct {                                                                       
  __IO uint32_t CTRL      ;             /* offset  0x0         AES DEC control register        */
  __IO uint32_t STAT      ;             /* offset  0x4         AES DEC status register         */
  __IO uint32_t RESERVED[2];            /* offset  0x8~0xc     RESERVED address                */
  __IO uint32_t KEY[8]    ;             /* offset  0x10~0x2c    AES DEC KEY register            */
  __IO uint32_t PT[4]     ;             /* offset  0x30~3c     AES DEC Plain Text register     */
  __IO uint32_t IV[4]     ;             /* offset  0x40~4c     AES DEC Initial Vector register */
  __IO uint32_t CT[4]     ;             /* offset  0x50~5c     AES DEC Cyper Text register     */
} AES_DEC_TypeDef;  

typedef struct {                                                                       
  __IO uint32_t CTRL      ;             /* offset  0x0         GHASH control register          */
  __IO uint32_t STAT      ;             /* offset  0x4         GHASH status register           */
  __IO uint32_t RESERVED[2];            /* offset  0x8~0xc     RESERVED address                */
  __IO uint32_t HT[4]     ;             /* offset  0x10~0x1c    GHASH sub key register          */
  __IO uint32_t AT[4]     ;             /* offset  0x20~0x2c   GHASH initial vector register   */
  __IO uint32_t CT[4]     ;             /* offset  0x30~0x3c   GHASH input vector register     */
  __IO uint32_t RT[4]     ;             /* offset  0x40~0x4c   GHASH output vector register    */
} GHASH_TypeDef;

/*--------------------- UART --------------------------------------------------*/
typedef struct {
  __IO uint32_t CTRL;
  __IO uint32_t BAUD;
  __IO uint32_t STA;
  __IO uint32_t TXD;
  __IO uint32_t RXD;
  __IO uint32_t FDIV;
} UART_TypeDef;

/*--------------------- TC --------------------------------------------------*/
typedef struct {
  __IO uint32_t CNT      ;              /* offset 0x4    */
  __IO uint32_t PS       ;              /* offset 0x8    */
  __IO uint32_t REVED1   ;              /* offset 0xc    */
  __IO uint32_t DN       ;              /* offset 0x10   */
  __IO uint32_t REVED2   ;              /* offset 0x14   */
  __IO uint32_t CCD0     ;              /* offset 0x18   */
  __IO uint32_t CCD1     ;              /* offset 0x1c   */
  __IO uint32_t CCFG     ;              /* offset 0x20   */
  __IO uint32_t CTRL     ;              /* offset 0x24   */
  __IO uint32_t CM0      ;              /* offset 0x28   */
  __IO uint32_t CM1      ;              /* offset 0x2c   */
  __IO uint32_t IE       ;              /* offset 0x30   */
  __IO uint32_t STA      ;              /* offset 0x34   */
} TC_TypeDef;
     
/*--------------------- SPI --------------------------------------------------*/
typedef struct {
  __IO uint32_t CTRL     ;              /* offset 0x00 */
  __IO uint32_t STA      ;              /* offset 0x04 */
  __IO uint32_t TX       ;              /* offset 0x08 */
  __IO uint32_t RX       ;              /* offset 0x0c */
} SPI_TypeDef; 

/*--------------------- I2C --------------------------------------------------*/
typedef struct {
  __IO uint32_t CTRL     ;              /* offset 0x0 */
  __IO uint32_t CLK      ;              /* offset 0x4 */
  __IO uint32_t STA      ;              /* offset 0x8 */
  __IO uint32_t ADDR     ;              /* offset 0xc */
  __IO uint32_t DATA     ;              /* offset 0x10 */
} I2C_TypeDef; 

/*--------------------- KBI --------------------------------------------------*/
typedef struct {
  __IO uint32_t CTRL     ;              /* offset 0x0          KBI control register         */
  __IO uint32_t SEL      ;              /* offset 0x4          KBI select register          */
  __IO uint32_t DATA     ;              /* offset 0x8          KBI data register            */
  __IO uint32_t MASK     ;              /* offset 0xc          KBI mask register            */
} KBI_TypeDef;

/*--------------------- WDT --------------------------------------------------*/
typedef struct {
  __IO uint32_t EN       ;              /* offset 0x0 */
  __IO uint32_t CTRL     ;              /* offset 0x4 */
  __IO uint32_t PASS     ;              /* offset 0x8 */
  __IO uint32_t LOSC;
  __IO uint32_t LOSC_EN;
  __IO uint32_t HALT;
  __IO uint32_t STBY;
} WDT_TypeDef;


/*--------------------- SYSCTL --------------------------------------------------*/
typedef struct {
  __IO uint32_t OSC_CTRL1;               /* offset = 0x0     */
  __IO uint32_t SYS_MODE;                /* offset = 0x4     */
  __IO uint32_t SYS_PD;                  /* offset = 0x8     */
  __IO uint32_t RESERVED1;               /* offset = 0xc     */
  __IO uint32_t OSC_CTRL2;               /* offset = 0x10     */
  __IO uint32_t SYS_RST;                 /* offset = 0x14    */
  __IO uint32_t MAP_CTRL;                /* offset = 0x18    */
  __IO uint32_t MOD0_EN;                 /* offset = 0x1c    */
  __IO uint32_t MOD1_EN;                 /* offset = 0x20    */
  __IO uint32_t INTC_EN;                 /* offset = 0x24    */
  __IO uint32_t KBI_EN;                  /* offset = 0x28    */
  __IO uint32_t CHIP_ID;                 /* offset = 0x2c    */
  __IO uint32_t SYS_PS;                  /* offset = 0x30    */
  __IO uint32_t IRFR_CTRL;               /* offset = 0x34    */
  __IO uint32_t RESERVED2[3];            /* offset = 0x38~0x40 */
  __IO uint32_t DEBUG_CTL;               /* offset = 0x44    */
  __IO uint32_t ANA_ADC;                 /* offset = 0x48    */
  __IO uint32_t ANA_CLK1;                /* offset = 0x4c    */
  __IO uint32_t ANA_CLK2;                /* offset = 0x50    */
  __IO uint32_t ANA_LDO;                 /* offset = 0x54    */
  __IO uint32_t RESERVED3;               /* offset = 0x58    */
  __IO uint32_t ANA_PSW;                 /* offset = 0x5c    */
  __IO uint32_t SYS_RST2;                /* offset = 0x60    */
  __IO uint32_t CHIP_SEL;                /* offset = 0x64    */   
  __IO uint32_t RESERVED4;               /* offset = 0x68    */ 
  __IO uint32_t ANA_CLK3;                /* offset = 0x6c    */ 
  __IO uint32_t ANA_SYS;                 /* offset = 0x70    */   
  __IO uint32_t DIE_VER;                 /* offset = 0x74    */
  __IO uint32_t TRIM_CFG1;               /* offset = 0x78    */
  __IO uint32_t TRIM_START;              /* offset = 0x7c    */
} SYSCTL_TypeDef;

/*--------------------- 7816 --------------------------------------------------*/
typedef struct {
  __IO uint32_t CTRL0      ;            /* offset = 0x0        */
  __IO uint32_t CTRL1      ;            /* offset = 0x4        */
  __IO uint32_t CLK        ;            /* offset = 0x8        */
  __IO uint32_t BDDIV0     ;            /* offset = 0xc        */
  __IO uint32_t BDDIV1     ;            /* offset = 0x10       */
  __IO uint32_t STA0       ;            /* offset = 0x14       */
  __IO uint32_t STA1       ;            /* offset = 0x18       */
  __IO uint32_t DATA0      ;            /* offset = 0x1c       */
  __IO uint32_t DATA1      ;            /* offset = 0x20       */
} ISO7816_TypeDef;  

/*-------------------- RTC ---------------------------------------------------*/
typedef struct
{
  __IO uint32_t CTRL;                               /*0x0 */
  __IO uint32_t SC;                                 /*0x4 */
  __IO uint32_t MN;                                 /*0x8 */
  __IO uint32_t HR;                                 /*0xc */
  __IO uint32_t DT;                                 /*0x10 */
  __IO uint32_t MO;                                 /*0x14 */
  __IO uint32_t YR;                                 /*0x18 */
  __IO uint32_t DW;                                 /*0x1c */
  __IO uint32_t CNT1;                               /*0x20 */
  __IO uint32_t CNT2;                               /*0x24 */
  __IO uint32_t SCA;                                /*0x28 */
  __IO uint32_t MNA;                                /*0x2c */
  __IO uint32_t HRA;                                /*0x30 */
  __IO uint32_t IE;                                 /*0x34 */
  __IO uint32_t IF;                                 /*0x38 */
  __IO uint32_t TEMP;                               /*0x3c */
  __IO uint32_t PS;                                 /*0x40 */
  __IO uint32_t TCPMD;                              /*0x44 */
  __IO uint32_t DOTA0;                              /*0x48 */
  __IO uint32_t ALPHAL;                             /*0x4c */
  __IO uint32_t ALPHAH;                             /*0x50 */
  __IO uint32_t XT0;                                /*0x54 */
  __IO uint32_t TADJ;                               /*0x58 */
  __IO uint32_t BETA;                               /*0x5c */
  __IO uint32_t ZT;                                 /*0x60 */
  __IO uint32_t DOTAT;                              /*0x64 */
  __IO uint32_t FATR;                               /*0x68 */
  __IO uint32_t LOSC_CFG1;                          /*0x6c */
  __IO uint32_t LOSC_CFG2;                          /*0x70 */
  __IO uint32_t FPTR;                               /*0x74 */
  __IO uint32_t FDTR1S;                             /*0x78 */
  __IO uint32_t FDTR30S;                            /*0x7c */
  __IO uint32_t LCNT;                               /*0x80 */
  __IO uint32_t FDTR120S;                           /*0x84 */  
  __IO uint32_t IOEN;                               /*0x88 */
  __IO uint32_t IOMODE;                             /*0x8c */  
  __IO uint32_t P44N0T0;                            /*0x90 */
  __IO uint32_t P44N0T1;                            /*0x94 */
  __IO uint32_t P44N0T2;                            /*0x98 */
  __IO uint32_t P44P0T0;                            /*0x9c */
  __IO uint32_t P44P0T1;                            /*0xa0 */
  __IO uint32_t P44P0T2;                            /*0xa4 */
  __IO uint32_t P44N1T0;                            /*0xa8 */
  __IO uint32_t P44N1T1;                            /*0xac */
  __IO uint32_t P44N1T2;                            /*0xb0 */
  __IO uint32_t P44P1T0;                            /*0xb4 */
  __IO uint32_t P44P1T1;                            /*0xb8 */
  __IO uint32_t P44P1T2;                            /*0xbc */  
  __IO uint32_t DMAEN;                              /*0xc0 */
  __IO uint32_t TEMPOS;                             /*0xc4 */
  __IO uint32_t TPSIN;                              /*0xc8 */  
  __IO uint32_t CALPS;                              /*0xcc */
  __IO uint32_t CAL_T[10];                          /*0xd0~0xF4 */
  __IO uint32_t RESERVED3[2];                       /*0xF8~0xFC */
  __IO uint32_t P45N0T0;                            /*0x100 */
  __IO uint32_t P45N0T1;                            /*0x104 */
  __IO uint32_t P45N0T2;                            /*0x108 */
  __IO uint32_t P45P0T0;                            /*0x10c */
  __IO uint32_t P45P0T1;                            /*0x110 */
  __IO uint32_t P45P0T2;                            /*0x114 */
  __IO uint32_t P45N1T0;                            /*0x118 */
  __IO uint32_t P45N1T1;                            /*0x11c */
  __IO uint32_t P45N1T2;                            /*0x120 */
  __IO uint32_t P45P1T0;                            /*0x124 */
  __IO uint32_t P45P1T1;                            /*0x128 */
  __IO uint32_t P45P1T2;                            /*0x12c */
  
} RTC_TypeDef;

/*-------------------- MADC ---------------------------------------------------*/
typedef struct
{
  __IO uint32_t SAR_CTRL    ;            /* offset = 0x00      */
  __IO uint32_t SAR_START   ;            /* offset = 0x04      */
  __IO uint32_t SAR_STAT    ;            /* offset = 0x08      */
  __IO uint32_t SAR_DATA    ;            /* offset = 0x0c      */
  __IO uint32_t LVD_CTRL   ;             /* offset = 0x10      */
  __IO uint32_t LVD_STAT   ;             /* offset = 0x14      */
  __IO uint32_t TPS_CTRL;                /* offset = 0x18      */
  __IO uint32_t SAR_TEST;                /* offset = 0x1c      */
  
  
  __IO uint32_t ANA_BGR   ;             /* offset = 0x20      */
  __IO uint32_t ANA_CTL   ;              /* offset = 0x24      */
  __IO uint32_t ANA_LDO;                 /* offset = 0x28      */
  __IO uint32_t ANA_RCH;                 /* offset = 0x2c      */  
  __IO uint32_t SAR_CTL2   ;             /* offset = 0x30      */
  __IO uint32_t ANA_TEST   ;             /* offset = 0x34      */
  __IO uint32_t ANA_TIEHL;               /* offset = 0x38      */
  __IO uint32_t ANA_PAD;                 /* offset = 0x3c      */
  __IO uint32_t ANA_RST;                 /* offset = 0x40      */
} MADC_TypeDef;

/*--------------------- INTC --------------------------------------------------*/
typedef struct {
  __IO uint32_t CTRL       ;             /* offset = 0x00         interrupt control register   */
  __IO uint32_t MODE       ;             /* offset = 0x04         interrupt mode register      */
  __IO uint32_t MASK       ;             /* offset = 0x08         interrupt mask register      */
  __IO uint32_t STA        ;             /* offset = 0x0c         interrupt status register    */
} INTC_TypeDef; 

/*--------------------- LCD --------------------------------------------------*/
typedef struct {
  __IO uint32_t CTRL       ;            /* offset = 0x00       */
  __IO uint32_t STA        ;            /* offset = 0x04       */
  __IO uint32_t CLKDIV     ;            /* offset = 0x08       */
  __IO uint32_t BLINK      ;            /* offset = 0x0c       */
  __IO uint32_t PS         ;            /* offset = 0x10       */
  __IO uint32_t RESCTL         ;        /* offset = 0x14       */
  __IO uint32_t TEST         ;          /* offset = 0x18       */
  __IO uint32_t RESERVED;               /* offset = 0x1c       */
  __IO uint8_t  BUF[40]    ;            /* offset = 0x20       */
} LCD_TypeDef;

/*--------------------- SYS TC --------------------------------------------------*/
typedef struct {
  __IO uint32_t CTRL  ;       
  __IO uint32_t LOAD  ;      
  __IO uint32_t VAL;   
} SYS_TC_TypeDef;

      
/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* Peripheral and SRAM base address */
#define LS_PERIPH_BASE                  ((uint32_t )0x40000000             )
#define HS_PERIPH_BASE                  ((uint32_t )0x50000000             )


//AHB base address
#define BB_OFFSET                       0x02000000
#define HSP_BASE                        HS_PERIPH_BASE
#define HSP_BB_BASE                     (HSP_BASE + BB_OFFSET                 )
#define GPIO_BASE                       HSP_BASE
#define GPIO_BB_BASE                    (HSP_BASE + BB_OFFSET                 )
#define CT_BASE                         (HSP_BASE + 0xC000                    )
#define AES_ENC_BASE                    (HSP_BASE + 0xD000                    )
#define AES_DEC_BASE                    (HSP_BASE + 0xE000                    )
#define GHASH_BASE                      (HSP_BASE + 0xF000                    )
#define SYS_TC_BASE                     (HSP_BASE + 0x10000                   )
#define FLASH_REG_BASE                  (HSP_BASE + 0x00014000                )

//APB base address
#define LSP_BASE                        LS_PERIPH_BASE
#define LSP_BB_BASE                     (LSP_BASE + BB_OFFSET                  )
#define UART0_BASE                      (LSP_BASE                              )
#define UART0_BB_BASE                   (LSP_BASE + BB_OFFSET                  )
#define UART1_BASE                      (LSP_BASE + 0x04000                    )
#define UART1_BB_BASE                   (LSP_BASE + BB_OFFSET + 0x04000 * 32   )
#define UART2_BASE                      (LSP_BASE + 0x08000                    )
#define UART2_BB_BASE                   (LSP_BASE + BB_OFFSET + 0x08000 * 32   )
#define UART3_BASE                      (LSP_BASE + 0x0C000                    )
#define UART3_BB_BASE                   (LSP_BASE + BB_OFFSET + 0x0c000 * 32   )
#define TC0_BASE                        (LSP_BASE + 0x10000                    )
#define TC0_BB_BASE                     (LSP_BASE + BB_OFFSET + 0x10000 * 32   )
#define TC1_BASE                        (LSP_BASE + 0x14000                    )
#define TC1_BB_BASE                     (LSP_BASE + BB_OFFSET + 0x14000 * 32   )
#define UART4_BASE                      (LSP_BASE + 0x18000                    )
#define UART4_BB_BASE                   (LSP_BASE + BB_OFFSET + 0x18000 * 32   )
#define UART5_BASE                      (LSP_BASE + 0x1C000                    )
#define UART5_BB_BASE                   (LSP_BASE + BB_OFFSET + 0x1C000 * 32   )
#define SPI_BASE                        (LSP_BASE + 0x20000                    )
#define I2C_BASE                        (LSP_BASE + 0x24000                    )
#define I2C_BB_BASE                     (LSP_BASE + BB_OFFSET + 0x24000 * 32   )
#define KBI_BASE                        (LSP_BASE + 0x28000                    )
#define MADC_BASE                       (LSP_BASE + 0x2C000                    )
#define WDT_BASE                        (LSP_BASE + 0x30000                    )
#define SYSCTL_BASE                     (LSP_BASE + 0x34000                    )
#define ISO7816_BASE                    (LSP_BASE + 0x38000                    )
#define RTC_BASE                        (LSP_BASE + 0x3C000                    )
#define INTC_BASE                       (LSP_BASE + 0x44000                    )
#define LCD_BASE                        (LSP_BASE + 0x48000                    )

/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
#define GPIO                        ((GPIO_TypeDef    *) GPIO_BASE          )
#define CT                          ((CT_TypeDef      *) CT_BASE            )
#define AES_ENC                     ((AES_ENC_TypeDef *) AES_ENC_BASE       )
#define AES_DEC                     ((AES_DEC_TypeDef *) AES_DEC_BASE       )
#define GHASH                       ((GHASH_TypeDef   *) GHASH_BASE         )
#define SYS_TC                      ((SYS_TC_TypeDef  *) SYS_TC_BASE        )
#define FLASH                       ((FLASH_TypeDef   *) FLASH_REG_BASE     )

#define UART0                       ((UART_TypeDef    *) UART0_BASE         )
#define UART1                       ((UART_TypeDef    *) UART1_BASE         )
#define UART2                       ((UART_TypeDef    *) UART2_BASE         )
#define UART3                       ((UART_TypeDef    *) UART3_BASE         )

#define TC0                         ((TC_TypeDef      *) TC0_BASE           )
#define TC1                         ((TC_TypeDef      *) TC1_BASE           )
#define UART4                       ((UART_TypeDef    *) UART4_BASE         )
#define UART5                       ((UART_TypeDef    *) UART5_BASE         )
#define SPI                         ((SPI_TypeDef     *) SPI_BASE           )
#define I2C                         ((I2C_TypeDef     *) I2C_BASE           )
#define KBI                         ((KBI_TypeDef     *) KBI_BASE           )
#define MADC                        ((MADC_TypeDef    *) MADC_BASE          )
#define WDT                         ((WDT_TypeDef     *) WDT_BASE           )

#define SYSCTL                      ((SYSCTL_TypeDef  *) SYSCTL_BASE        )
#define ISO7816                     ((ISO7816_TypeDef *) ISO7816_BASE       )
#define RTC                         ((RTC_TypeDef     *) RTC_BASE           )
#define INTC                        ((INTC_TypeDef    *) INTC_BASE          )
#define LCD                         ((LCD_TypeDef     *) LCD_BASE           )

#if 0
#if defined ( __CC_ARM   )
  #if defined (__TARGET_FPU_VFP)
    #define __FPU_PRESENT         1         /*!< FPU present or not                               */
  #else
    #define __FPU_PRESENT         0         /*!< FPU present or not                               */
  #endif
#else
    #define __FPU_PRESENT         0         /*!< FPU present or not                               */
#endif
#endif

/*@}*/ /* end of group RA9103D0_CMSIS */

#include "core_cm0.h"                       /* Cortex-M0 processor and core peripherals           */
#include "system_RA9103.h"                  /* RA9103D0 System  include file                      */


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/
/** @addtogroup RA9103D0_Peripherals RA9103D0 Peripherals
  RA9103D0 Device Specific Peripheral registers structures
  @{
*/

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


#ifdef __cplusplus
}
#endif


//#endif

#endif  // __RA9103F0_H__
