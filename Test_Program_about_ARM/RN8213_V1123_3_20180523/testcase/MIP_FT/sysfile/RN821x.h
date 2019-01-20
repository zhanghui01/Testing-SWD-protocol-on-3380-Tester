/**************************************************************************//**
 * @file     RN821x.h
 * @brief    CMSIS Cortex-M0 Core Peripheral Access Layer Header File
 *           for RN821x CM0 Device
 * @version  V1.00
 * @date     28. Sep 2012
 *
 * @note
 * Copyright (C) REnergy Micro-Electronics Co.,LTD.
 *
 *
 ******************************************************************************/


#ifndef __RN821X_H__
#define __RN821X_H__

#ifdef __cplusplus
 extern "C" {
#endif 
/** @addtogroup RN821x_Definitions RN821x Definitions
  This file defines all structures and symbols for RN821x:
    - registers and bitfields
    - peripheral base address
    - peripheral ID
    - Peripheral definitions
  @{
*/

/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/
/** @addtogroup RN821x_CMSIS Device CMSIS Definitions
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

/******  RN821x Specific Interrupt Numbers ********************************************************/
  RESERVED_IRQn                = 0,      /*!<  16 Reserved irq number                             */
  CMP_IRQn                     = 1,      /*!<  17 Voltage compare Interrupt                       */
  VCH_IRQn                     = 2,      /*!<  18 Power Switch Interrupt                          */
  RTC_IRQn                     = 3,      /*!<  19 RTC Interrupt                                   */
  EMU_IRQn                     = 4,      /*!<  20 EMU Interrupt                                   */
  MADC_IRQn                    = 5,      /*!<  21 Mux. ADC Interrupt                              */
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
  UART4_IRQn                   = 16,     /*!<  32 UART4 Interrupt                                 */
  UART5_IRQn                   = 17,     /*!<  33 UART5 Interrupt                                 */
  WDT_IRQn                     = 18,     /*!<  34 Watch Dog Interrupt                             */
  KBI_IRQn                     = 19,     /*!<  35 Key Interrupt                                   */
  LCD_IRQn                     = 20,     /*!<  36 LCD Interrupt                                   */
  CT_IRQn                      = 21,     /*!<  37 CT Interrupt                                    */
  DMA_IRQn                     = 22,     /*!<  38 DMA Interrupt                                   */
  NVM_IRQn                     = 23,     /*!<  39 NVM Interrupt                                   */
  EXT0_IRQn                    = 24,     /*!<  40 Extern0 Interrupt                               */
  EXT1_IRQn                    = 25,     /*!<  41 Extern1 Interrupt                               */
  EXT2_IRQn                    = 26,     /*!<  42 Extern2 Interrupt                               */
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

/*@}*/ /* end of group RN821x_CMSIS */

#include "core_cm0.h"                       /* Cortex-M0 processor and core peripherals           */
#include "system_RN821x.h"                  /* RN821x System  include file                      */


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/
/** @addtogroup RN821x_Peripherals RN821x Peripherals
  RN821x Device Specific Peripheral registers structures
  @{
*/

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/*--------------------- GPIO --------------------------------------------------*/
typedef struct {
  __IO uint32_t PMA     ;               /* offset 0x0          PortA mode register          */
  __IO uint32_t PA      ;               /* offset 0x4          PortA data register          */
  __IO uint32_t PCA0    ;               /* offset 0x8          PortA control register0      */
  __IO uint32_t PCA1    ;               /* offset 0xc          PortA control register1      */
  __IO uint32_t PUA     ;               /* offset 0x10         PortA pull-up register       */
  __IO uint32_t PIMA    ;               /* offset 0x14         PortA input mode register    */
  __IO uint32_t PIEA    ;               /* offset 0x18         PortA input enable register  */
  __IO uint32_t PMB     ;               /* offset 0x1c         PortB mode register          */
  __IO uint32_t PB      ;               /* offset 0x20         PortB data register          */
  __IO uint32_t PCB     ;               /* offset 0x24         PortB control register       */
  __IO uint32_t PUB     ;               /* offset 0x28         PortB pull-up register       */
  __IO uint32_t PIMB    ;               /* offset 0x2c         PortB input mode register    */
  __IO uint32_t PIEB    ;               /* offset 0x30         PortB input enable register  */
  __IO uint32_t PMC     ;               /* offset 0x34         PortC mode register          */
  __IO uint32_t PC      ;               /* offset 0x38         PortC data register          */
  __IO uint32_t PCC     ;               /* offset 0x3c         PortC control register       */
  __IO uint32_t PUC     ;               /* offset 0x40         PortC pull-up register       */
  __IO uint32_t PIEC    ;               /* offset 0x44         PortC input enable register  */
  __IO uint32_t PIMC    ;               /* offset 0x48         PortC input mode register    */
  __IO uint32_t REVED[5];               /* offset 0x4c~0x5c    RESERVED register            */
  __IO uint32_t PCE     ;               /* offset 0x60         PortE control register       */
  __IO uint32_t PASET   ;               /* offset 0x64         PortA data set register      */
  __IO uint32_t PACLR   ;               /* offset 0x68         PortA data clear register    */
  __IO uint32_t PBSET   ;               /* offset 0x6c         PortB data set register      */
  __IO uint32_t PBCLR   ;               /* offset 0x70         PortB data clear register    */
  __IO uint32_t PCSET   ;               /* offset 0x74         PortC data set register      */
  __IO uint32_t PCCLR   ;               /* offset 0x78         PortC data clear register    */
} GPIO_TypeDef;

/*--------------------- EMU --------------------------------------------------*/
typedef struct {
  __IO uint32_t EMUCON    ;             /* offset 0x0     */
  __IO uint32_t EMUCON2   ;             /* offset 0x4     */
  __IO uint32_t HFConst   ;             /* offset 0x8     */
  __IO uint32_t PStart    ;             /* offset 0xc     */
  __IO uint32_t QStart    ;             /* offset 0x10    */
  __IO uint32_t GPQA      ;             /* offset 0x14    */
  __IO uint32_t GPQB      ;             /* offset 0x18    */
  __IO uint32_t PhsA      ;             /* offset 0x1c    */
  __IO uint32_t PhsB      ;             /* offset 0x20    */
  __IO uint32_t QPhsCal   ;             /* offset 0x24    */
  __IO uint32_t APOSA     ;             /* offset 0x28    */
  __IO uint32_t APOSB     ;             /* offset 0x2c    */
  __IO uint32_t RPOSA     ;		          /* offset 0x30    */
  __IO uint32_t RPOSB     ;             /* offset 0x34    */
  __IO uint32_t IARMSOS   ;             /* offset 0x38    */
  __IO uint32_t IBRMSOS   ;             /* offset 0x3c    */
  __IO uint32_t URMSOS    ;             /* offset 0x40    */
  __IO uint32_t IAGAIN    ;             /* offset 0x44    */
  __IO uint32_t IBGAIN    ;             /* offset 0x48    */
  __IO uint32_t UGAIN     ;             /* offset 0x4c    */
  __IO uint32_t IADCOS    ;             /* offset 0x50    */
  __IO uint32_t IBDCOS    ;             /* offset 0x54    */
  __IO uint32_t UDCOS     ;             /* offset 0x58    */
  __IO uint32_t UADD      ;             /* offset 0x5c    */
  __IO uint32_t USAG      ;             /* offset 0x60    */
  __IO uint32_t IAPEAK    ;             /* offset 0x64    */
  __IO uint32_t IBPEAK    ;             /* offset 0x68    */
  __IO uint32_t UPEAK     ;             /* offset 0x6c    */
  __IO uint32_t D2FP      ;				      /* offset 0x70    */
  __IO uint32_t REVED1[19];             /* offset 0x74~0xdc   */
  __IO uint32_t PF2cnt    ;			        /* offset 0xc0  */
  __IO uint32_t QF2cnt    ;             /* offset 0xc4  */
  __IO uint32_t SF2cnt    ;             /* offset 0xc8  */
  __IO uint32_t REVED4[13];             /* offset 0xcc~0xfc  */
  __IO uint32_t PFcnt     ;             /* offset 0x100 */
  __IO uint32_t QFcnt     ;             /* offset 0x104 */
  __IO uint32_t SFcnt     ;             /* offset 0x108 */
  __IO uint32_t RmsIA     ;			        /* offset 0x10c */
  __IO uint32_t RmsIB     ;             /* offset 0x110 */
  __IO uint32_t RmsU      ;             /* offset 0x114 */
  __IO uint32_t FreqU     ;             /* offset 0x118 */
  __IO uint32_t PowerPA   ;             /* offset 0x11c */
  __IO uint32_t PowerPB   ;             /* offset 0x120 */
  __IO uint32_t PowerQA   ;             /* offset 0x124 */
  __IO uint32_t PowerQB   ;             /* offset 0x128 */
  __IO uint32_t PowerSA   ;             /* offset 0x12c  */
  __IO uint32_t PowerSB   ;             /* offset 0x130   */
  __IO uint32_t Energy_P  ;             /* offset 0x134   */
  __IO uint32_t Energy_P2 ;             /* offset 0x138   */
  __IO uint32_t Energy_Q  ;             /* offset 0x13c   */
  __IO uint32_t Energy_Q2 ;             /* offset 0x140   */
  __IO uint32_t Energy_S  ;             /* offset 0x144   */
  __IO uint32_t PFA       ;             /* offset 0x148   */
  __IO uint32_t PFB       ;             /* offset 0x14c   */
  __IO uint32_t ANGLEA    ;             /* offset 0x150   */
  __IO uint32_t ANGLEB    ;             /* offset 0x154   */
  __IO uint32_t EMUStatus ;             /* offset 0x158   */
  __IO uint32_t SPL_IA    ;             /* offset 0x15c   */
  __IO uint32_t SPL_IB    ;             /* offset 0x160   */
  __IO uint32_t SPL_U     ;             /* offset 0x164   */
  __IO uint32_t PowerPA2  ;             /* offset 0x168   */
  __IO uint32_t PowerPB2  ;             /* offset 0x16c   */
  __IO uint32_t Energy_S2 ;             /* offset 0x170   */
  __IO uint32_t SPL_PA    ;             /* offset 0x174   */
  __IO uint32_t SPL_PB    ;             /* offset 0x178   */
  __IO uint32_t SPL_QA    ;             /* offset 0x17c   */
  __IO uint32_t SPL_QB    ;             /* offset 0x180   */
  __IO uint32_t TRNG      ;             /* offset 0x184   */
  __IO uint32_t EMUStatus2;             /* offset 0x188   */
  __IO uint32_t IE        ;             /* offset 0x18c   */
  __IO uint32_t IF        ;             /* offset 0x190   */
  __IO uint32_t DMA_en    ;             /* offset 0x194  */
  __IO uint32_t RData     ;             /* offset 0x198  */
  __IO uint32_t WData     ;             /* offset 0x19c  */
  __IO uint32_t REVED3[2] ;             /* offset 0x1a0~0x1a4  */
  __IO uint32_t SPCMD     ;             /* offset 0x1a8  */
} EMU_TypeDef;                       

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
  __IO uint32_t KEY[8]    ;             /* offset  0x8~0x28    AES ENC KEY register            */
  __IO uint32_t PT[4]     ;             /* offset  0x2c~3c     AES ENC Plain Text register     */
  __IO uint32_t IV[4]     ;             /* offset  0x40~5c     AES ENC Initial Vector register */
  __IO uint32_t CT[4]     ;             /* offset  0x50~6c     AES ENC Cyper Text register     */
} AES_ENC_TypeDef;  

