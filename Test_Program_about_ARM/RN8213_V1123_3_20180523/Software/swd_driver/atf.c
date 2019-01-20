#include "atf.h"
#include "lpc17xx.h"
#include "swd_drv.h"
#include "serial.h"

extern void tc_printf(char * fmt,...);

//const unsigned long sys_atf_tbl_size = sizeof(sys_atf_tbl) / sizeof(s_atf_img);

void usDelay(uint32_t us)
{
  uint32_t    i, loops;
  
	if(us == 0)
		return;
	
  SystemCoreClockUpdate();
    
  //Max freq:100Mhz(10ns), max systick length:167ms
  loops = (us / 167000);
  for(i = 0; i < loops; i++)
  {
    SysTick->LOAD = 0xffffff;   
    SysTick->CTRL = 0x4;
    SysTick->VAL  = 0x0;
	  SysTick->LOAD = 0xffffff;//SystemCoreClock / 1000000 * us;
    
    SysTick->CTRL = 0x5;
    while(!(SysTick->CTRL & 0x10000))
    {
      ;
    }
    SysTick->CTRL = 4;
  }
  
  loops = us - (loops * 167000);
  if(loops != 0)
  {
    SysTick->LOAD = 0xffffff;   
    SysTick->CTRL = 0x4;
    SysTick->VAL  = 0x0;
	  SysTick->LOAD = SystemCoreClock / 1000000 * loops;
    
    SysTick->CTRL = 0x5;
    while(!(SysTick->CTRL & 0x10000))
    {
      ;
    }
    SysTick->CTRL = 4;
  }
}

unsigned long atf_img_run(ps_atf_img pImg) {
  unsigned long ack = SWD_ATF_IMG_LOAD_FAIL;

  if(pImg->load_remap == FLASH_RESIDENT)
  {
      if(pImg->load_addr < 0x08000000)
			ack = atf_flash_img_load(pImg);
	   else if(pImg->load_addr < 0x10000000)
		 	ack = atf_eeprom_img_load(pImg);
	   else 
		 	ack = atf_sram_img_load(pImg);
  }
  else if(pImg->load_remap == EEPROM_RESIDENT)
  {
      if(pImg->load_addr < 0x08000000)
			ack = atf_eeprom_img_load(pImg);
	   else if(pImg->load_addr < 0x10000000)
		 	ack = atf_flash_img_load(pImg);
	   else 
		 	ack = atf_sram_img_load(pImg);
  }
  else 
  {
      if(pImg->load_addr < 0x08000000)
			ack = atf_sram_img_load(pImg);
	   else if(pImg->load_addr < 0x10000000)
		 	ack = atf_eeprom_img_load(pImg);
	   else 
		 	ack = atf_flash_img_load(pImg);
  }

  if (ack != SWD_RESP_OK)
  {
	  DBG(" Fail to load !\n");
	  return ack;
  }    

  ack = atf_img_monitor(pImg);
  if(ack != TC_OK)
  {
      DBG("soc execute fail!(0x%x)\n", ack);
  }
  return ack;
}

static void atf_mip_wait_timeout(unsigned long fl_addr, unsigned long us)
{
    unsigned long ack;
	unsigned long td;
	uint32_t    i, loops;
  
	if(us == 0)
		return;
	
  SystemCoreClockUpdate();
    
  //Max freq:100Mhz(10ns), max systick length:167ms
  loops = (us / 167000);
  for(i = 0; i < loops; i++)
  {
    SysTick->LOAD = 0xffffff;   
    SysTick->CTRL = 0x4;
    SysTick->VAL  = 0x0;
	  SysTick->LOAD = 0xffffff;//SystemCoreClock / 1000000 * us;
    
    SysTick->CTRL = 0x5;
    while(!(SysTick->CTRL & 0x10000))
    {
      ack = swd_mem_rd32(fl_addr + 0x00800008, &td, 1);
	   if((ack == SWD_RESP_OK) && ((td & 0x1) == 0))
	   	{
	   	    SysTick->CTRL = 4;
			 return;
	   	}
    }
    SysTick->CTRL = 4;
  }
  
  loops = us - (loops * 167000);
  if(loops != 0)
  {
    SysTick->LOAD = 0xffffff;   
    SysTick->CTRL = 0x4;
    SysTick->VAL  = 0x0;
	  SysTick->LOAD = SystemCoreClock / 1000000 * loops;
    
    SysTick->CTRL = 0x5;
    while(!(SysTick->CTRL & 0x10000))
    {
      ack = swd_mem_rd32(fl_addr + 0x00800008, &td, 1);
	   if((ack == SWD_RESP_OK) && ((td & 0x1) == 0))
	   	{
	   	    SysTick->CTRL = 4;
			 return;
	   	}
    }
    SysTick->CTRL = 4;
  }
}

