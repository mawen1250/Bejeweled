/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**------File Info-----------------------------------------------------------------------------------------
** File name:               LTM024D130.h
** Latest modified Date:    2008-07-22
** Latest Version:          1.0
** Descriptions:            LTM024D130.c接口文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              张展威
** Created date:            2008-07-22
** Version:                 1.0
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/

#ifndef __LTM024D130_H
#define __LTM024D130_H


/*********************************************************************************************************
液晶接口示意图
VCC   22--| |--1  GND
VCC   21--| |--2  GND
VCC   20--| |--3  GND
VCCIO 19--| |--4  #RST
VCCIO 18--| |--5  NC
D0    17--| |--6  #RD
D1    16--| |--7  #WR
D2    15--| |--8  #CS
D3    14--| |--9  RS
D4    13--| |--10 D7
D5    12--| |--11 D6

VCC :2.6～3.3V
VCCIO :1.6～3.3V
*********************************************************************************************************/

/*********************************************************************************************************
硬件相关接口定义
*********************************************************************************************************/

#define  LCD_PERI               SYSCTL_PERIPH_GPIOA | SYSCTL_PERIPH_GPIOB

#define  LCD_RS_BASE            GPIO_PORTB_BASE                         /* RS - PB0                     */
#define  LCD_RS_PIN             GPIO_PIN_0

#define  LCD_CS_BASE            GPIO_PORTB_BASE                         /* CS - PB1                     */
#define  LCD_CS_PIN             GPIO_PIN_1

#define  LCD_WR_BASE            GPIO_PORTB_BASE                         /* WR - PB2                     */
#define  LCD_WR_PIN             GPIO_PIN_2

#define  LCD_RD_BASE            GPIO_PORTB_BASE                         /* RD - PB3                     */
#define  LCD_RD_PIN             GPIO_PIN_3

#define  LCD_RST_BASE           GPIO_PORTB_BASE                         /* RST - PB4                    */
#define  LCD_RST_PIN            GPIO_PIN_4

#define  LCD_EN_BASE            GPIO_PORTB_BASE                         /* RST - PB5                    */
#define  LCD_EN_PIN             GPIO_PIN_5

#define  DATA_PORT_BASE         GPIO_PORTA_BASE                         /* DATA[7:0] - PA[7:0]          */
#define  DATA_PORT_PIN          0xFF




/*********************************************************************************************************
定义LCM屏大小范围
*********************************************************************************************************/
#define  Gus_LCM_XMAX        240                                        /* 定义液晶x轴的点数            */
#define  Gus_LCM_YMAX        320                                        /* 定义液晶y轴的点数            */

/*********************************************************************************************************
设置颜色宏定义
*********************************************************************************************************/
#define   BLACK        0x0000                                           /* 黑色： 0, 0, 0               */
#define   NAVY         0x000F                                           /* 深蓝色： 0, 0, 128           */
#define   DGREEN       0x03E0                                           /* 深绿色： 0, 128, 0           */
#define   DCYAN        0x03EF                                           /* 深青色： 0, 128, 128         */
#define   MAROON       0x7800                                           /* 深红色：128, 0, 0            */
#define   PURPLE       0x780F                                           /* 紫色： 128, 0, 128           */
#define   OLIVE        0x7BE0                                           /* 橄榄绿：128, 128, 0          */
#define   LGRAY        0xC618                                           /* 灰白色：192, 192, 192        */
#define   DGRAY        0x7BEF                                           /* 深灰色：128, 128, 128        */
#define   BLUE         0x001F                                           /* 蓝色： 0, 0, 255             */
#define   GREEN        0x07E0                                           /* 绿色： 0, 255, 0             */
#define   CYAN         0x07FF                                           /* 青色： 0, 255, 255           */
#define   RED          0xF800                                           /* 红色： 255, 0, 0             */
#define   MAGENTA      0xF81F                                           /* 品红： 255, 0, 255           */
#define   YELLOW       0xFFE0                                           /* 黄色： 255, 255, 0           */
#define   WHITE        0xFFFF                                           /* 白色： 255, 255, 255         */
#define   IDMCOLOR(color) (((color & 0x001F) << 11) | ((color & 0xF800) >> 11) | (color & 0x07E0))
#define  GusBACKCOLOR  BLACK                                            /* 定义清屏色                   */


/*********************************************************************************************************
  矩形窗口数据结构
*********************************************************************************************************/
typedef struct
{
    unsigned short usXMin;                                              /* x起始坐标                    */
    unsigned short usYMin;                                              /* y起始坐标                    */
    unsigned short usXMax;                                              /* x结束坐标                    */
    unsigned short usYMax;                                              /* y结束坐标                    */
    unsigned short usXinZoon;                                           /* x在窗口中的偏移坐标          */
    unsigned short usYinZoon;                                           /* y在窗口中的偏移坐标          */
}RECTANGLE;


/*********************************************************************************************************
  函数声明
*********************************************************************************************************/
extern void __writeRegAddr8(unsigned char ucAddr);                      /* 写寄存器地址函数             */
extern void __writeData8(unsigned char pd);                             /* 写8位数据函数                */
extern unsigned char __readData8(void);
extern void __writeRegAddr16(unsigned short usAddr);
extern unsigned short __readData16(void);
extern void __writeData16(unsigned short val);                          /* 写16位数据函数               */

extern void __writeCommand(unsigned char dbw,unsigned char ucAddr,unsigned short val);
extern void __screenFill(unsigned short usPixValue);
extern unsigned long  __writeOneHzChar(unsigned char *pucMsk,
                                       unsigned short x0,
                                       unsigned short y0,
                                       unsigned short color);
extern unsigned long  __writeOneASCII(unsigned char *pucMsk,
                                      unsigned short x0,
                                      unsigned short y0,
                                      unsigned short color);

extern void tftLcdInit(void);                                           /* 初始化函数                   */
extern void setCoordinate(unsigned short x, unsigned short y);
extern void pixelDraw(unsigned short x, unsigned short y,unsigned short usValue);
unsigned short pixelRead(unsigned short x, unsigned short y);
extern void lineDrawH(unsigned short x1, unsigned short x2, unsigned short y, unsigned short usValue);
extern void lineDrawV(unsigned short x, unsigned short y1, unsigned short y2, unsigned short usValue);
extern void rectFill(RECTANGLE *pRect, unsigned long ulValue);
extern void writeString(unsigned char *pcStr,
                        unsigned short x0,
                        unsigned short y0,
                        unsigned short color);

extern void windowExactlySet(RECTANGLE *pRect);
extern void pictureFill(RECTANGLE *psZone, unsigned short *pusValue, unsigned long ulCounter);
extern void pictureFill8(RECTANGLE *psZone, unsigned char *pusValue, unsigned long ulCounter);

extern unsigned long colorTranslate(unsigned long ulValue);
extern void backlightOn(void);
extern void backlightOff(void);
extern void screenFlush(void *pvDisplayData);
void picture_Show(const char *path,
                  unsigned short xmin,
                  unsigned short ymin,
                  unsigned short xmax,
                  unsigned short ymax);
				  
#endif

/*********************************************************************************************************
** End of File
*********************************************************************************************************/
