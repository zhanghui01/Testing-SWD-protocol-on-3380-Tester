#ifndef __SWD_INTACTNESS_SWD_DRV_H
#define __SWD_INTACTNESS_SWD_DRV_H

#define CGF

#define SWD_RESP_OK                  1
#define SWD_RESP_WAIT                2
#define SWD_RESP_FAULT               4
#define SWD_RESP_PROTOCAL_ERROR      7
#define SWD_RESP_HALTED              8
#define SWD_RESP_BREAK               9
#define SWD_RESP_WATCH               10
#define SWD_RESP_VECTOR_CAPTURE      11
#define SWD_RESP_EXT_DBG_REQ         12
#define SWD_RESP_NOT_HALTED          13
#define SWD_RESP_NOT_BREAK           14
#define SWD_RESP_NOT_WATCH           15
#define SWD_RESP_NOT_VECTOR_CAPTURE  16
#define SWD_RESP_NOT_EXT_DBG_REQ     17
#define SWD_RESP_SVC                 18
#define SWD_RESP_NOT_SVC             19
#define SWD_RESP_RESET_VC_EN         20
#define SWD_RESP_NOT_RESET_VC_EN     21
#define SWD_RESP_HARDFAULT_VC_EN     22
#define SWD_RESP_NOT_HARDFAULT_VC_EN 23
#define SWD_RESP_DWT_EN              24
#define SWD_RESP_NOT_DWT_EN          25
#define SWD_RESP_REGRDY              26
#define SWD_RESP_NOT_REGRDY          27
#define SWD_RESP_HALT                28
#define SWD_RESP_NOT_HALT            29
#define SWD_RESP_SLEEP               30
#define SWD_RESP_NOT_SLEEP           31
#define SWD_RESP_LOCKUP              32
#define SWD_RESP_NOT_LOCKUP          33
#define SWD_RESP_RETIRE              34
#define SWD_RESP_NOT_RETIRE          35
#define SWD_RESP_RESET_ST            36
#define SWD_RESP_NOT_RESET_ST        37
#define SWD_RESP_DEBUG_ENABLE        38
#define SWD_RESP_NOT_DEBUG_ENABLE    39
#define SWD_RESP_DWT_MATCH           40
#define SWD_RESP_NOT_DWT_MATCH       41
#define SWD_RESP_RETIRE_ST           42
#define SWD_RESP_NOT_RETIRE_ST       43
#define SWD_RESP_DEBUG_STATE         44
#define SWD_RESP_NOT_DEBUG_STATE     45
#define SWD_RESP_SREGRDY             46
#define SWD_RESP_NOT_SREGRDY         47
#define SWD_RESP_BPU_EN              48
#define SWD_RESP_NOT_BPU_EN          49
#define SWD_RESP_NOT_MATCH       50

#define SWD_ABORT_DAPABORT         1
#define SWD_ABORT_STKCMPCLR        2
#define SWD_ABORT_STKERRCLR        4
#define SWD_ABORT_WDERRCLR         8
#define SWD_ABORT_ORUNERRCLR       0x10

#define SWD_DPID_RN8209            0x0BB11477

#define SWD_AP_CLASS_MEM           1
#define SWD_AP_CLASS_NON_MEM       0

#define SWD_COMPONENT_CLASS_GVC                   0
#define SWD_COMPONENT_CLASS_ROM_TABLE             1
#define SWD_COMPONENT_CLASS_DEBUG_COMPONENT       9
#define SWD_COMPONENT_CLASS_PERIPHERAL_TEST_BLOCK 0xB
#define SWD_COMPONENT_CLASS_OPTIMO_DATA_ENGINE    0xD
#define SWD_COMPONENT_CLASS_GENERIC_IP            0xE	
#define SWD_COMPONENT_CLASS_PRIME_CELL_IP         0xF

#define SWD_ROM_TABLE_COMPONENT_ID_PREAMBLE_L     0x00D
#define SWD_ROM_TABLE_COMPONENT_ID_PREAMBLE_H     0xB105

#define SWD_BUS_MAX_RETRY                         3