unsigned long atf_flash_wr(unsigned long to_addr,unsigned char * pBuf,unsigned long len) {
  unsigned long ack = SWD_RESP_OK;
  unsigned long i   = 0, td, fl_addr;

  // alignment mismatch and len equal to 0
  if ((len == 0) || ((to_addr & 3) != (((unsigned long)pBuf) & 3))) {
    return ack;
  }

  ack = swd_mem_rd32(0x40034018, &td, 1);  //remap
  td = td & 0x7;
  fl_addr = (td == 0) ? 0x0 : (td == 1) ? 0x08000000 : (td == 2) ? 0x10000000 : (td == 3) ? 0x18000000 : 0x0;
	
  td = 0xfeed0000;
  ack = swd_mem_wr32(fl_addr + 0x00800014, &td, 1);   //protect
  len = (len < 128) ? 128 : len;
  for(i = 0; (i * 128) < len; i++)
  {
      ack = swd_mem_wr32(fl_addr + 0x00800004, &i, 1); //pga
      td = 0xfeed000b;
      ack = swd_mem_wr32(fl_addr + 0x00800000, &td, 1); //page erase
      //usDelay(7500);
      atf_mip_wait_timeout(fl_addr, 7500);
	   //atf_flash_wait(fl_addr);
      td = 0xfeed0004;
      ack = swd_mem_wr32(fl_addr + 0x00800000, &td, 1); //clear hvpl
      atf_mip_wait_timeout(fl_addr, 100);
	   //atf_flash_wait(fl_addr);
      ack = swd_mem_wr8(fl_addr, pBuf + i * 128, 128);
	   td = 0xfeed000f;
	   ack = swd_mem_wr32(fl_addr + 0x00800000, &td, 1); //page program
	   //usDelay(4500);
	   atf_mip_wait_timeout(fl_addr, 4500);
	   //atf_flash_wait(fl_addr);
  }
	

  return ack;
}

unsigned long atf_eeprom_wr(unsigned long to_addr,unsigned char * pBuf,unsigned long len) {
  unsigned long ack = SWD_RESP_OK;
  unsigned long i   = 0, td, ee_addr;

  // alignment mismatch and len equal to 0
  if ((len == 0) || ((to_addr & 3) != (((unsigned long)pBuf) & 3))) {
    return ack;
  }

  ack = swd_mem_rd32(0x40034018, &td, 1);  //remap
  td = td & 0x7;
  ee_addr = (td == 1) ? 0x0 : 0x08000000;
	
  td = 0xfeed0000;
  ack = swd_mem_wr32(ee_addr + 0x00800014, &td, 1);   //protect
  len = (len < 64) ? 64 : len;
  for(i = 0; (i * 64) < len; i++)
  {
      ack = swd_mem_wr32(ee_addr + 0x00800004, &i, 1); //pga
      td = 0xfeed000b;
      ack = swd_mem_wr32(ee_addr + 0x00800000, &td, 1); //page erase
      //usDelay(3500);
      atf_mip_wait_timeout(ee_addr, 3500);
      td = 0xfeed0004;
      ack = swd_mem_wr32(ee_addr + 0x00800000, &td, 1); //clear hvpl
      atf_mip_wait_timeout(ee_addr, 100);
      ack = swd_mem_wr8(ee_addr, pBuf + i * 64, 64);
	   td = 0xfeed000f;
	   ack = swd_mem_wr32(ee_addr + 0x00800000, &td, 1); //page program
	   //usDelay(2500);
	   atf_mip_wait_timeout(ee_addr, 2500);
  }
	

  return ack;
}


