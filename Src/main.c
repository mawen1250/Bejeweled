/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**------File Info-----------------------------------------------------------------------------------------
** File name:               main.c
** Latest modified Date:    2008-07-22
** Latest Version:          1.0
** Descriptions:            Something about this file
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
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_gpio.h"
#include "sysctl.h"
#include "hw_flash.h"
#include "hw_ints.h"
#include "timer.h"
#include "interrupt.h"
#include "gpio.h"

#include "dataType.h"
#include "LTM024D130.h"
#include "ff.h"
#include "main.h"

#include "systemInit.h"
#include "touch.h"
#include "stdio.h"

#include "Bejeweled.h"

/*********************************************************************************************************
定义Timer
*********************************************************************************************************/
#define  T0_A                   TIMER0_BASE , TIMER_A
#define  T0_CFG_32_PER          TIMER0_BASE , TIMER_CFG_32_BIT_PER
#define  T0_A_INT_TIMEOUT       TIMER0_BASE , TIMER_TIMA_TIMEOUT


/*********************************************************************************************************
定义全局的系统时钟变量
*********************************************************************************************************/
volatile unsigned long  GulSysClock  =  12000000UL;


volatile INT32U GulDelayMs;


/*********************************************************************************************************
绘图窗口设置
*********************************************************************************************************/
#define V_INDEXOF    30                                                 /* 居中图片的往上偏移量         */

//#define X0           ((240 - IMAGE_WIDTH) / 2)                          /* IMAGE_WIDTH在gImage.h中定义  */
//#define Y0           ((320 - IMAGE_HIGH)  / 2 - V_INDEXOF)              /* IMAGE_HIGH在gImage.h中定义   */
//#define X1           ((240 - IMAGE_WIDTH) / 2 + IMAGE_WIDTH - 1)
//#define Y1           ((320 - IMAGE_HIGH)  / 2 + IMAGE_HIGH - 1 - V_INDEXOF)
const RECTANGLE GtFullZoon = {0,0,239,319,0,0};                         /* 定义全屏区域                 */
//const RECTANGLE GtImageZoon = {X0, Y0, X1, Y1, 0, 0};                   /* 定义图片显示区域             */


/*********************************************************************************************************
** Function name:           main
** Descriptions:            该范例程序演示了FTF液晶显示图片及字符串
*********************************************************************************************************/
int  main(void)
{
    //unsigned short usColor=0xFFFF;
    //jtagWait();                                                       /* 防止JTAG失效，重要！         */
    GulSysClock  =  clockInit();                                        /* 获取系统时钟，单位：Hz       */
    //clockInit();
    timer0Init();                                                       /* 定时器初始化                 */
    tftLcdInit();
    ledInit();
    ads7843_init();
    backlightOn();
    /*
    sint16 pX = 240 , pY = 320;
    char pstr[20];
    uint32 touchcount = 0;
    */
	unsigned int count_seed=0;
	
    Bootup(&count_seed);
    
    Bejeweled(&count_seed);

                                                                        // 输出文本文字                 
        //pixelDraw(100, 140, RED);                                       // 画点                         
        //usColor = pixelRead(100,140);
        //lineDrawH(80, 230, 50, usColor);                                // 画横线                       
        //lineDrawV(200, 10, 210, BLUE);                                  // 画竖线                       
        
//        delayMs(2000);                                                  // 2s                           
//        ledToggle(LED3);                                                // 指示灯取反                   
//        rectFill((RECTANGLE *)&GtFullZoon, WHITE);                      // 填充白色背景                 
//        pictureFill((RECTANGLE *)&GtImageZoon, (unsigned short *)gImage_pic1, 160 * 99);
//        pictureFill8((RECTANGLE *)&GtImageZoon, (unsigned char *)gImage_pic1, 240 * 149);
//        pictureFill((RECTANGLE *)&GtImageZoon, (unsigned short *)gImage_pic3, 160 * 106);
//        pictureFill8((RECTANGLE *)&GtImageZoon, (unsigned char *)gImage_pic2, 160 * 240);
                                                                        // 填充图片                 

    //rectFill((RECTANGLE *)&GtFullZoon, WHITE);                      // 填充白色背景
    
/*
    while (1)
    {        
        if(GetpXY(&pX, &pY))
        {
            RECTANGLE TouchRect = {ClippX(pX-15), ClippY(pY-15), ClippX(pX+14), ClippY(pY+14), 0, 0};
            sprintf(pstr, "X: %3d, Y: %3d                Count: %10d", pX, pY, touchcount++);
            rectFill((RECTANGLE *)&TouchRect, GREEN);
            writeString("Nyan Nyan Nyan ~~\r\n ——Yu Chun De Ren Lei",20,80,RED);
            writeString((uint8*)pstr, 20, 220, RED);                          // 输出文本文字
            RECTANGLE StrRect = {20, 220, 220, 250, 0, 0};
            delayMs(100);
            rectFill((RECTANGLE *)&TouchRect, WHITE);
            rectFill((RECTANGLE *)&StrRect, WHITE);
        }
        else
        {
            sprintf(pstr, "No Touch");
            writeString("Nyan Nyan Nyan ~~\r\n ——Yu Chun De Ren Lei",20,80,RED);
            writeString((uint8*)pstr, 20, 220, RED);                          // 输出文本文字
            delayMs(100);
            RECTANGLE StrRect = {20, 220, 220, 250, 0, 0};
            rectFill((RECTANGLE *)&StrRect, WHITE);
        }
        //delayMs(2000);                                                  // 2s                           
        //ledToggle(LED3);                                                // 取反指示等
     }
*/
}


