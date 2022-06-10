


/*************	功能说明	**************

双串口全双工中断方式收发通讯程序。
注意：
通过PC向MCU发送数据, MCU收到后通过串口把收到的数据原样返回.

******************************************/


#include	"config.h"
#include "stdio.h"
#include "USART.h"


void  delay_ms(unsigned int ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 13000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}



//========================================================================
// 函数: void main(void)
// 描述: 主函数。
// 参数: none.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-11-28
// 备注: 
//========================================================================
void main(void)
{
	unsigned int n;
//	P0M1 = 0;	P0M0 = 0;	//设置为准双向口
//	P1M1 = 0;	P1M0 = 0;	//设置为准双向口
//	P2M1 = 0;	P2M0 = 0;	//设置为准双向口
	P3M1 = 0;	P3M0 = 0;	//设置为准双向口
//	P4M1 = 0;	P4M0 = 0;	//设置为准双向口
//	P5M1 = 0;	P5M0 = 0;	//设置为准双向口
//	P6M1 = 0;	P6M0 = 0;	//设置为准双向口
//	P7M1 = 0;	P7M0 = 0;	//设置为准双向口
		//	S1_USE_P30P31();
 InitSerialPort(); 		//初始化串口,波特率9600，8bit数据位，1停止位，无校验
	EA = 1;	//允许总中断

//	PrintString1("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//SUART1发送一个字符串

	while (1)
	{
		delay_ms(500);
		n = ~0;
		printf("uint = ~0:%u \r\n",n);	//SUART1发送一个字符串
		n = !0;
		printf("uint = !0:%u \r\n",n);	//SUART1发送一个字符串
		P10 = !P10;
	}
}



