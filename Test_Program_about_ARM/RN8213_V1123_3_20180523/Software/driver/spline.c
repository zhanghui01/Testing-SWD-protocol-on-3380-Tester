#define _SPLINE_GLOBALS

// #include "spline.h"
#include "drv.h"

  uint32_t FT_times ;
static uint32_t iso7816ctrl_read(void)
{
  uint32_t temp;
  
  swd_connect(1);
  temp = 0x82;
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PS)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD0_EN)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC MOD0 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
  temp |= (1 << 13);
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->MOD0_EN)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC MOD0 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
  if(swd_mem_rd32((unsigned long)(&(ISO7816->CTRL1)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC MOD1 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
  return temp & 0x0fffffff;
}

//==============UID check====================
uint32_t UID_check(void)
{
  uint32_t temp;
  uint16_t UID;
  swd_connect(1);
  
  temp = 0x82;
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PS)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD0_EN)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC MOD0 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
  temp |= (1 << 2);
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->MOD0_EN)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC MOD0 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
  if(swd_mem_rd16((unsigned long)(&(*(uint32_t *)0x08c0008c)),(unsigned long *)&UID,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC MOD1 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
// 	
 if(UID!=0x1234)//if no CP fail(or not V15)
  {
		 if(swd_mem_rd16((unsigned long)(&(*(uint32_t *)0x08c0008a)),(unsigned long *)&UID,1) != SWD_RESP_OK)//CPV14
		{
			DBG_ERR("Write SOC MOD1 fail\n");
			return SWD_RESP_PROTOCAL_ERROR;
		}
		if(UID!=0x1234)//if no CP fail(or not V14)
		{
				if(swd_mem_rd16((unsigned long)(&(*(uint32_t *)0x08c00032)),(unsigned long *)&UID,1) != SWD_RESP_OK)//CPV14
				{
					DBG_ERR("Write SOC MOD1 fail\n");
					return SWD_RESP_PROTOCAL_ERROR;
				}
				if(UID!=0x1985)//if not TEST SAMPLE
				{
					exitFail(0) ;
				}
				else
				{
					if(swd_mem_rd16((unsigned long)(&(*(uint32_t *)0x08c0001c)),(unsigned long *)&UID,1) != SWD_RESP_OK)
					{
							DBG_ERR("Write SOC MOD1 fail\n");
							return SWD_RESP_PROTOCAL_ERROR;
					}
		
					if((UID==0x1230) ||(UID==0x1231)||(UID==0x1232)||(UID==0x1233)||(UID==0x1234)||(UID==0x1235)||(UID==0x1236)||(UID==0x1237)||(UID==0x1238))
					{	
							FT_times = UID+1;			
					}
					else
					{
							FT_times = 1;
					}	
					exit_UID_check_PASS();		
				}					
		}
		
		else
		{
			if(swd_mem_rd16((unsigned long)(&(*(uint32_t *)0x08c0001c)),(unsigned long *)&UID,1) != SWD_RESP_OK)
			{
			DBG_ERR("Write SOC MOD1 fail\n");
			return SWD_RESP_PROTOCAL_ERROR;
			}
		
					if((UID==0x1230) ||(UID==0x1231)||(UID==0x1232)||(UID==0x1233)||(UID==0x1234)||(UID==0x1235)||(UID==0x1236)||(UID==0x1237)||(UID==0x1238))
					{	
							FT_times = UID+1;			
					}
					else
					{
							FT_times = 1;
					}	
		exit_UID_check_PASS();
		
	}
  }
  else
	{
		if(swd_mem_rd16((unsigned long)(&(*(uint32_t *)0x08c0001c)),(unsigned long *)&UID,1) != SWD_RESP_OK)
		{
    DBG_ERR("Write SOC MOD1 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
		}
		
					if((UID==0x1230) ||(UID==0x1231)||(UID==0x1232)||(UID==0x1233)||(UID==0x1234)||(UID==0x1235)||(UID==0x1236)||(UID==0x1237)||(UID==0x1238))
					{	
							FT_times = UID+1;			
					}
					else
					{
							FT_times = 1;
					}		
		exit_UID_check_PASS();
		
	}
	return SUCCESS;
}
//=====================UID_adr_read==============
uint32_t UID_adr_read(unsigned long adr)
{
  uint32_t temp;
  
  swd_connect(1);
  
  temp = 0x82;
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->SYS_PS)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC PS fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  if(swd_mem_rd32((unsigned long)(&(SYSCTL->MOD0_EN)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC MOD0 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
  temp |= (1 << 2);
  if(swd_mem_wr32((unsigned long)(&(SYSCTL->MOD0_EN)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC MOD0 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  
  if(swd_mem_rd16((unsigned long)(&(*(uint32_t *)adr)),(unsigned long *)&temp,1) != SWD_RESP_OK)
  {
    DBG_ERR("Write SOC MOD1 fail\n");
    return SWD_RESP_PROTOCAL_ERROR;
  }
  return temp ;
}


//==============FT UID write ====================
uint32_t FT_UID_write(void)
{
  uint32_t i;
	uint32_t checksum;
	uint32_t temp;
	
	uint8_t   spl0[PAGE_SIZE];
	for(i=0;i<PAGE_SIZE;i++)
	{
		spl0[i]=mem_rd_reg8(EEPROM_SP0_BASE+i);
	}
	//================= Compare EEPROM Data right (8/64)=============
			temp =  ((spl0[23]<<8) | spl0[22]);
			DBG_MSG(" ANA_LDO= %4x \n",temp);
	
			if(spl0[0] != (F1 & 0xff))
		{
			return FAIL;
		}
			if(spl0[1] != ((F1 >>8) & 0xff) )
		{
			return FAIL;
		}
		
		if(spl0[16] != (ANAADC & 0xff))
		{
			return FAIL;
		}
			if(spl0[19] != ((ANACLK1 >>8) & 0xff) )
		{
			return FAIL;
		}
		
			if(spl0[24] != (ANABGR & 0xff))
		{
			return FAIL;
		}
			if(spl0[25] != ((ANABGR >>8) & 0xff) )
		{
			return FAIL;
		}
		
		if(spl0[32] != (TPSCTRL & 0xff))
		{
			return FAIL;
		}
			if(spl0[34] != (ADCTEST & 0xff))
		{
			return FAIL;
		}
			if(spl0[29] != 0x12)
		{
			return FAIL;
		}
	//============	write FT result  ==============
	spl0[30] = FT_PASS & 0xff;
  spl0[31] = (FT_PASS >> 8) & 0xff;	
		
	checksum = 0 - sumGen32((uint32_t *)spl0,15);

  spl0[PAGE_SIZE - 1] = (checksum >> 24) & 0xff;
  spl0[PAGE_SIZE - 2] = (checksum >> 16) & 0xff;
  spl0[PAGE_SIZE - 3] = (checksum >> 8) & 0xff;
  spl0[PAGE_SIZE - 4] = checksum & 0xff;
				
  //If read back the same. Do not Program again.	
	for(i=0;i<(PAGE_SIZE/4);i++)
	{
		if(mem_rd_reg32( +i*4) != *((uint32_t *)spl0 + 4*i))
		{
			break;
		}
	}				
				
	if(i == (PAGE_SIZE/4))
	{
		return SUCCESS;
	}			
				
  mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_PROT_OFFSET) ,0xfeed0000) ;   // lock out protection
		
  while( mem_rd_reg32((EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET)) & 0x1 );  //Wait Not Busy.				
  mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_CMD_OFFSET) ,ECMD_HVPL) ;      // clear HVPL
  
	for( i=0 ;i<PAGE_SIZE ;i++ )
	{
		mem_wr_reg8((EEPROM_BASE_REMAP3+i) ,*(spl0+i)) ;  // write HVPL
	}				
				
	mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_PGA_OFFSET) ,EE_SPL0_PGA) ;      //Set program page num.
		
  while( mem_rd_reg32((EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET)) & 0x1 );  //Wait Not Busy.					
  mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_CMD_OFFSET) ,ECMD_PE) ;         //Page Erase.
	
  while( mem_rd_reg32((EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET)) & 0x1 );  //Wait Not Busy.					
  mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_CMD_OFFSET) ,ECMD_PP) ;         //Page Program.	
		
	mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_PROT_OFFSET) ,0xfeedffff) ;   // lock protection	
	
	while( mem_rd_reg32((EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET)) & 0x1 );  //Wait Not Busy.	

  //Read Back and Confirm EEPROM Data right.	
	for(i=0;i<(PAGE_SIZE/4);i++)
	{
		temp = mem_rd_reg32(EEPROM_SP0_BASE+i*4);
		if(temp != *((uint32_t *)spl0 + i))
		{
			return FAIL;
		}
	}
	
	return SUCCESS;	
}
//===