typedef struct {                                                                       
  __IO uint32_t CTRL      ;             /* offset  0x0         AES DEC control register        */
  __IO uint32_t STAT      ;             /* offset  0x4         AES DEC status register         */
  __IO uint32_t KEY[8]    ;             /* offset  0x8~0x28    AES DEC KEY register            */
  __IO uint32_t PT[4]     ;             /* offset  0x2c~3c     AES DEC Plain Text register     */
  __IO uint32_t IV[4]     ;             /* offset  0x40~5c     AES DEC Initial Vector register */
  __IO uint32_t CT[4]     ;             /* offset  0x50~6c     AES DEC Cyper Text register     */
} AES_DEC_TypeDef;  

typedef struct {                                                                       
  __IO uint32_t CTRL      ;             /* offset  0x0         GHASH control register          */
  __IO uint32_t STAT      ;             /* offset  0x4         GHASH status register           */
  __IO uint32_t HT[4]     ;             /* offset  0x8~0x18    GHASH sub key register          */
  __IO uint32_t AT[4]     ;             /* offset  0x20~0x30   GHASH initial vector register   */
  __IO uint32_t CT[4]     ;             /* offset  0x30~0x40   GHASH input vector register     */
  __IO uint32_t RT[4]     ;             /* offset  0x50~0x60   GHASH output vector register    */
} GHASH_TypeDef;
/*--------------------- UART --------------------------------------------------*/
typedef struct {
  __IO uint32_t CTRL;
  __IO uint32_t BAUD;
  __IO uint32_t STA;
  __IO uint32_t TXD;
  __IO uint32_t RXD;
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
  __IO uint32_t CTRL     ;              /* offset 0x */
  __IO uint32_t STA      ;              /* offset 0x */
  __IO uint32_t TX       ;              /* offset 0x */
  __IO uint32_t RX       ;              /* offset 0x */
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
  __IO uint32_t LOSC     ;              /* offset 0xc */
  __IO uint32_t LOSC_EN  ;              /* offset 0x10 */
  __IO uint32_t HALT;                   /* offset 0x14 */
  __IO uint32_t STBY;                   /* offset 0x18 */ 
} WDT_TypeDef;


