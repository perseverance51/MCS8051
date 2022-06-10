#include "software.h"

uchar num , num1; //定义num , num1的数据类型为无符号字符型类型
num2 = 99;        //初始化此变量的值

void delayms(uint xms)   //简单延时
{
	uint a , b;
	for(a=xms; a>0; a--)
	 for(b=110; b>0; b--);
}

void Timer0_Rountine(void) interrupt 1  //中断号 定时器/计数器0中断  1为定时器/计数器0的中断号（重要）
{
	TH0 = (65536-45872)/256;      //方式1需要重装初值
	TL0 = (65536-45872)%256;
	num++;      //mun变量自加1
	if(num == 10)  //判断定时时间是否等于0.5s 为真之后 LED灯的电平状态转换
	{
		num = 0;   //清零，好判断下次计时
		LED =~ LED;//电平状态取反
	}
}

void TimerI_Routine(void) interrupt 3  //中断号 定时器/计数器1中断  1为定时器/计数器0的中断号 （重要）
{
	TH1 = (65536-45872)/256;  //方式1需要重装初值
	TL1 = (65536-45872)%256;
	num1++;   //mun1变量自加1
	if(num1 == 20)   //判断定时时间是否等于1s 
	{
		num1 = 0;   //清零，好判断下次计时
		num2--;     //mun2变量自减1  mun2的初始值为60
		if(num2 == 0) //判断是否计时结束
		   num2 = 60; //重新赋值，以便重新计时
	}
	
}
	