uint32_t sp0LinePro(void)
{
  uint32_t i;
	uint32_t checksum;
	uint32_t temp;
	uint32_t iso7816_cks;
	uint32_t chip_id;
  //20 half-word
    iso7816_cks = iso7816ctrl_read();
    chip_id = 0x0;
  if(iso7816_cks == 0x200ea00)
  {
    chip_id = 0xd0000000;   //RA9103D0
  }
  else if(iso7816_cks == 0x20ea00)
  {
    chip_id = 0xa0000000;   //RA9103A0
  }
  else
  {
  	return FAIL;
  }
  if((chip_id & 0xf0000000)==0xa0000000)
  {
  uint8_t   spl0[PAGE_SIZE] = {
		F1 & 0xff,        (F1 >> 8) & 0xff,        (F1 >> 16) & 0xff, (F1 >> 24) & 0xff, 
		F2 & 0xff,        (F2 >> 8) & 0xff,        (F2 >> 16) & 0xff, (F2 >> 24) & 0xff, 
		F3 & 0xff,        (F3 >> 8) & 0xff,        (F3 >> 16) & 0xff, (F3 >> 24) & 0xff, 
	  F4 & 0xff,        (F4 >> 8) & 0xff,        (F4 >> 16) & 0xff, (F4 >> 24) & 0xff, 
		ANAADC & 0xff,    (ANAADC >> 8) & 0xff,    ANACLK1 & 0xff,    (ANACLK1 >> 8) & 0xff, 
		ANACLK2 & 0xff,   (ANACLK2 >> 8) & 0xff,   ANALDO & 0xff,     (ANALDO >> 8) & 0xff, 
		ANABGR & 0xff,    (ANABGR >> 8) & 0xff,    ANAPSW & 0xff,     (ANAPSW >> 8) & 0xff, 
		0xff,             0xff,                    0xff,              0xff, 
		TPSCTRL & 0xff,   (TPSCTRL >> 8) & 0xff,   ADCTEST & 0xff,    (ADCTEST >> 8) & 0xff, 
		CACHECTRL & 0xff, (CACHECTRL >> 8) & 0xff, 0xff, 							0xff, 
		0xff,             0xff,                    0xff,              0xff, 
		0xff,             0xff,                    0xff,              0xff, 
		0xff,             0xff,                    0xff,              0xff, 
		0xff,             0xff,                    0xff,              0xff, 
		0xff,             0xff,                    0xff,              0xff, 
		0xff,             0xff,                    0xff,              0xff
  };
	//==================   UID_code_mask  ================
//FT_TEST_times
		spl0[28] = ((0x3<<4)|(FT_times & 0xf));
    spl0[29] = ((0x12) & 0xff);									//V112.3
//wafer_CHIP_ID	
		
		temp=UID_adr_read( WAFER_CHIPID );
		
    spl0[38] = temp & 0xff;
    spl0[39] = (temp >> 8) & 0xff;
//wafer_serial_1	
		temp=UID_adr_read( WAFER_SERIAL_1_ADR );
    spl0[40] = temp & 0xff;
    spl0[41] = (temp >> 8) & 0xff;
//wafer_serial_2
		temp=UID_adr_read( WAFER_SERIAL_2_ADR );
		spl0[42] = temp & 0xff;
    spl0[43] = (temp >> 8) & 0xff;
//wafer_number 
		temp=UID_adr_read( WAFER_NUM_ADR );  
		spl0[44] = temp & 0xff;
    spl0[45] = (temp >> 8) & 0xff;
//wafer_x  
		temp=UID_adr_read( WAFER_X_ADR );    
		spl0[46] = temp & 0xff;
    spl0[47] = (temp >> 8) & 0xff;
//wafer_y 
		temp=UID_adr_read( WAFER_Y_ADR );  
		spl0[48] = temp & 0xff;
    spl0[49] = (temp >> 8) & 0xff;
//wafer_cp_result 
		temp=UID_adr_read( WAFER_RESULT_ADR );  
		spl0[50] = temp & 0xff;
    spl0[51] = (temp >> 8) & 0xff;
// 	
	
	checksum = 0 - sumGen32((uint32_t *)spl0,15);

  spl0[PAGE_SIZE - 1] = (checksum >> 24) & 0xff;
  spl0[PAGE_SIZE - 2] = (checksum >> 16) & 0xff;
  spl0[PAGE_SIZE - 3] = (checksum >> 8) & 0xff;
  spl0[PAGE_SIZE - 4] = checksum & 0xff;
				
  //If read back the same. Do not Program again.	
	/*************************
	0xB1,0x00000000
	0x87,0x00000000
	0xBD,0x03000042
	0xA3,0x80000042
	0x8B,0x00000000
	0x9F,0x00000000
	0x9F,0x10003FFC
	***************************/
	for(i=0;i<(PAGE_SIZE/4);i++)
	{
		if(mem_rd_reg32( +i*4) != *((uint32_t *)spl0 + 4*i))
		{
			break;
		}
	}				
				
	if(i == (PAGE_SIZE/4))
	{
		return SUCCESS;
	}			
	DBG_MSG("Get_Judge_Start");			
	/***************************************
	0x8B,0x08800014
	0xBB,0xFEED0000					// lock out protection
	***************************************/
  mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_PROT_OFFSET) ,0xfeed0000) ;   // lock out protection
		
  while( mem_rd_reg32((EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET)) & 0x1 );  //Wait Not Busy.				
  mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_CMD_OFFSET) ,ECMD_HVPL) ;      // clear HVPL
  
	for( i=0 ;i<PAGE_SIZE ;i++ )
	{
		mem_wr_reg8((EEPROM_BASE_REMAP3+i) ,*(spl0+i)) ;  // write HVPL
	}				
	DBG_MSG("Get_Judge_End");		
	mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_PGA_OFFSET) ,EE_SPL0_PGA) ;      //Set program page num.
		
  while( mem_rd_reg32((EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET)) & 0x1 );  //Wait Not Busy.					
  mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_CMD_OFFSET) ,ECMD_PE) ;         //Page Erase.
	
  while( mem_rd_reg32((EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET)) & 0x1 );  //Wait Not Busy.					
  mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_CMD_OFFSET) ,ECMD_PP) ;         //Page Program.	
		
	mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_PROT_OFFSET) ,0xfeedffff) ;   // lock protection	
	
	while( mem_rd_reg32((EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET)) & 0x1 );  //Wait Not Busy.	

  //Read Back and Confirm EEPROM Data right.	
	for(i=0;i<(PAGE_SIZE/4);i++)
	{
		temp = mem_rd_reg32(EEPROM_SP0_BASE+i*4);
		if(temp != *((uint32_t *)spl0 + i))
		{
			return FAIL;
		}
	}
	
	return SUCCESS;
}

