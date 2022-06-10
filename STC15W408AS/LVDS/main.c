//假定测试芯片的工作频率为18.432MHz

//注意:测试本示例时,需在ISP下载时将低压复位功能关闭

#include "reg51.h"
#include "intrins.h"

sbit ELVD = IE^6;                   //低压检测中断使能位
sbit P11 = P1^1;                    //测试口

#define LVDF    0x20                //PCON.5,低压检测中断标志位

//-----------------------------------------

void LVD_ISR() interrupt 6 using 1
{
    P11 = !P11;                     //测试口
    PCON &= ~LVDF;                  //向PCON.5写0清LVD中断
}

void main()
{
    PCON &= ~LVDF;                  //上电后需要清LVD中断标志位
    ELVD = 1;                       //使能LVD中断
    EA = 1;                         //打开总中断开关
    P11= 1;
    while (1)
    {
        _nop_();
        _nop_();
    }
}
