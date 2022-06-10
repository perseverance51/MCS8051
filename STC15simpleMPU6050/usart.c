#include "usart.h"

#define		BaudRate1		9600L
//#define		UART1_BUF_LENGTH	32
#define	RX1_Lenth		32			//串口接收缓冲长度
#define	Timer1_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 1 重装值， 对应300KHZ


unsigned char	TX1_Cnt;	//发送计数
unsigned char	RX1_Cnt;	//接收计数
 unsigned char idata RX1_Buffer[32];
bit	B_TX1_Busy;	//发送忙标志
	//========================================================================
// 函数: void UART1_int (void) interrupt UART1_VECTOR
// 描述: UART1中断函数。
// 参数: nine.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-11-28
// 备注: 
//========================================================================
void UART1_int (void) interrupt UART1_VECTOR
{
	if(RI)
	{
		RI = 0;
		RX1_Buffer[RX1_Cnt] = SBUF;		//保存一个字节
		if(++RX1_Cnt >= RX1_Lenth)	RX1_Cnt = 0;	//避免溢出处理
	}

	if(TI)
	{
		TI = 0;
		B_TX1_Busy = 0;		//清除发送忙标志
	}
}

//extern bit	B_TX1_Busy;	//发送忙标志
void InitSerialPort(void)
{ 
	SCON = 0x50;			//8位数据，可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TH1 = (u8)(Timer1_Reload >> 8);
	TL1 = (u8)Timer1_Reload;
	
	ET1 = 0;		//禁止定时器%d中断
	TR1 = 1;		//定时器1开始计时
			ES=1; 				//串口中断开关,采用查询法时不用打开中断      
    	REN=1;      			//串口为工作方式1，允许接收数据
    	SM0=0;					//SM0 SM1串口工作方式选择，01：8位异步收发，波特率由定时器决定
    	SM1=1;
		EA = 1;	//允许总中断
}
//========================================================================
// 函数: void PrintString1(u8 *puts)
// 描述: 串口1发送字符串函数。
// 参数: puts:  字符串指针.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-11-28
// 备注: 
//========================================================================
void PrintString1(u8 *puts)	//发送一个字符串
{
    for (; *puts != 0;	puts++)   	//遇到停止符0结束
	{
		SBUF = *puts;
		B_TX1_Busy = 1;
		while(B_TX1_Busy);
	}
}

//重写putchar函数,方便调用printf串口打印输出
 char putchar(unsigned char c)
{
	     SBUF = c; 					//发送数据
     while(!TI); 					//等待发送完成
     TI=0; 							//清零发送标志位;
	return c;
}

