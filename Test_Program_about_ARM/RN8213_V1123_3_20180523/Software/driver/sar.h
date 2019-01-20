#ifndef  __SAR_H
#define  __SAR_H

//VBAT=3.6v,  vbat_sar_in=VBAT/4=0.9v,  VBAT_SAR_OUT=0.9/1.25*2^10=737=0x2e1
//vbat_sar_in +/- 20%=[2.88~4.32]
//VBAT_MIN = 0x24e;  VBAT_MAX = 0x375

//VBAT=3.6v,  vbat_sar_in=VBAT/4=0.9v,  VBAT_SAR_OUT=0.9/1.25*2^10=737=0x2e1
//vbat_sar_in +/- 4%=[2.456~3.744]
//VBAT_MIN = 0x2c3;  VBAT_MAX = 0x2ff
#define VBAT_MIN  0x2c3
#define VBAT_MAX  0x2ff

uint32_t SAR_QC(void);

#endif
