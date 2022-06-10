

#include "stdio.h"
#include "config.h"
//#include "STC15Fxxxx.H"
#include "MPU6050.H"

//#include "usart.h"
#include "delay.h"
#define		BaudRate1		9600L
#define		UART1_BUF_LENGTH	32
#define	RX1_Lenth		32			//串口接收缓冲长度
#define	Timer1_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 1 重装值， 对应300KHZ

u8	TX1_Cnt;	//发送计数
u8	RX1_Cnt;	//接收计数
bit	B_TX1_Busy;	//发送忙标志

u8 	idata RX1_Buffer[UART1_BUF_LENGTH];	//接收缓冲

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
	
}
//重写putchar函数,方便调用printf串口打印输出
 char putchar(unsigned char c)
{
	     SBUF = c; 					//发送数据
     while(!TI); 					//等待发送完成
     TI=0; 							//清零发送标志位;
	return c;
}
void	UART1_config(u8 brt)	// 选择波特率, 2: 使用Timer2做波特率, 其它值: 使用Timer1做波特率.
{
	/*********** 波特率使用定时器2 *****************/
	if(brt == 2)
	{
		AUXR |= 0x01;		//S1 BRT Use Timer2;
		//Timer1_Reload;
	}

	/*********** 波特率使用定时器1 *****************/
	else
	{
		TR1 = 0;
		AUXR &= ~0x01;		//S1 BRT Use Timer1;
		AUXR |=  (1<<6);	//Timer1 set as 1T mode
		TMOD &= ~(1<<6);	//Timer1 set As Timer
		TMOD &= ~0x30;		//Timer1_16bitAutoReload;
		TH1 = (u8)((65536UL - (MAIN_Fosc / 4) / BaudRate1) / 256);
		TL1 = (u8)((65536UL - (MAIN_Fosc / 4) / BaudRate1) % 256);
		ET1 = 0;	//禁止中断
		INT_CLKO &= ~0x02;	//不输出时钟
		TR1  = 1;
		
	}
	/*************************************************/
		//SCON = 0x50;			//8位数据，可变波特率
	//SCON = (SCON & 0x3f) | 0x40;	//UART1模式, 0x00: 同步移位输出, 0x40: 8位数据,可变波特率, 0x80: 9位数据,固定波特率, 0xc0: 9位数据,可变波特率
//	PS  = 1;	//高优先级中断
	ES  = 1;	//允许中断
	REN = 1;	//允许接收
	P_SW1 &= 0x3f;
	P_SW1 |= 0x80;		//UART1 switch to, 0x00: P3.0 P3.1, 0x40: P3.6 P3.7, 0x80: P1.6 P1.7 (必须使用内部时钟)
	PCON2 |=  (1<<4);	//内部短路RXD与TXD, 做中继, ENABLE,DISABLE

	B_TX1_Busy = 0;
	TX1_Cnt = 0;
	RX1_Cnt = 0;
}
	void main(void)
	{

	P0M1 = 0;	P0M0 = 0;	//设置为准双向口
	P1M1 = 0;	P1M0 = 0;	//设置为准双向口
	P2M1 = 0;	P2M0 = 0;	//设置为准双向口
	P3M1 = 0;	P3M0 = 0;	//设置为准双向口
	P4M1 = 0;	P4M0 = 0;	//设置为准双向口
	P5M1 = 0;	P5M0 = 0;	//设置为准双向口
	P6M1 = 0;	P6M0 = 0;	//设置为准双向口
	P7M1 = 0;	P7M0 = 0;	//设置为准双向口
	UART1_config(1);	// 选择波特率, 2: 使用Timer2做波特率, 其它值: 使用Timer1做波特率.
		InitSerialPort();
	//PrintString1("STC15W408AS UART1 Test Prgramme!\r\n");	//SUART1发送一个字符串
	EA = 1;	//允许总中断
	while (1)
	{
		delay_ms(500);
		printf("perseverance51 \r\n");	//SUART1发送一个字符串
		P10 = !P10;
	}
}


