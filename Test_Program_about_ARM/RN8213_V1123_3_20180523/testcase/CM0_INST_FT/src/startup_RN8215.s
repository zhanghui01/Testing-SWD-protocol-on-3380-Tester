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

SRAM_BASE_ADDR    EQU   0x00000000
SRAM_RESULT_ADDR  EQU   (SRAM_BASE_ADDR + 0x37f8)
SRAM_CHIPID_ADDR  EQU   (SRAM_BASE_ADDR + 0x37f0)

TEST_FAIL         EQU   0x12345670
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
                AREA    |.text|, CODE, READONLY


ADD_CARRY       PROC                
				        MOV   R1 , R0
				        MOV   R2 , R0
				        MOV   R3 , R0
				        MOV   R4 , R0
				        MOV   R5 , R0
				        MOV   R6 , R0
				        MOV   R7 , R0
				        MOV   R8 , R0
				        MOV   R9 , R0
				        MOV   R10, R0
				        MOV   R11, R0
				        MOV   R12, R0
				        
				        ADD   R0 , R0 , R0 
				        ADD   R1 , R1 , R1 
				        ADD   R2 , R2 , R2 
				        ADD   R3 , R3 , R3 
				        ADD   R4 , R4 , R4 
				        ADD   R5 , R5 , R5 
				        ADD   R6 , R6 , R6 
				        ADD   R7 , R7 , R7 
				        ADD   R8 , R8 , R8 
				        ADD   R9 , R9 , R9 
				        ADD   R10, R10, R10
				        ADD   R11, R11, R11
				        ADD   R12, R12, R12
				        
				        CMP   R1, R0
				        BNE   add_carry_fail
				        CMP   R2, R0
				        BNE   add_carry_fail
				        CMP   R3, R0
				        BNE   add_carry_fail
				        CMP   R4, R0
				        BNE   add_carry_fail
				        CMP   R5, R0
				        BNE   add_carry_fail
				        CMP   R6, R0
				        BNE   add_carry_fail
				        CMP   R7, R0
				        BNE   add_carry_fail
				        CMP   R8, R0
				        BNE   add_carry_fail
				        CMP   R9, R0
				        BNE   add_carry_fail
				        CMP   R10, R0
				        BNE   add_carry_fail
				        CMP   R11, R0
				        BNE   add_carry_fail
				        CMP   R12, R0
				        BNE   add_carry_fail
				        BX    LR
add_carry_fail
                BL    exit_err
                ENDP
               
MUL_CARRY       PROC                
				        MOV   R1 , R0
				        MOV   R2 , R0
				        MOV   R3 , R0
				        MOV   R4 , R0
				        MOV   R5 , R0
				        MOV   R6 , R0
				        MOV   R7 , R0
				        MOVS  R0 , #0x2
				        MOV   R8 , R1
				        
				        MULS  R1 , R0 , R1 
				        MULS  R2 , R0 , R2 
				        MULS  R3 , R0 , R3 
				        MULS  R4 , R0 , R4 
				        MULS  R5 , R0 , R5 
				        MULS  R6 , R0 , R6 
				        MULS  R7 , R0 , R7 
				        
				        ADD   R8, R8, R8
				        CMP   R1, R8
				        BNE   mul_carry_fail
				        CMP   R2, R8
				        BNE   mul_carry_fail
				        CMP   R3, R8
				        BNE   mul_carry_fail
				        CMP   R4, R8
				        BNE   mul_carry_fail
				        CMP   R5, R8
				        BNE   mul_carry_fail
				        CMP   R6, R8
				        BNE   mul_carry_fail
				        CMP   R7, R8
				        BNE   mul_carry_fail
				        
				        BX    LR
