/****************************************Copyright (c)****************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               fault.h
** Last modified Date:      2007.01.18
** Last Version:            1.0
** Description:             
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              ��չ��
** Created date:            2007.09.16
** Version:                 1.0
** Descriptions:            The original version ��ʼ�汾
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Description:             
*********************************************************************************************************/

#ifndef __FAULT_H
#define __FAULT_H


#define NVIC_MFSR    0xE000ED28//NVIC �еĴ洢������fault״̬�Ĵ���
#define NVIC_BFSR    0xE000ED29//NVIC �е�����fault״̬�Ĵ���
#define NVIC_UFSR    0xE000ED2A//NVIC �е��÷�fault״̬�Ĵ���
#define NVIC_HFSR    0xE000ED2C//NVIC �е�Ӳ��fault״̬�Ĵ���

#define DEBUGEVT     0x80000000//fault������¼�������
#define FORCED       0x40000000//fault������fault,�洢������fault,�����÷�fault�ϷõĽ��
#define VECTBL       0x00000002//fault����ȡ����ʱ������

#define MMARVALID    0x80      //��ʾMMAR��Ч
#define MSTKERR      0x10      //��ջʱ��������
#define MUNSTKERR    0x08      //��ջʱ�������� 
#define DACCVIOL     0x02      //ȡ���ݷ���Υ��(������ֹ)
#define IACCVIOL     0x01      //ȡָ����Υ��(ȡָ��ֹ)

#define BFARVALID    0x80      //��ʾBFAR��Ч
#define STKERR       0x10      //��ջʱ��������
#define UNSTKERR     0x08      //��ջʱ��������
#define IMPRECISERR  0x04      //��ջʱ��������
#define PRECISERR    0x02      //��ջʱ��������
#define IBUSERR      0x01      //��ջʱ��������

#define DIVBYZERO    0x0200    //��ʾ��������ʱ����Ϊ�������fault
#define UNALIGNED    0x0100    //δ������ʵ��µ�fault
#define NOCP         0x0080    //��ͼִ��Э���������ָ��
#define INVPC        0x0040    //���쳣����ʱ��ͼ�Ƿ��ؼ���EXC_RETURN��PC
#define INVSTATE     0x0020    //��ͼ����ARM״̬
#define UNDEFINSTR   0x0010    //ִ��δ����ָ��
    
/*********************************************************************************************************
��������
*********************************************************************************************************/
extern unsigned long getFaultPC(void);



#endif



/*********************************************************************************************************
END FILE
*********************************************************************************************************/