else if((chip_id & 0xf0000000)==0xd0000000)
{
  uint8_t   spl0[PAGE_SIZE] = {
		F1 & 0xff,        (F1 >> 8) & 0xff,        (F1 >> 16) & 0xff, (F1 >> 24) & 0xff, 
		F2 & 0xff,        (F2 >> 8) & 0xff,        (F2 >> 16) & 0xff, (F2 >> 24) & 0xff, 
		F3 & 0xff,        (F3 >> 8) & 0xff,        (F3 >> 16) & 0xff, (F3 >> 24) & 0xff, 
	  F4 & 0xff,        (F4 >> 8) & 0xff,        (F4 >> 16) & 0xff, (F4 >> 24) & 0xff, 
		ANAADC & 0xff,    (ANAADC >> 8) & 0xff,    ANACLK1 & 0xff,    (ANACLK1 >> 8) & 0xff, 
		ANACLK2_B & 0xff,   (ANACLK2_B >> 8) & 0xff,   ANALDO_B & 0xff,     (ANALDO_B >> 8) & 0xff, 
		ANABGR_B & 0xff,    (ANABGR_B >> 8) & 0xff,    ANAPSW_B & 0xff,     (ANAPSW_B >> 8) & 0xff, 
		0xff,             0xff,                    0xff,              0xff, 
		TPSCTRL & 0xff,   (TPSCTRL >> 8) & 0xff,   ADCTEST & 0xff,    (ADCTEST >> 8) & 0xff, 
		CACHECTRL & 0xff, (CACHECTRL >> 8) & 0xff, SOFT_CHIPID_B & 0xff,            (SOFT_CHIPID_B >> 8) &  0xff, 
		0xff,             0xff,                    0xff,              0xff, 
		0xff,             0xff,                    0xff,              0xff, 
		0xff,             0xff,                    0xff,              0xff, 
		0xff,             0xff,                    0xff,              0xff, 
		0xff,             0xff,                    0xff,              0xff, 
		0xff,             0xff,                    0xff,              0xff
  };
	
   			ldo18_trim();
		if( powswh_bin == 1 )
		{
			if( ((trim_ANALDO >>4) & 0x1) != 1 )
			{
				DBG_ERR("powswh_bin = %d,trim_ANALDO=%x! \n",powswh_bin,trim_ANALDO);
				exitFail(1) ;
			}
		}
		else if( powswh_bin == 2 )
		{
			if( ((trim_ANALDO >>4) & 0x1) != 0 )
			{
				DBG_ERR("powswh_bin = %d,trim_ANALDO=%x! \n",powswh_bin,trim_ANALDO);
				exitFail(1) ;
			}
		}

	
     spl0[22] = (( ANALDO_B & 0xf06f)  | (trim_ANALDO & 0x0f90) )& 0xff;
     spl0[23] = (((ANALDO_B & 0xf06f)  | (trim_ANALDO & 0x0f90) ) >> 8) & 0xff;		
	rcl_trim();
//	spl0[20] = trim_rcl & 0x0ff;
//	spl0[21] = (trim_rcl >>8) & 0xff;
	
    spl0[20] = (( ANACLK2_B & 0x07ff) | ((trim_rcl & 0x1f) << 11)) & 0xff;
    spl0[21] = (((ANACLK2_B & 0x07ff) | ((trim_rcl & 0x1f) << 11)) >> 8) & 0xff;
		
//==================   UID_code_mask  ================
//FT_TEST_times
		spl0[28] = ((0x3<<4)|(FT_times & 0xf));
    spl0[29] = ((0x12) & 0xff);									//V112.3
//wafer_CHIP_ID	
		temp=UID_adr_read( WAFER_CHIPID );
		if(temp == 0xffff)
		{
			spl0[38] = SOFT_CHIPID_B & 0xff;
			spl0[39] = (SOFT_CHIPID_B >> 8) & 0xff;
		}	
		else if((temp == 0x000B)||(temp == 0x00B1))
		{
			spl0[38] = temp & 0xff;
			spl0[39] = (temp >> 8) & 0xff;
		}			
		else
		{
			exitFail(1) ;
		}
//wafer_serial_1	
		temp=UID_adr_read( WAFER_SERIAL_1_ADR );
    spl0[40] = temp & 0xff;
    spl0[41] = (temp >> 8) & 0xff;
//wafer_serial_2
		temp=UID_adr_read( WAFER_SERIAL_2_ADR );
		spl0[42] = temp & 0xff;
    spl0[43] = (temp >> 8) & 0xff;
//wafer_number 
		temp=UID_adr_read( WAFER_NUM_ADR );  
		spl0[44] = temp & 0xff;
    spl0[45] = (temp >> 8) & 0xff;
//wafer_x  
		temp=UID_adr_read( WAFER_X_ADR );    
		spl0[46] = temp & 0xff;
    spl0[47] = (temp >> 8) & 0xff;
//wafer_y 
		temp=UID_adr_read( WAFER_Y_ADR );  
		spl0[48] = temp & 0xff;
    spl0[49] = (temp >> 8) & 0xff;
//wafer_cp_result 
		temp=UID_adr_read( WAFER_RESULT_ADR );  
		spl0[50] = temp & 0xff;
    spl0[51] = (temp >> 8) & 0xff;
// 	
	
	checksum = 0 - sumGen32((uint32_t *)spl0,15);

  spl0[PAGE_SIZE - 1] = (checksum >> 24) & 0xff;
  spl0[PAGE_SIZE - 2] = (checksum >> 16) & 0xff;
  spl0[PAGE_SIZE - 3] = (checksum >> 8) & 0xff;
  spl0[PAGE_SIZE - 4] = checksum & 0xff;
				
  //If read back the same. Do not Program again.	
	for(i=0;i<(PAGE_SIZE/4);i++)
	{
		if(mem_rd_reg32( +i*4) != *((uint32_t *)spl0 + 4*i))
		{
			break;
		}
	}				
				
	if(i == (PAGE_SIZE/4))
	{
		return SUCCESS;
	}			
				
  mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_PROT_OFFSET) ,0xfeed0000) ;   // lock out protection
		
  while( mem_rd_reg32((EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET)) & 0x1 );  //Wait Not Busy.				
  mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_CMD_OFFSET) ,ECMD_HVPL) ;      // clear HVPL
  
	for( i=0 ;i<PAGE_SIZE ;i++ )
	{
		mem_wr_reg8((EEPROM_BASE_REMAP3+i) ,*(spl0+i)) ;  // write HVPL
	}				
				
	mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_PGA_OFFSET) ,EE_SPL0_PGA) ;      //Set program page num.
		
  while( mem_rd_reg32((EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET)) & 0x1 );  //Wait Not Busy.					
  mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_CMD_OFFSET) ,ECMD_PE) ;         //Page Erase.
	
  while( mem_rd_reg32((EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET)) & 0x1 );  //Wait Not Busy.					
  mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_CMD_OFFSET) ,ECMD_PP) ;         //Page Program.	
		
	mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_PROT_OFFSET) ,0xfeedffff) ;   // lock protection	
	
	while( mem_rd_reg32((EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET)) & 0x1 );  //Wait Not Busy.	

  //Read Back and Confirm EEPROM Data right.	
	for(i=0;i<(PAGE_SIZE/4);i++)
	{
		temp = mem_rd_reg32(EEPROM_SP0_BASE+i*4);
		if(temp != *((uint32_t *)spl0 + i))
		{ 
			return FAIL;
		}
	}
	
	return SUCCESS;
}
return SUCCESS;
}

