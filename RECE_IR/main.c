//#include <STC15F2K60S2.H>
      //这个根据自己的51单片机芯片来修改。
//#include <STC89C5xRC.H>
#include <STC12C5A60S2.H>

#include "intrins.h"//如果延时函数当中所用了nop()就需要添加此头文件
#include <stdio.h> 	       // 为使用KEIL自带的库函数printf而加入

sbit INIR = P3^2;                //这个根据自己定义引脚，注意要带中断功能。
bit InFrared_Way = 0;
unsigned char Data[4] = {0};

void InFrared_Init(void);
void UartInit(void)		//
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xE0;		//设置定时初始值
	TH1 = 0xFE;		//设置定时初始值
	ET1 = 0;		//禁止定时器%d中断
	TR1 = 1;		//定时器1开始计时
	TI=1;			// 很关键，使用printf函数时必须有此命令
}
void Delay1000ms()		//@35MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 133;
	j = 255;
	k = 40;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void main(void)
{
//       TMOD = 0x01;
//       TR0 = 0;
//       IT0 = 1;
//       EX0 = 1;
		UartInit();
       EA = 1;
	Delay1000ms();
		printf ("STC12单片机 \n"); 
	Delay1000ms();
		printf ("Start waite IR_DATE \n"); 
       while (1)
       {
//              if (InFrared_Way == 1)
//              {
//                     EX0 = 0;
//                     TR0 = 1;
//                     InFrared_Init();
//                     TR0 = 1;
//                     EX0 = 1;
//                     InFrared_Way = 0;
//              }
       }
}

void Int_0(void) interrupt 0
{//外部中断0
       InFrared_Way  = 1;
}

void InFrared_Init(void)
{
       unsigned char i, j;     
       TH0 = 0;
       TL0 = 0;
       while (INIR == 0 && TH0 <= 35);
       if (INIR == 1)
       {
              while (INIR == 1 && TH0 <= 55);
              for (i = 0; i < 4; i++)
              {
                     for (j = 0; j < 8; j++)
                     {
                            TH0 = 0;
                            TL0 = 0;
                            while (INIR == 0 && TH0 <= 3);
                            while (INIR == 1);
                            Data[i] >>= 1;
                            if (TH0 >= 7)
                            {
                                   Data[i] |= 0x80;
                            }
                     }
              }
							printf ("RECE_DATA: %s \n",Data); 
       }
}
