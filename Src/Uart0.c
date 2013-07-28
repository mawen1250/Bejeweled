/****************************************Copyright (c)**************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.embedtools.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name:               Uart0.c
** Last modified Date:      2006.11.18
** Last Version:            v1.0
** Description:             ��������Դ�ļ�
** 
**------------------------------------------------------------------------------------------------------
** Created By:              Zhou Shaogang
** Created date:            2006.11.18
** Version:                 v1.0
** Descriptions:
**
**------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao ������
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
** Descriptions:            ��ʼ��Uart0 
** input parameters:        BaudRate:   ������
**                          Prio:       �ж����ȼ�
** Output parameters::      ��
** Returned value:          TRUE :      �ɹ�
**                          FALSE:      ʧ��
** Created by:              Zhou Shaogang
** Created Date:            2006.11.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao ������
** Modified date:           2007.10.20 
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
INT8U Uart0Init (INT32U  BaudRate, INT8U  Prio)
{
    if (BaudRate > 115200) {                                            /*  ������̫�ߣ����󷵻�       */
        return(FALSE);
    }
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);                        /*  ʹ�ܴ���0��Χ�豸          */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);                        /*  ʹ��GPIOA                  */
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);          /*  ����PA0,PA1ΪRXD0,TXD0     */
  
    UARTConfigSet(UART0_BASE, BaudRate, (UART_CONFIG_WLEN_8 |           /*  ���ô���0��8λ���ݣ�1λ��ʼ*/
                                                                        /*  λ��1λֹͣλ���û�������  */
                                         UART_CONFIG_STOP_ONE |
                                         UART_CONFIG_PAR_NONE) & 0xFFFFFFEF);

    UARTFifoLevelSet(UART0_BASE, 4, 4);
    IntEnable(INT_UART0);                                               /*  ʹ�ܴ���0ϵͳ�ж�           */
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);               /*  ʹ�ܴ���0�����жϺͽ��ճ�ʱ */
                                                                        /*  �ж�                        */
    IntPrioritySet(INT_UART0, Prio);                                    /*  �����ж����ȼ�              */
    UARTEnable(UART0_BASE);                                
    return(TRUE);
}


/*********************************************************************************************************
** Function name:           Uart0Send
** Descriptions:            ���Ͷ���ֽ�����
** input parameters:        Buffer:�������ݴ洢λ��
**                          NByte:�������ݸ���
** Output parameters::      ��
** Returned value:          ��
** Created by:              Zhou Shaogang
** Created Date:            2006.11.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao ������
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
**  Descriptions:           ����0�жϷ�����
** input parameters:        ��
** Output parameters::      ��
** Returned value:          ��
** Created by:              Zhou Shaogang
** Created Date:            2006.11.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao ������
** Modified date:           2007.10.20 
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void UART0_ISR (void)
{
    unsigned long ulStatus;
    ulStatus = UARTIntStatus(UART0_BASE, true);                         /*  ��ȡ��ʹ�ܵĴ���0�ж�״̬   */
    UARTIntClear(UART0_BASE, ulStatus);                                 /*  �����ǰ�Ĵ���0�ж�         */
    if((ulStatus & UART_INT_RT)||(ulStatus & UART_INT_RX)) {            /*  �����ж�                    */
        ucBuffer[ucNum] = UARTCharNonBlockingGet(UART0_BASE);           /*  ������յ�������            */        
        ucNum++; 
    }
    ucBit = 1;
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
