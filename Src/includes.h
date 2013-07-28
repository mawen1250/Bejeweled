/****************************************Copyright (c)****************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               includes.h
** Last modified Date:      2007.01.18
** Last Version:            v1.0
** Description:             uCOS-II's include file   uCOS-II��ͷ�ļ�
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              Steven Zhou ���ܸ�
** Created date:            2007.12.12
** Version:                 v1.0
** Descriptions:            The original version ��ʼ�汾
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Kang qinhua 
** Modified date:           2008.01.02   
** Version:                 v1.1
** Description:             The second version �ڶ���            
**
*********************************************************************************************************/

#ifndef  __INCLUDES_H
#define  __INCLUDES_H

#ifdef __cplusplus
extern "C" {
#endif
  
/*********************************************************************************************************
  Date types(Compiler specific)  �������ͣ��ͱ�������أ�                
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
  Standard header files ��׼ͷ�ļ�
*********************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


/*********************************************************************************************************
  Driver's header files  ������ͷ�ļ� 
*********************************************************************************************************/
#include  "ff.h"
#include  "LTM024D130.h"
#include  "hz16.h"
/*********************************************************************************************************
  User's header files �û�ͷ�ļ�
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