typedef struct{
  union {
    unsigned long word;
  struct{
    unsigned long DAPABORT   : 1;
    unsigned long STKCMPCLR  : 1;
    unsigned long STKERRCLR  : 1;
    unsigned long WDERRCLR   : 1;
    unsigned long ORUNERRCLR : 1;
    unsigned long reserved   : 27;
  }bit_fields;
  }ABORT;
  union {
    unsigned long word;
  struct{
    unsigned long reserved   : 1;
    unsigned long DESIGNER   : 11;
    unsigned long PARTNO     : 16;
    unsigned long Version    : 4;
  }bit_fields;
  }IDCODE;
  union {
    unsigned long word;
  struct{
    unsigned long ORUNDETECT   : 1;
    unsigned long STICKYORUN   : 1;
    unsigned long TRNMODE      : 2;
    unsigned long STICKYCMP    : 1;
    unsigned long STICKYERR    : 1;
    unsigned long READOK       : 1;
    unsigned long WDATAERR     : 1;
    unsigned long MASKLANE     : 4;
    unsigned long TRNCNT       : 12;
    unsigned long reserved     : 2;
    unsigned long CDBGRSTREQ   : 1;
    unsigned long CDBGRSTACK   : 1;
    unsigned long CDBGPWRUPREQ : 1;
    unsigned long CDBGPWRUPACK : 1;
    unsigned long CSYSPWRUPREQ : 1;
    unsigned long CSYSPWRUPACK : 1;
  }bit_fields;
  }CTRL_N_STAT;
  union {
    unsigned long word;
  struct{
    unsigned long CTRLSEL      : 1;
    unsigned long reserved     : 3;
    unsigned long APBANKSEL    : 4;
    unsigned long reserved1    : 16;
    unsigned long APSEl        : 8;
  }bit_fields;
  }SELECT;
  union {
    unsigned long word;
  struct{
    unsigned long PRESCALER    : 3;
    unsigned long reserved     : 3;
    unsigned long WIREMODE     : 2;
    unsigned long TURNROUND    : 2;
    unsigned long reserved1    : 22;
  }bit_fields;
  }WCR;
}s_swd_dp_reg_set, * ps_swd_dp_reg_set;

typedef struct{
  union {
    unsigned long word;
  struct{
    unsigned long SZIE         : 3;
    unsigned long reserved     : 1;
    unsigned long AddrInc      : 2;
    unsigned long DeviceEn     : 1;
    unsigned long TrInProg     : 1;
    unsigned long Mode         : 4;
    unsigned long reserved1    : 11;
    unsigned long SPIDEN       : 1;
    unsigned long Prot         : 7;
    unsigned long DbgSwEnable  : 1;
  }bit_fields;
  }WCR;
  unsigned long Big_endian;
  union {
    unsigned long word;
  struct{
    unsigned long EntryPresent : 1;
    unsigned long Format       : 1;
    unsigned long reserved     : 10;
    unsigned long BASEADDR     : 20;
  }bit_fields;
  }BASE;
  union {
    unsigned long word;
  struct{
    unsigned long ApType       : 4;
    unsigned long ApVariant    : 4;
    unsigned long reserved     : 8;
    unsigned long Class        : 1;
    unsigned long JEP106_id    : 7;
    unsigned long JEP106_cont  : 4;
    unsigned long Revision     : 4;
  }bit_fields;
  }IDR;
}s_swd_map_reg_set, * ps_swd_map_reg_set;

typedef struct{
  union{
    unsigned long word;
    struct{
    unsigned char ID0;
    unsigned char ID1;
    unsigned char ID2;
    unsigned char ID3;
  }IDS;
  struct{
    unsigned long Preamble_L      : 12;
      unsigned long Component_Class : 4;
    unsigned long Preamble_H      : 16;
  }bit_fields;
  }ComponentID;
  union{
    unsigned long long word;
    struct{
    unsigned char ID0;
    unsigned char ID1;
    unsigned char ID2;
    unsigned char ID3;
    unsigned char ID4;
    unsigned char ID5;
    unsigned char ID6;
    unsigned char ID7;
  }IDS;
  struct{
    unsigned long PartNumber      : 12;
      unsigned long JEP106_id       : 7;
    unsigned long JEP106_used     : 1;
    unsigned long Revision        : 4;
    unsigned long CustMod         : 4;
    unsigned long RevAnd          : 4;
    unsigned long JEP106_cont     : 4;
    unsigned long Count_4KB       : 4;
    unsigned long reserved        : 24;
  }bit_fields;
  }PeripheralID;
}s_swd_debug_component,* ps_swd_debug_component;

