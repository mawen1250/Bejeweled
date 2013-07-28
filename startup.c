//*****************************************************************************
//
// startup_ewarm.c - Startup code for use with IAR's Embedded Workbench,
//                   version 5.
//
// Copyright (c) 2005-2008 Luminary Micro, Inc.  All rights reserved.
// 
// Software License Agreement
// 
// Luminary Micro, Inc. (LMI) is supplying this software for use solely and
// exclusively on LMI's microcontroller products.
// 
// The software is owned by LMI and/or its suppliers, and is protected under
// applicable copyright laws.  All rights are reserved.  You may not combine
// this software with "viral" open-source software in order to form a larger
// program.  Any use in violation of the foregoing restrictions may subject
// the user to criminal sanctions under applicable laws, as well as to civil
// liability for the breach of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
// CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2752 of the Stellaris Peripheral Driver Library.
//
//*****************************************************************************

//*****************************************************************************
//
// Enable the IAR extensions for this source file.
//
//*****************************************************************************
#pragma language=extended

#include "src\fault.h"
#include  "hw_types.h"
//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
static void NmiSR(void);
static void FaultISR(void);
static void IntDefaultHandler(void);

//*****************************************************************************
//
// External declarations for the interrupt handlers used by the application.
//
//*****************************************************************************


//*****************************************************************************
//
// The entry point for the application startup code.
//
//*****************************************************************************
extern void __iar_program_start(void);


extern void ISR_CAN0Handler(void);
//*****************************************************************************
//
// Reserve space for the system stack.
//
//*****************************************************************************
//static unsigned long pulStack[400] @ ".noinit";
static unsigned long pulStack[1024] @ ".noinit";

//*****************************************************************************
//
// A union that describes the entries of the vector table.  The union is needed
// since the first entry is the stack pointer and the remainder are function
// pointers.
//
//*****************************************************************************
typedef union
{
    void (*pfnHandler)(void);
    unsigned long ulPtr;
}
uVectorEntry;

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
__root const uVectorEntry __vector_table[] @ ".intvec" =
{
    { .ulPtr = (unsigned long)pulStack + sizeof(pulStack) },
                                            // 0 The initial stack pointer
    __iar_program_start,                    // 1 The reset handler
    NmiSR,                                  // 2 The NMI handler
    FaultISR,                               // 3 The hard fault handler
    IntDefaultHandler,                      // 4 The MPU fault handler
    IntDefaultHandler,                      // 5 The bus fault handler
    IntDefaultHandler,                      // 6 The usage fault handler
    0,                                      // 7 Reserved
    0,                                      // 8 Reserved
    0,                                      // 9 Reserved
    0,                                      // 10 Reserved
    IntDefaultHandler,                      // 11 SVCall handler
    IntDefaultHandler,                      // 12 Debug monitor handler
    0,                                      // 13 Reserved
    IntDefaultHandler,                      // 14 The PendSV handler
    IntDefaultHandler,                      // 15 The SysTick handler
    IntDefaultHandler,                      // 16 GPIO Port A
    IntDefaultHandler,                      // 17 GPIO Port B
    IntDefaultHandler,                      // 18 GPIO Port C
    IntDefaultHandler,                      // 19 GPIO Port D
    IntDefaultHandler,                      // 20 GPIO Port E
    IntDefaultHandler,                      // 21 UART0 Rx and Tx
    IntDefaultHandler,                      // 22 UART1 Rx and Tx
    IntDefaultHandler,                      // 23 SSI0 Rx and Tx
    IntDefaultHandler,                      // 24 I2C0 Master and Slave
    IntDefaultHandler,                      // 25 PWM Fault
    IntDefaultHandler,                      // 26 PWM Generator 0
    IntDefaultHandler,                      // 27 PWM Generator 1
    IntDefaultHandler,                      // 28 PWM Generator 2
    IntDefaultHandler,                      // 29 Quadrature Encoder 0
    IntDefaultHandler,                      // 30 ADC Sequence 0
    IntDefaultHandler,                      // 31 ADC Sequence 1
    IntDefaultHandler,                      // 32 ADC Sequence 2
    IntDefaultHandler,                      // 33 ADC Sequence 3
    IntDefaultHandler,                      // 34 Watchdog timer
    IntDefaultHandler,                      // 35 Timer 0 subtimer A
    IntDefaultHandler,                      // 36 Timer 0 subtimer B
    IntDefaultHandler,                      // 37 Timer 1 subtimer A
    IntDefaultHandler,                      // 38 Timer 1 subtimer B
    IntDefaultHandler,                      // 39 Timer 2 subtimer A
    IntDefaultHandler,                      // 40 Timer 2 subtimer B
    IntDefaultHandler,                      // 41 Analog Comparator 0
    IntDefaultHandler,                      // 42 Analog Comparator 1
    IntDefaultHandler,                      // 43 Analog Comparator 2
    IntDefaultHandler,                      // 44 System Control (PLL, OSC, BO)
    IntDefaultHandler,                      // 45 FLASH Control
    IntDefaultHandler,                      // 46 GPIO Port F
    IntDefaultHandler,                      // 47 GPIO Port G
    IntDefaultHandler,                      // 48 GPIO Port H
    IntDefaultHandler,                      // 49 UART2 Rx and Tx
    IntDefaultHandler,                      // 50 SSI1 Rx and Tx
    IntDefaultHandler,                      // 51 Timer 3 subtimer A
    IntDefaultHandler,                      // 52 Timer 3 subtimer B
    IntDefaultHandler,                      // 53 I2C1 Master and Slave
    IntDefaultHandler,                      // 54 Quadrature Encoder 1
    IntDefaultHandler,                      // 55 CAN0
    IntDefaultHandler,                      // 56 CAN1
    IntDefaultHandler,                      // 57 CAN2
    IntDefaultHandler,                      // 58 Ethernet
    IntDefaultHandler                       // 59 Hibernate
};

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
static void
NmiSR(void)
{
    volatile unsigned char ucFlg = 1;
    //
    // Go into an infinite loop.
    //
    while(ucFlg)
    {
      ;
    }
}
__no_init volatile unsigned long GulFaultPC @0x20000000;
static unsigned long GulFaultFlg;

