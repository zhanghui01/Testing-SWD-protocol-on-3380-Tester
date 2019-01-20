SET_DEC_FILE "RN8213.dec"
HEADER    SWDCLK,  SWDIO;
SPM_PATTERN(CAPTURE_WORD)
{
                 *1 0*TS1,RPT 10;  //wait
/***********Header: 0x00***********/ 
Capture_addr::   *1 0*; //bit 0
                 *1 0*; //bit 1
                 *1 0*; //bit 2
                 *1 0*; //bit 3
                 *1 0*; //bit 4
                 *1 0*; //bit 5
                 *1 0*; //bit 6
Capture_end:     *1 0*; //bit 7
                 *1 X*; //Trn 
                 *1 H*; //ACK 
                 *1 L*;    
                 *1 L*;    
/***********Write_Data:0x00000000***********/
Capture_data::   *1 K* ;//Capture bit 0
                 *1 K* ;//Capture bit 1
                 *1 K* ;//Capture bit 2
                 *1 K* ;//Capture bit 3
                 *1 K* ;//Capture bit 4
                 *1 K* ;//Capture bit 5
                 *1 K* ;//Capture bit 6
                 *1 K* ;//Capture bit 7
                 *1 K* ;//Capture bit 8
                 *1 K* ;//Capture bit 9
                 *1 K* ;//Capture bit 10
                 *1 K* ;//Capture bit 11
                 *1 K* ;//Capture bit 12
                 *1 K* ;//Capture bit 13
                 *1 K* ;//Capture bit 14
                 *1 K* ;//Capture bit 15
                 *1 K* ;//Capture bit 16
                 *1 K* ;//Capture bit 17
                 *1 K* ;//Capture bit 18
                 *1 K* ;//Capture bit 19
                 *1 K* ;//Capture bit 20
                 *1 K* ;//Capture bit 21
                 *1 K* ;//Capture bit 22
                 *1 K* ;//Capture bit 23
                 *1 K* ;//Capture bit 24
                 *1 K* ;//Capture bit 25
                 *1 K* ;//Capture bit 26
                 *1 K* ;//Capture bit 27
                 *1 K* ;//Capture bit 28
                 *1 K* ;//Capture bit 29
                 *1 K* ;//Capture bit 30
Capture_data_end:*1 K* ;//Capture bit 31
                 *1 K* ;//Parity
                 *1 0* RPT 10;//wait_idle
                 *1 0*; 
}