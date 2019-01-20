#include <rn821x.h>
#include <eeprom_driver.h>
#include "common.h"

#define EE_POR_NO_TBC         (0xfeed0000 | 0x1)
#define EE_TBC                (0xfeed0000 | 0x2)
#define EE_POR                (0xfeed0000 | 0x3)
#define EE_CLR_HVPL           (0xfeed0000 | 0x4)
#define EE_WAKEUP             (0xfeed0000 | 0x5)
#define EE_STANDBY            (0xfeed0000 | 0x7)
#define EE_ERASE_CHIP         (0xfeed0000 | 0x9)
#define EE_ERASE_SECTOR       (0xfeed0000 | 0xa)
#define EE_ERASE_PAGE         (0xfeed0000 | 0xb)
#define EE_PROG_CHIP          (0xfeed0000 | 0xc)
#define EE_PROG_HALF_CHIP     (0xfeed0000 | 0xd)
#define EE_PROG_HALF_SECOTR   (0xfeed0000 | 0xe)
#define EE_PROG_PAGE          (0xfeed0000 | 0xf)

//const uint32_t fl_tus[6] =    {1, 1, 3, 7, 15, 31};
//const uint32_t fl_tms[4][6] = {{28, 806, 806, 806, 806, 806},  //PageProg
//                              {30, 855, 855, 855, 855, 855},   //PageErase
//                              {31, 875, 875, 875, 875, 875},   //BulkProg
//                              {31, 895, 895, 895, 895, 895}};  //BulkErase

const uint32_t ee_tus[6] =    {1, 1, 3, 7, 15, 31};
//const uint32_t ee_tus[6] =    {1, 3, 5, 9, 17, 35};
const uint32_t ee_tms[4][6] = {{28, 806, 806, 806, 806, 806},
                              {30, 855, 855, 855, 855, 855},
                              {31, 875, 875, 875, 875, 875},
                              {31, 895, 895, 895, 895, 895}};

//const uint32_t fl_tp = 3;
//const uint32_t fl_te = 6;
const uint32_t ee_tp = 1;
const uint32_t ee_te = 2;
															
typedef enum
{
	PageProg = 0,
	PageErase = 1,
	BulkProg = 2,
	BulkErase = 3,
}
MIP_CMD;

//typedef struct {
//  __IO uint32_t CMD;               /* offset 0x0   command           */
//  __IO uint32_t PGA;               /* offset 0x4   page address      */
//  __IO uint32_t STA;               /* offset 0x8   status            */
//  __IO uint32_t TBC0;              /* offset 0xc   trim byte 0       */
//  __IO uint32_t TBC1;              /* offset 0x10  trim byte 1       */
//  __IO uint32_t PROT;              /* offset 0x14  sector protect bit*/  
//  __IO uint32_t TW;                /* offset 0x18 cache contrl */
//  __IO uint32_t CC_CTRL;           /* offset 0x1c  ce control */
//  __IO uint32_t TUS;               /* offset 0x20 cache hit counter low word */
//  __IO uint32_t TMS;               /* offset 0x24 cache hit counter low word */
//  __IO uint32_t TP;                /* offset 0x28 cache hit counter low word */
//  __IO uint32_t TE;                /* offset 0x2c cache hit counter low word */
//  __IO uint32_t CC_HIT_LO;         /* offset 0x30 cache hit counter low word */
//  __IO uint32_t CC_HIT_HI;         /* offset 0x34 cache hit counter high word */
//  __IO uint32_t CC_INST_LO;        /* offset 0x38 cache inst counter low word */
//  __IO uint32_t CC_INST_HI;    
//} FLASHTypeDef;

typedef struct {
  __IO uint32_t CMD;               /* offset 0x0   command           */
  __IO uint32_t PGA;               /* offset 0x4   page address      */
  __IO uint32_t STA;               /* offset 0x8   status            */
  __IO uint32_t TBC0;              /* offset 0xc   trim byte 0       */
  __IO uint32_t TBC1;              /* offset 0x10  trim byte 1       */
  __IO uint32_t PROT;              /* offset 0x14  sector protect bit*/
  __IO uint32_t TW;                /* offset 0x18 cache contrl        */
  __IO uint32_t RESERVED;          /* offset 0x1c                     */
  __IO uint32_t TUS;               /* offset 0x20 cache hit counter low word */
  __IO uint32_t TMS;               /* offset 0x24 cache hit counter low word */
  __IO uint32_t TP;                /* offset 0x28 cache hit counter low word */
  __IO uint32_t TE;                /* offset 0x2c cache hit counter low word */
} EEPROMTypeDef;

uint32_t  /*fl_mem_base, */ee_mem_base;
//#define FLASH_T  ((FLASHTypeDef *)(FL_MEM_BASE + 0x00800000))
//#define EEPROM_T ((EEPROMTypeDef *)(EE_MEM_BASE + 0x00800000))

