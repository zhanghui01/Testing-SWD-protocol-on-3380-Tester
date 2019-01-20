#include "drv.h"

#define FL_POR_NO_TBC         (0xfeed0000 | 0x1)
#define FL_TBC                (0xfeed0000 | 0x2)
#define FL_POR                (0xfeed0000 | 0x3)
#define FL_CLR_HVPL           (0xfeed0000 | 0x4)
#define FL_WAKEUP             (0xfeed0000 | 0x5)
#define FL_QACTIVE            (0xfeed0000 | 0x6)
#define FL_STANDBY            (0xfeed0000 | 0x7)
#define FL_ERASE_PLANE        (0xfeed0000 | 0x8)
#define FL_ERASE_CHIP         (0xfeed0000 | 0x9)
#define FL_ERASE_ROW          (0xfeed0000 | 0xa)
#define FL_ERASE_PAGE         (0xfeed0000 | 0xb)
#define FL_PROG_CHIP          (0xfeed0000 | 0xc)
#define FL_PROG_HALF_CHIP     (0xfeed0000 | 0xd)
#define FL_PROG_ROW           (0xfeed0000 | 0xe)
#define FL_PROG_PAGE          (0xfeed0000 | 0xf)

#define FLASHTBC0RST          0x010A0D0F
#define FLASHTBC1RST          0xF0030306

#define BIT_ALL_EN              0x0
#define BIT_ALL_DIS             (0x1 << 0)
#define BIT_PAGE_PROG_EN        (0x1 << 8)
#define BIT_ROW_PROG_EN         (0x1 << 9)
#define BIT_HALF_BULK_PROG_EN   (0x1 << 10)
#define BIT_BULK_PROG_EN        (0x1 << 11)
#define BIT_PAGE_ERASE_EN       (0x1 << 12)
#define BIT_ROW_ERASE_EN        (0x1 << 13)
#define BIT_BULK_ERASE_EN       (0x1 << 14)
#define BIT_PLANE_ERASE_EN      (0x1 << 15)

const uint32_t fl_tus[6] =    {1, 1, 3, 7, 15, 31};
const uint32_t fl_tms[4][6] = {{28, 806, 806, 806, 806, 806},  //PageProg
                              {30, 855, 855, 855, 855, 855},  //PageErase
                              {31, 875, 875, 875, 875, 875},  //BulkProg
                              {31, 895, 895, 895, 895, 895}}; //BulkErase

//const uint32_t ee_tus[6] =    {1, 1, 3, 7, 15, 31};
//const uint32_t ee_tms[4][6] = {{28, 806, 806, 806, 806, 806},
//                              {30, 855, 855, 855, 855, 855},
//                              {31, 875, 875, 875, 875, 875},
//                              {31, 895, 895, 895, 895, 895}};

const uint32_t fl_tp = 3;
const uint32_t fl_te = 6;
//const uint32_t ee_tp = 1;
//const uint32_t ee_te = 2;
															
typedef enum
{
	PageProg = 0,
	PageErase = 1,
	BulkProg = 2,
	BulkErase = 3,
}
MIP_CMD;

typedef struct {
  __IO uint32_t CMD;               /* offset 0x0   command           */
  __IO uint32_t PGA;               /* offset 0x4   page address      */
  __IO uint32_t STA;               /* offset 0x8   status            */
  __IO uint32_t TBC0;              /* offset 0xc   trim byte 0       */
  __IO uint32_t TBC1;              /* offset 0x10  trim byte 1       */
  __IO uint32_t PROT;              /* offset 0x14  sector protect bit*/  
  __IO uint32_t TW;                /* offset 0x18 cache contrl */
  __IO uint32_t CC_CTRL;           /* offset 0x1c  ce control */
  __IO uint32_t TUS;               /* offset 0x20 cache hit counter low word */
  __IO uint32_t TMS;               /* offset 0x24 cache hit counter low word */
  __IO uint32_t TP;                /* offset 0x28 cache hit counter low word */
  __IO uint32_t TE;                /* offset 0x2c cache hit counter low word */
  __IO uint32_t CC_HIT_LO;         /* offset 0x30 cache hit counter low word */
  __IO uint32_t CC_HIT_HI;         /* offset 0x34 cache hit counter high word */
  __IO uint32_t CC_INST_LO;        /* offset 0x38 cache inst counter low word */
  __IO uint32_t CC_INST_HI;    
} FLASHTypeDef;

