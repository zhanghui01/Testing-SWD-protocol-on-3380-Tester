#include <rn821x.h>

#define CHIP_SIZE             (32 * 1024)
#define PAGE_SIZE             (64)
#define PAGE_SIZE_C           (128)
#define PROT_SIZE             (2 * 1024)
#define SECTOR_SIZE           (64 * PAGE_SIZE)

#define FL_MEM_BASE  (((SYSCTL->MAP_CTRL & 0x7) == 0) ? 0x0 : ((SYSCTL->MAP_CTRL & 0x7) == 1) ? 0x08000000 : ((SYSCTL->MAP_CTRL & 0x7) == 2) ? 0x10000000 : 0x18000000)
#define EE_MEM_BASE  (((SYSCTL->MAP_CTRL & 0x7) == 0) ? 0x08000000 : ((SYSCTL->MAP_CTRL & 0x7) == 1) ? 0x0: ((SYSCTL->MAP_CTRL & 0x7) == 2) ? 0x08000000 : 0x08000000)
//#define EE_MEM_BASE 0x08000000

//void ee_check(void);
void ee_por(void);
void ee_wakeup(void);
void ee_chip_erase(void);
void ee_page_erase(uint32_t pg);
void ee_sector_erase(uint32_t sect);
void ee_chip_prog(uint8_t * data);
void ee_page_prog(uint32_t pg, uint8_t * data);
void ee_half_chip_prog(uint8_t evenOdd, uint8_t * data);
void ee_half_sector_prog(uint8_t evenOdd, uint8_t * data);
