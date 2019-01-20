;/*****************************************************************************
; * @file:    startup_RA9103F0.s
; * @purpose: CMSIS Cortex-M0 Core Device Startup File
; *           for RA9103F0
; * @version: V1.00
; * @date:    2017/1/10
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; *
; *
; *****************************************************************************/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
Stack_Size      EQU     500
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
                
                DCD     SYSCLKCAL_HANDLER
                DCD     LVDCMP_HANDLER
                DCD     0
                DCD     RTC_HANDLER
                DCD     0
                DCD     SARADC_HANDLER
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
                DCD     UART4_HANDLER
                DCD     UART5_HANDLER
                DCD     WDT_HANDLER
                DCD     KBI_HANDLER
                DCD     LCD_HANDLER
                DCD     SYS_TC_HANDLER
                DCD     0
                DCD     0
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
                IMPORT  SystemInit
                IMPORT  __main
				
				; LDR     R0, =0x50014000
				; LDR     R1, =0xfeed0003
				; STR     R1, [R0, #0]
				; LDR     R1, =0xA549908
				; STR     R1, [R0, #0xc]
				; LDR     R1, =0xBA110101
				; STR     R1, [R0, #0x10]
				; LDR     R1, =0xfeed0002
				; LDR     R0, =0x50014000
				; STR     R1, [R0, #0]
				
				
				
				
				; LDR     R0, =0x50014020
				; LDR     R1, =0xfeed0003
				; STR     R1, [R0, #0]
				; LDR     R1, =0xA549908
				; STR     R1, [R0, #0xc]
				; LDR     R1, =0xBA110101
				; STR     R1, [R0, #0x10]
				; LDR     R1, =0xfeed0002
				; LDR     R0, =0x50014020
				; STR     R1, [R0, #0]
				

                ;LDR     R0, =SystemInit
                ;BLX     R0
                LDR     R0, =__main
                BX      R0
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
SYS_TC_HANDLER    PROC							  
                EXPORT  SYS_TC_HANDLER			  [WEAK]
                B       .
                ENDP
LCD_HANDLER     PROC							  
                EXPORT  LCD_HANDLER				  [WEAK]
                B       .
                ENDP   
KBI_HANDLER     PROC							  
                EXPORT  KBI_HANDLER				  [WEAK]
                B       .
                ENDP
WDT_HANDLER     PROC							  
                EXPORT  WDT_HANDLER				  [WEAK]
                B       .
                ENDP
UART5_HANDLER     PROC							  
                EXPORT UART5_HANDLER				  [WEAK]
                B       .
                ENDP
UART4_HANDLER     PROC							  
                EXPORT UART4_HANDLER				  [WEAK]
                B       .
                ENDP
TC1_HANDLER     PROC 							  
                EXPORT  TC1_HANDLER				  [WEAK]
                B       .
                ENDP
TC0_HANDLER     PROC							  
                EXPORT  TC0_HANDLER				  [WEAK]
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
I2C_HANDLER     PROC							  
                EXPORT  I2C_HANDLER				  [WEAK]
                B       .
                ENDP
SPI_HANDLER     PROC							  
                EXPORT  SPI_HANDLER				  [WEAK]
                B       .
                ENDP
UART3_HANDLER   PROC							  
                EXPORT  UART3_HANDLER			  [WEAK]
                B       .
                ENDP
UART2_HANDLER   PROC							  
                EXPORT  UART2_HANDLER			  [WEAK]
                B       .
                ENDP
UART1_HANDLER   PROC                              
                EXPORT  UART1_HANDLER			  [WEAK]
                B       .
                ENDP
UART0_HANDLER    PROC							  
                EXPORT  UART0_HANDLER			  [WEAK]
                B       .
                ENDP
SARADC_HANDLER    PROC							  
                EXPORT  SARADC_HANDLER  	  [WEAK]
                B       .
                ENDP
RTC_HANDLER     PROC							  
                EXPORT  RTC_HANDLER				  [WEAK]
                B       .
                ENDP
LVDCMP_HANDLER   PROC							  
                EXPORT  LVDCMP_HANDLER			    [WEAK]
                B       .
                ENDP
SYSCLKCAL_HANDLER   PROC							  
                EXPORT  SYSCLKCAL_HANDLER			    [WEAK]
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
