/**
  ***************************************************************************************
  * 文  件：延时函数实现源文件
  * 芯  片：AT89S52
  * 晶  振：12MHz
  ***************************************************************************************
  */
 
#include "delay.h"  //延时函数实现头文件
 
/**
  ***************************************************************************************
  * 函  数：延时ms函数
  * 参  数：t：延时ms值，0~4294967295
  * 返回值：无
  ***************************************************************************************
  */
void delay_ms(unsigned long t)  //延时ms函数
{
	unsigned char i;  //计数变量
	for(;t>0;t--)
		for(i=200;i>0;i--);  //不精确，此处循环值需测试
}