#define SWD_PKT_REQ_START          1
#define SWD_PKT_REQ_AP             2
#define SWD_PKT_REQ_DP             0
#define SWD_PKT_REQ_WRITE          0
#define SWD_PKT_REQ_READ           0x4
#define SWD_PKT_REQ_ADDR_DP_IDCODE 0
#define SWD_PKT_REQ_ADDR_DP_ABORT  0
#define SWD_PKT_REQ_ADDR_DP_CTLSTA 8
#define SWD_PKT_REQ_ADDR_DP_WCR    8
#define SWD_PKT_REQ_ADDR_DP_RESEND 0x10
#define SWD_PKT_REQ_ADDR_DP_SELECT 0x10
#define SWD_PKT_REQ_ADDR_DP_RDBUFF 0x18

#define SWD_PKT_REQ_DPRAC(x)       (SWD_PKT_REQ_START | SWD_PKT_REQ_DP | SWD_PKT_REQ_READ | (x))
#define SWD_PKT_REQ_DPWAC(x)	   (SWD_PKT_REQ_START | SWD_PKT_REQ_DP | SWD_PKT_REQ_WRITE | (x))
#define SWD_PKT_REQ_READ_IDCODE    SWD_PKT_REQ_DPRAC(SWD_PKT_REQ_ADDR_DP_IDCODE)
#define SWD_PKT_REQ_WRITE_ABORT    SWD_PKT_REQ_DPWAC(SWD_PKT_REQ_ADDR_DP_ABORT)
#define SWD_PKT_REQ_READ_CTLSTA    SWD_PKT_REQ_DPRAC(SWD_PKT_REQ_ADDR_DP_CTLSTA)
#define SWD_PKT_REQ_WRITE_CTLSTA   SWD_PKT_REQ_DPWAC(SWD_PKT_REQ_ADDR_DP_CTLSTA)
#define SWD_PKT_REQ_READ_WCR       SWD_PKT_REQ_DPRAC(SWD_PKT_REQ_ADDR_DP_WCR)
#define SWD_PKT_REQ_WRITE_WCR      SWD_PKT_REQ_DPWAC(SWD_PKT_REQ_ADDR_DP_WCR)
#define SWD_PKT_REQ_READ_RESEND    SWD_PKT_REQ_DPRAC(SWD_PKT_REQ_ADDR_DP_RESEND)
#define SWD_PKT_REQ_WRITE_SELECT   SWD_PKT_REQ_DPWAC(SWD_PKT_REQ_ADDR_DP_SELECT)
#define SWD_PKT_REQ_READ_RDBUFF    SWD_PKT_REQ_DPRAC(SWD_PKT_REQ_ADDR_DP_RDBUFF)

#define SWD_PKT_REQ_CTRL_SEL       1
#define SWD_PKT_REQ_WCR_SEL        0

#define SWD_AP_REG_VALUE_CFG       0
#define SWD_AP_REG_VALUE_BASE      0xE00FF003
#define SWD_AP_REG_VALUE_IDR       0x04770021

#define SOC_CHIP_ID_ADDR           0x4003402C
//#define SOC_CHIP_ID_VALUE          0x00821900
#define SOC_CHIP_ID_VALUE          0x00008215

#define SOC_ROM_TABLE_BASE         0xE00FF000
#define SOC_ROM_TABLE_ENTRY0_ADDR  (SOC_ROM_TABLE_BASE + 0x000)
#define SOC_ROM_TABLE_ENTRY1_ADDR  (SOC_ROM_TABLE_BASE + 0x004)
#define SOC_ROM_TABLE_ENTRY2_ADDR  (SOC_ROM_TABLE_BASE + 0x008)
#define SOC_ROM_TABLE_ENTRY3_ADDR  (SOC_ROM_TABLE_BASE + 0x00C)
#define SOC_ROM_TABLE_MEMTYPE_ADDR (SOC_ROM_TABLE_BASE + 0xFCC)
#define SOC_ROM_TABLE_PID0_ADDR    (SOC_ROM_TABLE_BASE + 0xFE0)
#define SOC_ROM_TABLE_PID1_ADDR    (SOC_ROM_TABLE_BASE + 0xFE4)
#define SOC_ROM_TABLE_PID2_ADDR    (SOC_ROM_TABLE_BASE + 0xFE8)
#define SOC_ROM_TABLE_PID3_ADDR    (SOC_ROM_TABLE_BASE + 0xFEC)
#define SOC_ROM_TABLE_PID4_ADDR    (SOC_ROM_TABLE_BASE + 0xFD0)
#define SOC_ROM_TABLE_PID5_ADDR    (SOC_ROM_TABLE_BASE + 0xFD4)
#define SOC_ROM_TABLE_PID6_ADDR    (SOC_ROM_TABLE_BASE + 0xFD8)
#define SOC_ROM_TABLE_PID7_ADDR    (SOC_ROM_TABLE_BASE + 0xFDC)
#define SOC_ROM_TABLE_CID0_ADDR    (SOC_ROM_TABLE_BASE + 0xFF0)
#define SOC_ROM_TABLE_CID1_ADDR    (SOC_ROM_TABLE_BASE + 0xFF4)
#define SOC_ROM_TABLE_CID2_ADDR    (SOC_ROM_TABLE_BASE + 0xFF8)
#define SOC_ROM_TABLE_CID3_ADDR    (SOC_ROM_TABLE_BASE + 0xFFC)