//typedef struct {
//  __IO uint32_t CMD;               /* offset 0x0   command           */
//  __IO uint32_t PGA;               /* offset 0x4   page address      */
//  __IO uint32_t STA;               /* offset 0x8   status            */
//  __IO uint32_t TBC0;              /* offset 0xc   trim byte 0       */
//  __IO uint32_t TBC1;              /* offset 0x10  trim byte 1       */
//  __IO uint32_t PROT;              /* offset 0x14  sector protect bit*/
//  __IO uint32_t TW;                /* offset 0x18 cache contrl        */
//  __IO uint32_t RESERVED;          /* offset 0x1c                     */
//  __IO uint32_t TUS;               /* offset 0x20 cache hit counter low word */
//  __IO uint32_t TMS;               /* offset 0x24 cache hit counter low word */
//  __IO uint32_t TP;                /* offset 0x28 cache hit counter low word */
//  __IO uint32_t TE;                /* offset 0x2c cache hit counter low word */
//} EEPROMTypeDef;

uint32_t  fl_mem_base/*, ee_mem_base*/;
//#define FLASH_T  ((FLASHTypeDef *)(FL_MEM_BASE + 0x00800000))
//#define EEPROM_T ((EEPROMTypeDef *)(EE_MEM_BASE + 0x00800000))

static void getRemapAddr(void)
{
	uint8_t map;
	
	map = SYSCTL->MAP_CTRL & 0x7;
	
	switch (map)
	{
		case 0:
		  fl_mem_base = 0x0;
//		  ee_mem_base = 0x08000000;
		  break;
		case 1:
		  fl_mem_base = 0x08000000;
//		  ee_mem_base = 0x0;
		  break;
		case 2:
		  fl_mem_base = 0x10000000;
//		  ee_mem_base = 0x08000000;
		  break;
		case 3:
		  fl_mem_base = 0x18000000;
//		  ee_mem_base = 0x08000000;
		  break;
	}
}

void fl_check(FLASHTypeDef *fl_t){
  while(fl_t->STA & 0x1)
    ;
}

void fl_hvpl_clear(void){
	FLASHTypeDef *fl_t = (FLASHTypeDef *)(fl_mem_base + 0x00800000);
	
	getRemapAddr();
  fl_check(fl_t);
  
  fl_t->CMD = FL_CLR_HVPL;
  fl_check(fl_t);
}

static void setMipTime(uint32_t freq, MIP_CMD cmd)
{
	FLASHTypeDef *fl_t = (FLASHTypeDef *)(fl_mem_base + 0x00800000);
//	EEPROMTypeDef *ee_t = (EEPROMTypeDef *)(ee_mem_base + 0x00800000);
	
	getRemapAddr();
	switch (freq)
	{
		case 32768:
			fl_t->TUS = fl_tus[0];
		  fl_t->TMS = fl_tms[cmd][0];
//		  ee_t->TUS = ee_tus[0];
//		  ee_t->TMS = fl_tms[cmd][0];
		  break;
		case 1843200:
			fl_t->TUS = fl_tus[1];
		  fl_t->TMS = fl_tms[cmd][1];
//		  ee_t->TUS = ee_tus[1];
//		  ee_t->TMS = fl_tms[cmd][1];
		  break;
		case 3686400:
			fl_t->TUS = fl_tus[2];
		  fl_t->TMS = fl_tms[cmd][2];
//		  ee_t->TUS = ee_tus[2];
//		  ee_t->TMS = fl_tms[cmd][2];
		  break;
		case 7372800:
			fl_t->TUS = fl_tus[3];
		  fl_t->TMS = fl_tms[cmd][3];
//		  ee_t->TUS = ee_tus[3];
//		  ee_t->TMS = fl_tms[cmd][3];
		  break;
		case 14745600:
			fl_t->TUS = fl_tus[4];
		  fl_t->TMS = fl_tms[cmd][4];
//		  ee_t->TUS = ee_tus[4];
//		  ee_t->TMS = fl_tms[cmd][4];
		  break;
		case 29491200:
			fl_t->TUS = fl_tus[5];
		  fl_t->TMS = fl_tms[cmd][5];
//		  ee_t->TUS = ee_tus[5];
//		  ee_t->TMS = fl_tms[cmd][5];
		  break;	  
	}
	fl_t->TP = fl_tp;
	fl_t->TE = fl_te;
//	ee_t->TP = ee_tp;
//	ee_t->TE = ee_te;
}

