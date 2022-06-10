#ifndef __MAIN_H__
#define __MAIN_H__

#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char	//类型转义
#define uint unsigned int	//类型转义

/*重要: 
	第一片的74HC595的引脚14要接到第二片的74HC595的引脚9, 千万不能搞反
	第一片的74HC595的引脚12接第二片的74HC595的引脚12
	第一片的74HC595的引脚11接第二片的74HC595的引脚11

其它引脚接法, 请看电路图
*/

sbit LEDARRAY_DI = P3^4;	//第二片74HC595的引脚14
sbit LEDARRAY_LAT = P3^5;	//第二片74HC595的引脚12
sbit LEDARRAY_CLK = P3^6;	//第二片74HC595的引脚11 


//函数或者变量声明
extern void Delay_ms(uint n);
extern void Send_Data(uchar dat);
extern void Send_Byte(uchar dat);

#endif