unsigned long atf_sram_wr(unsigned long to_addr,unsigned char * pBuf,unsigned long len) {
  unsigned long ack = SWD_RESP_OK;
  unsigned long i   = 0;

  // alignment mismatch and len equal to 0
  if ((len == 0) || ((to_addr & 3) != (((unsigned long)pBuf) & 3))) {
    return ack;
  }

  // preceding mis-alignment conceal
  if ((to_addr & 3) != 0) {
    i = 4 - (to_addr & 3);
    ack = swd_mem_wr8(to_addr,pBuf,i);
  }
  to_addr += i;
  pBuf += i;
  len -= i;

  if (ack == SWD_RESP_OK) {
    i = len & 0xFFFFFFFC;
#ifndef CGF
    ack = swd_mem_wr32(to_addr,(unsigned long *)pBuf,i>>2);
#else
    ack = swd_mem_wr32_mem(to_addr,(unsigned long *)pBuf,i>>2);
#endif
	to_addr += i;
    pBuf += i;
    len -= i;

    // postfix mis-allignment conceal
	if (ack == SWD_RESP_OK) {
	  if (len != 0) {
	    ack = swd_mem_wr32(to_addr,(unsigned long *)pBuf,len);
	  }
    }
  }

  return ack;
}

unsigned long atf_remap_set(unsigned long remap) {
  unsigned long td, ack;

  td = 0x82;
  ack = swd_mem_wr32(SOC_REG_PS, &td, 1);
  if(ack != SWD_RESP_OK)
		return ack;
	
  td = remap & 3;
  return swd_mem_wr32(SOC_REG_REMAP_ADDR,&td,1);
}

unsigned long soc_init(void)
{
    unsigned long td, ack, con_loop = 0;

    do
    {
        ack = swd_connect();
		 con_loop++;
    }while((ack != SWD_RESP_OK) && (con_loop < 4));
		
	if(ack != SWD_RESP_OK)
   {
       DBG_ERR("SWD connect fail(0x%x)\n", ack);
	}
	
    td = 0x82;
	ack = swd_mem_wr32(SOC_REG_PS, &td, 1);
	if(ack != SWD_RESP_OK)
   {
       DBG_ERR("write SOC PS reg fail(0x%x)\n", ack);
	}

	return ack;
}

unsigned long soc_probe(unsigned char halt)
{
    unsigned long td = 0;
	unsigned long ack = 0, state;

    printf("<><><><><><><>Probe SOC<><><><><><><>\n");
	if(halt)
	{
	    state = swd_debug_halt();
		if(state != SWD_RESP_DEBUG_STATE)
		{
		    printf("halt soc fail(0x%x)\t", state);
			return state;
		}
		else
		{
		    printf("\tsoc in debug state\n");
		}
	}
	else
	{
		state = swd_debug_state_chk();
  	    if(state == SWD_RESP_DEBUG_STATE)
	    {
	        printf("\tsoc in debug state\n");
	     }
  	     else if(state == SWD_RESP_NOT_DEBUG_STATE)
	     {
	         printf("\tsoc not in debug state\n");
	      }
	      else
	      {
	           printf("\tget invalid response from debug_chk(0x%x)\n", state);
		       return state;
	       }
	 }

	ack = swd_mem_rd32(0x40034018, &td, 1);
	if(ack != SWD_RESP_OK)
	{
	    printf("\tget invalid response from RD REMAP REG(0x%x)\n", ack);
		return ack;
	}
	printf("\tsoc remap value: 0x%x\n", td);

	ack = swd_mem_rd32(0x40034014, &td, 1);
	if(ack != SWD_RESP_OK)
	{
	    printf("\tget invalid response from RD RESET REG(0x%x)\n", ack);
		return ack;
	}
	printf("\tsoc reset value: 0x%x\n", td);

	ack = swd_mem_rd32(0x40034000, &td, 1);
	if(ack != SWD_RESP_OK)
	{
	    printf("\tget invalid response from RD OSC_CTRL1 REG(0x%x)\n", ack);
		return ack;
	}
	printf("\tsoc osc_ctrl1 value: 0x%x\n", td);

   if(state == SWD_RESP_DEBUG_STATE)
   	{
	  	ack = swd_core_reg_rd(15, &td);
		if(ack != SWD_RESP_OK)
		{
		    printf("\tget invalid response from PC(0x%x)\n", ack);
			return ack;
		}
		printf("\tsoc pc value: 0x%x\n", td);

		ack = swd_core_reg_rd(13, &td);
		if(ack != SWD_RESP_OK)
		{
		    printf("\tget invalid response from SP(0x%x)\n", ack);
			return ack;
		}
		printf("\tsoc sp value: 0x%x\n", td);
   	}

	if(halt)
	{
	    ack = swd_debug_haltexit();
		 if(ack != SWD_RESP_NOT_DEBUG_STATE)
		 {
		     printf("\texit halt fail(0x%x)\n", ack);
			  return ack;
		 }
	}
	return ack;
}

