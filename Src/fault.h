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
** Created By:              张展威
** Created date:            2007.09.16
** Version:                 1.0
** Descriptions:            The original version 初始版本
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Description:             
*********************************************************************************************************/

#ifndef __FAULT_H
#define __FAULT_H


#define NVIC_MFSR    0xE000ED28//NVIC 中的存储器管理fault状态寄存器
#define NVIC_BFSR    0xE000ED29//NVIC 中的总线fault状态寄存器
#define NVIC_UFSR    0xE000ED2A//NVIC 中的用法fault状态寄存器
#define NVIC_HFSR    0xE000ED2C//NVIC 中的硬件fault状态寄存器

#define DEBUGEVT     0x80000000//fault因调试事件而产生
#define FORCED       0x40000000//fault是总线fault,存储器管理fault,或是用法fault上访的结果
#define VECTBL       0x00000002//fault是在取向量时发生的

#define MMARVALID    0x80      //表示MMAR有效
#define MSTKERR      0x10      //入栈时发生错误
#define MUNSTKERR    0x08      //出栈时发生错误 
#define DACCVIOL     0x02      //取数据访问违例(数据中止)
#define IACCVIOL     0x01      //取指访问违例(取指中止)

#define BFARVALID    0x80      //表示BFAR有效
#define STKERR       0x10      //入栈时发生错误
#define UNSTKERR     0x08      //入栈时发生错误
#define IMPRECISERR  0x04      //入栈时发生错误
#define PRECISERR    0x02      //入栈时发生错误
#define IBUSERR      0x01      //入栈时发生错误

#define DIVBYZERO    0x0200    //表示除法运算时除数为零引起的fault
#define UNALIGNED    0x0100    //未对齐访问导致的fault
#define NOCP         0x0080    //试图执行协处理器相关指令
#define INVPC        0x0040    //在异常返回时试图非法地加载EXC_RETURN到PC
#define INVSTATE     0x0020    //试图切入ARM状态
#define UNDEFINSTR   0x0010    //执行未定义指令
    
/*********************************************************************************************************
函数声明
*********************************************************************************************************/
extern unsigned long getFaultPC(void);



#endif



/*********************************************************************************************************
END FILE
*********************************************************************************************************/