#define SOC_ROM_TABLE_ENTRY0_VALUE  0xfff0f003
#define SOC_ROM_TABLE_ENTRY1_VALUE  0xfff02003
#define SOC_ROM_TABLE_ENTRY2_VALUE  0xfff03003
#define SOC_ROM_TABLE_ENTRY3_VALUE  0
#define SOC_ROM_TABLE_MEMTYPE_VALUE 1
#define SOC_ROM_TABLE_PID0_VALUE    0x71
#define SOC_ROM_TABLE_PID1_VALUE    0xb4
#define SOC_ROM_TABLE_PID2_VALUE    0x0b
#define SOC_ROM_TABLE_PID3_VALUE    0
#define SOC_ROM_TABLE_PID4_VALUE    0x4
#define SOC_ROM_TABLE_PID5_VALUE    0
#define SOC_ROM_TABLE_PID6_VALUE    0
#define SOC_ROM_TABLE_PID7_VALUE    0
#define SOC_ROM_TABLE_CID0_VALUE    0xd
#define SOC_ROM_TABLE_CID1_VALUE    0x10
#define SOC_ROM_TABLE_CID2_VALUE    0x5
#define SOC_ROM_TABLE_CID3_VALUE    0xb1

#define CM0_DBG_SCS_BASE            0xE000E000
#define CM0_DBG_DWT_BASE            0xE0001000
#define CM0_DBG_BPU_BASE            0xE0002000

#define CM0_DBG_SCS_REG_DHCSR_ADDR  (CM0_DBG_SCS_BASE + 0xDF0)
#define CM0_DBG_SCS_REG_DCRSR_ADDR  (CM0_DBG_SCS_BASE + 0xDF4)
#define CM0_DBG_SCS_REG_DCRDR_ADDR  (CM0_DBG_SCS_BASE + 0xDF8)
#define CM0_DBG_SCS_REG_DEMCR_ADDR  (CM0_DBG_SCS_BASE + 0xDFC)
#define CM0_DBG_SCS_REG_SHCSR_ADDR  (CM0_DBG_SCS_BASE + 0xD24)
#define CM0_DBG_SCS_REG_DFSR_ADDR   (CM0_DBG_SCS_BASE + 0xD30)

#define CM0_DBG_CORE_REG_READ       0
#define CM0_DBG_CORE_REG_WRITE      1
#define CM0_DBG_REGSEL_R0           0
#define CM0_DBG_REGSEL_R1           1
#define CM0_DBG_REGSEL_R2           2
#define CM0_DBG_REGSEL_R3           3
#define CM0_DBG_REGSEL_R4           4
#define CM0_DBG_REGSEL_R5           5
#define CM0_DBG_REGSEL_R6           6
#define CM0_DBG_REGSEL_R7           7
#define CM0_DBG_REGSEL_R8           8
#define CM0_DBG_REGSEL_R9           9
#define CM0_DBG_REGSEL_R10          10
#define CM0_DBG_REGSEL_R11          11
#define CM0_DBG_REGSEL_R12          12
#define CM0_DBG_REGSEL_SP           13
#define CM0_DBG_REGSEL_LR           14
#define CM0_DBG_REGSEL_PC           15
#define CM0_DBG_REGSEL_XPSR         16
#define CM0_DBG_REGSEL_MSP          17
#define CM0_DBG_REGSEL_PSP          18
#define CM0_DBG_REGSEL_PRIMASK      20

#define CM0_DBG_DHCSR_WR_KEY        0xA05F0000