/*--------------------- SYSCTL --------------------------------------------------*/
typedef struct {
  __IO uint32_t OSC_CTRL1;           /* offset = 0x0   */       
  __IO uint32_t SYS_MODE;            /* offset = 0x4   */
  __IO uint32_t SYS_PD;              /* offset = 0x8   */
  __IO uint32_t ADC_CTRL;            /* offset = 0xc   */
  __IO uint32_t OSC_CTRL2;           /* offset = 0x10   */
  __IO uint32_t SYS_RST;             /* offset = 0x14   */
  __IO uint32_t MAP_CTRL;            /* offset = 0x18   */
  __IO uint32_t MOD0_EN;             /* offset = 0x1c   */ 
  __IO uint32_t MOD1_EN;             /* offset = 0x20   */
  __IO uint32_t INTC_EN;             /* offset = 0x24   */
  __IO uint32_t KBI_EN;              /* offset = 0x28   */
  __IO uint32_t CHIP_ID;             /* offset = 0x2c   */
  __IO uint32_t SYS_PS;              /* offset = 0x30   */
  __IO uint32_t RESERVED[3];         /* offset = 0x34~3C   */
  __IO uint32_t TEST_CTRL;           /* offset = 0x40   */
  __IO uint32_t DEBUG_CTRL;          /* offset = 0x44   */ 
  __IO uint32_t ANA_ADC;             /* offset = 0x48   */
  __IO uint32_t ANA_CLK1;            /* offset = 0x4c   */ 
  __IO uint32_t ANA_CLK2;            /* offset = 0x50   */ 
  __IO uint32_t ANA_LDO;             /* offset = 0x54   */
  __IO uint32_t ANA_BGR;             /* offset = 0x58   */
  __IO uint32_t ANA_PSW;             /* offset = 0x5c   */
  __IO uint32_t SYS_RST2;            /* offset = 0x60   */
  __IO uint32_t CHIP_SEL;            /* offset = 0x64   */
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
  __IO uint32_t CTRL        ;           /* offset = 0x     */
  __IO uint32_t SC          ;           /* offset = 0x     */
  __IO uint32_t MN          ;           /* offset = 0x     */
  __IO uint32_t HR          ;           /* offset = 0x     */
  __IO uint32_t DT          ;           /* offset = 0x     */
  __IO uint32_t MO          ;           /* offset = 0x     */
  __IO uint32_t YR          ;           /* offset = 0x     */
  __IO uint32_t DW          ;           /* offset = 0x     */
  __IO uint32_t CNT1        ;           /* offset = 0x     */
  __IO uint32_t CNT2        ;           /* offset = 0x     */
  __IO uint32_t SCA         ;           /* offset = 0x     */
  __IO uint32_t MNA         ;           /* offset = 0x     */
  __IO uint32_t HRA         ;           /* offset = 0x     */
  __IO uint32_t IE          ;           /* offset = 0x     */
  __IO uint32_t IF          ;           /* offset = 0x     */
  __IO uint32_t TEMP        ;           /* offset = 0x     */
  __IO uint32_t PS          ;           /* offset = 0x     */
  __IO uint32_t TCPMD       ;           /* offset = 0x     */
  __IO uint32_t DOTA0       ;           /* offset = 0x     */
  __IO uint32_t ALPHAL      ;           /* offset = 0x     */
  __IO uint32_t ALPHAH      ;           /* offset = 0x     */
  __IO uint32_t XT0         ;           /* offset = 0x     */
  __IO uint32_t TADJ        ;           /* offset = 0x     */
  __IO uint32_t BETA        ;           /* offset = 0x     */
  __IO uint32_t ZT          ;           /* offset = 0x     */
  __IO uint32_t DOTAT       ;           /* offset = 0x     */
  __IO uint32_t FATR        ;           /* offset = 0x     */
  __IO uint32_t DELTCX      ;           /* offset = 0x     */
  __IO uint32_t CPS         ;           /* offset = 0x     */
  __IO uint32_t FPTR        ;           /* offset = 0x     */
  __IO uint32_t FDTR1S      ;           /* offset = 0x     */
  __IO uint32_t FDTR20S     ;           /* offset = 0x     */
  __IO uint32_t LCNT        ;           /* offset = 0x     */
  __IO uint32_t TLCNT       ;           /* offset = 0x     */
  __IO uint32_t RESERVED[14];           /* offset = 0x     */
  __IO uint32_t DMAEN       ;           /* offset = 0x     */
  __IO uint32_t TEMPOS      ;           /* offset = 0x     */
  __IO uint32_t TPSIN       ;           /* offset = 0x     */
} RTC_TypeDef;