mul_carry_fail
                BL    exit_err
                ENDP
                
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
				
				
				;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
				; INST test
				;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
				; 1. Addition
				LDR   R0, =0x0
				BL    ADD_CARRY
				LDR   R0, =0x1
				BL    ADD_CARRY
				LDR   R0, =0x2
				BL    ADD_CARRY
				LDR   R0, =0x4
				BL    ADD_CARRY
				LDR   R0, =0x6
				BL    ADD_CARRY
				LDR   R0, =0x8
				BL    ADD_CARRY
				LDR   R0, =0x10
				BL    ADD_CARRY
				LDR   R0, =0x20
				BL    ADD_CARRY
				LDR   R0, =0x40
				BL    ADD_CARRY
				LDR   R0, =0x80
				BL    ADD_CARRY
				LDR   R0, =0x100
				BL    ADD_CARRY
				LDR   R0, =0x200
				BL    ADD_CARRY
				LDR   R0, =0x400
				BL    ADD_CARRY
				LDR   R0, =0x800
				BL    ADD_CARRY
				LDR   R0, =0x1000
				BL    ADD_CARRY
				LDR   R0, =0x2000
				BL    ADD_CARRY
				LDR   R0, =0x4000
				BL    ADD_CARRY
				LDR   R0, =0x8000
				BL    ADD_CARRY
				LDR   R0, =0x10000
				BL    ADD_CARRY
				LDR   R0, =0x20000
				BL    ADD_CARRY
				LDR   R0, =0x40000
				BL    ADD_CARRY
				LDR   R0, =0x80000
				BL    ADD_CARRY
				LDR   R0, =0x100000
				BL    ADD_CARRY
				LDR   R0, =0x200000
				BL    ADD_CARRY
				LDR   R0, =0x400000
				BL    ADD_CARRY
				LDR   R0, =0x800000
				BL    ADD_CARRY
				LDR   R0, =0x1000000
				BL    ADD_CARRY
				LDR   R0, =0x2000000
				BL    ADD_CARRY
				LDR   R0, =0x4000000
				BL    ADD_CARRY
				LDR   R0, =0x8000000
				BL    ADD_CARRY
				LDR   R0, =0x10000000
				BL    ADD_CARRY
				LDR   R0, =0x20000000
				BL    ADD_CARRY
				LDR   R0, =0x40000000
				BL    ADD_CARRY
				LDR   R0, =0x80000000
				BL    ADD_CARRY
				
				; 2. Multiply
				LDR   R0, =0x1
				BL    MUL_CARRY
				LDR   R0, =0x2
				BL    MUL_CARRY
				LDR   R0, =0x4
				BL    MUL_CARRY
				LDR   R0, =0x6
				BL    MUL_CARRY
				LDR   R0, =0x8
				BL    MUL_CARRY
				LDR   R0, =0x10
				BL    MUL_CARRY
				LDR   R0, =0x20
				BL    MUL_CARRY
				LDR   R0, =0x40
				BL    MUL_CARRY
				LDR   R0, =0x80
				BL    MUL_CARRY
				LDR   R0, =0x100
				BL    MUL_CARRY
				LDR   R0, =0x200
				BL    MUL_CARRY
				LDR   R0, =0x400
				BL    MUL_CARRY
				LDR   R0, =0x800
				BL    MUL_CARRY
				LDR   R0, =0x1000
				BL    MUL_CARRY
				LDR   R0, =0x2000
				BL    MUL_CARRY
				LDR   R0, =0x4000
				BL    MUL_CARRY
				LDR   R0, =0x8000
				BL    MUL_CARRY
				LDR   R0, =0x10000
				BL    MUL_CARRY
				LDR   R0, =0x20000
				BL    MUL_CARRY
				LDR   R0, =0x40000
				BL    MUL_CARRY
				LDR   R0, =0x80000
				BL    MUL_CARRY
				LDR   R0, =0x100000
				BL    MUL_CARRY
				LDR   R0, =0x200000
				BL    MUL_CARRY
				LDR   R0, =0x400000
				BL    MUL_CARRY
				LDR   R0, =0x800000
				BL    MUL_CARRY
				LDR   R0, =0x1000000
				BL    MUL_CARRY
				LDR   R0, =0x2000000
				BL    MUL_CARRY
				LDR   R0, =0x4000000
				BL    MUL_CARRY
				LDR   R0, =0x8000000
				BL    MUL_CARRY
				LDR   R0, =0x10000000
				BL    MUL_CARRY
				LDR   R0, =0x20000000
				BL    MUL_CARRY
				LDR   R0, =0x40000000
				BL    MUL_CARRY
				LDR   R0, =0x80000000
				BL    MUL_CARRY
				
				;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
				;3. SHIFT inst.
				LDR   R0, =0xffffffff
				LDR   R1, =0x7fffffff
				LSRS  R2, R0, #1
				CMP   R1, R2
				BNE   exit_err
				
				LDR   R0, =0x7fffffff
				LDR   R1, =0xfffffffe
				LSLS  R2, R0, #1
				CMP   R1, R2
				BNE   exit_err
								
				;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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