unsigned long atf_flash_img_load(ps_atf_img pImg) {
  unsigned long ack = swd_debug_halt();

  if(soc_init() == SWD_RESP_OK){
      DBG("    setting loading remap ...\n");
      ack = atf_remap_set(pImg->load_remap);
  	  if (ack == SWD_RESP_OK) {
        printf("    writing image ...\n");
		    //LPC_GPIO0->FIOSET |= (1 << 3);
        ack = atf_flash_wr(pImg->load_addr,pImg->pImg,pImg->img_len);
		    //LPC_GPIO0->FIOCLR |= (1 << 3);
	    if (ack == SWD_RESP_OK) {
          printf("    verifying image ...\n");
		   //LPC_GPIO0->FIOSET |= (1 << 3);
	      ack = atf_mem_verify(pImg->load_addr,pImg->pImg,pImg->img_len);
		   //LPC_GPIO0->FIOCLR |= (1 << 3);
		  if (ack == SWD_RESP_OK) {
            printf("    setting executing remap ...\n");
		     //ack = atf_remap_set(pImg->exec_remap);
		     if (ack == SWD_RESP_OK) {
              printf("    starting image ...\n");
		       ack = atf_img_exec(pImg);
		     }
		   }
	     }
    }
  }

  if ((ack != SWD_RESP_DEBUG_STATE) && (ack != SWD_RESP_OK))
    DBG_ERR("    fail to ignite(0x%x) ...\n", ack);

  return ack;
}

unsigned long atf_eeprom_img_load(ps_atf_img pImg) {
  unsigned long ack = swd_debug_halt();

  if(soc_init() == SWD_RESP_OK){
    if (ack == SWD_RESP_DEBUG_STATE) {
      DBG("    setting loading remap ...\n");
      ack = atf_remap_set(pImg->load_remap);
  	  if (ack == SWD_RESP_OK) {
        printf("    writing image ...\n");
		    //LPC_GPIO0->FIOSET |= (1 << 3);
        ack = atf_eeprom_wr(pImg->load_addr,pImg->pImg,pImg->img_len);
		    //LPC_GPIO0->FIOCLR |= (1 << 3);
	    if (ack == SWD_RESP_OK) {
          printf("    verifying image ...\n");
		   //LPC_GPIO0->FIOSET |= (1 << 3);
	      ack = atf_mem_verify(pImg->load_addr,pImg->pImg,pImg->img_len);
		   //LPC_GPIO0->FIOCLR |= (1 << 3);
		  if (ack == SWD_RESP_OK) {
            printf("    setting executing remap ...\n");
		     //ack = atf_remap_set(pImg->exec_remap);
		     if (ack == SWD_RESP_OK) {
              printf("    starting image ...\n");
		       ack = atf_img_exec(pImg);
		     }
		   }
	     }
	   }
    }
  }

  if ((ack != SWD_RESP_DEBUG_STATE) && (ack != SWD_RESP_OK))
    DBG_ERR("    fail to ignite(0x%x) ...\n", ack);

  return ack;
}

