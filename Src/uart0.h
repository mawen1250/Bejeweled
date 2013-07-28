/****************************************Copyright (c)**************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.embedtools.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name:          Uart0.h
** Last modified Date: 2006-11-18
** Last Version:       v1.0
** Description:        ��������ͷ�ļ�
** 
**------------------------------------------------------------------------------------------------------
** Created By:         Zhou Shaogang
** Created date:       2006-11-18
** Version:            v1.0
** Descriptions:
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
********************************************************************************************************/
  
#ifndef __UART0_H__
#define __UART0_H__

#include "hw_ints.h"// hw_ints.h - Macros that define the interrupt assignment on Stellaris.
#include "hw_memmap.h"// hw_memmap.h - Macros defining the memory map of Stellaris.
#include "hw_types.h"// hw_types.h - Common types and macros.
#include "gpio.h"// gpio.h - Defines and Macros for GPIO API.
#include "interrupt.h"// interrupt.h - Prototypes for the NVIC Interrupt Controller Driver.
#include "uart.h"// uart.h - Defines and Macros for the UART.

#include "..\HeadFile\dataType.h"                                       /* ���峣���������ͼ���������ֵ */


/************************************************************************************
** Function name: Uart0Init
** Descriptions:  ��ʼ��Uart0 
** Input:         BaudRate: ������
**                Prio:     �ж����ȼ�
** Output:        TRUE :�ɹ�
**                FALSE:ʧ��
** Created by:    Zhou Shaogang
** Created Date:  2006-11-18
**----------------------------------------------------------------------------------
** Modified by:
** Modified Date: 
**----------------------------------------------------------------------------------
************************************************************************************/
extern INT8U GucUartBuf[200];
extern BOOLEAN GbUartReceive;
extern INT8U GucUartReNum;
extern INT8U Uart0Init(INT32U BaudRate, INT8U Prio);


/************************************************************************************
** Function name: Uart0Send
** Descriptions:  ���Ͷ���ֽ�����
** Input:         Buffer:�������ݴ洢λ��
**                NByte:�������ݸ���
** Output:        ��
** Created by:    Zhou Shaogang 
** Created Date:  2006-11-18
**----------------------------------------------------------------------------------
** Modified by:
** Modified Date: 
**----------------------------------------------------------------------------------
************************************************************************************/
extern void Uart0Send(INT8U *Buffer, INT16U NByte);

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
extern void UART0_ISR(void);


/* �궨�壬����FIFO���շ���� */
#ifndef UART_O_IFLS
  #define UART_O_IFLS  0x00000034  
#endif

#define UARTFifoLevelSet(ulBase, RecLvl, TraLvl)  HWREG(ulBase + UART_O_IFLS) = (HWREG(ulBase + UART_O_IFLS) & 0xFFFFFFC0) | (RecLvl<<3) | TraLvl

/* �궨�壬��鷢��FIFO�Ƿ�Ϊ�� */
#ifndef UART_O_FR
  #define UART_O_FR  0x00000018  
#endif
#ifndef UART_FR_TXFE
  #define UART_FR_TXFE 0x00000080  
#endif
#define UARTTraFifoEmp(ulBase) ( (HWREG(ulBase + UART_O_FR) & UART_FR_TXFE) ? true : false )

#endif
