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
** Created by:              ��չ��
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
����Timer
*********************************************************************************************************/
#define  T0_A                   TIMER0_BASE , TIMER_A
#define  T0_CFG_32_PER          TIMER0_BASE , TIMER_CFG_32_BIT_PER
#define  T0_A_INT_TIMEOUT       TIMER0_BASE , TIMER_TIMA_TIMEOUT


/*********************************************************************************************************
����ȫ�ֵ�ϵͳʱ�ӱ���
*********************************************************************************************************/
volatile unsigned long  GulSysClock  =  12000000UL;


volatile INT32U GulDelayMs;


/*********************************************************************************************************
��ͼ��������
*********************************************************************************************************/
#define V_INDEXOF    30                                                 /* ����ͼƬ������ƫ����         */

//#define X0           ((240 - IMAGE_WIDTH) / 2)                          /* IMAGE_WIDTH��gImage.h�ж���  */
//#define Y0           ((320 - IMAGE_HIGH)  / 2 - V_INDEXOF)              /* IMAGE_HIGH��gImage.h�ж���   */
//#define X1           ((240 - IMAGE_WIDTH) / 2 + IMAGE_WIDTH - 1)
//#define Y1           ((320 - IMAGE_HIGH)  / 2 + IMAGE_HIGH - 1 - V_INDEXOF)
const RECTANGLE GtFullZoon = {0,0,239,319,0,0};                         /* ����ȫ������                 */
//const RECTANGLE GtImageZoon = {X0, Y0, X1, Y1, 0, 0};                   /* ����ͼƬ��ʾ����             */


/*********************************************************************************************************
** Function name:           main
** Descriptions:            �÷���������ʾ��FTFҺ����ʾͼƬ���ַ���
*********************************************************************************************************/
int  main(void)
{
    //unsigned short usColor=0xFFFF;
    //jtagWait();                                                       /* ��ֹJTAGʧЧ����Ҫ��         */
    GulSysClock  =  clockInit();                                        /* ��ȡϵͳʱ�ӣ���λ��Hz       */
    //clockInit();
    timer0Init();                                                       /* ��ʱ����ʼ��                 */
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

                                                                        // ����ı�����                 
        //pixelDraw(100, 140, RED);                                       // ����                         
        //usColor = pixelRead(100,140);
        //lineDrawH(80, 230, 50, usColor);                                // ������                       
        //lineDrawV(200, 10, 210, BLUE);                                  // ������                       
        
//        delayMs(2000);                                                  // 2s                           
//        ledToggle(LED3);                                                // ָʾ��ȡ��                   
//        rectFill((RECTANGLE *)&GtFullZoon, WHITE);                      // ����ɫ����                 
//        pictureFill((RECTANGLE *)&GtImageZoon, (unsigned short *)gImage_pic1, 160 * 99);
//        pictureFill8((RECTANGLE *)&GtImageZoon, (unsigned char *)gImage_pic1, 240 * 149);
//        pictureFill((RECTANGLE *)&GtImageZoon, (unsigned short *)gImage_pic3, 160 * 106);
//        pictureFill8((RECTANGLE *)&GtImageZoon, (unsigned char *)gImage_pic2, 160 * 240);
                                                                        // ���ͼƬ                 

    //rectFill((RECTANGLE *)&GtFullZoon, WHITE);                      // ����ɫ����
    
/*
    while (1)
    {        
        if(GetpXY(&pX, &pY))
        {
            RECTANGLE TouchRect = {ClippX(pX-15), ClippY(pY-15), ClippX(pX+14), ClippY(pY+14), 0, 0};
            sprintf(pstr, "X: %3d, Y: %3d                Count: %10d", pX, pY, touchcount++);
            rectFill((RECTANGLE *)&TouchRect, GREEN);
            writeString("Nyan Nyan Nyan ~~\r\n ����Yu Chun De Ren Lei",20,80,RED);
            writeString((uint8*)pstr, 20, 220, RED);                          // ����ı�����
            RECTANGLE StrRect = {20, 220, 220, 250, 0, 0};
            delayMs(100);
            rectFill((RECTANGLE *)&TouchRect, WHITE);
            rectFill((RECTANGLE *)&StrRect, WHITE);
        }
        else
        {
            sprintf(pstr, "No Touch");
            writeString("Nyan Nyan Nyan ~~\r\n ����Yu Chun De Ren Lei",20,80,RED);
            writeString((uint8*)pstr, 20, 220, RED);                          // ����ı�����
            delayMs(100);
            RECTANGLE StrRect = {20, 220, 220, 250, 0, 0};
            rectFill((RECTANGLE *)&StrRect, WHITE);
        }
        //delayMs(2000);                                                  // 2s                           
        //ledToggle(LED3);                                                // ȡ��ָʾ��
     }
*/
}


