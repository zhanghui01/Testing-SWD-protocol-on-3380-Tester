/******************************************************************************
 * @file     sysctrl_reg.h
 * @brief    System control driver header file for RN821x series SOC
 * @author   Renergy Technology
 * @version  V1.01
 * @date V1.01 2017-06-23 modify address define of SYS_TC_BASE from (0x52000000 + 0x10000  ) to (0x50000000 + 0x10000) to avoid err change of p00 p01
 * 			V1.00 2017-05-03 INITIAL
 *
 * @note
 * Copyright (C) 2017, Renergy Technology Inc. All rights reserved.
 *
 *
 ******************************************************************************/
#ifndef __SYSCTRL_REG_H
#define __SYSCTRL_REG_H


#include <stdint.h>                      /* standard types definitions  */     


/*--------------------- SYS TC --------------------------------------------------*/
typedef struct {
  __IO uint32_t CTRL  ;       
  __IO uint32_t LOAD  ;      
  __IO uint32_t VAL;   
} SYS_TC_TypeDef;
#define SYS_TC_BASE                     (0x50000000 + 0x10000  )    
#define SYS_TC						((SYS_TC_TypeDef  *) SYS_TC_BASE        )    

#define SPL0_OFFSET  0x00c08000
#define PAGE_SIZE    64

/* reg offset address in eeprom spline */
#define ANAADC_OFFSET      8
#define ANACLK1_OFFSET     9
#define ANACLK2_OFFSET     10
#define ANALDO_OFFSET      11
#define ANABGR_OFFSET      12
#define ANAPSW_OFFSET      13
//#define RTCTADJ_OFFSET     14
//#define RTCZT_OFFSET       15
#define TPSCTRL_OFFSET     16
#define ADCTEST_OFFSET     17
#define CACHECTRL_OFFSET   18

#define RTCMODE_OFFSET     4
#define RTCDOTA0_OFFSET    5
#define RTCALPHAL_OFFSET   6
#define RTCALPHAH_OFFSET   7
#define RTCXT0_OFFSET      8
#define RTCBETA_OFFSET     9
#define WDTCTRL_OFFSET     10
#define EMAP_OFFSET        11
#define WDTHALT_OFFSET     12
#define WDTSTBY_OFFSET     13	
#define RTCTADJ_OFFSET     14
#define RTCZT_OFFSET       15
#define EMAP4_OFFSET       30

/* reg address */
#define ANAADC_REG         *(uint32_t *)(0x40034048)
#define ANACLK1_REG        *(uint32_t *)(0x4003404c)
#define ANACLK2_REG        *(uint32_t *)(0x40034050)
#define ANALDO_REG         *(uint32_t *)(0x40034054)
#define ANABGR_REG         *(uint32_t *)(0x40034058)
#define ANAPSW_REG         *(uint32_t *)(0x4003405c)
#define ANAPWR_REG         *(uint32_t *)(0x40034068)
#define ANACLK3_REG        *(uint32_t *)(0x4003406c)
#define ANASYS_REG         *(uint32_t *)(0x40034070)

#define LOSCCFG2_REG       *(uint32_t *)(0x4003C070)
#define RTCTADJ_REG        *(uint32_t *)(0x4003C058)
#define RTCZT_REG          *(uint32_t *)(0x4003C060)
#define RTCMODE_REG        *(uint32_t *)(0x4003C044)
#define RTCDOTA0_REG       *(uint32_t *)(0x4003C048)
#define RTCALPHAL_REG      *(uint32_t *)(0x4003C04c)
#define RTCALPHAH_REG      *(uint32_t *)(0x4003C050)
#define RTCXT0_REG         *(uint32_t *)(0x4003C054)
#define RTCBETA_REG        *(uint32_t *)(0x4003C05c)

#define TPSCTRL_REG        *(uint32_t *)(0x4002C018)
#define ADCTEST_REG        *(uint32_t *)(0x4002C01c)
#define MADCANABGR_REG     *(uint32_t *)(0x4002C020)
#define MADCANALDO_REG     *(uint32_t *)(0x4002C028)
#define MADCANARCH_REG     *(uint32_t *)(0x4002C02c)
#define MADCSARCTL2_REG    *(uint32_t *)(0x4002C030)
#define ANATIEHL_REG       *(uint32_t *)(0x4002C038)

#define FLCFG_REG          *(uint32_t *)(0x50014054)       

#define WDTCTRL_REG        *(uint32_t *)(0x40030004)
#define WDTHALT_REG        *(uint32_t *)(0x40030014)
#define WDTSTBY_REG        *(uint32_t *)(0x40030018)

#define CHIP_RA9103A   0
#define CHIP_RA9103D   1
#define CHIP_RA9103C   2
#define CHIP_RA9103E   3
#define CHIP_RA9103F   4

//for RA9103A/D
#define FL_TW_32K_98NS  (0)//(0)
#define FL_TW_1M_98NS   (0)//(0)
#define FL_TW_3M_98NS   (0)//(0)
#define FL_TW_7M_98NS   (2)//(2)
#define FL_TW_14M_98NS  (2)//(2)
#define FL_TW_29M_98NS  (5)//(5)

//for RA9103C/E/F
#define FL_TW_32K_198NS  (0)//(0)
#define FL_TW_1M_198NS   (0)//(0)
#define FL_TW_3M_198NS   (1)//(1)
#define FL_TW_7M_198NS   (2)//(2)
#define FL_TW_14M_198NS  (4)//(3)
#define FL_TW_29M_198NS  (8)//(8)

#define EE_TW_32K_98NS   (0)//(0)
#define EE_TW_1M_98NS    (0)//(0)
#define EE_TW_3M_98NS    (0)//(0)
#define EE_TW_7M_98NS    (1)//(0)
#define EE_TW_14M_98NS   (2)//(2)
#define EE_TW_29M_98NS   (5)//(3)

#define EE_TW_32K_198NS  (0)//(0)
#define EE_TW_1M_198NS   (0)//(0)
#define EE_TW_3M_198NS   (1)//(0)
#define EE_TW_7M_198NS   (2)//(0)
#define EE_TW_14M_198NS  (5)//(2)
#define EE_TW_29M_198NS  (10)//(3)


#endif