//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void FaultISR(void)
{
    asm("TST LR, #0x4\n"
    "ITE EQ\n"
    "MRSEQ R0, MSP\n"
    "MRSNE R0, PSP\n"
    "LDR R1,[R0,#24]\n"
    "MOV32 R0,#0x20000000\n"
    "STR R1, [R0]");                                                    /* 取得入栈前的PC值GulFaultPC   */
    GulFaultFlg = HWREG(NVIC_HFSR);                                     /* 读取硬件fault状态寄存器      */
    GulFaultPC  = GulFaultPC;                                           /* 入栈前的PC,可能是引起fault   */
                                                                        /* 的PC                         */
    if (GulFaultFlg & VECTBL) {
        while(1){;}                                                     /* fault是在取向量时发生的      */
    } else if (GulFaultFlg & DEBUGEVT) {
        while(1){;}                                                     /* fault因调试事件而产生        */
    } else {
        GulFaultFlg = HWREG(NVIC_MFSR);                                 /* 读取存储器管理fault状态寄存器*/
        if (GulFaultFlg > 0) {
            while(1){;}                                                 /* 存储器管理faults 可能对未使能*/
        }                                                               /* 时钟的外设进行了读写操作     */
        
        GulFaultFlg = HWREG(NVIC_BFSR);                                 /* 读取总线fault状态寄存器      */
        if (GulFaultFlg > 0) {
            while(1){;}                                                 /* 总线错误引起fault            */
        }
        
        GulFaultFlg = HWREG(NVIC_UFSR);                                 /* 读取用法fault状态寄存器      */
        if (GulFaultFlg > 0) {
            while(1){;}                                                 /* 用法错误引起fault，如除数为0 */
        }
    }
    
    //
    // Go into an infinite loop.
    //
    while(1){;}
}



//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void IntDefaultHandler(void)
{   
    volatile unsigned char ucFlg = 1;
    //
    // Go into an infinite loop.
    //
    while(ucFlg)
    {
      ;
    }
}