/*********************************************************************************************************
LED初始化
*********************************************************************************************************/
void ledInit(void)
{
    SysCtlPeriEnable(LED_PERI);                                         /* 使能LED所在的GPIO端口        */
    GPIOPinTypeOut(LED3);                                               /* 设置LED1所在管脚为输出       */
    ledOn(LED3);                                                        /* 熄灭LED1                     */
}


/*********************************************************************************************************
系统时钟初始化，并返回时钟频率（单位：Hz）
*********************************************************************************************************/
/*
unsigned long clockInit(void)
{
    #if PLL_EN == 0                                                     // Not use PLL 不使用PLL        
    SysCtlClockSet(CCLK_DIV | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | EXT_CLK);
                                                                        // System clock=                
                                                                        // EXT_CLK/CCLK_DIV             
                                                                        // 系统时钟=EXT_CLK/CCLK_DIV    
    SysCtlLDOSet(SYSCTL_LDO_2_25V);                                     // 设置LDO输出电压              
    
    #else                                                               // Use PLL 使用PLL              
    SysCtlLDOSet(SYSCTL_LDO_2_75V);                                     // 设置LDO输出电压              
    SysCtlClockSet(CCLK_DIV | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | EXT_CLK);
                                                                        // System clock=200MHz/CCLK_DIV 
                                                                        // 系统时钟=200MHz/CCLK_DIV     
    #endif
    
    return(SysCtlClockGet());                                           // 返回当前的系统时钟频率       
}
*/

/*********************************************************************************************************
** 定时器的中断服务函数
** 每毫秒中断一次
*********************************************************************************************************/
void  Timer0A_ISR(void)
{
    TimerIntClear(T0_A_INT_TIMEOUT);                                    /* 清除定时器超时中断，重要！   */
    if (GulDelayMs < 0xFFFFFFFFUL) {
        GulDelayMs++;                                                   /* 在delayMs()函数中清零，用于  */
                                                                        /* 精确延时                     */
    }
}


/*********************************************************************************************************
精确延时函数 GulDelayMs由定时器中断每1ms加1
*********************************************************************************************************/
void  delayMs(unsigned long  ulVal)
{
    GulDelayMs = 0;
    while (GulDelayMs < ulVal);
}



/*********************************************************************************************************
定时器初始化
*********************************************************************************************************/
void  timer0Init(void)
{
    IntRegister(INT_TIMER0A, Timer0A_ISR);                              /* 注册中断服务函数             */
    
    SysCtlPeriEnable(SYSCTL_PERIPH_TIMER0);                             /* 使能定时器模块               */
    TimerConfigure(T0_CFG_32_PER);                                      /* 配置定时器为32位周期定时器   */
    TimerLoadSet(T0_A , GulSysClock/1000);                              /* 设置定时器初值               */
    TimerIntEnable(T0_A_INT_TIMEOUT);                                   /* 使能定时器超时中断           */
    IntEnable(INT_TIMER0A);                                             /* 使能定时器中断               */
    IntMasterEnable();                                                  /* 使能处理器中断               */
    GulDelayMs = 0;
    TimerEnable(T0_A);                                                  /* 使能定时器计数               */
}


/*********************************************************************************************************
防止JTAG失效
*********************************************************************************************************/
/*
void  jtagWait(void)
{
    SysCtlPeriEnable(KEY_PERI);                                         // 使能KEY所在的GPIO端口        
    GPIOPinTypeIn(KEY_BASE , KEY_PIN);                                  // 设置KEY所在管脚为输入        
    while (keyGet() == 0) {
        ;
    }
    SysCtlPeriDisable(KEY_PERI);                                        // 禁止KEY所在的GPIO端口        
}
*/
/*********************************************************************************************************
** End of File
*********************************************************************************************************/
