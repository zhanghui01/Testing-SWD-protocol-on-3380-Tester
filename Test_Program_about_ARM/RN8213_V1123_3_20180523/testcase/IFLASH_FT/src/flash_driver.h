#include <rn821x.h>

#define CHIP_SIZE             (192 * 1024)
#define  CHIP_SIZE_C          (128 * 1024)
#define PROT_SIZE             (16 * 1024)
#define SECTOR_SIZE           (16 * 1024)

#define FL_MEM_BASE  ((SYSCTL->MAP_CTRL & 0x7) == 0) ? 0x0 : ((SYSCTL->MAP_CTRL & 0x7) == 1) ? 0x08000000 : ((SYSCTL->MAP_CTRL & 0x7) == 2) ? 0x10000000 : 0x18000000
#define EE_MEM_BASE  ((SYSCTL->MAP_CTRL & 0x7) == 0) ? 0x08000000 : ((SYSCTL->MAP_CTRL & 0x7) == 1) ? 0x0: ((SYSCTL->MAP_CTRL & 0x7) == 2) ? 0x08000000 : 0x08000000

void fl_por(void);
void fl_chip_erase(void);
void fl_page_erase(uint32_t pg);
void fl_hvpl_clear(void);
void fl_chip_prog(uint8_t * data);
void fl_page_prog(uint32_t pg, uint8_t * data);