#define CM0_DBG_DWT_REG_CTRL_ADDR   (CM0_DBG_DWT_BASE + 0x000)
#define CM0_DBG_DWT_REG_PCSR_ADDR   (CM0_DBG_DWT_BASE + 0x01C)
#define CM0_DBG_DWT_REG_COMP0_ADDR  (CM0_DBG_DWT_BASE + 0x020)
#define CM0_DBG_DWT_REG_MASK0_ADDR  (CM0_DBG_DWT_BASE + 0x024)
#define CM0_DBG_DWT_REG_FUNC0_ADDR  (CM0_DBG_DWT_BASE + 0x028)
#define CM0_DBG_DWT_REG_COMP1_ADDR  (CM0_DBG_DWT_BASE + 0x030)
#define CM0_DBG_DWT_REG_MASK1_ADDR  (CM0_DBG_DWT_BASE + 0x034)
#define CM0_DBG_DWT_REG_FUNC1_ADDR  (CM0_DBG_DWT_BASE + 0x038)

#define CM0_DBG_DWT_FUNC_IADDR      0x4
#define CM0_DBG_DWT_FUNC_DADDR_RO   0x5
#define CM0_DBG_DWT_FUNC_DADDR_WO   0x6
#define CM0_DBG_DWT_FUNC_DADDR_RW   0x7

#define CM0_DBG_BPU_REG_CTRL_ADDR   (CM0_DBG_BPU_BASE + 0x000)
#define CM0_DBG_BPU_REG_COMP0_ADDR  (CM0_DBG_BPU_BASE + 0x008)
#define CM0_DBG_BPU_REG_COMP1_ADDR  (CM0_DBG_BPU_BASE + 0x00C)
#define CM0_DBG_BPU_REG_COMP2_ADDR  (CM0_DBG_BPU_BASE + 0x010)
#define CM0_DBG_BPU_REG_COMP3_ADDR  (CM0_DBG_BPU_BASE + 0x014)

#define CM0_SCB_REG_AIRCR_ADDR      0xE000ED0C
#define CM0_SCB_REG_AIRCR_WR_KEY    0x05FA0000

#define MAX_SWD_AP_PKT_SIZE         1024

#define SWD_DELAY1        __nop();__nop();__nop();__nop()
#define SWD_DELAY2        __nop();__nop();__nop();
#define SWD_DELAY3        __nop();__nop();
#define SWD_DELAY4        __nop();
#define SWD_DELAY5        

#define SWD_DELAY          SWD_DELAY4
/* P1.1 :SWDIO;  P1.4 :SWDCLK */
#define __set_swd_io_mask()       (LPC_GPIO1->FIOMASK = (~((1 << 1) | (1 << 4))))
#define __set_swd_io_as_out()    (LPC_GPIO1->FIODIR |= (1 << 1))
#define __set_swd_io_as_in()     (LPC_GPIO1->FIODIR &= (~(1 << 1)))
#define __set_swd_clk_as_out()   (LPC_GPIO1->FIODIR |= (1 << 4))
#define __set_swd_clk_as_in()    (LPC_GPIO1->FIODIR &= (~(1 << 4)))
#define __swd_idle()             {\
	                __set_swd_io_mask();\
                   __set_swd_io_as_out();\
                   __set_swd_clk_as_out();\
                   __set_swd_clk_to_low();\
                   __set_swd_io_to_low();\
                 }
#define __swd_init()             {\
                   LPC_PINCON->PINSEL2 &= (~((0x3 << 2) | (0x3 << 8)));\
                   __swd_idle();\
                 }

#define __set_swd_clk_to_high()  {\
                   LPC_GPIO1->FIOSET = (1 << 4);\
                 }

#define __set_swd_clk_to_low()   {\
                   LPC_GPIO1->FIOCLR = (1 << 4);\
                 }

#define __set_swd_io_to_high()   {\
                   LPC_GPIO1->FIOSET = (1 << 1);\
                 }

#define __set_swd_io_to_low()    {\
                   LPC_GPIO1->FIOCLR = (1 << 1);\
                 }

#define __set_swd_io(a)              {\
	                LPC_GPIO1->FIOPIN = (a);\
                 }
static  unsigned long __get_swd_io(void) __attribute__((always_inline));
static  unsigned long __get_swd_io(void) {
#ifndef CGF
  __set_swd_io_as_in();
#endif
  return ((LPC_GPIO1->FIOPIN >> 1) & 1);
}