/*-------------------- MADC ---------------------------------------------------*/
typedef struct
{
  __IO uint32_t AD_CTRL    ;            /* offset = 0x      */
  __IO uint32_t AD_START   ;            /* offset = 0x      */
  __IO uint32_t AD_STAT    ;            /* offset = 0x      */
  __IO uint32_t AD_DATA    ;            /* offset = 0x      */
  __IO uint32_t LVD_CTRL   ;            /* offset = 0x      */
  __IO uint32_t LVD_STAT   ;            /* offset = 0x      */
  __IO uint32_t TPS_CTRL   ;            /* offset = 0x      */
  __IO uint32_t AD_TEST    ;            /* offset = 0x      */
} MADC_TypeDef;

/*--------------------- INTC --------------------------------------------------*/
typedef struct {
  __IO uint32_t CTRL       ;            /* offset = 0x         interrupt control register   */
  __IO uint32_t MODE       ;            /* offset = 0x         interrupt mode register      */
  __IO uint32_t MASK       ;            /* offset = 0x         interrupt mask register      */
  __IO uint32_t STA        ;            /* offset = 0x         interrupt status register    */
} INTC_TypeDef; 

/*--------------------- LCD --------------------------------------------------*/
typedef struct {
  __IO uint32_t CTRL       ;            /* offset = 0x       */
  __IO uint32_t STA        ;            /* offset = 0x       */
  __IO uint32_t CLKDIV     ;            /* offset = 0x       */
  __IO uint32_t BLINK      ;            /* offset = 0x       */
  __IO uint32_t PS         ;            /* offset = 0x       */
  __IO uint32_t RESERVED[3];            /* offset = 0x       */
  __IO uint32_t BUF[12]    ;            /* offset = 0x       */
} LCD_TypeDef;


