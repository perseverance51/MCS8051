//#include <STC12C5A60S2.H>
//#include <STC8.H>
//#include <STC15F2K60S2.H>
#include "STC15Wxx.h"
#include "intrins.h"
#include <stdio.h> 	       // 为使用KEIL自带的库函数printf而加入
bit busy;
void UartInit(void)		//9600bps@16MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x5F;		//设置定时初始值
	T2H = 0xFE;		//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	TI=1;			// 很关键，使用printf函数时必须有此命令
}
/*----------------------------
UART 中断服务程序
-----------------------------*/
//void Uart() interrupt 4 using 1
//{
// if (RI)
// {
// RI = 0; //清除RI位
// P0 = SBUF; //P0显示串口数据
////P22 = RB8; //P2.2显示校验位
// }
// if (TI)
// {
// TI = 0; //清除TI位
// busy = 0; //清忙标志
// }
//}
void Delay1000ms()		//@16MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 61;
	j = 204;
	k = 243;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void main()
{
	UartInit();
//	EA =1;
	 while(1)
		 	{
				printf("perseverance51 \n"); 
				Delay1000ms();
				printf ("hello world! \n"); 
	}
}