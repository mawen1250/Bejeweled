

#ifndef __MAIN_H
#define __MAIN_H



/*********************************************************************************************************
System Clock(CCLK) Setting   系统时钟(CCLK) 设定
CCLK must be less than or equal to  20MHz/50MHz(depend on the max. cclk of the CPU)
CCLK必须小于或等于20MHz/50MHz(根据单片机的最大CCLK而定)
If PLL_EN=0, CCLK=EXT_CLK/CCLK_DIV, CCLK must <=20MHZ/50MHz 
If PLL_EN>0, CCLK=200MHz/CCLK_DIV, CCLK must <=20MHZ/50MHz
*********************************************************************************************************/
#define  EXT_CLK            SYSCTL_XTAL_6MHZ                            /* external clock 外部时钟     */ 
#define  PLL_EN             1                                           /* 1：Enable PLL 使能PLL        */
#define  CCLK_DIV           SYSCTL_SYSDIV_4                             /* CCLK divider CCLK分频系数    */

  
/*********************************************************************************************************
  将较长的标识符定义成较短的形式
*********************************************************************************************************/
#define  SysCtlPeriEnable       SysCtlPeripheralEnable
#define  SysCtlPeriDisable      SysCtlPeripheralDisable
#define  GPIOPinTypeIn          GPIOPinTypeGPIOInput
#define  GPIOPinTypeOut         GPIOPinTypeGPIOOutput
#define  GPIOPinTypeOD          GPIOPinTypeGPIOOutputOD


 
/*********************************************************************************************************
  定义KEY
*********************************************************************************************************/
#define  KEY_PERI               SYSCTL_PERIPH_GPIOG
#define  KEY_BASE               GPIO_PORTG_BASE
#define  KEY_PIN                GPIO_PIN_5
#define  keyGet()               GPIOPinRead(KEY_BASE , KEY_PIN)




/*********************************************************************************************************
  定义LED
*********************************************************************************************************/
#define  LED_PERI               SYSCTL_PERIPH_GPIOD | SYSCTL_PERIPH_GPIOG
#define  LED_BASE               GPIO_PORTG_BASE
#define  LED1                   GPIO_PORTD_BASE,GPIO_PIN_0
#define  LED2                   GPIO_PORTG_BASE,GPIO_PIN_2
#define  LED3                   GPIO_PORTG_BASE,GPIO_PIN_3
#define  ledOn(LED_PIN)         GPIOPinWrite(LED_PIN, 0x00)
#define  ledOff(LED_PIN)        GPIOPinWrite(LED_PIN, 0xFF)
#define  ledToggle(LED_PIN)     GPIOPinWrite(LED_PIN, ~GPIOPinRead(LED_PIN))



extern void delayMs(unsigned long  ulVal);
extern void timer0Init(void);
extern void jtagWait(void);
extern unsigned long clockInit(void);
extern void ledInit(void);
#endif