static void sendCmd(uint32_t cmd)
{
	FLASHTypeDef *fl_t;
	
	getRemapAddr();
	fl_t = (FLASHTypeDef *)(fl_mem_base + 0x00800000);
  switch(cmd)
  {
    case FL_ERASE_PLANE:
    case FL_ERASE_CHIP:
      setMipTime(getSysclk(), BulkErase);
      break;
    case FL_ERASE_ROW:
    case FL_ERASE_PAGE:
      setMipTime(getSysclk(), PageErase);
      break;
      
    case FL_PROG_CHIP:
    case FL_PROG_HALF_CHIP:
      setMipTime(getSysclk(), BulkProg);
      break;
    case FL_PROG_ROW:
    case FL_PROG_PAGE:
      setMipTime(getSysclk(), PageProg);
      break;
  }
    
  fl_t->CMD = cmd;
  fl_check(fl_t);
}

void fl_por(void){
	FLASHTypeDef *fl_t;
	
	getRemapAddr();
	fl_t = (FLASHTypeDef *)(fl_mem_base + 0x00800000);
  SYSCTL->SYS_PS = 0x82;
//  SYSCTL->TEST_CTRL |= (0XA5730000 | (1 << 8));
  SYSCTL->SYS_PS = 0x0;
  
  fl_check(fl_t);
  fl_t->CMD = FL_POR;
  fl_check(fl_t);
	
	fl_t->TE = 6;
	fl_t->TP = 3;
}

void fl_chip_erase(void){
	FLASHTypeDef *fl_t;
  uint32_t prot;
  
	getRemapAddr();
	fl_t = (FLASHTypeDef *)(fl_mem_base + 0x00800000);
  fl_check(fl_t);
  
  prot = fl_t->PROT;
  fl_t->PROT = 0xfeed0000;
	sendCmd(FL_ERASE_CHIP);
  fl_check(fl_t);
  fl_t->PROT = prot;
}

void fl_page_erase(uint32_t pg){
	FLASHTypeDef *fl_t;
  uint32_t prot;
	
	getRemapAddr();
  
	fl_t = (FLASHTypeDef *)(fl_mem_base + 0x00800000);
  if(pg > (FL_CHIP_SIZE / FL_PAGE_SIZE + 4))
    return;
  
  fl_check(fl_t);
  prot = fl_t->PROT;
  fl_t->PGA = pg;
  fl_t->PROT = 0xfeedffff & (~(1 << (pg / (FL_PROT_SIZE / FL_PAGE_SIZE))));
	sendCmd(FL_ERASE_PAGE);
  fl_check(fl_t);
  
  fl_t->PROT = prot;
}

void fl_chip_prog(uint8_t * data){
	FLASHTypeDef *fl_t;
  uint32_t prot, i;
	
	getRemapAddr();
	fl_t = (FLASHTypeDef *)(fl_mem_base + 0x00800000);
  
  fl_check(fl_t);
  
  prot = fl_t->PROT;
  fl_t->PROT = 0xfeed0000;
  
	sendCmd(FL_ERASE_CHIP);
  fl_check(fl_t);
  
  fl_t->CMD = FL_CLR_HVPL;
  fl_check(fl_t);
  
  for(i = 0; i < FL_PAGE_SIZE; i++)
    *(uint8_t *)(fl_mem_base + i) = data[i];
  
	sendCmd(FL_PROG_CHIP);
  fl_check(fl_t);
  
  fl_t->PROT = prot;
}

void fl_page_prog(uint32_t pg, uint8_t * data){
	FLASHTypeDef *fl_t;
  uint32_t prot, i;
	
	getRemapAddr();
	fl_t = (FLASHTypeDef *)(fl_mem_base + 0x00800000);
  
  if(pg > (FL_CHIP_SIZE / FL_PAGE_SIZE + 4))
    return;
  fl_check(fl_t);
  
  prot = fl_t->PROT;
  fl_t->PROT = 0xfeed0000;
  fl_t->PGA = pg;
  
	sendCmd(FL_ERASE_PAGE);
  fl_check(fl_t);
  
  fl_t->CMD = FL_CLR_HVPL;
  fl_check(fl_t);
  
  for(i = 0; i < FL_PAGE_SIZE; i++)
    *(uint8_t *)(fl_mem_base + i) = data[i];
  
	sendCmd(FL_PROG_PAGE);
  fl_check(fl_t);
  
  fl_t->PROT = prot;
}