static void getRemapAddr(void)
{
	uint8_t map;
	
	map = SYSCTL->MAP_CTRL & 0x7;
	
	switch (map)
	{
		case 0:
//		  fl_mem_base = 0x0;
		  ee_mem_base = 0x08000000;
		  break;
		case 1:
//		  fl_mem_base = 0x08000000;
		  ee_mem_base = 0x0;
		  break;
		case 2:
//		  fl_mem_base = 0x10000000;
		  ee_mem_base = 0x08000000;
		  break;
		case 3:
//		  fl_mem_base = 0x18000000;
		  ee_mem_base = 0x08000000;
		  break;
	}
}

void ee_check(EEPROMTypeDef *ee_t){
  while(ee_t->STA & 0x1)
    ;
}

static void setMipTime(uint32_t freq, MIP_CMD cmd)
{
//	FLASHTypeDef *fl_t = (FLASHTypeDef *)(fl_mem_base + 0x00800000);
	EEPROMTypeDef *ee_t = (EEPROMTypeDef *)(ee_mem_base + 0x00800000);
	
	getRemapAddr();
	switch (freq)
	{
		case 32768:
//			fl_t->TUS = fl_tus[0];
//		  fl_t->TMS = fl_tms[cmd][0];
		  ee_t->TUS = ee_tus[0];
		  ee_t->TMS = ee_tms[cmd][0];
		  break;
		case 1843200:
//			fl_t->TUS = fl_tus[1];
//		  fl_t->TMS = fl_tms[cmd][1];
		  ee_t->TUS = ee_tus[1];
		  ee_t->TMS = ee_tms[cmd][1];
		  break;
		case 3686400:
//			fl_t->TUS = fl_tus[2];
//		  fl_t->TMS = fl_tms[cmd][2];
		  ee_t->TUS = ee_tus[2];
		  ee_t->TMS = ee_tms[cmd][2];
		  break;
		case 7372800:
//			fl_t->TUS = fl_tus[3];
//		  fl_t->TMS = fl_tms[cmd][3];
		  ee_t->TUS = ee_tus[3];
		  ee_t->TMS = ee_tms[cmd][3];
		  break;
		case 14745600:
//			fl_t->TUS = fl_tus[4];
//		  fl_t->TMS = fl_tms[cmd][4];
		  ee_t->TUS = ee_tus[4];
		  ee_t->TMS = ee_tms[cmd][4];
		  break;
		case 29491200:
//			fl_t->TUS = fl_tus[5];
//		  fl_t->TMS = fl_tms[cmd][5];
		  ee_t->TUS = ee_tus[5];
		  ee_t->TMS = ee_tms[cmd][5];
		  break;	
    default:              /* RC clock */
//			fl_t->TUS = fl_tus[1];
//		  fl_t->TMS = fl_tms[cmd][1];
		  ee_t->TUS = ee_tus[1];
		  ee_t->TMS = ee_tms[cmd][1];
		  break;		
	}
//	fl_t->TP = fl_tp;
//	fl_t->TE = fl_te;
	ee_t->TP = ee_tp;
	ee_t->TE = ee_te;
}

static void sendCmd(uint32_t cmd)
{
	EEPROMTypeDef *ee_t;
	
	getRemapAddr();
	ee_t = (EEPROMTypeDef *)(ee_mem_base + 0x00800000);
  switch(cmd)
  {
    case EE_ERASE_CHIP:
    case EE_ERASE_SECTOR:
      setMipTime(getSysclk(), BulkErase);
      break;
    case EE_ERASE_PAGE:
      setMipTime(getSysclk(), PageErase);
      break;
      
    case EE_PROG_CHIP:
    case EE_PROG_HALF_CHIP:
    case EE_PROG_HALF_SECOTR:
      setMipTime(getSysclk(), BulkProg);
      break;
    case EE_PROG_PAGE:
      setMipTime(getSysclk(), PageProg);
      break;
  }
    
  ee_t->CMD = cmd;
  ee_check(ee_t);
}

void ee_por(void){
	EEPROMTypeDef *ee_t;
	
	getRemapAddr();
	ee_t = (EEPROMTypeDef *)(ee_mem_base + 0x00800000);
  SYSCTL->SYS_PS = 0x82;
  SYSCTL->MOD0_EN |= (0xA5890000 | (1 << 2));   /* enable EEPROM_T AHB clock */
  
  ee_check(ee_t);
  ee_t->CMD = EE_POR;
  ee_check(ee_t);
  
  ee_t->TE = 2;
  ee_t->TP = 1;
}

void ee_wakeup(void){
	EEPROMTypeDef *ee_t;
	
	getRemapAddr();
	ee_t = (EEPROMTypeDef *)(ee_mem_base + 0x00800000);
  ee_t->CMD = EE_WAKEUP;  
}

void ee_hvpl_clear(void){
	EEPROMTypeDef *ee_t;
	
	getRemapAddr();
	ee_t = (EEPROMTypeDef *)(ee_mem_base + 0x00800000);
	
  ee_check(ee_t);
  
  ee_t->CMD = EE_CLR_HVPL;
  ee_check(ee_t);
}

