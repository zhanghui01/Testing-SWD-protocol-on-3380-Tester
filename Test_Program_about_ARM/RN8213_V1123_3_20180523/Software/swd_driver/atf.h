#ifndef __SWD_INTACTNESS_ATF_H
#define __SWD_INTACTNESS_ATF_H

#define SWD_ATF_NOT_INIT       0
#define SWD_ATF_INIT_OK        1
#define SWD_ATF_IMG_LOAD_OK    2
#define SWD_ATF_IMG_LOAD_FAIL  3

#define  TC_START        0xa5a5a5a5u
#define  TC_OK              0x98765432u
#define  TC_FAIL           0x12345678u

#define SOC_REG_PS                       0x40034030
#define SOC_REG_REMAP_ADDR     0x40034018

#define SOC_MONITOR_ADDR          0x37f8

typedef enum{
    FLASH_RESIDENT = 0
  , EEPROM_RESIDENT  = 1
  , SRAM_RESIDENT = 2
}e_atf_img_resident_type,* pe_atf_img_resident_type;


typedef struct{
  unsigned long           load_addr;
  unsigned long           load_remap;
  unsigned long           img_len;
  unsigned char *         descr;
  unsigned long           timeout;
  unsigned char *         pImg;
}s_atf_img,*ps_atf_img;

//memory program and verify
unsigned long atf_flash_wr(unsigned long to_addr,unsigned char * pBuf,unsigned long len) ;
unsigned long atf_eeprom_wr(unsigned long to_addr,unsigned char * pBuf,unsigned long len) ;
unsigned long atf_sram_wr(unsigned long to_addr,unsigned char * pBuf,unsigned long len);
unsigned long atf_mem_verify(unsigned long to_addr,unsigned char * pBuf,unsigned long len);

//execute and monitor soc
unsigned long atf_img_exec(ps_atf_img pImg);
unsigned long atf_img_monitor(ps_atf_img pImg);

unsigned long atf_flash_img_load(ps_atf_img pImg);
unsigned long atf_eeprom_img_load(ps_atf_img pImg);
unsigned long atf_sram_img_load(ps_atf_img pImg);

unsigned long atf_remap_set(unsigned long remap);
unsigned long soc_probe(unsigned char halt);

/**
 * @brief atf_img_run
 *    load and execute image, return execute result
 * @param 
 *    image data structure
 * @return 
 *    image run result
 *      TC_FAIL: image run fail
 *      TC_OK:   image run pass
 *      other:      image load fail
 *
 */
unsigned long atf_img_run(ps_atf_img pImg);

#endif
