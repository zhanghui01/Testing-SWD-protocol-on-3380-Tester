;/*****************************************************************************
; * @file:    startup_RN8215.s
; * @purpose: CMSIS Cortex-M0 Core Device Startup File
; *           for RN8215
; * @version: V1.00
; * @date:    2012/12/3
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; *
; *
; *****************************************************************************/

; res: 0x100027fc
; chip:0x100027f8

SRAM_BASE_ADDR    EQU   0x00000000

SRAM_RESULT_ADDR  EQU   (SRAM_BASE_ADDR + 0x37f8)
SRAM_CHIPID_ADDR  EQU   (SRAM_BASE_ADDR + 0x27f8)

TEST_FAIL         EQU   0x12345678
TEST_PASS         EQU   0x98765432

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
Stack_Size      EQU     0x00000100
UNUSEDIRQ       EQU     0
UNUSEDUART      EQU     0
                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset
                        
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler
                
                DCD     0
                DCD     CMP_HANDLER
                DCD     VCH_HANDLER
                DCD     RTC_HANDLER
                DCD     EMU_HANDLER
                DCD     MADC_HANDLER
                DCD     UART0_HANDLER
                DCD     UART1_HANDLER
                DCD     UART2_HANDLER
                DCD     UART3_HANDLER
                DCD     SPI_HANDLER
                DCD     I2C_HANDLER   
                DCD     ISO78160_HANDLER
                DCD     ISO78161_HANDLER
                DCD     TC0_HANDLER
                DCD     TC1_HANDLER
                DCD     0
                DCD     UART5_HANDLER
                DCD     WDT_HANDLER
                DCD     KBI_HANDLER
                DCD     LCD_HANDLER
                DCD     CT_HANDLER
                DCD     DMA_HANDLER
                DCD     0
                DCD     EXT0_HANDLER
                DCD     0
                DCD     EXT2_HANDLER
                DCD     0
                DCD     0
                DCD     0
                DCD     EXT6_HANDLER
                DCD     EXT7_HANDLER
                
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors


;=================================================================
; MarchX: (D) W0 -H- (U) R0W0W1R1 -H- (U) R1W1W0R0 (D) R0
;             M0         M1               M2           M3
;=================================================================
                AREA    |.text|, CODE, READONLY
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;  M0(W0) = write R2 as decreasing addressing sequence(from R1 to R0)
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
MARCHX_M0Proc  PROC
               MOVS    R4, R1
			   