/*--------------------- DMA --------------------------------------------------*/
typedef struct {
  __IO uint32_t IE         ;            /* offset = 0x        */
  __IO uint32_t STA        ;            /* offset = 0x        */
  __IO uint32_t REVED0[2]  ;            /* offset = 0x        */
  __IO uint32_t C0CTRL     ;            /* offset = 0x        */
  __IO uint32_t C0SRC      ;            /* offset = 0x        */
  __IO uint32_t C0DST      ;            /* offset = 0x        */
  __IO uint32_t C0STA      ;            /* offset = 0x        */
  __IO uint32_t C1CTRL     ;            /* offset = 0x        */
  __IO uint32_t C1SRC      ;            /* offset = 0x        */
  __IO uint32_t C1DST      ;            /* offset = 0x        */
  __IO uint32_t C1STA      ;            /* offset = 0x        */
  __IO uint32_t C2CTRL     ;            /* offset = 0x        */
  __IO uint32_t C2SRC      ;            /* offset = 0x        */
  __IO uint32_t C2DST      ;            /* offset = 0x        */
  __IO uint32_t C2STA      ;            /* offset = 0x        */
  __IO uint32_t C3CTRL     ;            /* offset = 0x        */
  __IO uint32_t C3SRC      ;            /* offset = 0x        */
  __IO uint32_t C3DST      ;            /* offset = 0x        */
  __IO uint32_t C3STA      ;            /* offset = 0x        */
} DMA_TypeDef;

