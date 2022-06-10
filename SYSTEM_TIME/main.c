/*
利用定时器0实现51单片机输出系统上电累计运行时间
u32:长整型的变量最大值是4294967295毫秒，
换算下来就是49天17时2分47秒，然后溢出从0开始继续计时

*/
//#include <reg51.h>
#include <STC89C5xRC.H>
#include <intrins.h>

#define  interval  1000 //设置延时时间间隔
sbit RCK=P2^1;//定义74HC595存储寄存器时钟引脚:ST_CP
sbit SCK=P2^2;//定义74HC595移位寄存器时钟引脚:SH_CP
sbit SER=P2^3;//定义74HC595串行输入引脚:DS
sbit led=P2^0;
 
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;
 
u32 sysRunmillis;	//系统运行时间计数，保存单片机从上电复位以来运行的时间，单位是毫秒。该数值由定时器T0的中断响应子函数更新
u8 hour,min,sec;	//当前系统运行时间换算成小时数、分数和秒数
u8 strTime[]={0,0,0,0,0,0,0,0};	//保存时分秒每1位上的数字（0~9），分隔符‘-’的值是10
 
u8 DigCode[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40};//共阴极数码管段码
 
//把全局变量sysRunmillis保存的毫秒数转换成时分秒格式，把时分秒的每一位数字（0~9）保存在数组strTime里。
void millis2time()
{
	u32 t;
  t = sysRunmillis/1000ul;	  //毫秒转换为秒
	hour = t/3600%24;			  //计算小时数	
	min = (t%3600)/60;		  //计算分
	sec = t%60;				  //计算秒
	//以下处理时、分、秒每1位，如时分秒数值小于10，补加0占位。
	if(hour<10)
	{
		strTime[0]=0;
		strTime[1]=hour;
	}
	else
	{
		strTime[0]=hour/10;
		strTime[1]=hour%10;
	}
	if(min<10)
	{
		strTime[3]=0;
		strTime[4]=min;
	}
	else
	{
		strTime[3]=min/10;
		strTime[4]=min%10;
	}
	if(sec<10)
	{
		strTime[6]=0;
		strTime[7]=sec;
	}
	else
	{
		strTime[6]=sec/10;
		strTime[7]=sec%10;
	}
}
 
//给74HC95发送数码管位选和段码数据
void _74HC595SendByte(u8 wei,u8 duan)
{
	u8 i;
	RCK=0;
	for(i=0;i<8;i++)
	{
		SCK=0;
		_nop_();
		SER=wei>>7;
		SCK=1;
		_nop_();
		wei<<=1;
	}
	for(i=0;i<8;i++)
	{
		SCK=0;
		_nop_();
		SER=duan>>7;
		SCK=1;
		_nop_();
		duan<<=1;
	}
	RCK=1;
}
 
//初始化定时器T0
void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	//计数初值，每1毫秒中断1次
	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值
//    EA = 1;
//    ET0 = 1;//T0的溢出中断允位
	IE |= 0x82;//代替前面2句
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}


 
void Timer0Interrupt() interrupt 1
{
	//传统51单片机没有16位自动重载模式,Proteus仿真时需要加上下面2句
	TL0 = 0x20;		//设置定时初始值
	TH0 = 0xD1;		//设置定时初始值
    sysRunmillis++;	//每1毫秒加1
}
 
void delayms(u16 ms)   //毫秒延时函数
{
    u32 temp;
	temp = sysRunmillis ;
    while(sysRunmillis - temp < ms );
}
 
void main()
{
	u8 i;
	u32 previousMillis = 0;//上一个时间
	u32 currentMillis;//当前时间
	Timer0Init();
	strTime[2]=10;
	strTime[5]=10;

	while(1)
	{
		currentMillis = sysRunmillis;
		millis2time();
		for(i=0;i<8;i++)
		{//数码管显示
			_74HC595SendByte(~(0x01<<i),DigCode[strTime[i]]);
		}
		if (currentMillis - previousMillis >= interval) {//每隔1秒翻转一次
    previousMillis = currentMillis;
			led=!led;
		}
	}
}