#include "reg51.h"
typedef unsigned char u8;
typedef unsigned int u16;
//-----------------------------------------------
#define FOSC 11059200L
#define T5MS (65536-5*FOSC/1000) //1T模式，110592MHz
//#define T5MS (65536-5*FOSC/12/1000) //12T模式，110592MHz
sfr AUXR = 0x8e; //Auxiliary register
sbit P10 = P1^0;
bit	B_5ms;			//5ms标志
 unsigned int 	msecond;		//计时
//-----------------------------------------------
/* Timer0 interrupt routine */
void tm0_isr() interrupt 1 using 1
{
		B_5ms = 1;
}
void Timer0Init(void)		//5毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	// AUXR &= 0x7f; //定时器0为12T模式
	TMOD &= 0xF0;		//设置定时器模式
//	TL0 = 0x00;		//设置定时初始值
//	TH0 = 0x28;		//设置定时初始值
		 TL0 = T5MS; //初始化计时值
		 TH0 = T5MS >> 8;
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1; //使能定时器0中断
	EA = 1;
}

//-----------------------------------------------
/* main program */
void main()
{
	Timer0Init();
	while(1)
	{
	if(B_5ms)	//5ms到达标志位
		{
			B_5ms = 0;
			if(++msecond >= 200)	//计数到达1S
			{
				msecond = 0;
				 P10 = ! P10; //将测试口取反
			}
		}
	}
}