/*--------------------- NVM --------------------------------------------------*/
typedef struct {
  __IO uint32_t IE         ;            /* offset = 0x0        */
  __IO uint32_t IF         ;            /* offset = 0x4        */
  __IO uint32_t LSCFG      ;            /* offset = 0x8        */
  __IO uint32_t LSDCOS     ;            /* offset = 0xc        */
  __IO uint32_t LSTHO      ;            /* offset = 0x10        */
  __IO uint32_t LSRMS      ;            /* offset = 0x14        */
  __IO uint32_t LSRMS1     ;            /* offset = 0x18        */
  __IO uint32_t DMAEN      ;            /* offset = 0x1c        */
} NVM_TypeDef;

      
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
#define EMU_BASE                        (HSP_BASE + 0x4000                    )
#define DBG_BASE                        (HSP_BASE + 0x8000                    )
#define CT_BASE                         (HSP_BASE + 0xC000                    )
#define AES_ENC_BASE                    (HSP_BASE + 0xD000                    )
#define AES_DEC_BASE                    (HSP_BASE + 0xE000                    )
#define GHASH_BASE                      (HSP_BASE + 0xF000                    )
#define ROM_TABLE_BASE                  0xF000_0000

//APB base address
#define LSP_BASE                        LS_PERIPH_BASE
#define LSP_BB_BASE                     (LSP_BASE + BB_OFFSET                  )
#define UART0_BASE                  (LSP_BASE                              )
#define UART0_BB_BASE               (LSP_BASE + BB_OFFSET                  )
#define UART1_BASE                  (LSP_BASE + 0x04000                    )
#define UART1_BB_BASE               (LSP_BASE + BB_OFFSET + 0x04000 * 32   )
#define UART2_BASE                  (LSP_BASE + 0x08000                    )
#define UART2_BB_BASE               (LSP_BASE + BB_OFFSET + 0x08000 * 32   )
#define UART3_BASE                  (LSP_BASE + 0x0C000                    )
#define UART3_BB_BASE               (LSP_BASE + BB_OFFSET + 0x0c000 * 32   )
#define TC0_BASE                        (LSP_BASE + 0x10000                    )
#define TC0_BB_BASE                     (LSP_BASE + BB_OFFSET + 0x10000 * 32   )
#define TC1_BASE                        (LSP_BASE + 0x14000                    )
#define TC1_BB_BASE                     (LSP_BASE + BB_OFFSET + 0x14000 * 32   )
#define UART4_BASE                  (LSP_BASE + 0x18000                    )
#define UART4_BB_BASE               (LSP_BASE + BB_OFFSET + 0x18000 * 32   )
#define UART5_BASE                  (LSP_BASE + 0x1C000                    )
#define UART5_BB_BASE               (LSP_BASE + BB_OFFSET + 0x1C000 * 32   )
#define SPI_BASE                        (LSP_BASE + 0x20000                    )
#define I2C_BASE                        (LSP_BASE + 0x24000                    )
#define I2C_BB_BASE                     (LSP_BASE + BB_OFFSET + 0x24000 * 32   )
#define KBI_BASE                        (LSP_BASE + 0x28000                    )
#define MADC_BASE                       (LSP_BASE + 0x2C000                    )
#define WDT_BASE                        (LSP_BASE + 0x30000                    )
#define SYSCTL_BASE                     (LSP_BASE + 0x34000                    )
#define ISO7816_BASE                    (LSP_BASE + 0x38000                    )
#define RTC_BASE                        (LSP_BASE + 0x3C000                    )
#define NVM_BASE                        (LSP_BASE + 0x40000                    )
#define INTC_BASE                       (LSP_BASE + 0x44000                    )
#define LCD_BASE                        (LSP_BASE + 0x48000                    )
#define DMA_BASE                        (LSP_BASE + 0x4C000                    )