unsigned long atf_sram_img_load(ps_atf_img pImg) {
  unsigned long ack = swd_debug_halt();

  if(soc_init() == SWD_RESP_OK){
    if (ack == SWD_RESP_DEBUG_STATE) {
      DBG("    setting loading remap ...\n");
      ack = atf_remap_set(pImg->load_remap);
  	  if (ack == SWD_RESP_OK) {
        DBG("    writing image ...\n");
		 //LPC_GPIO0->FIOSET |= (1 << 3);
        ack = atf_sram_wr(pImg->load_addr,pImg->pImg,pImg->img_len);
		 //LPC_GPIO0->FIOCLR |= (1 << 3);
	    if (ack == SWD_RESP_OK) {
          DBG("    verifying image ...\n");
		   //LPC_GPIO0->FIOSET |= (1 << 3);
	      ack = atf_mem_verify(pImg->load_addr,pImg->pImg,pImg->img_len);
		   //LPC_GPIO0->FIOCLR |= (1 << 3);
		  if (ack == SWD_RESP_OK) {
            DBG("    setting executing remap ...\n");
		     //ack = atf_remap_set(pImg->exec_remap);
		     if (ack == SWD_RESP_OK) {
              DBG("    starting image ...\n");
		       ack = atf_img_exec(pImg);
		     }
		   }
	     }
	   }
    }
  }

  if ((ack != SWD_RESP_DEBUG_STATE) && (ack != SWD_RESP_OK))
    DBG_ERR("    fail to ignite(0x%x) ...\n", ack);

  return ack;
}

unsigned long atf_mem_verify(unsigned long to_addr,unsigned char * pBuf,unsigned long len) {
  unsigned long ack = SWD_RESP_OK;//SWD_RESP_PROTOCAL_ERROR;
  unsigned long i   = 0;

#if 1
#ifdef CGF
   // alignment mismatch and len equal to 0
  if ((len == 0) || ((to_addr & 3) != (((unsigned long)pBuf) & 3))) {
    return ack;
  }

  // preceding mis-alignment conceal
  if ((to_addr & 3) != 0) {
    i = 4 - (to_addr & 3);
    ack = swd_mem_verf8(to_addr, pBuf, i);
	 if(ack == SWD_RESP_NOT_MATCH)
	 {
	     return ack;
	 }
  }
  to_addr += i;
  pBuf += i;
  len -= i;

  if (ack == SWD_RESP_OK) {
    i = len & 0xFFFFFFFC;
	{
        ack = swd_mem_verf32(to_addr, (unsigned long *)pBuf, i/4);
		 if(ack == SWD_RESP_NOT_MATCH)
		 {
            return ack;
		 }
	}
  }
#endif
#endif

#ifndef CGF
  for (i = 0; i < len; i ++) {
    ack = swd_mem_rd8(to_addr + i,&td,1);
	if (ack == SWD_RESP_OK) {
	  if ((td & 0xFF) != pBuf[i]) {
	    return SWD_RESP_PROTOCAL_ERROR;
	  }
	} else {
	  return ack;
	}
  }
#endif

  return ack;
}

