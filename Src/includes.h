/****************************************Copyright (c)****************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               includes.h
** Last modified Date:      2007.01.18
** Last Version:            v1.0
** Description:             uCOS-II's include file   uCOS-II的头文件
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              Steven Zhou 周绍刚
** Created date:            2007.12.12
** Version:                 v1.0
** Descriptions:            The original version 初始版本
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Kang qinhua 
** Modified date:           2008.01.02   
** Version:                 v1.1
** Description:             The second version 第二版            
**
*********************************************************************************************************/

#ifndef  __INCLUDES_H
#define  __INCLUDES_H

#ifdef __cplusplus
extern "C" {
#endif
  
/*********************************************************************************************************
  Date types(Compiler specific)  数据类型（和编译器相关）                
*********************************************************************************************************/
typedef unsigned char   uint8;                                          /* Unsigned  8 bit quantity     */
typedef signed   char   int8;                                           /* Signed    8 bit quantity     */
typedef unsigned short  uint16;                                         /* Unsigned  16 bit quantity    */
typedef signed   short  int16;                                          /* Signed    16 bit quantity    */
typedef unsigned int    uint32;                                         /* Unsigned  32 bit quantity    */
typedef signed   int    int32;                                          /* Signed    32 bit quantity    */
typedef float           fp32;                                           /* Single    precision          */
                                                                        /* floating  point              */
typedef double          fp64;                                           /* Double    precision          */
                                                                        /* floating  point              */

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL  0
#endif


/*********************************************************************************************************
  Standard header files 标准头文件
*********************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


/*********************************************************************************************************
  Driver's header files  驱动库头文件 
*********************************************************************************************************/
#include  "ff.h"
#include  "LTM024D130.h"
#include  "hz16.h"
/*********************************************************************************************************
  User's header files 用户头文件
*********************************************************************************************************/
//#include <Main.h>

//#include "buzzer.h"
//#include "desk.h"

#ifdef __cplusplus
}
#endif

#endif
/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/
