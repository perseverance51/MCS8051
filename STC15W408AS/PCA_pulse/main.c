//用CCP/PCA的16位捕获模式测脉冲宽度的程序
//单片机型号：STC15W408AS
//工作频率一般为11.0592MHz

#include "reg51.h"
#include "intrins.h"
#include <stdio.h> 	       // 为使用KEIL自带的库函数printf而加入
#define FOSC    11059200L

typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long DWORD;
BYTE cnt;                           //存储PCA计时溢出次数
DWORD count0;                       //记录上一次的捕获值
DWORD count1;                       //记录本次的捕获值
DWORD length;                       //存储信号的时间长度(count1 - count0)

sfr P0M1 = 0x93;
sfr P0M0 = 0x94;
sfr P1M1 = 0x91;
sfr P1M0 = 0x92;
sfr P2M1 = 0x95;
sfr P2M0 = 0x96;
sfr P3M1 = 0xb1;
sfr P3M0 = 0xb2;
sfr P4M1 = 0xb3;
sfr P4M0 = 0xb4;
sfr P5M1 = 0xC9;
sfr P5M0 = 0xCA;

sfr P_SW1       = 0xA2;             //外设功能切换寄存器1

#define CCP_S0 0x10                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5

sfr CCON        =   0xD8;           //PCA控制寄存器
sbit CCF0       =   CCON^0;         //PCA模块0中断标志
sbit CCF1       =   CCON^1;         //PCA模块1中断标志
sbit CR         =   CCON^6;         //PCA定时器运行控制位
sbit CF         =   CCON^7;         //PCA定时器溢出标志
sfr CMOD        =   0xD9;           //PCA模式寄存器
sfr CL          =   0xE9;           //PCA定时器低字节
sfr CH          =   0xF9;           //PCA定时器高字节
sfr CCAPM0      =   0xDA;           //PCA模块0模式寄存器
sfr CCAP0L      =   0xEA;           //PCA模块0捕获寄存器 LOW
sfr CCAP0H      =   0xFA;           //PCA模块0捕获寄存器 HIGH
sfr CCAPM1      =   0xDB;           //PCA模块1模式寄存器
sfr CCAP1L      =   0xEB;           //PCA模块1捕获寄存器 LOW
sfr CCAP1H      =   0xFB;           //PCA模块1捕获寄存器 HIGH
sfr CCAPM2      =   0xDC;           //PCA模块2模式寄存器
sfr CCAP2L      =   0xEC;           //PCA模块2捕获寄存器 LOW
sfr CCAP2H      =   0xFC;           //PCA模块2捕获寄存器 HIGH
sfr PCA_PWM0    =   0xf2;           //PCA模块0的PWM寄存器
sfr PCA_PWM1    =   0xf3;           //PCA模块1的PWM寄存器
sfr PCA_PWM2    =   0xf4;           //PCA模块2的PWM寄存器

sfr AUXR        =   0x8E;   //0000,0000 辅助寄存器
sfr T2H         =   0xD6;   //0000,0000 T2高字节
sfr T2L         =   0xD7;   //0000,0000 T2低字节
sbit EPCA       =   IE^6;           //PCA中断允许位

/*------------------------------------------------
                 串口初始化函数，波特率9600
------------------------------------------------*/
 void UART_init(void)
{
		SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR &= 0xFB;		//定时器时钟12T模式
	T2L = 0xE8;		//设置定时初始值
	T2H = 0xFF;		//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	TI=1;			// 很关键，使用printf函数时必须有此命令
	ES = 1; //开启串口中断
	EA=1;			   //开启总中断
}
//UART1 发送串口数据
void UART1_SendData(char dat)
{
    ES=0;           //关串口中断
    SBUF=dat;           
    while(TI!=1);   //等待发送成功
    TI=0;           //清除发送中断标志
    ES=1;           //开串口中断
}
 
//UART1 发送字符串reentrant
//void UART1_SendString(char *s)
//{
//    while(*s)//检测字符串结束符
//    {
//        UART1_SendData(*s++);//发送当前字符
//    }
//}
 
//重写putchar函数
char putchar(char c) 
{
    UART1_SendData(c);
    return c;
}

void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 22;
	j = 3;
	k = 227;
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
		P0M1 = 0x03; 
		P0M0 = 0x00; //P0(双双双双双双高高)
//    P1M0 = 0x00;
//    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;
    ACC = P_SW1;
    ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=0
    P_SW1 = ACC;                    //(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
    
//  ACC = P_SW1;
//  ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=1 CCP_S1=0
//  ACC |= CCP_S0;                  //(P3.4/ECI_2, P3.5/CCP0_2, P3.6/CCP1_2, P3.7/CCP2_2)
//  P_SW1 = ACC;  
//  
//  ACC = P_SW1;
//  ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=1
//  ACC |= CCP_S1;                  //(P2.4/ECI_3, P2.5/CCP0_3, P2.6/CCP1_3, P2.7/CCP2_3)
//  P_SW1 = ACC;  

    CCON = 0;                       //初始化PCA控制寄存器
                                    //PCA定时器停止
                                    //清除CF标志
                                    //清除模块中断标志
    CL = 0;                         //复位PCA寄存器
    CH = 0;
    CCAP0L = 0;
    CCAP0H = 0;
//    CMOD = 0x09;                    //设置PCA时钟源为系统时钟SYSclk/4,且使能PCA计时溢出中断
		CMOD = 0x01;    //SYSclk/12
    CCAPM0 = 0x21;                  //PCA模块0为16位捕获模式(上升沿捕获,可测从高电平开始的整个周期),且产生捕获中断
//  CCAPM0 = 0x11;                  //PCA模块0为16位捕获模式(下降沿捕获,可测从低电平开始的整个周期),且产生捕获中断
 // CCAPM0 = 0x31;                  //PCA模块0为16位捕获模式(上升沿/下降沿捕获,可测高电平或者低电平宽度),且产生捕获中断

    CR = 1;                         //PCA定时器开始工作
    EA = 1;

    cnt = 0;
    count0 = 0;
    count1 = 0;
		UART_init();
//	printf("STC15W408AS \n");
//	printf("TRG!!!\n");
    while (1){
//			if(length > 0){
//				CCAPM0 = 0x00; //关闭比较功能
//			printf("Pulse_length: %lu\n",length);
//			length = 0 ;
//			Delay500ms();
//			CCAPM0 = 0x21; //开启比较功能
//			}
		}
}

void PCA_isr() interrupt 7
{	
 if (CF)
 {
 CF = 0;
 cnt++; //PCA计时溢出次数+1
 }
 if (CCF0)
 {
 CCF0 = 0;
 count0 = count1; //备份上一次的捕获值
 ((BYTE *)&count1)[3] = CCAP0L; //保存本次的捕获值
 ((BYTE *)&count1)[2] = CCAP0H;
 ((BYTE *)&count1)[1] = cnt;
 ((BYTE *)&count1)[0] = 0;
 length = count1 - count0; //计算两次捕获的差值,即得到时间长度
	printf("Pulse_length: %lu\n",length);
 }

}