unsigned long atf_img_exec(ps_atf_img pImg) {
  unsigned long ack = SWD_RESP_PROTOCAL_ERROR, td;
  unsigned long i = 0;
  unsigned long core_reg_addr_tbl[] = {
      CM0_DBG_REGSEL_R0      
    , CM0_DBG_REGSEL_R1      
    , CM0_DBG_REGSEL_R2      
    , CM0_DBG_REGSEL_R3      
    , CM0_DBG_REGSEL_R4      
    , CM0_DBG_REGSEL_R5      
    , CM0_DBG_REGSEL_R6      
    , CM0_DBG_REGSEL_R7      
    , CM0_DBG_REGSEL_R8      
    , CM0_DBG_REGSEL_R9      
    , CM0_DBG_REGSEL_R10     
    , CM0_DBG_REGSEL_R11     
    , CM0_DBG_REGSEL_R12     
    , CM0_DBG_REGSEL_SP      
    , CM0_DBG_REGSEL_LR      
    , CM0_DBG_REGSEL_PC      
    , CM0_DBG_REGSEL_XPSR    
    , CM0_DBG_REGSEL_MSP     
    , CM0_DBG_REGSEL_PSP     
    , CM0_DBG_REGSEL_PRIMASK 
  };
  unsigned long core_reg_value_tbl[] = {
       0 ,  1 ,  2 ,  3
    ,  4 ,  5 ,  6 ,  7
    ,  8 ,  9 , 10 , 11
	, 12 
	, 0
	, 14
	, 0
    , 0x01000000    
	, 0
	, 0
    , 0 
  };

  core_reg_value_tbl[13] = ((unsigned long *)(pImg->pImg))[0];
  core_reg_value_tbl[15] = ((unsigned long *)(pImg->pImg))[1];
  core_reg_value_tbl[17] = ((unsigned long *)(pImg->pImg))[0];
  core_reg_value_tbl[18] = ((unsigned long *)(pImg->pImg))[0];

#ifndef CGF
  ack = swd_reset_vc_enable();
  ack = swd_debug_reset();
  if (ack != SWD_RESP_RESET_ST) {
    return ack;
  }
#endif
  for (i = 0; i < 20 ; i ++) {
    ack = swd_core_reg_wr(core_reg_addr_tbl[i],core_reg_value_tbl[i]);
	if (ack != SWD_RESP_OK)
	  return ack;
  }

  ack = swd_halted_chk();
  ack = swd_reset_vc_disable();
  //swd_probe_soc(0);
#ifndef CGF
  ack = swd_debug_disable();
#else
  ack = swd_debug_haltexit();
#endif
#ifdef CGF
  ack = swd_halted_chk();
  ack = swd_mem_rd32(0x40034030, &td, 1);
  ack = swd_mem_rd32(0x40034018, &td, 1);
  ack = swd_core_reg_rd(15, &td);
  ack = swd_core_reg_rd(13, &td);
#endif
  return ack;
}

unsigned long atf_img_monitor(ps_atf_img pImg) {
  unsigned long ack = 0;
  unsigned long td = 0;
  unsigned long monitor_addr;
  unsigned long i, loops;

  ack = swd_mem_rd32(SOC_REG_REMAP_ADDR, &td, 1);
  if(ack != SWD_RESP_OK)
		return ack;

  td = td & 0x7;
  monitor_addr = (td == 2) ? 0x0 : 0x10000000;
  monitor_addr += SOC_MONITOR_ADDR;
  
	if(pImg->timeout == 0)
	{
	    do
	    {
            ack = swd_mem_rd32(monitor_addr,&td,1);
	    }while((ack == SWD_RESP_OK) && (td != TC_OK) && (td != TC_FAIL));
		if(ack != SWD_RESP_OK)
		     return TC_FAIL;
		else
			return td;
	}
	
  SystemCoreClockUpdate();
    
  //Max freq:100Mhz(10ns), max systick length:167ms
  loops = (pImg->timeout / 167);
  for(i = 0; i < loops; i++)
  {
    SysTick->LOAD = 0xffffff;   
    SysTick->CTRL = 0x4;
    SysTick->VAL  = 0x0;
	 SysTick->LOAD = 0xffffff;//SystemCoreClock / 1000000 * us;
    
    SysTick->CTRL = 0x5;
    while(!(SysTick->CTRL & 0x10000))
    {
      ack = swd_mem_rd32(monitor_addr,&td,1);
	   if((ack == SWD_RESP_OK) && ((td == TC_OK) || (td == TC_FAIL)))
	   	{
	   	    SysTick->CTRL = 4;
			 return td;
	   	}
    }
    SysTick->CTRL = 4;
  }
  
  loops = pImg->timeout - (loops * 167);
  if(loops != 0)
  {
    SysTick->LOAD = 0xffffff;   
    SysTick->CTRL = 0x4;
    SysTick->VAL  = 0x0;
	 SysTick->LOAD = SystemCoreClock / 1000 * loops;
    
    SysTick->CTRL = 0x5;
    while(!(SysTick->CTRL & 0x10000))
    {
      ack = swd_mem_rd32(monitor_addr,&td,1);
	   if((ack == SWD_RESP_OK) && ((td == TC_OK) || (td == TC_FAIL)))
	   	{
	   	    SysTick->CTRL = 4;
			 return td;
	   	}
    }
    SysTick->CTRL = 4;
  }

  //return TC_FAIL;
  return td;
}

