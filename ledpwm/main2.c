/*
*******************************************************************************
* 文件名：main.c
* 描  述：呼吸灯
* 备  注：T0用来产生PWM波，T1通过查表法改变占空比
*         不再使用while(1) 里面的做法
*******************************************************************************
*/

#include <STC15F2K60S2.H>
#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long
#define MAIN_Fosc		11059200uL	//定义主时钟
sbit PWMOUT = P1^0;

u32 PeriodCnt = 0;
u8 HighRH = 0;
u8 HighRL = 0;
u8 LowRH = 0;
u8 LowRL = 0;
u8 T1RH = 0;
u8 T1RL = 0;


void ConfigPWM(u16 fr, u8 dc);
void ConfigTimer1(u16 ms);

void main()
{
	EA = 1;
	P1M1 &= 0xFE; 
	P1M0 |= 0x01; //P1.0(推挽模式)
 	ConfigPWM(2000, 50);
	ConfigTimer1(100);	

 	while(1);
}

//========================================================================
// 函数: void ConfigPWM(u16 fr, u8 dc)
// 描述: pwm频率设置.
// 参数: fr频率参数，当前占空比值
// 返回: none.
// 版本: 
//========================================================================

void ConfigPWM(u16 fr, u8 dc)
{
 	u16 high, low;
	
	PeriodCnt = (MAIN_Fosc/12)/fr;
	high = (PeriodCnt*dc)/100;//占空比初值
	low = PeriodCnt - high;
	high = 65536 - high;
	low = 65536 - low;
	HighRH = (u8)(high>>8);
	HighRL = (u8)high;
	LowRH = (u8)(low>>8);
	LowRL = (u8)low;

	TMOD &= 0xF0;
	TMOD |= 0x01;
	TH0 = HighRH;
	TL0 = HighRL;
	ET0 = 1;
	TR0 = 1;
	PWMOUT = 1;	
}
//========================================================================
// 函数: AdjustPWM(u8 dc)
// 描述: pwm占空比设置.
// 参数: dc当前占空比值
// 返回: none.
// 版本: 
//========================================================================
void AdjustPWM(u8 dc)
{
	u16 high, low;

	high = (PeriodCnt*dc)/100;
	low = PeriodCnt - high;
	high = 65536 - high;
	low = 65536 - low;
	HighRH = (u8)(high>>8);
	HighRL = (u8)high;
	LowRH = (u8)(low>>8);
	LowRL = (u8)low;
}

void ConfigTimer1(u16 ms)
{
 	u32 tmp;
	
	tmp = MAIN_Fosc/12;
	tmp = (tmp*ms)/1000;
	tmp = 65536 - tmp;
	T1RH = (u8)(tmp >> 8);
	T1RL = (u8)tmp;
	TMOD &= 0x0F;
	TMOD |= 0x10;
	TH1 = T1RH;
	TL1 = T1RL;
	ET1 = 1;
	TR1 = 1;		
}

void InterruptTimer0() interrupt 1
{
 	if(PWMOUT == 1)
	{
	 	TH0 = LowRH;
		TL0 = LowRL;
		PWMOUT = 0;
	}
	else
	{
	 	TH0 = HighRH;
		TL0 = HighRL;
		PWMOUT = 1;
	}
}

void InterruptTimer1() interrupt 3
{
	static u8 dir = 0;
	static u8 index = 0;
	u8 xdata Table[] = {
		2, 3, 5, 11, 16, 25, 36, 44, 54, 66, 76, 87, 92, 96
	};
	
	AdjustPWM(Table[index]);//调节占空比
	
	if(dir == 0)
	{
	 	index++;
		if(index == 13)
			dir = 1;
	}
	if(dir == 1)
	{
	 	index--;
		if(index == 0)
			dir = 0;
	}
	
			
}