/*********************************************************************************************************
LED��ʼ��
*********************************************************************************************************/
void ledInit(void)
{
    SysCtlPeriEnable(LED_PERI);                                         /* ʹ��LED���ڵ�GPIO�˿�        */
    GPIOPinTypeOut(LED3);                                               /* ����LED1���ڹܽ�Ϊ���       */
    ledOn(LED3);                                                        /* Ϩ��LED1                     */
}


/*********************************************************************************************************
ϵͳʱ�ӳ�ʼ����������ʱ��Ƶ�ʣ���λ��Hz��
*********************************************************************************************************/
/*
unsigned long clockInit(void)
{
    #if PLL_EN == 0                                                     // Not use PLL ��ʹ��PLL        
    SysCtlClockSet(CCLK_DIV | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | EXT_CLK);
                                                                        // System clock=                
                                                                        // EXT_CLK/CCLK_DIV             
                                                                        // ϵͳʱ��=EXT_CLK/CCLK_DIV    
    SysCtlLDOSet(SYSCTL_LDO_2_25V);                                     // ����LDO�����ѹ              
    
    #else                                                               // Use PLL ʹ��PLL              
    SysCtlLDOSet(SYSCTL_LDO_2_75V);                                     // ����LDO�����ѹ              
    SysCtlClockSet(CCLK_DIV | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | EXT_CLK);
                                                                        // System clock=200MHz/CCLK_DIV 
                                                                        // ϵͳʱ��=200MHz/CCLK_DIV     
    #endif
    
    return(SysCtlClockGet());                                           // ���ص�ǰ��ϵͳʱ��Ƶ��       
}
*/

/*********************************************************************************************************
** ��ʱ�����жϷ�����
** ÿ�����ж�һ��
*********************************************************************************************************/
void  Timer0A_ISR(void)
{
    TimerIntClear(T0_A_INT_TIMEOUT);                                    /* �����ʱ����ʱ�жϣ���Ҫ��   */
    if (GulDelayMs < 0xFFFFFFFFUL) {
        GulDelayMs++;                                                   /* ��delayMs()���������㣬����  */
                                                                        /* ��ȷ��ʱ                     */
    }
}


/*********************************************************************************************************
��ȷ��ʱ���� GulDelayMs�ɶ�ʱ���ж�ÿ1ms��1
*********************************************************************************************************/
void  delayMs(unsigned long  ulVal)
{
    GulDelayMs = 0;
    while (GulDelayMs < ulVal);
}



/*********************************************************************************************************
��ʱ����ʼ��
*********************************************************************************************************/
void  timer0Init(void)
{
    IntRegister(INT_TIMER0A, Timer0A_ISR);                              /* ע���жϷ�����             */
    
    SysCtlPeriEnable(SYSCTL_PERIPH_TIMER0);                             /* ʹ�ܶ�ʱ��ģ��               */
    TimerConfigure(T0_CFG_32_PER);                                      /* ���ö�ʱ��Ϊ32λ���ڶ�ʱ��   */
    TimerLoadSet(T0_A , GulSysClock/1000);                              /* ���ö�ʱ����ֵ               */
    TimerIntEnable(T0_A_INT_TIMEOUT);                                   /* ʹ�ܶ�ʱ����ʱ�ж�           */
    IntEnable(INT_TIMER0A);                                             /* ʹ�ܶ�ʱ���ж�               */
    IntMasterEnable();                                                  /* ʹ�ܴ������ж�               */
    GulDelayMs = 0;
    TimerEnable(T0_A);                                                  /* ʹ�ܶ�ʱ������               */
}


/*********************************************************************************************************
��ֹJTAGʧЧ
*********************************************************************************************************/
/*
void  jtagWait(void)
{
    SysCtlPeriEnable(KEY_PERI);                                         // ʹ��KEY���ڵ�GPIO�˿�        
    GPIOPinTypeIn(KEY_BASE , KEY_PIN);                                  // ����KEY���ڹܽ�Ϊ����        
    while (keyGet() == 0) {
        ;
    }
    SysCtlPeriDisable(KEY_PERI);                                        // ��ֹKEY���ڵ�GPIO�˿�        
}
*/
/*********************************************************************************************************
** End of File
*********************************************************************************************************/
