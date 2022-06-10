/***********************************************************
程序功能：ADC采集电压信号（0-5V或3.3V）
***********************************************************/
#include "reg51.h"
#include "intrins.h"
#include <stdio.h> 	       // 为使用KEIL自带的库函数printf而加入

sfr ADC_CONTR = 0XBC;
sfr ADC_RES   = 0XBD;
sfr ADC_LOW2  = 0XBE;
sfr P1ASF     = 0X9D;
sfr AUXR = 0x8E; //辅助寄存器
typedef unsigned char BYTE;
typedef unsigned int WORD;
 
#define ADC_POWER   0X80
#define ADC_FLAG    0X10
#define ADC_START   0X08
#define ADC_SPEEDLL 0X00
#define ADC_SPEEDL  0X20
#define ADC_SPEEDH  0X40
#define ADC_SPEEDHH 0X60
 
void InitADC();
void Delay(WORD n);
WORD GetADCResult(BYTE ch);
void DisplayADCResult(BYTE Channel);//选择0-7通道其中一个作为转换对象
 
 void delay_ms(WORD t)//利用定时器作为延时
{
  WORD count = 0;
  TMOD |= 0x01;
  TH0 = 0xFC;
  TL0 = 0x66;
  TR0 = 1;
  while (1)
  {
    if (TF0 == 1)
    {
      count++;
      TF0 = 0;
      TH0 = 0xFC;
      TL0 = 0x66;
    }
    if (count >= t)
    {
      break;
    }
  }
}

 /*----------------------------
初始化串口
----------------------------*/
void InitUart()
{
//下面代码设置定时器1
	TMOD = 0x20;	// 0010 0000 定时器1工作于方式2（8位自动重装方式）
	TH1  = 0xFD;	// 波特率：9600 /11.0592MHZ
	TL1  = 0xFD;	// 波特率：9600 /11.0592MHZ
	TR1  = 1;
	//下面代码设置定串口
	AUXR = 0x00;             // 很关键，8051定时器速度，使用定时器1作为波特率发生器，S1ST2=0
	SCON = 0x50; 	// 0101 0000 SM0.SM1=01(最普遍的8位通信）,REN=1（允许接受）
	TI=1;			// 很关键，使用printf函数时必须有此命令
}
void main()
{
	 InitUart();   //Init UART, use to show ADC result
	InitADC();
	while(1)
	{
		delay_ms(1000);
		DisplayADCResult(1);		
	}
}
//平均滤波//
#define N 100
float AD_average(BYTE Channel)
{
	float value=0;
	BYTE count;
	for(count=0;count<100;count++)
	{
	value+=GetADCResult(Channel);   //100次采样
	}
//value/=202.5;
// value/=205;
// value/=2048;
// value/=512;
 value/=100;		   //求平均
 value=value*5.0/1024;//val/1024*5
 return value;
}

void DisplayADCResult(BYTE Channel)
{
	float ADResult;
//	WORD Result;
	ADResult=AD_average(Channel)*100;//电压值放大100倍
//	Result=(int)ADResult;
printf("电压值 = %.4f \n",ADResult); //保留浮点数小数点后4位
		
}
 
WORD GetADCResult(BYTE ch)
{
	ADC_CONTR=ADC_POWER|ADC_SPEEDLL|ch|ADC_START;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	while(!(ADC_CONTR&ADC_FLAG));
	ADC_CONTR&=~ADC_FLAG;
 
	return ADC_RES*4+ADC_LOW2;
}
 
void InitADC()
{
	P1ASF = 0XFF;
	ADC_RES = 0;
	ADC_CONTR = ADC_POWER|ADC_SPEEDLL;
	Delay(2);
}
 
void Delay(WORD t)
{
	WORD x;
	while(t--)
	{
		x = 5000;
		while(x--);
	}
}
 
 