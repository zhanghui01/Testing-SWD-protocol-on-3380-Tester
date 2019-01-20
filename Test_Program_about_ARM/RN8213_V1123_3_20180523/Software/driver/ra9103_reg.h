
//All the resigster is define in REMAP3

#define SYS_OSC_CTRL1   0x40034000                          
#define SYS_SYS_MODE    0x40034004                           
#define SYS_SYS_PD      0x40034008                            
#define SYS_ADC_CTRL    0x4003400c                           
#define SYS_OSC_CTRL2   0x40034010                           
#define SYS_SYS_RST     0x40034014                            
#define SYS_SYS_CTRL    0x40034018                           
#define SYS_MOD0_EN     0x4003401c                            
#define SYS_MOD1_EN     0x40034020                            
#define SYS_INTC_EN     0x40034024                           
#define SYS_KBI_EN      0x40034028                            
#define SYS_CHIP_ID     0x4003402c                            
#define SYS_SYS_PS      0x40034030                                                
#define SYS_TEST_CTRL   0x40034040                          
#define SYS_DEBUG_CTRL  0x40034044                         
#define SYS_ANA_ADC     0x40034048                            
#define SYS_ANA_CLK1    0x4003404c    
#define SYS_ANA_CLK2    0x40034050   
#define SYS_ANA_LDO     0x40034054                            
#define SYS_ANA_BGR     0x40034058    
#define SYS_ANA_PSW     0x4003405c
#define SYS_SYS_RST2    0x40034060  
#define SYS_CHIP_SEL    0x40034064  

#define MADC_AD_CTRL    0x4002c000                                    
#define MADC_AD_START   0x4002c004 
#define MADC_AD_STAT    0x4002c008 
#define MADC_AD_DATA    0x4002c00c         
#define MADC_LVD_CTRL   0x4002c010                                        
#define MADC_LVD_STAT   0x4002c014 
#define MADC_TPS_CTRL   0x4002c018 
#define MADC_AD_TEST    0x4002c01c        

#define RTC_CTRL        0x4003c000                   
#define RTC_SC          0x4003c004                  
#define RTC_MN          0x4003c008                   
#define RTC_HR          0x4003c00c                        
#define RTC_DT          0x4003c010                 
#define RTC_MO          0x4003c014                      
#define RTC_YR          0x4003c018                       
#define RTC_DW          0x4003c01c                    
#define RTC_CNT1        0x4003c020                       
#define RTC_CNT2        0x4003c024                 
#define RTC_SCA         0x4003c028                     
#define RTC_MNA         0x4003c02c                           
#define RTC_HRA         0x4003c030              
#define RTC_IE          0x4003c034            
#define RTC_IFLAG       0x4003c038                      
#define RTC_TEMP        0x4003c03c                     
#define RTC_PS          0x4003c040               
#define RTC_TCPMD       0x4003c044                     
#define RTC_DOTA0       0x4003c048                       
#define RTC_ALPHAL      0x4003c04c                        
#define RTC_ALPHAH      0x4003c050                      
#define RTC_XT0         0x4003c054                       
#define RTC_TADJ        0x4003c058                        
#define RTC_BETA        0x4003c05c                       
#define RTC_ZT          0x4003c060
#define RTC_DOTAT       0x4003c064                    
#define RTC_FATR        0x4003c068                   
#define RTC_DELTCX      0x4003c06c    
#define RTC_CPS         0x4003c070           
#define RTC_FPTR        0x4003c074    
#define RTC_FDTR1S      0x4003c078
#define RTC_FDTR20S     0x4003c07c   
#define RTC_LCNT        0x4003c080  
#define RTC_DMAEN       0x4003c0c0  
#define RTC_TEMPOS      0x4003c0c4  
#define RTC_TPSIN       0x4003c0c8  


#define GPIO_PMA        0x50000000
#define GPIO_PA         0x50000004
#define GPIO_PCA0       0x50000008
#define GPIO_PCA1       0x5000000c
#define GPIO_PUA        0x50000010
#define GPIO_PIMA       0x50000014
#define GPIO_PIEA       0x50000018
#define GPIO_PMB        0x5000001c
#define GPIO_PB         0x50000020
#define GPIO_PCB        0x50000024
#define GPIO_PUB        0x50000028
#define GPIO_PIMB       0x5000002c
#define GPIO_PIEB       0x50000030
#define GPIO_PMC        0x50000034
#define GPIO_PC         0x50000038
#define GPIO_PCC        0x5000003c
#define GPIO_PUC        0x50000040
#define GPIO_PIEC       0x50000044
#define GPIO_PMD        0x50000048
#define GPIO_PD         0x5000004c
#define GPIO_PCD        0x50000050
#define GPIO_PUD        0x50000054
#define GPIO_PIMD       0x50000058
#define GPIO_PIED       0x5000005c
#define GPIO_PCE        0x50000060


#define TC0_CNT         0x40010000
#define TC0_PS          0x40010004
#define TC0_PSB         0x40010008
#define TC0_DN          0x4001000c
#define TC0_DNB         0x40010010
#define TC0_CCD0        0x40010014
#define TC0_CCD1        0x40010018
#define TC0_CCFG        0x4001001c
#define TC0_CTRL        0x40010020
#define TC0_CM0         0x40010024
#define TC0_CM1         0x40010028
#define TC0_IE          0x4001002c
#define TC0_STA         0x40010030
#define TC0_CLR         0x40010034


