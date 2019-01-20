SET_DEC_FILE "RN8213.dec"
HEADER    SWDCLK,  SWDIO;
SPM_PATTERN(READ_WORD)
{
                 *1 0*TS1,RPT 10;  //wait
/***********Header: 0x00***********/ 
wread_addr::     *1 0*; //bit 0
                 *1 0*; //bit 1
                 *1 0*; //bit 2
                 *1 0*; //bit 3
                 *1 0*; //bit 4
                 *1 0*; //bit 5
                 *1 0*; //bit 6
wread_end:       *1 0*; //bit 7
                 *1 X*; //Trn 
                 *1 H*; //ACK 
                 *1 L*;    
                 *1 L*;    
/***********Write_Data:0x00000000***********/
wread_data::     *1 0* ;//bit 0
                 *1 0* ;//bit 1
                 *1 0* ;//bit 2
                 *1 0* ;//bit 3
                 *1 0* ;//bit 4
                 *1 0* ;//bit 5
                 *1 0* ;//bit 6
                 *1 0* ;//bit 7
                 *1 0* ;//bit 8
                 *1 0* ;//bit 9
                 *1 0* ;//bit 10
                 *1 0* ;//bit 11
                 *1 0* ;//bit 12
                 *1 0* ;//bit 13
                 *1 0* ;//bit 14
                 *1 0* ;//bit 15
                 *1 0* ;//bit 16
                 *1 0* ;//bit 17
                 *1 0* ;//bit 18
                 *1 0* ;//bit 19
                 *1 0* ;//bit 20
                 *1 0* ;//bit 21
                 *1 0* ;//bit 22
                 *1 0* ;//bit 23
                 *1 0* ;//bit 24
                 *1 0* ;//bit 25
                 *1 0* ;//bit 26
                 *1 0* ;//bit 27
                 *1 0* ;//bit 28
                 *1 0* ;//bit 29
                 *1 0* ;//bit 30
wread_data_end:  *1 0* ;//bit 31
                 *1 0* ;//Parity
                 *1 0* RPT 10;//wait_idle
                 *1 0*; 
}