void          swd_nop(unsigned long bit_len);
void          swd_drv_init(void);
void          swd_tx(const unsigned char * pBuf,unsigned long bit_len);
void          swd_tx8(const unsigned char * pBuf);
void          swd_tx32(const unsigned long * pBuf);
void          swd_rx(unsigned char * pBuf,unsigned long bit_len);
void          swd_rx8(unsigned char * pBuf);
void          swd_rx32(unsigned long * pBuf);
unsigned long swd_parity_cal(unsigned long data,unsigned long bit_len,unsigned long start_pos);
void          swd_pkt_kickoff(void);
unsigned long swd_pkt_req(unsigned char pkt);
unsigned long swd_line_reset(unsigned long * pID);
#ifndef CGF
void          swd_fault_clear(void);
#else
unsigned long swd_fault_clear(void);
#endif
unsigned long __swd_dp_wr_i(unsigned long pkt_req,unsigned long wdata);
#ifdef CGF
unsigned long __swd_dp_wr_i_mem(unsigned long pkt_req, unsigned long wdata);
unsigned long __swd_dp_rd_i_mem(unsigned long pkt_req, unsigned long * pRdata);
#endif
unsigned long __swd_dp_rd_i(unsigned long pkt_req,unsigned long * pRdata);
unsigned long swd_dp_wr(unsigned long pkt_req,unsigned long wdata);
#ifdef CGF
unsigned long swd_dp_wr_mem(unsigned long pkt_req, unsigned long wdata);
unsigned long swd_dp_rd_mem(unsigned long pkt_req, unsigned long * pRdata);
#endif
unsigned long swd_dp_rd(unsigned long pkt_req,unsigned long * pRdata);
//unsigned long swd_dp_id_get(unsigned long * pID);
//unsigned long swd_dp_select_set(unsigned long cs);
unsigned long swd_dp_ctrl_n_stat_get(unsigned long * pCns);
unsigned long swd_dp_ctrl_n_stat_set(unsigned long cns);
unsigned long swd_connect(void);
unsigned long swd_dap_detect(void);
unsigned long swd_dap_idle(void);
void          swd_ctrl_n_stat_prn(unsigned long cns);

#define       swd_dp_id_get(x)     swd_dp_rd(SWD_PKT_REQ_READ_IDCODE,(x))
#define  	  swd_dp_select_set(x) swd_dp_wr(SWD_PKT_REQ_WRITE_SELECT,(x))
#define       swd_dp_abort_set(x)  swd_dp_wr(SWD_PKT_REQ_WRITE_ABORT,(x))
#define       swd_dp_wcr_get(x)    swd_dp_rd(SWD_PKT_REQ_READ_WCR,(x))
#define       swd_dp_wcr_set(x)    swd_dp_wr(SWD_PKT_REQ_WRITE_WCR,(x))

unsigned long swd_apacc_init(unsigned long aps,unsigned long rbn);
unsigned long swd_ap_rd(unsigned long addr,unsigned long * pRdata);
unsigned long swd_ap_wr(unsigned long addr,unsigned long wdata);
#ifdef CGF
unsigned long swd_ap_wr_mem(unsigned long addr, unsigned long wdata);
unsigned long swd_ap_rd_mem(unsigned long addr, unsigned long * pRdata);
#endif