uint32_t temperLinePro(void)
{
  uint32_t  i;
	uint32_t  checksum;
	uint32_t temp;
  //20 half-word
  uint8_t   spl1[PAGE_SIZE] = {
		CP0 & 0xff,      (CP0 >> 8) & 0xff,      (CP0 >> 16) & 0xff,      (CP0 >> 24) & 0xff,
		PASSWORD & 0xff, (PASSWORD >> 8) & 0xff, (PASSWORD >> 16) & 0xff, (PASSWORD >> 24) & 0xff, 
		RTCMODE & 0xff,  (RTCMODE >> 8) & 0xff,  RTCDOTA0 & 0xff,         (RTCDOTA0 >> 8) & 0xff, 
		RTCALPHAL & 0xff,(RTCALPHAL >> 8) & 0xff,RTCALPHAH & 0xff,        (RTCALPHAH >> 8) & 0xff, 
		RTCXT0 & 0xff,   (RTCXT0 >> 8) & 0xff,   RTCBETA & 0xff,          (RTCBETA >> 8) & 0xff, 
		WDTCTRL & 0xff,  (WDTCTRL >> 8) & 0xff,  EMAP & 0xff,             (EMAP >> 8) & 0xff, 
		WDTHALT & 0xff,  (WDTHALT >> 8) & 0xff,  WDTSTBY & 0xff,          (WDTSTBY >> 8) & 0xff, 
		RTCTADJ & 0xff,  (RTCTADJ >> 8) & 0xff,  RTCZT & 0xff,            (RTCZT >> 8) & 0xff, 
		0xff,            0xff,                   0xff,                    0xff, 
		0xff,            0xff, 			             0xff,                    0xff, 
		0xff,            0xff,                   0xff,                    0xff, 
		0xff,            0xff,                   0xff,                    0xff, 
		0xff,            0xff, 			             0xff,                    0xff, 
		0xff,            0xff,                   0xff,                    0xff, 
		0xff,            0xff, 			             0xff,                    0xff, 
		0xff,            0xff,                   0xff,                    0xff
	};

	uint32_t eeprom_data[PAGE_SIZE/4];

						
	for(i=0;i<(PAGE_SIZE/4);i++)
	{
		eeprom_data[i] = mem_rd_reg32(EEPROM_SP1_BASE+i*4);
	}
	
	checksum = 0 - sumGen32(eeprom_data,14);	
	/*
	if((checksum == eeprom_data[14]) && (eeprom_data[13] == 0xffffffff))     //Special Line 1 has define. not change other data.
	{
		for(i=0;i<2;i++)    //CP0,PASSWORD
	  {
		  *((uint32_t *)spl1 + i) = eeprom_data[i];
		}
		for(i=3;i<4;i++)   //RTC_ALPHAL,RTC_ALPHAH
	  {
		  *((uint32_t *)spl1 + i) = eeprom_data[i];
		}		
		for(i=5;i<7;i++)   //WDT_CTRL,EMAP,WDT_HATL,WDT_STBY.
	  {
		  *((uint32_t *)spl1 + i) = eeprom_data[i];
		}		
	}					
	*/			
	//Update TADJ
	spl1[28] = tadj & 0xff;
  spl1[29] = tadj >> 8;											
	
	checksum = 0 - sumGen32((uint32_t *)spl1,14);	

  spl1[PAGE_SIZE - 5] = (checksum >> 24) & 0xff;
  spl1[PAGE_SIZE - 6] = (checksum >> 16) & 0xff;
  spl1[PAGE_SIZE - 7] = (checksum >> 8) & 0xff;
  spl1[PAGE_SIZE - 8] = checksum & 0xff;
  			
				
  mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_PROT_OFFSET) ,0xfeed0000) ;   // lock out protection
		
  while( mem_rd_reg32((EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET)) & 0x1 );  //Wait Not Busy.				
  mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_CMD_OFFSET) ,ECMD_HVPL) ;      // clear HVPL
  
	for( i=0 ;i<PAGE_SIZE ;i++ )
	{
		mem_wr_reg8((EEPROM_BASE_REMAP3+i) ,*(spl1+i)) ;  // write HVPL
	}				
				
	mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_PGA_OFFSET) ,EE_SPL1_PGA) ;      //Set program page num.
		
  while( mem_rd_reg32((EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET)) & 0x1 );  //Wait Not Busy.					
  mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_CMD_OFFSET) ,ECMD_PE) ;         //Page Erase.
	
  while( mem_rd_reg32((EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET)) & 0x1 );  //Wait Not Busy.					
  mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_CMD_OFFSET) ,ECMD_PP) ;         //Page Program.	
		
	mem_wr_reg32((EEPROM_BASE_REMAP3+EEPROM_PROT_OFFSET) ,0xfeedffff) ;   // lock protection	
	
  while( mem_rd_reg32((EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET)) & 0x1 );  //Wait Not Busy.	
	
  //Read Back and Confirm EEPROM Data right.	
	for(i=0;i<(PAGE_SIZE/4);i++)
	{
		temp = mem_rd_reg32(EEPROM_SP1_BASE+i*4);
		
		if(temp != *((uint32_t *)spl1 + i))
		{
			return FAIL;
		}
	}
	
	return SUCCESS;
}