void ee_chip_erase(void){
  uint32_t prot;
	EEPROMTypeDef *ee_t;
	
	getRemapAddr();
	ee_t = (EEPROMTypeDef *)(ee_mem_base + 0x00800000);
  
  ee_check(ee_t);
  
  prot = ee_t->PROT;
  ee_t->PROT = 0xfeed0000;
	sendCmd(EE_ERASE_CHIP);
  ee_check(ee_t);
  ee_t->PROT = prot;
}

void ee_page_erase(uint32_t pg)
{
  uint32_t prot, i;
	EEPROMTypeDef *ee_t;
	
	getRemapAddr();
	ee_t = (EEPROMTypeDef *)(ee_mem_base + 0x00800000);
  
  if(pg > (EE_CHIP_SIZE / EE_PAGE_SIZE + 4))
    return;
  
  ee_check(ee_t);
  prot = ee_t->PROT;
	ee_t->PGA = pg;
  ee_t->PROT = 0xfeedffff & (~(1 << (pg / (EE_PROT_SIZE / EE_PAGE_SIZE))));
	for(i = 0; i < EE_PAGE_SIZE; i++)
	{
		*(uint8_t *)(EE_MEM_BASE + i) = 0x0;
	}
  sendCmd(EE_ERASE_PAGE);
  ee_check(ee_t);
  
  ee_t->PROT = prot;
}


void ee_chip_prog(uint8_t * data){
  uint32_t prot, i;
	EEPROMTypeDef *ee_t;
	
	getRemapAddr();
	ee_t = (EEPROMTypeDef *)(ee_mem_base + 0x00800000);
  
  ee_check(ee_t);
  
  prot = ee_t->PROT;
  ee_t->PROT = 0xfeed0000;
  
  sendCmd(EE_ERASE_CHIP);
  ee_check(ee_t);
  
  ee_t->CMD = EE_CLR_HVPL;
  ee_check(ee_t);
  
  for(i = 0; i < EE_PAGE_SIZE; i++)
    *(uint8_t *)(EE_MEM_BASE + i) = data[i];
  
  sendCmd(EE_PROG_CHIP);
  ee_check(ee_t);
  
  ee_t->PROT = prot;
}

void ee_half_chip_prog(uint8_t evenOdd, uint8_t * data){
  uint32_t prot, i;
	EEPROMTypeDef *ee_t;
	
	getRemapAddr();
	ee_t = (EEPROMTypeDef *)(ee_mem_base + 0x00800000);
  
  ee_check(ee_t);
  
  prot = ee_t->PROT;
  ee_t->PROT = 0xfeed0000;
  
  sendCmd(EE_ERASE_CHIP);
  ee_check(ee_t);
  
  ee_t->CMD = EE_CLR_HVPL;
  ee_check(ee_t);
  
  for(i = 0; i < EE_PAGE_SIZE; i++)
    *(uint8_t *)(EE_MEM_BASE + i) = data[i];
  
  ee_t->PGA = evenOdd;   /* 0: Even, 1: Odd */
  sendCmd(EE_PROG_HALF_CHIP);
  ee_check(ee_t);
  
  ee_t->PROT = prot;
}

void ee_half_sector_prog(uint8_t evenOdd, uint8_t * data){
  uint32_t prot, i;
	EEPROMTypeDef *ee_t;
	
	getRemapAddr();
	ee_t = (EEPROMTypeDef *)(ee_mem_base + 0x00800000);
  
  ee_check(ee_t);
  
  prot = ee_t->PROT;
  ee_t->PROT = 0xfeed0000;
  
  ee_t->PGA = evenOdd;
  sendCmd(EE_ERASE_CHIP);
  ee_check(ee_t);
  
  ee_t->CMD = EE_CLR_HVPL;
  ee_check(ee_t);
  
  for(i = 0; i < EE_PAGE_SIZE; i++)
    *(uint8_t *)(EE_MEM_BASE + i) = data[i];
  
  ee_t->PGA = evenOdd;   /* 0: Even, 1: Odd */
  sendCmd(EE_PROG_HALF_SECOTR);
  ee_check(ee_t);
  
  ee_t->PROT = prot;
}

void ee_page_prog(uint32_t pg, uint8_t * data){
  uint32_t prot, i;
	EEPROMTypeDef *ee_t;
	
	getRemapAddr();
	ee_t = (EEPROMTypeDef *)(ee_mem_base + 0x00800000);
  
  if(pg > (EE_CHIP_SIZE / EE_PAGE_SIZE + 4))
    return;
  ee_check(ee_t);
  
  prot = ee_t->PROT;
  ee_t->PROT = 0xfeed0000;
  ee_t->PGA = pg;
  
  ee_t->CMD = EE_CLR_HVPL;
  ee_check(ee_t);
  
  for(i = 0; i < EE_PAGE_SIZE; i++)
    *(uint8_t *)(EE_MEM_BASE + i) = data[i];
	
  sendCmd(EE_ERASE_PAGE);
  ee_check(ee_t);
  
  sendCmd(EE_PROG_PAGE);
  ee_check(ee_t);
  
  ee_t->PROT = prot;
}
