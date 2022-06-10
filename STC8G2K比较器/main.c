/*STC8G2单片机中断方式获取P36引脚电压值与内部1.19V电压进行比较，将结果输出到P10，
当P36引脚电压高于1.19v时，则P10输出高电平，低于1.19V则触发中断，P10输出低电平*/
#include <STC8.H>

//#include "reg51.h"
#include "intrins.h"
#include <stdio.h> 	       // 为使用KEIL自带的库函数printf而加入
//sfr CMPCR1 = 0xe6;
//sfr CMPCR2 = 0xe7;
//sfr P0M1 = 0x93;
//sfr P0M0 = 0x94;
//sfr P1M1 = 0x91;
//sfr P1M0 = 0x92;
//sfr P2M1 = 0x95;
//sfr P2M0 = 0x96;
//sfr P3M1 = 0xb1;
//sfr P3M0 = 0xb2;
//sfr P4M1 = 0xb3;
//sfr P4M0 = 0xb4;
//sfr P5M1 = 0xc9;
//sfr P5M0 = 0xca;
//sbit P10 = P1^0;
//sbit P11 = P1^1;
void Delay1000ms()		//@30.000MHz
{
	unsigned char i, j, k;
	_nop_();
	i = 153;
	j = 49;
	k = 161;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void UART_init(void)
{		  
	//下面代码设置定时器1
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x5F;		//设置定时初始值
	TH1 = 0xFE;		//设置定时初始值
	ET1 = 0;		//禁止定时器%d中断
	TR1 = 1;		//定时器1开始计时
//	TMOD = 0x20;	// 0010 0000 定时器1工作于方式2（8位自动重装方式）
//	TH1  = 0xFD;	// 波特率：9600 /11.0592MHZ
//	TL1  = 0xFD;	// 波特率：9600 /11.0592MHZ

	//下面代码设置定串口
//	AUXR = 0x00;             // 很关键，使用定时器1作为波特率发生器，S1ST2=0
//	SCON = 0x50; 	// 0101 0000 SM0.SM1=01(最普遍的8位通信）,REN=1（允许接受）
	TI=1;			// 很关键，使用printf函数时必须有此命令
} 

//void cmp() interrupt 21 using 1
//{
//	CMPCR1 &= 0xbf;//手动清除清除中断标志，1011,1111
//	P10 = (CMPCR1 & 0x01); //将比较器结果CMPRES输出到测试口显示
//}
void main()
{	
	int value;
    P0M0 = 0x00;//设置个IO端口为准双向口
    P0M1 = 0x00;
    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;
		P10 =0;//初始P10端口为低电平
    CMPCR2 = 0x00;
    CMPCR2 &= ~0x80; //比较器正向输出
// CMPCR2 |= 0x80; //比较器反向输出
    CMPCR2 &= ~0x40; //使能 0.1us 滤波
// CMPCR2 |= 0x40; //禁止 0.1us 滤波
// CMPCR2 &= ~0x3f; //比较器结果直接输出
    CMPCR2 |= 0x10; //比较器结果经过 16 个去抖时钟后输出
    CMPCR1 = 0x00;
    CMPCR1 |= 0x30; //使能比较器边沿中断
// CMPCR1 &= ~0x20; //禁止比较器上升沿中断
// CMPCR1 |= 0x20; //使能比较器上升沿中断
// CMPCR1 &= ~0x10; //禁止比较器下降沿中断
// CMPCR1 |= 0x10; //使能比较器下降沿中断
    CMPCR1 &= ~0x08; //P3.7 为 CMP+输入脚
// CMPCR1 |= 0x08; //ADC 输入脚为 CMP+输入脚
//		CMPCR1 &= ~0x04; //内部 1.19V 参考信号源为 CMP-输入脚
   CMPCR1 |= 0x04; //P3.6 为 CMP-输入脚
// CMPCR1 &= ~0x02; //禁止比较器输出
    CMPCR1 |= 0x02; //使能比较器输出
    CMPCR1 |= 0x80; //使能比较器模块
		UART_init();	 // 初始化串口
		EA =1;
    while (1)
    {
  value = (CMPCR1 & 0x01);
	P10 =value; //将比较器结果CMPRES输出到测试口显示
			printf ("value= %d \n",value); 	       // 10进制输出
			Delay1000ms();
    }
}
