SET_DEC_FILE "RN8213.dec"
HEADER    SWDCLK,  SWDIO;
SPM_PATTERN(GET_CHIP_ID)
{
START:  *0 0* TS1, RPT 200;//LineRset
        *1 1* RPT 56;//LineRset
         /******0xE79E*****/
        *1 0*; //bit 0
        *1 1*; //bit 1
        *1 1*; //bit 2
        *1 1*; //bit 3
        *1 1*; //bit 4
        *1 0*; //bit 5
        *1 0*; //bit 6
        *1 1*; //bit 7
        *1 1*; //bit 8
        *1 1*; //bit 9
        *1 1*; //bit 10
        *1 0*; //bit 11
        *1 0*; //bit 12
        *1 1*; //bit 13
        *1 1*; //bit 14
        *1 1*; //bit 15
        *1 1* RPT 56;//LineRset
        *1 0* RPT 16;  //wait
        ///swd_read,Check DP IDCODE(0x0DD882EE)
        *1 1* ;  //Data Head
        *1 0* ;  // 0->Dp reg
        *1 1* ;  // 1->Read
        *1 0* ;  // DP reg num 2 location so,DP address is 0x00
        *1 0* ;  // DP reg num 3 location
        *1 1* ;  // Parity 
        *1 0* ;  // stop alaways 0
        *1 1* ;  // park alaways 1
        *1 X* ;  //Trn
        ///ACK
        *1 H* ;  // OK
        *1 L* ;   
        *1 L* ;  
        ///Rdata[0:31]
 /*       
        *1 K*; //bit 0
        *1 K*; //bit 1
        *1 K*; //bit 2
        *1 K*; //bit 3
        *1 K*; //bit 4
        *1 K*; //bit 5
        *1 K*; //bit 6
        *1 K*; //bit 7
        *1 K*; //bit 8
        *1 K*; //bit 9
        *1 K*; //bit 10
        *1 K*; //bit 11
        *1 K*; //bit 12
        *1 K*; //bit 13
        *1 K*; //bit 14
        *1 K*; //bit 15
        *1 K*; //bit 16
        *1 K*; //bit 17
        *1 K*; //bit 18
        *1 K*; //bit 19
        *1 K*; //bit 20
        *1 K*; //bit 21
        *1 K*; //bit 22
        *1 K*; //bit 23
        *1 K*; //bit 24
        *1 K*; //bit 25
        *1 K*; //bit 26
        *1 K*; //bit 27
        *1 K*; //bit 28
        *1 K*; //bit 29
        *1 K*; //bit 30
        *1 K*; //bit 31
        *1 K*; //Praity is 1,total bit1=15 
        *1 0* RPT 3 ; //WAIT,IDLE 
 */       
        /******0xBB11477*****/
        *1 H*; //bit 0
        *1 H*; //bit 1
        *1 H*; //bit 2
        *1 L*; //bit 3
        *1 H*; //bit 4
        *1 H*; //bit 5
        *1 H*; //bit 6
        *1 L*; //bit 7
        *1 L*; //bit 8
        *1 L*; //bit 9
        *1 H*; //bit 10
        *1 L*; //bit 11
        *1 H*; //bit 12
        *1 L*; //bit 13
        *1 L*; //bit 14
        *1 L*; //bit 15
        *1 H*; //bit 16
        *1 L*; //bit 17
        *1 L*; //bit 18
        *1 L*; //bit 19
        *1 H*; //bit 20
        *1 H*; //bit 21
        *1 L*; //bit 22
        *1 H*; //bit 23
        *1 H*; //bit 24
        *1 H*; //bit 25
        *1 L*; //bit 26
        *1 H*; //bit 27
        *1 L*; //bit 28
        *1 L*; //bit 29
        *1 L*; //bit 30
        *1 L*; //bit 31
        *1 1*; //Praity is 1,total bit1=15
        *1 0* RPT 3 ; //WAIT,IDLE
       
/***********Header: 0x81***********/ 
     *1 1*; //bit 0
     *1 0*; //bit 1
     *1 0*; //bit 2
     *1 0*; //bit 3
     *1 0*; //bit 4
     *1 0*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //Trn 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
     *1 X*; //Trn 
/***********Write_Data:0x0000001E***********/
     *1 0*; //bit 0
     *1 1*; //bit 1
     *1 1*; //bit 2
     *1 1*; //bit 3
     *1 1*; //bit 4
     *1 0*; //bit 5
     *1 0*; //bit 6
     *1 0*; //bit 7
     *1 0*; //bit 8
     *1 0*; //bit 9
     *1 0*; //bit 10
     *1 0*; //bit 11
     *1 0*; //bit 12
     *1 0*; //bit 13
     *1 0*; //bit 14
     *1 0*; //bit 15
     *1 0*; //bit 16
     *1 0*; //bit 17
     *1 0*; //bit 18
     *1 0*; //bit 19
     *1 0*; //bit 20
     *1 0*; //bit 21
     *1 0*; //bit 22
     *1 0*; //bit 23
     *1 0*; //bit 24
     *1 0*; //bit 25
     *1 0*; //bit 26
     *1 0*; //bit 27
     *1 0*; //bit 28
     *1 0*; //bit 29
     *1 0*; //bit 30
     *1 0*; //bit 31
     *1 0*; //Praity is 0,total bit1=4
     *1 0* RPT 3;//wait_idle
/***********Header: 0xA9***********/ 
     *1 1*; //bit 0
     *1 0*; //bit 1
     *1 0*; //bit 2
     *1 1*; //bit 3
     *1 0*; //bit 4
     *1 1*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //Trn 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
     *1 X*; //Trn 
/***********Write_Data:0x50000000***********/
     *1 0*; //bit 0
     *1 0*; //bit 1
     *1 0*; //bit 2
     *1 0*; //bit 3
     *1 0*; //bit 4
     *1 0*; //bit 5
     *1 0*; //bit 6
     *1 0*; //bit 7
     *1 0*; //bit 8
     *1 0*; //bit 9
     *1 0*; //bit 10
     *1 0*; //bit 11
     *1 0*; //bit 12
     *1 0*; //bit 13
     *1 0*; //bit 14
     *1 0*; //bit 15
     *1 0*; //bit 16
     *1 0*; //bit 17
     *1 0*; //bit 18
     *1 0*; //bit 19
     *1 0*; //bit 20
     *1 0*; //bit 21
     *1 0*; //bit 22
     *1 0*; //bit 23
     *1 0*; //bit 24
     *1 0*; //bit 25
     *1 0*; //bit 26
     *1 0*; //bit 27
     *1 1*; //bit 28
     *1 0*; //bit 29
     *1 1*; //bit 30
     *1 0*; //bit 31
     *1 0*; //Praity is 0,total bit1=2
     *1 0* RPT 3;//wait_idle
/***********Header: 0x8D***********/ 
     *1 1*; //bit 0
     *1 0*; //bit 1
     *1 1*; //bit 2
     *1 1*; //bit 3
     *1 0*; //bit 4
     *1 0*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //IDLE 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
/***********Read_Data:0xF0000000***********/
     *1 L*; //bit 0
     *1 L*; //bit 1
     *1 L*; //bit 2
     *1 L*; //bit 3
     *1 L*; //bit 4
     *1 L*; //bit 5
     *1 L*; //bit 6
     *1 L*; //bit 7
     *1 L*; //bit 8
     *1 L*; //bit 9
     *1 L*; //bit 10
     *1 L*; //bit 11
     *1 L*; //bit 12
     *1 L*; //bit 13
     *1 L*; //bit 14
     *1 L*; //bit 15
     *1 L*; //bit 16
     *1 L*; //bit 17
     *1 L*; //bit 18
     *1 L*; //bit 19
     *1 L*; //bit 20
     *1 L*; //bit 21
     *1 L*; //bit 22
     *1 L*; //bit 23
     *1 L*; //bit 24
     *1 L*; //bit 25
     *1 L*; //bit 26
     *1 L*; //bit 27
     *1 H*; //bit 28
     *1 H*; //bit 29
     *1 H*; //bit 30
     *1 H*; //bit 31
     *1 L*; //Praity is 0,total bit1=4
     *1 0* RPT 3;//wait_idle
/***********Header: 0xB1***********/ 
     *1 1*; //bit 0
     *1 0*; //bit 1
     *1 0*; //bit 2
     *1 0*; //bit 3
     *1 1*; //bit 4
     *1 1*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //Trn 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
     *1 X*; //Trn 
/***********Write_Data:0x00000000***********/
     *1 0*; //bit 0
     *1 0*; //bit 1
     *1 0*; //bit 2
     *1 0*; //bit 3
     *1 0*; //bit 4
     *1 0*; //bit 5
     *1 0*; //bit 6
     *1 0*; //bit 7
     *1 0*; //bit 8
     *1 0*; //bit 9
     *1 0*; //bit 10
     *1 0*; //bit 11
     *1 0*; //bit 12
     *1 0*; //bit 13
     *1 0*; //bit 14
     *1 0*; //bit 15
     *1 0*; //bit 16
     *1 0*; //bit 17
     *1 0*; //bit 18
     *1 0*; //bit 19
     *1 0*; //bit 20
     *1 0*; //bit 21
     *1 0*; //bit 22
     *1 0*; //bit 23
     *1 0*; //bit 24
     *1 0*; //bit 25
     *1 0*; //bit 26
     *1 0*; //bit 27
     *1 0*; //bit 28
     *1 0*; //bit 29
     *1 0*; //bit 30
     *1 0*; //bit 31
     *1 0*; //Praity is 0,total bit1=0
     *1 0* RPT 3;//wait_idle
/***********Header: 0x87***********/ 
     *1 1*; //bit 0
     *1 1*; //bit 1
     *1 1*; //bit 2
     *1 0*; //bit 3
     *1 0*; //bit 4
     *1 0*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //IDLE 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
/***********Read_Data:0x00000000***********/
     *1 L*; //bit 0
     *1 L*; //bit 1
     *1 L*; //bit 2
     *1 L*; //bit 3
     *1 L*; //bit 4
     *1 L*; //bit 5
     *1 L*; //bit 6
     *1 L*; //bit 7
     *1 L*; //bit 8
     *1 L*; //bit 9
     *1 L*; //bit 10
     *1 L*; //bit 11
     *1 L*; //bit 12
     *1 L*; //bit 13
     *1 L*; //bit 14
     *1 L*; //bit 15
     *1 L*; //bit 16
     *1 L*; //bit 17
     *1 L*; //bit 18
     *1 L*; //bit 19
     *1 L*; //bit 20
     *1 L*; //bit 21
     *1 L*; //bit 22
     *1 L*; //bit 23
     *1 L*; //bit 24
     *1 L*; //bit 25
     *1 L*; //bit 26
     *1 L*; //bit 27
     *1 L*; //bit 28
     *1 L*; //bit 29
     *1 L*; //bit 30
     *1 L*; //bit 31
     *1 L*; //Praity is 0,total bit1=0
     *1 0* RPT 3;//wait_idle
/***********Header: 0xBD***********/ 
     *1 1*; //bit 0
     *1 0*; //bit 1
     *1 1*; //bit 2
     *1 1*; //bit 3
     *1 1*; //bit 4
     *1 1*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //IDLE 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
/***********Read_Data:0x03000040***********/
     *1 L*; //bit 0
     *1 L*; //bit 1
     *1 L*; //bit 2
     *1 L*; //bit 3
     *1 L*; //bit 4
     *1 L*; //bit 5
     *1 H*; //bit 6
     *1 L*; //bit 7
     *1 L*; //bit 8
     *1 L*; //bit 9
     *1 L*; //bit 10
     *1 L*; //bit 11
     *1 L*; //bit 12
     *1 L*; //bit 13
     *1 L*; //bit 14
     *1 L*; //bit 15
     *1 L*; //bit 16
     *1 L*; //bit 17
     *1 L*; //bit 18
     *1 L*; //bit 19
     *1 L*; //bit 20
     *1 L*; //bit 21
     *1 L*; //bit 22
     *1 L*; //bit 23
     *1 H*; //bit 24
     *1 H*; //bit 25
     *1 L*; //bit 26
     *1 L*; //bit 27
     *1 L*; //bit 28
     *1 L*; //bit 29
     *1 L*; //bit 30
     *1 L*; //bit 31
     *1 H*; //Praity is 1,total bit1=3
     *1 0* RPT 3;//wait_idle
/***********Header: 0xA3***********/ 
     *1 1*; //bit 0
     *1 1*; //bit 1
     *1 0*; //bit 2
     *1 0*; //bit 3
     *1 0*; //bit 4
     *1 1*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //Trn 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
     *1 X*; //Trn 
/***********Write_Data:0x80000042***********/
     *1 0*; //bit 0
     *1 1*; //bit 1
     *1 0*; //bit 2
     *1 0*; //bit 3
     *1 0*; //bit 4
     *1 0*; //bit 5
     *1 1*; //bit 6
     *1 0*; //bit 7
     *1 0*; //bit 8
     *1 0*; //bit 9
     *1 0*; //bit 10
     *1 0*; //bit 11
     *1 0*; //bit 12
     *1 0*; //bit 13
     *1 0*; //bit 14
     *1 0*; //bit 15
     *1 0*; //bit 16
     *1 0*; //bit 17
     *1 0*; //bit 18
     *1 0*; //bit 19
     *1 0*; //bit 20
     *1 0*; //bit 21
     *1 0*; //bit 22
     *1 0*; //bit 23
     *1 0*; //bit 24
     *1 0*; //bit 25
     *1 0*; //bit 26
     *1 0*; //bit 27
     *1 0*; //bit 28
     *1 0*; //bit 29
     *1 0*; //bit 30
     *1 1*; //bit 31
     *1 1*; //Praity is 1,total bit1=3
     *1 0* RPT 3;//wait_idle
/***********Header: 0x8B***********/ 
     *1 1*; //bit 0
     *1 1*; //bit 1
     *1 0*; //bit 2
     *1 1*; //bit 3
     *1 0*; //bit 4
     *1 0*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //Trn 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
     *1 X*; //Trn 
/***********Write_Data:0x4003402C***********/
     *1 0*; //bit 0
     *1 0*; //bit 1
     *1 1*; //bit 2
     *1 1*; //bit 3
     *1 0*; //bit 4
     *1 1*; //bit 5
     *1 0*; //bit 6
     *1 0*; //bit 7
     *1 0*; //bit 8
     *1 0*; //bit 9
     *1 0*; //bit 10
     *1 0*; //bit 11
     *1 0*; //bit 12
     *1 0*; //bit 13
     *1 1*; //bit 14
     *1 0*; //bit 15
     *1 1*; //bit 16
     *1 1*; //bit 17
     *1 0*; //bit 18
     *1 0*; //bit 19
     *1 0*; //bit 20
     *1 0*; //bit 21
     *1 0*; //bit 22
     *1 0*; //bit 23
     *1 0*; //bit 24
     *1 0*; //bit 25
     *1 0*; //bit 26
     *1 0*; //bit 27
     *1 0*; //bit 28
     *1 0*; //bit 29
     *1 1*; //bit 30
     *1 0*; //bit 31
     *1 1*; //Praity is 1,total bit1=7
     *1 0* RPT 3;//wait_idle
/***********Header: 0x9F***********/ 
     *1 1*; //bit 0
     *1 1*; //bit 1
     *1 1*; //bit 2
     *1 1*; //bit 3
     *1 1*; //bit 4
     *1 0*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //IDLE 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
/***********Read_Data:0x00000000***********/
     *1 L*; //bit 0
     *1 L*; //bit 1
     *1 L*; //bit 2
     *1 L*; //bit 3
     *1 L*; //bit 4
     *1 L*; //bit 5
     *1 L*; //bit 6
     *1 L*; //bit 7
     *1 L*; //bit 8
     *1 L*; //bit 9
     *1 L*; //bit 10
     *1 L*; //bit 11
     *1 L*; //bit 12
     *1 L*; //bit 13
     *1 L*; //bit 14
     *1 L*; //bit 15
     *1 L*; //bit 16
     *1 L*; //bit 17
     *1 L*; //bit 18
     *1 L*; //bit 19
     *1 L*; //bit 20
     *1 L*; //bit 21
     *1 L*; //bit 22
     *1 L*; //bit 23
     *1 L*; //bit 24
     *1 L*; //bit 25
     *1 L*; //bit 26
     *1 L*; //bit 27
     *1 L*; //bit 28
     *1 L*; //bit 29
     *1 L*; //bit 30
     *1 L*; //bit 31
     *1 L*; //Praity is 0,total bit1=0
     *1 0* RPT 3;//wait_idle
/***********Header: 0x9F***********/ 
     *1 1*; //bit 0
     *1 1*; //bit 1
     *1 1*; //bit 2
     *1 1*; //bit 3
     *1 1*; //bit 4
     *1 0*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //IDLE 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
     *1 H*; //bit 0
     *1 L*; //bit 1
     *1 L*; //bit 2
     *1 H*; //bit 3
     *1 H*; //bit 4
     *1 L*; //bit 5
     *1 L*; //bit 6
     *1 L*; //bit 7
     *1 L*; //bit 8
     *1 H*; //bit 9
     *1 L*; //bit 10
     *1 L*; //bit 11
     *1 L*; //bit 12
     *1 L*; //bit 13
     *1 L*; //bit 14
     *1 H*; //bit 15
     *1 L*; //bit 16
     *1 L*; //bit 17
     *1 L*; //bit 18
     *1 L*; //bit 19
     *1 L*; //bit 20
     *1 L*; //bit 21
     *1 L*; //bit 22
     *1 L*; //bit 23
     *1 L*; //bit 24
     *1 L*; //bit 25
     *1 L*; //bit 26
     *1 L*; //bit 27
     *1 L*; //bit 28
     *1 L*; //bit 29
     *1 L*; //bit 30
     *1 L*; //bit 31
     *1 H*; //Praity is 1,total bit1=5
     *1 0* RPT 3;//wait_idle
 /***********Header: 0xB1***********/ 
     *1 1*; //bit 0
     *1 0*; //bit 1
     *1 0*; //bit 2
     *1 0*; //bit 3
     *1 1*; //bit 4
     *1 1*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //Trn 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
     *1 X*; //Trn 
 /***********Write_Data:0x00000000***********/
     *1 0*; //bit 0
     *1 0*; //bit 1
     *1 0*; //bit 2
     *1 0*; //bit 3
     *1 0*; //bit 4
     *1 0*; //bit 5
     *1 0*; //bit 6
     *1 0*; //bit 7
     *1 0*; //bit 8
     *1 0*; //bit 9
     *1 0*; //bit 10
     *1 0*; //bit 11
     *1 0*; //bit 12
     *1 0*; //bit 13
     *1 0*; //bit 14
     *1 0*; //bit 15
     *1 0*; //bit 16
     *1 0*; //bit 17
     *1 0*; //bit 18
     *1 0*; //bit 19
     *1 0*; //bit 20
     *1 0*; //bit 21
     *1 0*; //bit 22
     *1 0*; //bit 23
     *1 0*; //bit 24
     *1 0*; //bit 25
     *1 0*; //bit 26
     *1 0*; //bit 27
     *1 0*; //bit 28
     *1 0*; //bit 29
     *1 0*; //bit 30
     *1 0*; //bit 31
     *1 0*; //Praity is 0,total bit1=0
     *1 0* RPT 3;//wait_idle
 /***********Header: 0x87***********/ 
     *1 1*; //bit 0
     *1 1*; //bit 1
     *1 1*; //bit 2
     *1 0*; //bit 3
     *1 0*; //bit 4
     *1 0*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //IDLE 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
 /***********Read_Data:0x00000000***********/
     *1 L*; //bit 0
     *1 L*; //bit 1
     *1 L*; //bit 2
     *1 L*; //bit 3
     *1 L*; //bit 4
     *1 L*; //bit 5
     *1 L*; //bit 6
     *1 L*; //bit 7
     *1 L*; //bit 8
     *1 L*; //bit 9
     *1 L*; //bit 10
     *1 L*; //bit 11
     *1 L*; //bit 12
     *1 L*; //bit 13
     *1 L*; //bit 14
     *1 L*; //bit 15
     *1 L*; //bit 16
     *1 L*; //bit 17
     *1 L*; //bit 18
     *1 L*; //bit 19
     *1 L*; //bit 20
     *1 L*; //bit 21
     *1 L*; //bit 22
     *1 L*; //bit 23
     *1 L*; //bit 24
     *1 L*; //bit 25
     *1 L*; //bit 26
     *1 L*; //bit 27
     *1 L*; //bit 28
     *1 L*; //bit 29
     *1 L*; //bit 30
     *1 L*; //bit 31
     *1 L*; //Praity is 0,total bit1=0
     *1 0* RPT 3;//wait_idle
 /***********Header: 0xBD***********/ 
     *1 1*; //bit 0
     *1 0*; //bit 1
     *1 1*; //bit 2
     *1 1*; //bit 3
     *1 1*; //bit 4
     *1 1*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //IDLE 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
 /***********Read_Data:0x03000042***********/
     *1 L*; //bit 0
     *1 H*; //bit 1
     *1 L*; //bit 2
     *1 L*; //bit 3
     *1 L*; //bit 4
     *1 L*; //bit 5
     *1 H*; //bit 6
     *1 L*; //bit 7
     *1 L*; //bit 8
     *1 L*; //bit 9
     *1 L*; //bit 10
     *1 L*; //bit 11
     *1 L*; //bit 12
     *1 L*; //bit 13
     *1 L*; //bit 14
     *1 L*; //bit 15
     *1 L*; //bit 16
     *1 L*; //bit 17
     *1 L*; //bit 18
     *1 L*; //bit 19
     *1 L*; //bit 20
     *1 L*; //bit 21
     *1 L*; //bit 22
     *1 L*; //bit 23
     *1 H*; //bit 24
     *1 H*; //bit 25
     *1 L*; //bit 26
     *1 L*; //bit 27
     *1 L*; //bit 28
     *1 L*; //bit 29
     *1 L*; //bit 30
     *1 L*; //bit 31
     *1 L*; //Praity is 0,total bit1=4
     *1 0* RPT 3;//wait_idle
 /***********Header: 0xA3***********/ 
     *1 1*; //bit 0
     *1 1*; //bit 1
     *1 0*; //bit 2
     *1 0*; //bit 3
     *1 0*; //bit 4
     *1 1*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //Trn 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
     *1 X*; //Trn 
 /***********Write_Data:0x80000041***********/
     *1 1*; //bit 0
     *1 0*; //bit 1
     *1 0*; //bit 2
     *1 0*; //bit 3
     *1 0*; //bit 4
     *1 0*; //bit 5
     *1 1*; //bit 6
     *1 0*; //bit 7
     *1 0*; //bit 8
     *1 0*; //bit 9
     *1 0*; //bit 10
     *1 0*; //bit 11
     *1 0*; //bit 12
     *1 0*; //bit 13
     *1 0*; //bit 14
     *1 0*; //bit 15
     *1 0*; //bit 16
     *1 0*; //bit 17
     *1 0*; //bit 18
     *1 0*; //bit 19
     *1 0*; //bit 20
     *1 0*; //bit 21
     *1 0*; //bit 22
     *1 0*; //bit 23
     *1 0*; //bit 24
     *1 0*; //bit 25
     *1 0*; //bit 26
     *1 0*; //bit 27
     *1 0*; //bit 28
     *1 0*; //bit 29
     *1 0*; //bit 30
     *1 1*; //bit 31
     *1 1*; //Praity is 1,total bit1=3
     *1 0* RPT 3;//wait_idle
 /***********Header: 0x8B***********/ 
     *1 1*; //bit 0
     *1 1*; //bit 1
     *1 0*; //bit 2
     *1 1*; //bit 3
     *1 0*; //bit 4
     *1 0*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //Trn 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
     *1 X*; //Trn 
 /***********Write_Data:0x08C0008C***********/
     *1 0*; //bit 0
     *1 0*; //bit 1
     *1 1*; //bit 2
     *1 1*; //bit 3
     *1 0*; //bit 4
     *1 0*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 0*; //bit 8
     *1 0*; //bit 9
     *1 0*; //bit 10
     *1 0*; //bit 11
     *1 0*; //bit 12
     *1 0*; //bit 13
     *1 0*; //bit 14
     *1 0*; //bit 15
     *1 0*; //bit 16
     *1 0*; //bit 17
     *1 0*; //bit 18
     *1 0*; //bit 19
     *1 0*; //bit 20
     *1 0*; //bit 21
     *1 1*; //bit 22
     *1 1*; //bit 23
     *1 0*; //bit 24
     *1 0*; //bit 25
     *1 0*; //bit 26
     *1 1*; //bit 27
     *1 0*; //bit 28
     *1 0*; //bit 29
     *1 0*; //bit 30
     *1 0*; //bit 31
     *1 0*; //Praity is 0,total bit1=6
     *1 0* RPT 3;//wait_idle
 /***********Header: 0x9F***********/ 
     *1 1*; //bit 0
     *1 1*; //bit 1
     *1 1*; //bit 2
     *1 1*; //bit 3
     *1 1*; //bit 4
     *1 0*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //IDLE 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
 /***********Read_Data:0x00000000***********/
     *1 L*; //bit 0
     *1 L*; //bit 1
     *1 L*; //bit 2
     *1 L*; //bit 3
     *1 L*; //bit 4
     *1 L*; //bit 5
     *1 L*; //bit 6
     *1 L*; //bit 7
     *1 L*; //bit 8
     *1 L*; //bit 9
     *1 L*; //bit 10
     *1 L*; //bit 11
     *1 L*; //bit 12
     *1 L*; //bit 13
     *1 L*; //bit 14
     *1 L*; //bit 15
     *1 L*; //bit 16
     *1 L*; //bit 17
     *1 L*; //bit 18
     *1 L*; //bit 19
     *1 L*; //bit 20
     *1 L*; //bit 21
     *1 L*; //bit 22
     *1 L*; //bit 23
     *1 L*; //bit 24
     *1 L*; //bit 25
     *1 L*; //bit 26
     *1 L*; //bit 27
     *1 L*; //bit 28
     *1 L*; //bit 29
     *1 L*; //bit 30
     *1 L*; //bit 31
     *1 L*; //Praity is 0,total bit1=0
     *1 0* RPT 3;//wait_idle
 /***********Header: 0x9F***********/ 
     *1 1*; //bit 0
     *1 1*; //bit 1
     *1 1*; //bit 2
     *1 1*; //bit 3
     *1 1*; //bit 4
     *1 0*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //IDLE 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
 /***********Read_Data:0xFFFF1234***********/
     *1 L*; //bit 0
     *1 L*; //bit 1
     *1 H*; //bit 2
     *1 L*; //bit 3
     *1 H*; //bit 4
     *1 H*; //bit 5
     *1 L*; //bit 6
     *1 L*; //bit 7
     *1 L*; //bit 8
     *1 H*; //bit 9
     *1 L*; //bit 10
     *1 L*; //bit 11
     *1 H*; //bit 12
     *1 L*; //bit 13
     *1 L*; //bit 14
     *1 L*; //bit 15
     *1 H*; //bit 16
     *1 H*; //bit 17
     *1 H*; //bit 18
     *1 H*; //bit 19
     *1 H*; //bit 20
     *1 H*; //bit 21
     *1 H*; //bit 22
     *1 H*; //bit 23
     *1 H*; //bit 24
     *1 H*; //bit 25
     *1 H*; //bit 26
     *1 H*; //bit 27
     *1 H*; //bit 28
     *1 H*; //bit 29
     *1 H*; //bit 30
     *1 H*; //bit 31
     *1 H*; //Praity is 1,total bit1=21
     /*
     *1 K*; //bit 0
     *1 K*; //bit 1
     *1 K*; //bit 2
     *1 K*; //bit 3
     *1 K*; //bit 4
     *1 K*; //bit 5
     *1 K*; //bit 6
     *1 K*; //bit 7
     *1 K*; //bit 8
     *1 K*; //bit 9
     *1 K*; //bit 10
     *1 K*; //bit 11
     *1 K*; //bit 12
     *1 K*; //bit 13
     *1 K*; //bit 14
     *1 K*; //bit 15
     *1 K*; //bit 16
     *1 K*; //bit 17
     *1 K*; //bit 18
     *1 K*; //bit 19
     *1 K*; //bit 20
     *1 K*; //bit 21
     *1 K*; //bit 22
     *1 K*; //bit 23
     *1 K*; //bit 24
     *1 K*; //bit 25
     *1 K*; //bit 26
     *1 K*; //bit 27
     *1 K*; //bit 28
     *1 K*; //bit 29
     *1 K*; //bit 30
     *1 K*; //bit 31
     *1 K*; //Praity is 1,total bit1=15 
     */
     *1 0* RPT 3;//wait_idle
 /***********Header: 0xB1***********/ 
     *1 1*; //bit 0
     *1 0*; //bit 1
     *1 0*; //bit 2
     *1 0*; //bit 3
     *1 1*; //bit 4
     *1 1*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //Trn 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
     *1 X*; //Trn 
 /***********Write_Data:0x00000000***********/
     *1 0*; //bit 0
     *1 0*; //bit 1
     *1 0*; //bit 2
     *1 0*; //bit 3
     *1 0*; //bit 4
     *1 0*; //bit 5
     *1 0*; //bit 6
     *1 0*; //bit 7
     *1 0*; //bit 8
     *1 0*; //bit 9
     *1 0*; //bit 10
     *1 0*; //bit 11
     *1 0*; //bit 12
     *1 0*; //bit 13
     *1 0*; //bit 14
     *1 0*; //bit 15
     *1 0*; //bit 16
     *1 0*; //bit 17
     *1 0*; //bit 18
     *1 0*; //bit 19
     *1 0*; //bit 20
     *1 0*; //bit 21
     *1 0*; //bit 22
     *1 0*; //bit 23
     *1 0*; //bit 24
     *1 0*; //bit 25
     *1 0*; //bit 26
     *1 0*; //bit 27
     *1 0*; //bit 28
     *1 0*; //bit 29
     *1 0*; //bit 30
     *1 0*; //bit 31
     *1 0*; //Praity is 0,total bit1=0
     *1 0* RPT 3;//wait_idle
 /***********Header: 0x87***********/ 
     *1 1*; //bit 0
     *1 1*; //bit 1
     *1 1*; //bit 2
     *1 0*; //bit 3
     *1 0*; //bit 4
     *1 0*; //bit 5
     *1 0*; //bit 6
     *1 1*; //bit 7
     *1 X*; //IDLE 
     *1 H*; //ACK 
     *1 L*;    
     *1 L*;    
 /***********Read_Data:0x00000000***********/
     *1 L*; //bit 0
     *1 L*; //bit 1
     *1 L*; //bit 2
     *1 L*; //bit 3
     *1 L*; //bit 4
     *1 L*; //bit 5
     *1 L*; //bit 6
     *1 L*; //bit 7
     *1 L*; //bit 8
     *1 L*; //bit 9
     *1 L*; //bit 10
     *1 L*; //bit 11
     *1 L*; //bit 12
     *1 L*; //bit 13
     *1 L*; //bit 14
     *1 L*; //bit 15
     *1 L*; //bit 16
     *1 L*; //bit 17
     *1 L*; //bit 18
     *1 L*; //bit 19
     *1 L*; //bit 20
     *1 L*; //bit 21
     *1 L*; //bit 22
     *1 L*; //bit 23
     *1 L*; //bit 24
     *1 L*; //bit 25
     *1 L*; //bit 26
     *1 L*; //bit 27
     *1 L*; //bit 28
     *1 L*; //bit 29
     *1 L*; //bit 30
     *1 L*; //bit 31
     *1 L*; //Praity is 0,total bit1=0
     *1 0* RPT 3;//wait_idle
     *1 0*;
            
}