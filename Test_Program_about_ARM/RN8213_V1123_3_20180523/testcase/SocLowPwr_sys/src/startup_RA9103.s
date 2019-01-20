;/*****************************************************************************
; * @file:    startup_RA9103.s
; * @purpose: CMSIS Cortex-M0 Core Device Startup File
; *           for RA9103
; * @version: V1.00
; * @date:    14. APR 2011
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; *
; *
; *****************************************************************************/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
Stack_Size      EQU     0x00000800
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
                DCD     TC2_HANDLER
                DCD     TC3_HANDLER
                DCD     WDT_HANDLER
                DCD     KBI_HANDLER
                DCD     LCD_HANDLER
                DCD     CT_HANDLER
                DCD     DMA_HANDLER
                DCD     NVM_HANDLER
                DCD     EXT0_HANDLER
                DCD     EXT1_HANDLER
                DCD     EXT2_HANDLER
                DCD     EXT3_HANDLER
                DCD     EXT4_HANDLER
                DCD     EXT5_HANDLER
                DCD     EXT6_HANDLER
                DCD     EXT7_HANDLER
                
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors


                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
;                IMPORT  SystemInit
                ;;trigger gpio
;                MOVS   R0,  #0x40 ;0x40034020 = 0xa5800020
;				LSLS   R1,  R0, #8
;				LSLS   R2,  R1, #16
;				ADDS   R0,  R1, R2
;				MOVS   R1,  #3
;				LSLS   R1,  R1, #16
;				ADDS   R0,  R0, R1
;				MOVS   R1,  #0x20
;				ADDS   R0,  R0, R1
;				
;				MOVS   R1,  #0xA5
;				LSLS   R1,  R1, #24
;				MOVS   R2,  #0x80
;				LSLS   R2,  R2, #16
;				ADDS   R1,  R1, R2
;				MOVS   R2,  #0x20
;				ADDS   R1,  R1, R2
;				
;				STR    R1,  [R0]
;				
;				;P4.0/1
;				MOVS   R0,  #0x50;0x5000001c = fffffffc
;				LSLS   R0,  R0, #24
;				MOVS   R1,  #0x1c
;				ADDS   R0,  R0,  R1
;				
;				MOVS   R1,  #0xff
;				LSLS   R1,  R1, #8 
;				LSLS   R2,  R1, #8 
;				ADDS   R2,  R1, R2 
;				LSLS   R1,  R1, #16
;				ADDS   R1,  R1, R2
;				MOVS   R2,  #0xfc
;				ADDS   R1,  R2, R1				
;				STR    R1,  [R0]
				
;				ADDS   R0,  #0x4   ;0x50000020
;				MOVS   R1,  #0x1
;				STR    R1,  [R0]
				;;end of trigger gpio
				
				
				;;RA9103F0 flash trimֵ
				; LDR     R0, =0x5001400c
				; LDR     R1, =0xA549908
				; STR     R1, [R0, #0]
				; LDR     R1, =0xBA110101
				; STR     R1, [R0, #4]
				; LDR     R1, =0xfeed0002
				; LDR     R0, =0x50014000
				; STR     R1, [R0, #0]
				
				
				
				
				; LDR     R0, =0x5001402c
				; LDR     R1, =0xA549908
				; STR     R1, [R0, #0]
				; LDR     R1, =0xBA110101
				; STR     R1, [R0, #4]
				; LDR     R1, =0xfeed0002
				; LDR     R0, =0x50014020
				; STR     R1, [R0, #0]
				
				
                IMPORT  __main		
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

;                IF UNUSEDIRQ <> 1
NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler                 PROC
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
EXT1_HANDLER    PROC							  
                EXPORT  EXT1_HANDLER			  [WEAK]
                B       .
                ENDP
EXT2_HANDLER    PROC							  
                EXPORT  EXT2_HANDLER			  [WEAK]
                B       .
                ENDP
EXT3_HANDLER    PROC							  
                EXPORT  EXT3_HANDLER			  [WEAK]
                B       .
                ENDP
EXT4_HANDLER    PROC							  
                EXPORT  EXT4_HANDLER			  [WEAK]
                B       .
                ENDP
EXT5_HANDLER    PROC							  
                EXPORT  EXT5_HANDLER			  [WEAK]
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
NVM_HANDLER     PROC							  
                EXPORT  NVM_HANDLER			  [WEAK]
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
TC2_HANDLER     PROC							  
                EXPORT TC2_HANDLER				  [WEAK]
                B       .
                ENDP
TC3_HANDLER     PROC							  
                EXPORT  TC3_HANDLER				  [WEAK]
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
                
                
;                IMPORT  NMI_Handler
;                IMPORT  HardFault_Handler
;                IMPORT  SVC_Handler
;                IMPORT  PendSV_Handler
;                IMPORT  SysTick_Handler
;                IMPORT  EXT0_HANDLER
;                IMPORT  EXT1_HANDLER
;                IMPORT  EXT2_HANDLER
;                IMPORT  EXT3_HANDLER
;                IMPORT  EXT4_HANDLER
;                IMPORT  EXT5_HANDLER
;                IMPORT  EXT6_HANDLER
;                IMPORT  EXT7_HANDLER
;                IMPORT  LVD0_HANDLER
;                IMPORT  LVD1_HANDLER
;                IMPORT  LVD2_HANDLER
;                IMPORT  RTC_HANDLER
;                IMPORT  EMU_HANDLER
;                IMPORT  MADC_HANDLER
;                IMPORT  UART0_HANDLER
;                IMPORT  UART1_HANDLER
;                IMPORT  UART2_HANDLER
;                IMPORT  UART3_HANDLER
;                IMPORT  SPI_HANDLER
;                IMPORT  I2C_HANDLER
;                IMPORT  ISO78162_HANDLER
;                IMPORT  ISO78161_HANDLER
;                IMPORT  TC0_HANDLER
;                IMPORT  TC1_HANDLER
;                IMPORT TC2_HANDLER
;                IMPORT  TC3_HANDLER
;                IMPORT  WDT_HANDLER
;                IMPORT  KBI_HANDLER
;                IMPORT  LCD_HANDLER
;                IMPORT  CT_HANDLER
;                IMPORT  DMA_HANDLER
;                IMPORT  EMPRY_HANDLER
;                ENDIF

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
