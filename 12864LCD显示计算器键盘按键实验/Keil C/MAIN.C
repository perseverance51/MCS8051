#include <reg51.h>
#include <intrins.h>
#include "LCD_12864.H"
#include "KeyPad.H"
#include "DotMatri.H"
#define uchar unsigned char
#define uint unsigned int

//键盘扫描开启标志其值由外部中断 0 控制
bit KeyPressDown = 0;
uchar T_Count = 0;
sbit SPK = P3^7;
//Keybord_Chars 中数字与符号编码与键盘按键对照表
uchar code KeyPosTable[]=
{
    7,8,9,10,
    4,5,6,11,
    1,2,3,12,
    15,0,14,13
};
//蜂鸣器发声
void Beep()
{
    uchar i,x=20;
    while (x--)
    {
        for(i=0; i<120; i++);
        SPK=~SPK;
    }
}
//主程序
void main()
{
    uchar i;
    LCD_Initialize(); //初始化 LCD
    for (i=0; i<7; i++) //从第一页开始左边距 16 点显示 7 个 16*16 点阵的中文提示信息
    Display_A_WORD_String (1,16*(i+1),1,Word_String[i]);
    P1=0x0f;
    IE=0x83; //允许外部 0 和定时器 0 中断
    IT0=1; //设为下降沿中断方式外部中断 0 用于启停键盘
    TH0=(65536-5000)/256;//50ms 定时
    TL0=(65536-5000)%256;
    while(1)
    {
//如果有按键按下则处理按键
        if(KeyPressDown==1)
        {//扫描处理
            Beep();
            KeyPressDown=0;
            Display_A_char(4,55,Keyboard_Chars[KeyPosTable[KeyScan()] ]);
            TR0=0;
        }
    }
}
//外部中断 0 控制消抖延时
void EX0_INT() interrupt 0
{
    TR0=1;//开启定时器 0延时 300ms 消抖
}
//定时器用于消抖并确认有键按下启动主程序中的按键扫描
void T0_INT() interrupt 1
{
    if(++T_Count==6) //50*6=300ms 延时抖动
    {
        T_Count=0;
        KeyPressDown=1; //确定有键按下
    }
    TH0=(65526-50000)/256; //50ms 定时
    TL0=(65526-50000)%256;
}