/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
#define GPIO                            ((GPIO_TypeDef    *) GPIO_BASE          )
#define EMU                             ((EMU_TypeDef     *) EMU_BASE           )
#define DBG                             ((DBG_TypeDef     *) DBG_BASE           )
#define CT                              ((CT_TypeDef      *) CT_BASE            )
#define AES_ENC                         ((AES_ENC_TypeDef *) AES_ENC_BASE       )
#define AES_DEC                         ((AES_DEC_TypeDef *) AES_DEC_BASE       )
#define GHASH                           ((GHASH_TypeDef   *) GHASH_BASE         )

#define UART0                       ((UART_TypeDef    *) UART0_BASE         )
#define UART1                       ((UART_TypeDef    *) UART1_BASE         )
#define UART2                       ((UART_TypeDef    *) UART2_BASE         )
#define UART3                       ((UART_TypeDef    *) UART3_BASE         )

#define TC0                         ((TC_TypeDef      *) TC0_BASE           )
#define TC1                         ((TC_TypeDef      *) TC1_BASE           )
#define UART4                       ((UART_TypeDef      *) UART4_BASE           )
#define UART5                       ((UART_TypeDef      *) UART5_BASE           )
#define SPI                             ((SPI_TypeDef     *) SPI_BASE           )
#define I2C                             ((I2C_TypeDef     *) I2C_BASE           )
#define KBI                             ((KBI_TypeDef     *) KBI_BASE           )
#define MADC                            ((MADC_TypeDef    *) MADC_BASE          )
#define WDT                             ((WDT_TypeDef     *) WDT_BASE           )

#define SYSCTL                          ((SYSCTL_TypeDef  *) SYSCTL_BASE        )
#define ISO7816                         ((ISO7816_TypeDef *) ISO7816_BASE       )
#define RTC                             ((RTC_TypeDef     *) RTC_BASE           )
#define INTC                            ((INTC_TypeDef    *) INTC_BASE          )
#define LCD                             ((LCD_TypeDef     *) LCD_BASE           )
#define DMA                             ((DMA_TypeDef     *) DMA_BASE           )
#define NVM                             ((NVM_TypeDef     *) NVM_BASE           )

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

/*@}*/ /* end of group RN821x_CMSIS */

#include "core_cm0.h"                       /* Cortex-M0 processor and core peripherals           */
#include "system_RN821x.h"                  /* RN821x System  include file                      */


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/
/** @addtogroup RN821x_Peripherals RN821x Peripherals
  RN821x Device Specific Peripheral registers structures
  @{
*/

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


#ifdef __cplusplus
}
#endif


//#endif

#endif  // __RN821x_H__
