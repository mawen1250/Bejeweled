/****************************************Copyright (c)**************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.embedtools.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name:               Uart0.c
** Last modified Date:      2006.11.18
** Last Version:            v1.0
** Description:             串口驱动源文件
** 
**------------------------------------------------------------------------------------------------------
** Created By:              Zhou Shaogang
** Created date:            2006.11.18
** Version:                 v1.0
** Descriptions:
**
**------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao 倪力考
** Modified date:           2007.10.20 
** Version:                 v1.1
** Description:
**
********************************************************************************************************/

#include "uart0.h"

INT8U ucBuffer[200] = {0};
INT8U ucBit = 0;
INT8U ucNum = 0;

/*********************************************************************************************************
** Function name:           Uart0Init
** Descriptions:            初始化Uart0 
** input parameters:        BaudRate:   波特率
**                          Prio:       中断优先级
** Output parameters::      无
** Returned value:          TRUE :      成功
**                          FALSE:      失败
** Created by:              Zhou Shaogang
** Created Date:            2006.11.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao 倪力考
** Modified date:           2007.10.20 
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
INT8U Uart0Init (INT32U  BaudRate, INT8U  Prio)
{
    if (BaudRate > 115200) {                                            /*  波特率太高，错误返回       */
        return(FALSE);
    }
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);                        /*  使能串口0外围设备          */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);                        /*  使能GPIOA                  */
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);          /*  设置PA0,PA1为RXD0,TXD0     */
  
    UARTConfigSet(UART0_BASE, BaudRate, (UART_CONFIG_WLEN_8 |           /*  配置串口0，8位数据，1位起始*/
                                                                        /*  位，1位停止位，用户波特率  */
                                         UART_CONFIG_STOP_ONE |
                                         UART_CONFIG_PAR_NONE) & 0xFFFFFFEF);

    UARTFifoLevelSet(UART0_BASE, 4, 4);
    IntEnable(INT_UART0);                                               /*  使能串口0系统中断           */
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);               /*  使能串口0接收中断和接收超时 */
                                                                        /*  中断                        */
    IntPrioritySet(INT_UART0, Prio);                                    /*  设置中断优先级              */
    UARTEnable(UART0_BASE);                                
    return(TRUE);
}


/*********************************************************************************************************
** Function name:           Uart0Send
** Descriptions:            发送多个字节数据
** input parameters:        Buffer:发送数据存储位置
**                          NByte:发送数据个数
** Output parameters::      无
** Returned value:          无
** Created by:              Zhou Shaogang
** Created Date:            2006.11.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao 倪力考
** Modified date:           2007.10.20 
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void Uart0Send (INT8U  *Buffer, INT16U  NByte)
{
    while (NByte) {
        if ( UARTSpaceAvail(UART0_BASE) ) {
            UARTCharNonBlockingPut(UART0_BASE, *Buffer++);
            NByte--;
        }
    }
    while ( !UARTTraFifoEmp(UART0_BASE) ) {
        ;
    }
}


/*********************************************************************************************************
** Function name:           UART0_ISR
**  Descriptions:           串口0中断服务函数
** input parameters:        无
** Output parameters::      无
** Returned value:          无
** Created by:              Zhou Shaogang
** Created Date:            2006.11.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao 倪力考
** Modified date:           2007.10.20 
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void UART0_ISR (void)
{
    unsigned long ulStatus;
    ulStatus = UARTIntStatus(UART0_BASE, true);                         /*  读取已使能的串口0中断状态   */
    UARTIntClear(UART0_BASE, ulStatus);                                 /*  清除当前的串口0中断         */
    if((ulStatus & UART_INT_RT)||(ulStatus & UART_INT_RX)) {            /*  接收中断                    */
        ucBuffer[ucNum] = UARTCharNonBlockingGet(UART0_BASE);           /*  保存接收到的数据            */        
        ucNum++; 
    }
    ucBit = 1;
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
