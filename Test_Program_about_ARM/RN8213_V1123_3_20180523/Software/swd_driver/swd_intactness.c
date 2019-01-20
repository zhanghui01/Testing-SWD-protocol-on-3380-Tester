//#include "lpc1700.h"
//#include "target.h"
//#include "LPC1700PinCfg.h"
#include "lpc17xx.h"
#include "swd_drv.h"
#include "stdio.h"
#include "stdarg.h"
#include "atf.h"
#include "serial.h"
#include "testcase.h"

#define TESTCASE          "SIV"

extern void tc_printf(char * fmt,...);
extern void uart0_init(void);
void testfail(void);
// SWD GPIO Test , pass on 2012/5/21 @ SZR#4
#define  DEBUG_SWD_IO   0
#define  DEBUG_SWD_DRV  0
#define  DEBUG_SWD_ATF  1

#define BUF_LEN 512
char buf[BUF_LEN];

int main(void) {
  unsigned long stat, i, ack;
  //extern const s_atf_img sys_atf_tbl[];
  const unsigned long sys_atf_tbl_size = sizeof(sys_atf_tbl) / sizeof(s_atf_img);

  //initial GPIO for time test(P0.3)
  LPC_PINCON->PINSEL0 &= (~(0x3 << 6));
  LPC_GPIO0->FIODIR |= (1 << 3);
  LPC_GPIO0->FIOCLR |= (1 << 3);
	//SWD hardware Initialization
  swd_drv_init();
	
	//Uart Initialization
  SER_Init();

	printf("test LPC1759 swd performance...\n");
	
	printf("SW DAP detecting...");
	stat = swd_dap_detect();
	if (stat != SWD_RESP_OK)
	{
        printf("\tswd dap detect fail(0x%x)\n", stat);
		 testfail();
	}
	printf("pass\n");
/*
    while(1)
   	{
	   printf("------------probe soc after DAP detect---------------\n");
      soc_probe(0);
	   printf("------------Reset SOC---------------\n");
	  swd_debug_reset();
	  printf("------------probe soc after SOC reset---------------\n");
	  soc_probe(0);

     printf("------------Reconnect SWD---------------\n");
	  swd_connect();
	  printf("------------probe soc after DAP reconnect---------------\n");
	  soc_probe(0);
	  printf("------------SOC Reset again ---------------\n");
	  swd_debug_reset();
   	}
*/

	printf("dump data into sram memory...\n");

   for(i = 0; i < sys_atf_tbl_size; i++)
   	{
   	    printf(">>>>>>>>>>>>>run testcase: %s>>>>>>>>>>>\n", sys_atf_tbl[i].descr);
 	    ack = atf_img_run((ps_atf_img)(&sys_atf_tbl[i]));
		if(ack == TC_OK)
		{
		    printf("\tTEST PASS TEST PASS TEST PASS\n");
		}
		else if(ack == TC_FAIL)
		{
		    printf("\t!!!TEST FAIL TEST FAIL TEST FAIL!!!\n");
		}
		else
		{
		    printf("\t!!!SWD FAIL SWD FAIL SWD FAILl!!!\n");
		}		
	    printf("<<<<<<<<<<<<<<finish<<<<<<<<<<<<<\n");			
   	}

  while(1)
  {
      ;
   }
}

void tc_printf(char * fmt,...) {
  char buf[256];
  va_list ap;
  printf("[%s] ",TESTCASE);
  va_start(ap,fmt);
  vsprintf(buf,fmt,ap);
  va_end(ap);
  printf("%s",buf);
}

void testfail(void)
{
    while(1)
    {
        ;
    }
}