unsigned long swd_mem_rd(unsigned long addr,unsigned long * pBuf,unsigned long len,unsigned long size);
unsigned long swd_mem_wr(unsigned long addr,unsigned long * pBuf,unsigned long len,unsigned long size);
#ifdef CGF
unsigned long swd_mem_verf(unsigned long addr, unsigned long * pBuf, unsigned long len, unsigned long size);
unsigned long swd_mem_wr_mem(unsigned long addr, unsigned long * pBuf, unsigned long len, unsigned long size);
#endif
unsigned long swd_mem_rd_1k(unsigned long addr,unsigned long * pBuf,unsigned long len,unsigned long size);
unsigned long swd_mem_wr_1k(unsigned long addr,unsigned long * pBuf,unsigned long len,unsigned long size);
#ifdef CGF
unsigned long swd_mem_verf_1k(unsigned long addr, unsigned long * pBuf, unsigned long len, unsigned long size);
unsigned long swd_mem_wr_1k_mem(unsigned long addr, unsigned long * pBuf, unsigned long len, unsigned long size);
#endif
#define  swd_mem_rd32(x,y,z)  swd_mem_rd((x),(y),(z),2)
#define  swd_mem_rd16(x,y,z)  swd_mem_rd((x),((unsigned long *)(y)),(z),1)
#define  swd_mem_rd8(x,y,z)   swd_mem_rd((x),((unsigned long *)(y)),(z),0)
#define  swd_mem_wr32(x,y,z)  swd_mem_wr((x),(y),(z),2)
#define  swd_mem_wr16(x,y,z)  swd_mem_wr((x),((unsigned long *)(y)),(z),1)
#define  swd_mem_wr8(x,y,z)   swd_mem_wr((x),((unsigned long *)(y)),(z),0)
#ifdef CGF
#define  swd_mem_verf32(x,y,z)  swd_mem_verf((x),(y),(z),2)
#define  swd_mem_verf16(x,y,z)  swd_mem_verf((x),((unsigned long *)(y)),(z),1)
#define  swd_mem_verf8(x,y,z)   swd_mem_verf((x),((unsigned long *)(y)),(z),0)
#define  swd_mem_wr32_mem(x,y,z)  swd_mem_wr_mem((x),(y),(z),2)
#define  swd_mem_wr16_mem(x,y,z)  swd_mem_wr_mem((x),((unsigned long *)(y)),(z),1)
#define  swd_mem_wr8_mem(x,y,z)   swd_mem_wr_mem((x),((unsigned long *)(y)),(z),0)
#endif
void delay(unsigned long dly);

unsigned long swd_core_reg_rd(unsigned long rs,unsigned long * pRdata);
unsigned long swd_core_reg_wr(unsigned long rs,unsigned long wdata);
#define  swd_core_r0_rd(x)         swd_core_reg_rd(CM0_DBG_REGSEL_R0,(x));
#define  swd_core_r1_rd(x)         swd_core_reg_rd(CM0_DBG_REGSEL_R1,(x));
#define  swd_core_r2_rd(x)         swd_core_reg_rd(CM0_DBG_REGSEL_R2,(x));
#define  swd_core_r3_rd(x)         swd_core_reg_rd(CM0_DBG_REGSEL_R3,(x));
#define  swd_core_r4_rd(x)         swd_core_reg_rd(CM0_DBG_REGSEL_R4,(x));
#define  swd_core_r5_rd(x)         swd_core_reg_rd(CM0_DBG_REGSEL_R5,(x));
#define  swd_core_r6_rd(x)         swd_core_reg_rd(CM0_DBG_REGSEL_R6,(x));
#define  swd_core_r7_rd(x)         swd_core_reg_rd(CM0_DBG_REGSEL_R7,(x));
#define  swd_core_r8_rd(x)         swd_core_reg_rd(CM0_DBG_REGSEL_R8,(x));
#define  swd_core_r9_rd(x)         swd_core_reg_rd(CM0_DBG_REGSEL_R9,(x));
#define  swd_core_r10_rd(x)        swd_core_reg_rd(CM0_DBG_REGSEL_R10,(x));
#define  swd_core_r11_rd(x)        swd_core_reg_rd(CM0_DBG_REGSEL_R11,(x));
#define  swd_core_r12_rd(x)        swd_core_reg_rd(CM0_DBG_REGSEL_R12,(x));
#define  swd_core_sp_rd(x)         swd_core_reg_rd(CM0_DBG_REGSEL_SP,(x));
#define  swd_core_pc_rd(x)         swd_core_reg_rd(CM0_DBG_REGSEL_PC,(x));
#define  swd_core_xpsr_rd(x)       swd_core_reg_rd(CM0_DBG_REGSEL_XPSR,(x));
#define  swd_core_msp_rd(x)        swd_core_reg_rd(CM0_DBG_REGSEL_MSP,(x));
#define  swd_core_psp_rd(x)        swd_core_reg_rd(CM0_DBG_REGSEL_PSP,(x));
#define  swd_core_primask_rd(x)    swd_core_reg_rd(CM0_DBG_REGSEL_PRIMASK,(x));
#define  swd_core_r0_wr(x)         swd_core_reg_wr(CM0_DBG_REGSEL_R0,(x));
#define  swd_core_r1_wr(x)         swd_core_reg_wr(CM0_DBG_REGSEL_R1,(x));
#define  swd_core_r2_wr(x)         swd_core_reg_wr(CM0_DBG_REGSEL_R2,(x));
#define  swd_core_r3_wr(x)         swd_core_reg_wr(CM0_DBG_REGSEL_R3,(x));
#define  swd_core_r4_wr(x)         swd_core_reg_wr(CM0_DBG_REGSEL_R4,(x));
#define  swd_core_r5_wr(x)         swd_core_reg_wr(CM0_DBG_REGSEL_R5,(x));
#define  swd_core_r6_wr(x)         swd_core_reg_wr(CM0_DBG_REGSEL_R6,(x));
#define  swd_core_r7_wr(x)         swd_core_reg_wr(CM0_DBG_REGSEL_R7,(x));
#define  swd_core_r8_wr(x)         swd_core_reg_wr(CM0_DBG_REGSEL_R8,(x));
#define  swd_core_r9_wr(x)         swd_core_reg_wr(CM0_DBG_REGSEL_R9,(x));
#define  swd_core_r10_wr(x)        swd_core_reg_wr(CM0_DBG_REGSEL_R10,(x));
#define  swd_core_r11_wr(x)        swd_core_reg_wr(CM0_DBG_REGSEL_R11,(x));
#define  swd_core_r12_wr(x)        swd_core_reg_wr(CM0_DBG_REGSEL_R12,(x));
#define  swd_core_sp_wr(x)         swd_core_reg_wr(CM0_DBG_REGSEL_SP,(x));
#define  swd_core_pc_wr(x)         swd_core_reg_wr(CM0_DBG_REGSEL_PC,(x));
#define  swd_core_xpsr_wr(x)       swd_core_reg_wr(CM0_DBG_REGSEL_XPSR,(x));
#define  swd_core_msp_wr(x)        swd_core_reg_wr(CM0_DBG_REGSEL_MSP,(x));
#define  swd_core_psp_wr(x)        swd_core_reg_wr(CM0_DBG_REGSEL_PSP,(x));
#define  swd_core_primask_wr(x)    swd_core_reg_wr(CM0_DBG_REGSEL_PRIMASK,(x));

