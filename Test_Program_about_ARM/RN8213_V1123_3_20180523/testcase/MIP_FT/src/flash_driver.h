#include <rn821x.h>




void fl_por(void);
void fl_chip_erase(void);
void fl_page_erase(uint32_t pg);
void fl_hvpl_clear(void);
void fl_chip_prog(uint8_t * data);
void fl_page_prog(uint32_t pg, uint8_t * data);
