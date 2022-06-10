
/*************	功能说明	**************
 使用说明：使用前确认MAIN_Fosc(时钟频率）和 波特率
双串口全双工中断方式收发通讯程序。
设置Timer2做波特率发生器
通过PC向MCU发送数据, MCU收到后通过串口把收到的数据原样返回.

******************************************/


#define 	MAIN_Fosc			16000000uL	//定义主时钟
#include "stdio.h"
#include	"STC15Fxxxx.H"

#define		BaudRate1			115200uL		//波特率
#define		UART1_BUF_LENGTH	32
#define		Timer2_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer2 重装值

u8	TX1_Cnt;	//发送计数
u8	RX1_Cnt;	//接收计数
bit	B_TX1_Busy;	//发送忙标志

u8 	idata RX1_Buffer[UART1_BUF_LENGTH];	//接收缓冲


void	UART1_config();	// 选择波特率使用Timer2做波特率
 char putchar(unsigned char c);//串口打印函数
//void 	PrintString1(u8 *puts);
void  delay_ms(unsigned int ms);//延时函数

//========================================================================
// 函数: void main(void)
// 描述: 主函数。
//========================================================================
void main(void)
{
	P1M1 |=0x00; P1M0 |=0x01;//设置P10为推挽输出
//	P0M1 = 0;	P0M0 = 0;	//设置为准双向口
//	P1M1 = 0;	P1M0 = 0;	//设置为准双向口
//	P2M1 = 0;	P2M0 = 0;	//设置为准双向口
//	P3M1 = 0;	P3M0 = 0;	//设置为准双向口
//	P4M1 = 0;	P4M0 = 0;	//设置为准双向口
//	P5M1 = 0;	P5M0 = 0;	//设置为准双向口
//	P6M1 = 0;	P6M0 = 0;	//设置为准双向口
//	P7M1 = 0;	P7M0 = 0;	//设置为准双向口
	S1_USE_P30P31();
	UART1_config();	// 选择波特率, 2: 使用Timer2做波特率, 其它值: 使用Timer1做波特率.
	EA = 1;	//允许总中断

	printf("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//UART1发送一个字符串

	while (1)
	{
		printf("\t perseverance51 \r\n");	
		delay_ms(500);
		P10 = ~P10;
		if((TX1_Cnt != RX1_Cnt) && (!B_TX1_Busy))	//收到数据, 发送空闲
		{
			SBUF = RX1_Buffer[TX1_Cnt];		//把收到的数据远样返回
			B_TX1_Busy = 1;
			if(++TX1_Cnt >= UART1_BUF_LENGTH)	TX1_Cnt = 0;
		}
	}
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
///*
//void PrintString1(u8 *puts)	//发送一个字符串
//{
//    for (; *puts != 0;	puts++)   	//遇到停止符0结束
//	{
//		SBUF = *puts;
//		B_TX1_Busy = 1;
//		while(B_TX1_Busy);
//	}
//}
//*/
//----------------------------------------------------------   
//  函数名称：putchar(unsigned char c)
//  函数功能：串口发送一个字节
//		功能：包含stdio.h实现printf打印
//----------------------------------------------------------  
 char putchar(unsigned char c)
{
	     SBUF = c; 					//发送数据
			B_TX1_Busy = 1;
		while(B_TX1_Busy);
//     while(!TI); 					//等待发送完成
//     TI=0; 							//清零发送标志位;
	return c;
}

//========================================================================
// 函数: void	UART1_config(u8 brt)
// 描述: UART1初始化函数。
// 参数: brt: 选择波特率, 2: 使用Timer2做波特率, 其它值: 使用Timer1做波特率.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-11-28
// 备注: 
//========================================================================
void	UART1_config()	// 选择波特率使用Timer2做波特率
{
	/*********** 波特率使用定时器2 *****************/
		SCON = 0x50;		//8位数据,可变波特率
		AUXR |= 0x01;		//S1 BRT Use Timer2;
		AUXR |= 0x04;		//定时器时钟1T模式
	TH2 = Timer2_Reload	 / 256;
	TL2 = Timer2_Reload	 % 256;
		AUXR |= 0x10;		//定时器2开始计时

//SCON = (SCON & 0x3f) | 0x40;	//UART1模式, 0x00: 同步移位输出, 0x40: 8位数据,可变波特率, 0x80: 9位数据,固定波特率, 0xc0: 9位数据,可变波特率
//	PS  = 1;	//高优先级中断
	ES  = 1;	//允许中断
	REN = 1;	//允许接收
	P_SW1 &= 0x3f;//串口在[P3.0/RxD,P3.1/TxD]
	P_SW1 |= 0x00;		//UART1 switch to, 0x00: P3.0 P3.1, 0x40: P3.6 P3.7, 0x80: P1.6 P1.7 (必须使用内部时钟)
//	PCON2 |=  (1<<4);	//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
	PCON2 &=0x3f;//主时钟不对外输出时钟,
	B_TX1_Busy = 0;
	TX1_Cnt = 0;
	RX1_Cnt = 0;
}


//========================================================================
// 函数: void UART1_int (void) interrupt UART1_VECTOR
// 描述: UART1中断函数。
// 参数: UART1_VECTOR（4）
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
		RX1_Buffer[RX1_Cnt] = SBUF;
		if(++RX1_Cnt >= UART1_BUF_LENGTH)	RX1_Cnt = 0;	//防溢出
	}

	if(TI)
	{
		TI = 0;
		B_TX1_Busy = 0;
	}
}

//========================================================================
// 函数: void  delay_ms(unsigned char ms)
// 描述: 延时函数。
// 参数: ms,要延时的ms数, 这里只支持1~255ms. 自动适应主时钟.
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
//========================================================================
void  delay_ms(unsigned int ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 13000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}