#define WDT_EN          0x40030000
#define WDT_CTRL        0x40030004
#define WDT_PASS        0x40030008


#define FLASH_BASE_REMAP3  		(0x18000000)                              /*!< (FLASH     ) Base Address */  
#define EEPROM_BASE_REMAP3 		(0x08000000)                              /*!< (EEPROM    ) Base Address */ 
#define SRAM_BASE_REMAP3   		(0x10000000)                              /*!< (SRAM      ) Base Address */
#define ROM_BASE_REMAP3    		(0x00000000)                              /*!< (ROM       ) Base Address */ 


//#define EEPROM_SP0_BASE        (0x08c08000)
//#define EEPROM_SP1_BASE        (0x08c08040)
//#define EEPROM_SP0_OFFSET      (0x00c08000)
//#define EEPROM_CMD_OFFSET      (0x00800000)                              /*EEPROM Command Register Offset Address */ 
//#define EEPROM_PGA_OFFSET      (0x00800004)                              /*EEPROM Page Address Register Offset Address */  
//#define EEPROM_STA_OFFSET      (0x00800008)                              /*EEPROM Status Register Offset Address */  
//#define EEPROM_TBC0_OFFSET     (0x0080000C)                              /*EEPROM Calibration Register Offset Address */  
//#define EEPROM_TBC1_OFFSET     (0x00800010)                              /*EEPROM Wait Register Offset Address */                                                
//#define EEPROM_PROT_OFFSET     (0x00800014)                              /*EEPROM Port Register Offset Address */                                                
//#define EEPROM_TW_OFFSET       (0x00800018)                              /*EEPROM Wait time Register Offset Address */                                                                                            
//#define EEPROM_TUS_OFFSET      (0x00800020)                              /*EEPROM us time Register Offset Address */                                                
//#define EEPROM_TMS_OFFSET      (0x00800024)                              /*EEPROM ms time Register Offset Address */                                                
//#define EEPROM_TP_OFFSET       (0x00800028)                              /*EEPROM Program time Register Offset Address */                                              
//#define EEPROM_TE_OFFSET       (0x0080002c)                              /*EEPROM Erase time Register Offset Address */    


#define EEPROM_SP0_BASE        (0x08c08000)
#define EEPROM_SP1_BASE        (0x08c08040)
#define EEPROM_SP0_OFFSET      (0x00c08000)
#define EEPROM_CMD_OFFSET      (0x00800000)                              /*EEPROM Command Register Offset Address */ 
#define EEPROM_PGA_OFFSET      (0x00800004)                              /*EEPROM Page Address Register Offset Address */  
#define EEPROM_STA_OFFSET      (0x00800008)                              /*EEPROM Status Register Offset Address */  
#define EEPROM_TBC0_OFFSET     (0x0080000C)                              /*EEPROM Calibration Register Offset Address */  
#define EEPROM_TBC1_OFFSET     (0x00800010)                              /*EEPROM Wait Register Offset Address */                                                
#define EEPROM_PROT_OFFSET     (0x00800014)                              /*EEPROM Port Register Offset Address */                                                
#define EEPROM_TW_OFFSET       (0x00800018)                              /*EEPROM Wait time Register Offset Address */                                                                                            
#define EEPROM_TUS_OFFSET      (0x00800020)                              /*EEPROM us time Register Offset Address */                                                
#define EEPROM_TMS_OFFSET      (0x00800024)                              /*EEPROM ms time Register Offset Address */                                                
#define EEPROM_TP_OFFSET       (0x00800028)                              /*EEPROM Program time Register Offset Address */                                              
#define EEPROM_TE_OFFSET       (0x0080002c)                              /*EEPROM Erase time Register Offset Address */    

#define FLASH_CMD_OFFSET      (0x00800000)                              /*Flash Command Register Offset Address */ 
#define FLASH_PGA_OFFSET      (0x00800004)                              /*Flash Page Address Register Offset Address */  
#define FLASH_STA_OFFSET      (0x00800008)                              /*Flash Status Register Offset Address */  
#define FLASH_TBC0_OFFSET     (0x0080000C)                              /*Flash Calibration Register Offset Address */  
#define FLASH_TBC1_OFFSET     (0x00800010)                              /*Flash Wait Register Offset Address */                                                
#define FLASH_PROT_OFFSET     (0x00800014)                              /*Flash Port Register Offset Address */                                                
#define FLASH_TW_OFFSET       (0x00800018)                              /*Flash Wait time Register Offset Address */                                                
#define FLASH_CC_CTRL_OFFSET  (0x0080001c)                              /*Flash Cache Register Offset Address */                                                
#define FLASH_TUS_OFFSET      (0x00800020)                              /*Flash us time Register Offset Address */                                                
#define FLASH_TMS_OFFSET      (0x00800024)                              /*Flash ms time Register Offset Address */                                                
#define FLASH_TP_OFFSET       (0x00800028)                              /*Flash Program time Register Offset Address */                                              
#define FLASH_TE_OFFSET       (0x0080002c)                              /*Flash Erase time Register Offset Address */  
