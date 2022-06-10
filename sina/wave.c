#include "reg52.h"
#include <math.h>
#define ADC_PORT P2 //ADC0832连接端口
//5种波
#define W_SINE 0
#define W_SQU 1
#define W_TRI 2
#define W_TRA 3
#define W_SAW 4
#define PI 3.1415926f //圆周率

//简化类型符写法
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

sbit ADC_CS_WR=P3^0; //如图，ADC0832使能口
//5个LED，接P0口
sbit LED0=P0^0;
sbit LED1=P0^1;
sbit LED2=P0^2;
sbit LED3=P0^3;
sbit LED4=P0^4;
//5个按键
sbit KEY0=P1^0;
sbit KEY1=P1^1;
sbit KEY2=P1^2;
sbit KEY3=P1^3;
sbit KEY4=P1^4;
//全局变量
u8 volatile mode;//模式：正弦波……
u16 freq;//频率
u8 time;//计次参数
u8 AM;//调幅


void Timer0Init(void);
void scanKey(void);
void sys_init(void);

void sine_wave(u8 location);//正弦波
void squ_wave(u8 location);//方……
void tri_wave(u8 location);//三……
void tra_wave(u8 location);//梯……
void saw_wave(u8 location);//锯……

void Timer0Init(void)		//100微秒@12.000MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x02;		//设置定时器模式
	TL0 = 0x9C;		//设置定时初值
	TH0 = 0x9C;		//设置定时重载值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	EA = 1;         //开总中断
	ET0 = 1;        //开定时器0中断
}

void scanKey(void)//按键扫描
{
	if(KEY0==0)
	{
		mode=0;
		P0=0xFF;//灭所有灯
		LED0=0;
	}
	
	if(KEY1==0)
	{
		mode=1;
		P0=0xFF;//灭所有灯
		LED1=0;
	}
	
	if(KEY2==0)
	{
		mode=2;
		P0=0xFF;//灭所有灯
		LED2=0;
	}
	
	if(KEY3==0)
	{
		mode=3;
		P0=0xFF;//灭所有灯
		LED3=0;
	}
	
	if(KEY4==0)
	{
		mode=4;
		P0=0xFF;//灭所有灯
		LED4=0;
	}
}

void sys_init(void)//系统初始化
{
	time=0;
	ADC_CS_WR=0;
	ADC_PORT=0;
	mode=0;//默认输出正弦波
	freq=100;//默认频率100Hz
	AM=255;//最大幅度
}
void sine_wave(u8 location)//输出正弦波
{
	double x=(double)location/50*PI;//把0-100放缩到0-2派(pai，没有那个符号，好难受(╯﹏╰))
	u8 y=(sin(x)*(AM/2)+(AM/2));//算出y，并放缩到0-254（因为ADC范围0-AM，感觉这个芯片好落后）
	ADC_PORT=y;
}

void squ_wave(u8 location)//方……
{
	if(location<50)
		ADC_PORT=AM;
	else
		ADC_PORT=0x00;
}

void tri_wave(u8 location)//三……
{
	//为了简化，在单周期输出V字形
	u8 y;
	if(location<50)
		y=(50-location)*AM/50;
	else
		y=(location-50)*AM/50;
	ADC_PORT=y;
}
void tra_wave(u8 location)//梯……
{
	u8 y;
	if(location<25)
		y=0xFF;
	else if(location<50)
		y=(50-location)*AM/25;
	else if(location<75)
		y=0;
	else
		y=(location-75)*AM/25;
	ADC_PORT=y;
}
void saw_wave(u8 location)//锯……
{
	ADC_PORT=location*AM/100;
	//用（100-location）也以变成反向锯齿
}

void Timer0Work() interrupt 1 //中断服务函数
{
	switch(mode)
	{
		case W_SINE:sine_wave((u8)(time*freq/100)%100);break;//计算出波的位置
		case W_SQU:squ_wave((u8)((time*freq/100)%100));break;
		case W_TRI:tri_wave((u8)((time*freq/100)%100));break;
		case W_TRA:tra_wave((u8)((time*freq/100)%100));break;
		case W_SAW:saw_wave((u8)((time*freq/100)%100));break;
	}
	time++;
	if(time>=100)//计数100次
		time=0;
}

int main(void)
{
	sys_init();
	Timer0Init();
	while(1)
	{
		scanKey();
	}
}