m0_loop
			   SUBS    R4, R4, #0x4
			   STR     R2, [R4, #0x0]
			   CMP     R4, R0
			   BNE     m0_loop
               BX      LR
               ENDP

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;  M1/M2(R0W0W1R1) = read-compare and write with R2, write and read-compare with R3 as increasing addressing sequence(from R0 to R1)
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
MARCHX_M12Proc  PROC
               MOVS    R4, R0
			   
m12_loop
               LDR     R5, [R4, #0x0]
			   CMP     R5, R2
			   BEQ     m12_w0
			   BL      exit_err
m12_w0
			   STR     R2, [R4, #0x0]
			   STR     R3, [R4, #0x0]
			   LDR     R5, [R4, #0x0]
			   CMP     R5, R3
			   BEQ     m12_end
			   BL      exit_err
m12_end
			   ADDS    R4, R4, #0x4
			   CMP     R4, R1
			   BNE     m12_loop
               BX      LR
               ENDP
			   
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;  M3(R0) = read-compare R2 as decreasing addressing sequence(from R1 to R0)
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
MARCHX_M3Proc  PROC
               MOVS    R4, R1
			   
m34_loop
               SUBS    R4, R4, #0x4
               LDR     R5, [R4, #0x0]
			   CMP     R5, R2
			   BEQ     m34_end
			   BL      exit_err
m34_end
			   CMP     R4, R0
			   BNE     m34_loop
               BX      LR
               ENDP

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; MARCHX HOLD DELAY
; R2 = 0x10000, means 11ms
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
MARCHX_DELAY   PROC
               LDR     R2, =0x50000
			   LDR     R3, =0x0
marchx_delay_loop
			   ADDS    R3, R3, #0x1
			   CMP     R2, R3
			   BNE     marchx_delay_loop
			   BX      LR
               ENDP



;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; MarchX: (D) W0 -H- (U) R0W0W1R1 -H- (U) R1W1W0R0 (D) R0
;             M0         M1               M2           M3
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
MARCHX_PROC    PROC
               MOV     R7, LR
			   
               LDR    R2, =0x00000000
			   BL     MARCHX_M0Proc
               LDR    R2, =0x00000000
			   LDR    R3, =0xffffffff
			   BL     MARCHX_M12Proc
			   BL     MARCHX_DELAY
               LDR    R2, =0xffffffff
			   LDR    R3, =0x00000000
			   BL     MARCHX_M12Proc
			   BL     MARCHX_DELAY
               LDR    R2, =0x00000000
			   BL     MARCHX_M3Proc
			   
			   
               LDR    R2, =0xffffffff
			   BL     MARCHX_M0Proc
               LDR    R2, =0xffffffff
			   LDR    R3, =0x00000000
			   BL     MARCHX_M12Proc
			   BL     MARCHX_DELAY
               LDR    R2, =0x00000000
			   LDR    R3, =0xffffffff
			   BL     MARCHX_M12Proc
			   BL     MARCHX_DELAY
               LDR    R2, =0xffffffff
			   BL     MARCHX_M3Proc
			   
			   MOV     LR, R7
			   BX      LR
               ENDP



;=================================================================
; MarchC-:(D) W0 (U) R0W1 (U) R1W0 (D) R0W1 (D) R1W0 (D) R0
;             M0     M1       M2       M3       M4       M5
;=================================================================
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;  M0(W0) = write R2 as decreasing addressing sequence(from R1 to R0)
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
MARCHC_M0Proc  PROC
               MOVS    R4, R1
			   
marchc_m0proc_loop
			   SUBS    R4, R4, #0x4
			   STR     R2, [R4, #0x0]
			   CMP     R4, R0
			   BNE     marchc_m0proc_loop
               BX      LR
               ENDP

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;  M1/M2(R0W1) = read-compare with R2 and write from R3 as increasing addressing sequence(from R0 to R1)
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
MARCHC_M12Proc  PROC
               MOVS    R4, R0
			   
marchc_m12proc_loop
               LDR     R5, [R4, #0x0]
			   CMP     R5, R2
			   BEQ     marchc_m12proc_w0
			   BL      exit_err
marchc_m12proc_w0
			   STR     R3, [R4, #0x0]
			   ADDS    R4, R4, #0x4
			   CMP     R4, R1
			   BNE     marchc_m12proc_loop
               BX      LR
               ENDP

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;  M3/M4(R0W1) = read-compare with R2 and write from R3 as decreasing addressing sequence(from R1 to R0)
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
MARCHC_M34Proc  PROC
               MOVS    R4, R1
			   
marchc_m34proc_loop
               SUBS    R4, R4, #0x4
               LDR     R5, [R4, #0x0]
			   CMP     R5, R2
			   BEQ     marchc_m34proc_w1
			   BL      exit_err
marchc_m34proc_w1
			   STR     R3, [R4, #0x0]
			   CMP     R4, R0
			   BNE     marchc_m34proc_loop
               BX      LR
               ENDP
			   
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;  M5(R0) = read-compare R2 as decreasing addressing sequence(from R1 to R0)
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
MARCHC_M5Proc  PROC
               MOVS    R4, R1
			   
marchc_m5proc_loop
               SUBS    R4, R4, #0x4
               LDR     R5, [R4, #0x0]
			   CMP     R5, R2
			   BEQ     marchc_m5proc_end
			   BL      exit_err
marchc_m5proc_end
			   CMP     R4, R0
			   BNE     marchc_m5proc_loop
               BX      LR
               ENDP
			   
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; MarchC-:(D) W0 (U) R0W1 (U) R1W0 (D) R0W1 (D) R1W0 (D) R0
;             M0     M1       M2       M3       M4       M5
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
MARCHC_PROC    PROC
               MOV     R7, LR
			   
               LDR    R2, =0x00000000
			   BL     MARCHC_M0Proc
               LDR    R2, =0x00000000
			   LDR    R3, =0xffffffff
			   BL     MARCHC_M12Proc
               LDR    R2, =0xffffffff
			   LDR    R3, =0x00000000
			   BL     MARCHC_M12Proc
               LDR    R2, =0x00000000
			   LDR    R3, =0xffffffff
			   BL     MARCHC_M34Proc
               LDR    R2, =0xffffffff
			   LDR    R3, =0x0
			   BL     MARCHC_M34Proc
               LDR    R2, =0x00000000
			   BL     MARCHC_M5Proc
			   
               ;2. DB1:0x55555555 & 0xaaaaaaaa		
			   LDR   R2, =0x55555555
			   BL     MARCHC_M0Proc
			   LDR   R2, =0x55555555
			   LDR   R3, =0xaaaaaaaa
			   BL     MARCHC_M12Proc
			   LDR   R2, =0xaaaaaaaa
			   LDR   R3, =0x55555555
			   BL     MARCHC_M12Proc
			   LDR   R2, =0x55555555
			   LDR   R3, =0xaaaaaaaa
			   BL     MARCHC_M34Proc
			   LDR   R2, =0xaaaaaaaa
			   LDR   R3, =0x55555555
			   BL     MARCHC_M34Proc
			   LDR   R2, =0x55555555
			   BL     MARCHC_M5Proc
			   
			   ;3. DB2:0x33333333 & 0xcccccccc
			   LDR   R2, =0x33333333
			   BL     MARCHC_M0Proc
			   LDR   R2, =0x33333333
			   LDR   R3, =0xcccccccc
			   BL     MARCHC_M12Proc
			   LDR   R2, =0xcccccccc
			   LDR   R3, =0x33333333
			   BL     MARCHC_M12Proc
			   LDR   R2, =0x33333333
			   LDR   R3, =0xcccccccc
			   BL     MARCHC_M34Proc
			   LDR   R2, =0xcccccccc
			   LDR   R3, =0x33333333
			   BL     MARCHC_M34Proc
			   LDR   R2, =0x33333333
			   BL     MARCHC_M5Proc
			   
               ;4. DB3:0x0f0f0f0f & 0xf0f0f0f0
			   LDR   R2, =0x0f0f0f0f
			   BL     MARCHC_M0Proc
			   LDR   R2, =0x0f0f0f0f
			   LDR   R3, =0xf0f0f0f0
			   BL     MARCHC_M12Proc
			   LDR   R2, =0xf0f0f0f0
			   LDR   R3, =0x0f0f0f0f
			   BL     MARCHC_M12Proc
			   LDR   R2, =0x0f0f0f0f
			   LDR   R3, =0xf0f0f0f0
			   BL     MARCHC_M34Proc
			   LDR   R2, =0xf0f0f0f0
			   LDR   R3, =0x0f0f0f0f
			   BL     MARCHC_M34Proc
			   LDR   R2, =0x0f0f0f0f
			   BL     MARCHC_M5Proc
			   
               ;5. DB4:0x00ff00ff & 0xff00ff00
			   LDR   R2, =0x00ff00ff
			   BL     MARCHC_M0Proc
			   LDR   R2, =0x00ff00ff
			   LDR   R3, =0xff00ff00
			   BL     MARCHC_M12Proc
			   LDR   R2, =0xff00ff00
			   LDR   R3, =0x00ff00ff
			   BL     MARCHC_M12Proc
			   LDR   R2, =0x00ff00ff
			   LDR   R3, =0xff00ff00
			   BL     MARCHC_M34Proc
			   LDR   R2, =0xff00ff00
			   LDR   R3, =0x00ff00ff
			   BL     MARCHC_M34Proc
			   LDR   R2, =0x00ff00ff
			   BL     MARCHC_M5Proc
			   
               ;6. DB5:0x0000ffff & 0xffff0000
			   LDR   R2, =0x0000ffff
			   BL     MARCHC_M0Proc
			   LDR   R2, =0x0000ffff
			   LDR   R3, =0xffff0000
			   BL     MARCHC_M12Proc
			   LDR   R2, =0xffff0000
			   LDR   R3, =0x0000ffff
			   BL     MARCHC_M12Proc
			   LDR   R2, =0x0000ffff
			   LDR   R3, =0xffff0000
			   BL     MARCHC_M34Proc
			   LDR   R2, =0xffff0000
			   LDR   R3, =0x0000ffff
			   BL     MARCHC_M34Proc
			   LDR   R2, =0x0000ffff
			   BL     MARCHC_M5Proc
			   
			   ;7. DB6:0xffffffff & 0x00000000
			   
               LDR    R2, =0xffffffff
			   BL     MARCHC_M0Proc
               LDR    R2, =0xffffffff
			   LDR    R3, =0x00000000
			   BL     MARCHC_M12Proc
               LDR    R2, =0x00000000
			   LDR    R3, =0xffffffff
			   BL     MARCHC_M12Proc
               LDR    R2, =0xffffffff
			   LDR    R3, =0x00000000
			   BL     MARCHC_M34Proc
               LDR    R2, =0x00000000
			   LDR    R3, =0xffffffff
			   BL     MARCHC_M34Proc
               LDR    R2, =0xffffffff
			   BL     MARCHC_M5Proc
			   
			   
			   
			   MOV     LR, R7
			   BX      LR
               ENDP			   
;
;1. DB0:0x00000000 & 0xffffffff
;2. DB1:0x55555555 & 0xaaaaaaaa
;3. DB2:0x33333333 & 0xcccccccc
;4. DB3:0x0f0f0f0f & 0xf0f0f0f0
;5. DB4:0x00ff00ff & 0xff00ff00
;6. DB5:0x0000ffff & 0xffff0000
;
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
				
				;{P12,P11,P10}=000
				; SYSCTL->SYS_PS = 0x82
				; SYSCTL->OSC_CTRL2 = 0x13
				; SYSCTL->MOD1_EN = 0x220
				; GPIO->PMA = 0xfffff8ff  ;P10,11,12
				; GPIO->PA = 0x0;
				; GPIO->PCA1 = 0xf000;
				
				LDR   R0, =0x40034000
				LDR   R1, =0x82
				STR   R1, [R0, #0x30]
				LDR   R1, =0x13
				STR   R1, [R0, #0x10]
				LDR   R1, =0x220
				STR   R1, [R0, #0x20]
				
				LDR   R0, =0x50000000
				LDR   R1, =0xfffff8ff
				STR   R1, [R0, #0x00]
				LDR   R1, =0x0
				STR   R1, [R0, #0x04]
				LDR   R1, =0xf000
				STR   R1, [R0, #0x0c]
				
				; SYSCTL->OSC_CTRL1 = 0x0
				; SYSCTL->SYS_MODE = 0xD
				LDR   R0, =0x40034000
				
				;wait hosc flag
				LDR   R2, =0x8000
wait_hosc
				LDR   R1, [R0, #0]
				ANDS  R1, R1, R2
				CMP   R1, R2
				BNE   wait_hosc
				
				LDR   R1, =0x0
				STR   R1, [R0, #0x0]
				LDR   R1, =0xD
				STR   R1, [R0, #0x4]
				
				;A. test 0x2000~0x2800
				LDR   R0, =(SRAM_BASE_ADDR + 0x2000)  ;start address
				LDR   R1, =(SRAM_BASE_ADDR + 0x4000)   ;end address
                BL    MARCHC_PROC
                BL    MARCHX_PROC
				
				;B. copy code from 0x0 to 0x2000
				LDR   R0, =(SRAM_BASE_ADDR + 0)
				LDR   R1, =2048    ;code size=2kB
				LDR   R2, =(SRAM_BASE_ADDR + 0x2000)
				LDR   R3, =0x0
copy_code
				LDR   R4, [R0, R3]
				STR   R4, [R2, R3]
				ADDS  R3, #0x4
				CMP   R3, R1
				BNE   copy_code
				
				;C. test 0x0~0x2000
				LDR   R0, =ram_top
				LDR   R1, =0x2000
				ADDS  R0, R0, R1    ;run into high part of sram
				BX    R0
ram_top
				LDR   R0, =(SRAM_BASE_ADDR + 0x0000)  ;start address
				LDR   R1, =(SRAM_BASE_ADDR + 0x2000)  ;size	
                BL    MARCHC_PROC
                BL    MARCHX_PROC
                ;============================================
							
				LDR   R0, =SRAM_RESULT_ADDR
				LDR   R1, =TEST_PASS
				STR   R1, [R0, #0x00]
				B     exit_pass
exit_err
				LDR   R0, =SRAM_RESULT_ADDR
				LDR   R1, =TEST_FAIL
				STR   R1, [R0, #0x00]
exit_pass
                LDR   R0, =0x40030000
				LDR   R1, =0xbb
				STR   R1, [R0, #0x0]
                B     exit_pass
				ENDP				
				

; Dummy Exception Handlers (infinite loops which can be modified)

;                IF UNUSEDIRQ <> 1
NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP		                  
EXT0_HANDLER    PROC                              
                EXPORT  EXT0_HANDLER			  [WEAK]
                B       .
                ENDP
EXT2_HANDLER    PROC							  
                EXPORT  EXT2_HANDLER			  [WEAK]
                B       .
                ENDP
EXT6_HANDLER    PROC							  
                EXPORT  EXT6_HANDLER			  [WEAK]
                B       .
                ENDP
EXT7_HANDLER    PROC							  
                EXPORT  EXT7_HANDLER			  [WEAK]
                B       .
                ENDP
DMA_HANDLER     PROC							  
                EXPORT  DMA_HANDLER				  [WEAK]
                B       .
                ENDP   
CT_HANDLER      PROC							  
                EXPORT  CT_HANDLER				  [WEAK]
                B       .
                ENDP                             
RTC_HANDLER     PROC							  
                EXPORT  RTC_HANDLER				  [WEAK]
                B       .
                ENDP
EMU_HANDLER     PROC							  
                EXPORT  EMU_HANDLER				  [WEAK]
                B       .
                ENDP
MADC_HANDLER    PROC							  
                EXPORT  MADC_HANDLER			  [WEAK]
                B       .
                ENDP
UART0_HANDLER    PROC							  
                EXPORT  UART0_HANDLER			  [WEAK]
                B       .
                ENDP
UART1_HANDLER   PROC                              
                EXPORT  UART1_HANDLER			  [WEAK]
                B       .
                ENDP
UART2_HANDLER   PROC							  
                EXPORT  UART2_HANDLER			  [WEAK]
                B       .
                ENDP
UART3_HANDLER   PROC							  
                EXPORT  UART3_HANDLER			  [WEAK]
                B       .
                ENDP
SPI_HANDLER     PROC							  
                EXPORT  SPI_HANDLER				  [WEAK]
                B       .
                ENDP
I2C_HANDLER     PROC							  
                EXPORT  I2C_HANDLER				  [WEAK]
                B       .
                ENDP
ISO78161_HANDLER    PROC						  
                EXPORT  ISO78161_HANDLER		  [WEAK]
                B       .
                ENDP
ISO78160_HANDLER    PROC						  
                EXPORT  ISO78160_HANDLER		  [WEAK]
                B       .
                ENDP
TC0_HANDLER     PROC							  
                EXPORT  TC0_HANDLER				  [WEAK]
                B       .
                ENDP
TC1_HANDLER     PROC 							  
                EXPORT  TC1_HANDLER				  [WEAK]
                B       .
                ENDP
UART5_HANDLER     PROC							  
                EXPORT  UART5_HANDLER				  [WEAK]
                B       .
                ENDP
WDT_HANDLER     PROC							  
                EXPORT  WDT_HANDLER				  [WEAK]
                B       .
                ENDP
KBI_HANDLER     PROC							  
                EXPORT  KBI_HANDLER				  [WEAK]
                B       .
                ENDP
LCD_HANDLER     PROC							  
                EXPORT  LCD_HANDLER				  [WEAK]
                B       .
                ENDP    
VCH_HANDLER     PROC							  
                EXPORT  VCH_HANDLER				  [WEAK]
                B       .
                ENDP                
CMP_HANDLER   PROC							  
                EXPORT  CMP_HANDLER			    [WEAK]
                B       .
                ENDP
                
                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
