#include "drv.h"


void ee_por(void);
void ee_wakeup(void);
void ee_chip_erase(void);
void ee_page_erase(uint32_t pg);
void ee_chip_prog(uint8_t * data);
void ee_page_prog(uint32_t pg, uint8_t * data);
void ee_half_chip_prog(uint8_t evenOdd, uint8_t * data);
void ee_half_sector_prog(uint8_t evenOdd, uint8_t * data);
