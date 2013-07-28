#include "systemInit.h"


#define LCD_TOUCH_PERIPH        SYSCTL_PERIPH_GPIOC | SYSCTL_PERIPH_GPIOB
#define LCD_TOUCH_PORT          GPIO_PORTC_BASE
#define LCD_TOUCH_CLK           GPIO_PIN_4
#define LCD_TOUCH_DIN           GPIO_PIN_5
#define LCD_TOUCH_BUSY          GPIO_PIN_6
#define LCD_TOUCH_DOUT          GPIO_PIN_7

#define LCD_TOUCH_EN_BASE       GPIO_PORTB_BASE
#define LCD_TOUCH_EN            GPIO_PIN_1   

#define CHX   0xD0      //B10010000
#define CHY   0x90      //B11010000
extern void ads7843_init(void);
extern void ads7843_write(unsigned char cmd);
extern unsigned short ads7843_read(void);
extern unsigned short ads7843_get_X(void);
extern unsigned short ads7843_get_Y(void);