unsigned long swd_halted_chk(void);
unsigned long swd_break_chk(void);
unsigned long swd_watch_chk(void);
unsigned long swd_vector_capture_chk(void);
unsigned long swd_ext_dbg_req_chk(void);
unsigned long swd_svc_chk(void);
unsigned long swd_dwt_enable(void);
unsigned long swd_hardfault_vc_enable(void);
unsigned long swd_reset_vc_enable(void);
unsigned long swd_dwt_disable(void);
unsigned long swd_hardfault_vc_disable(void);
unsigned long swd_reset_vc_disable(void);
unsigned long swd_dwt_en_chk(void);
unsigned long swd_hardfault_vc_en_chk(void);
unsigned long swd_reset_vc_en_chk(void);
unsigned long swd_debug_enable(void);
unsigned long swd_debug_disable(void);
#ifdef CGF
unsigned long swd_debug_haltexit(void) ;
#endif
unsigned long swd_debug_en_chk(void);
unsigned long swd_debug_halt(void);
unsigned long swd_debug_step(void);
unsigned long swd_sregrdy_chk(void);
unsigned long swd_debug_state_chk(void);
unsigned long swd_sleep_chk(void);
unsigned long swd_lockup_chk(void);
unsigned long swd_retire_st_chk(void);
unsigned long swd_reset_st_chk(void);

typedef struct{
  unsigned long comp;
  unsigned long mask;
  unsigned long func;
}s_swd_dwt, * ps_swd_dwt;

unsigned long swd_dwt_numcomp_get(unsigned long * pNumComp);
#define  swd_dwt_eia_sample_get(x)  swd_mem_rd32(CM0_DBG_DWT_REG_PCSR_ADDR,(x),1);
unsigned long swd_dwt_match_chk(void);
unsigned long swd_dwt_set(unsigned long cmp_idx,ps_swd_dwt pDwt);
unsigned long swd_dwt_get(unsigned long cmp_idx,ps_swd_dwt pDwt);

unsigned long swd_bpu_numcode_get(unsigned long * pNumCode);
unsigned long swd_bpu_enable(void);
unsigned long swd_bpu_disable(void);
unsigned long swd_bpu_en_chk(void);
unsigned long swd_bpu_set(unsigned long cmp_idx,unsigned long cmp_addr);
unsigned long swd_bpu_get(unsigned long cmp_idx,unsigned long * cmp_addr);

unsigned long swd_debug_reset(void);

#endif
