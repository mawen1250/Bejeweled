#include "touch.h"

void ads7843_init(void)
{
    SysCtlPeriEnable(LCD_TOUCH_PERIPH);
    GPIOPinTypeOut(LCD_TOUCH_PORT,LCD_TOUCH_CLK);
    GPIOPinTypeOut(LCD_TOUCH_PORT,LCD_TOUCH_DIN);
    GPIOPinTypeIn(LCD_TOUCH_PORT,LCD_TOUCH_BUSY);
    GPIOPinTypeIn(LCD_TOUCH_PORT,LCD_TOUCH_DOUT);
    GPIOPinTypeOut(LCD_TOUCH_EN_BASE,LCD_TOUCH_EN);
}
void ads7843_select(void)
{
    GPIOPinWrite(LCD_TOUCH_EN_BASE,LCD_TOUCH_EN,0x00);  //cs = 0
}
void ads7843_deselect(void)
{
    GPIOPinWrite(LCD_TOUCH_EN_BASE,LCD_TOUCH_EN,0xFF);  //cs = 1
}
void ads7843_write(unsigned char cmd)
{
    unsigned char i, temp;
    temp = 0x80;
    GPIOPinWrite(LCD_TOUCH_PORT,LCD_TOUCH_CLK,0);     //clk = 0
    for(i = 0; i < 8; i++)
    {/*将控制字分解成位传入ads7843,从而ads7843发送命令*/
        if(cmd&temp) GPIOPinWrite(LCD_TOUCH_PORT,LCD_TOUCH_DIN,0xFF);//DIN = 1
        else  GPIOPinWrite(LCD_TOUCH_PORT,LCD_TOUCH_DIN,0x00);//DIN = 0;
        //模拟时钟发送数据
        GPIOPinWrite(LCD_TOUCH_PORT,LCD_TOUCH_CLK,0xFF);     //clk = 1
        SysCtlDelay(100); // 延时
        GPIOPinWrite(LCD_TOUCH_PORT,LCD_TOUCH_CLK,0x00);     //clk = 0
        SysCtlDelay(100); // 延时
        temp = temp>>1;
    }
    return;
}
unsigned short ads7843_read(void)//12位转换
{
    unsigned  short ack =0;
    unsigned char i;
    GPIOPinWrite(LCD_TOUCH_PORT,LCD_TOUCH_CLK,0);     //clk = 0
    SysCtlDelay(50); // 延时1ms
  //  while(GPIOPinRead(LCD_TOUCH_PORT,LCD_TOUCH_BUSY) & 0x40){}; //while busy == 1
    for(i = 0; i < 12; i++)
    {
        GPIOPinWrite(LCD_TOUCH_PORT,LCD_TOUCH_CLK,0xff);     //clk = 1
        SysCtlDelay(100); // 延时
        GPIOPinWrite(LCD_TOUCH_PORT,LCD_TOUCH_CLK,0X00);     //clk = 0 /*时钟下降沿读取*/
        SysCtlDelay(50); // 延时
        if(GPIOPinRead(LCD_TOUCH_PORT,LCD_TOUCH_DOUT)&0x80)
            ack += 1;
        SysCtlDelay(50); // 延时
        ack = ack << 1;
    }
    for(i =0; i < 4; i++)
    {
        GPIOPinWrite(LCD_TOUCH_PORT,LCD_TOUCH_CLK,0xFF);     //clk = 1
        SysCtlDelay(100); // 延时
        GPIOPinWrite(LCD_TOUCH_PORT,LCD_TOUCH_CLK,0x00);     //clk = 0 
        SysCtlDelay(100); // 延时
    }
    return ack;
}
unsigned short ads7843_get_X(void)
{
    unsigned short X;
    ads7843_select();
    SysCtlDelay(50); // 延时
    ads7843_write(CHX);
    X = ads7843_read();
    ads7843_deselect();
    return X;
}

    
    unsigned short ads7843_get_Y(void)
{
  
    unsigned short Y;
    ads7843_select();
    SysCtlDelay(50);
    ads7843_write(CHY);
    Y = ads7843_read();
    ads7843_deselect();
    return Y;
}