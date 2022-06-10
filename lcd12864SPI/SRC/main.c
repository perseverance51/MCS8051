/*  说明：
	//如果有个别汉字是乱码，可以在汉字后面补上"\xfd"
	//或者给Keil软件打汉字乱码补丁
	//http://www.stcisp.com/keilfdfix/keilfdfix.zip
	//Keil软件安装路径C:\Keil_v5\C51\BIN


*/

#include <STC89C5xRC.H>
#include "intrins.h"
#include "st7920.h"

#define uint8_t unsigned char
	
void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	i = 2;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}


void delay_ms(unsigned int i)
{
	while(i--)
		Delay1ms();
}


void main(void)
{
	LCD_init();
	LCD_clear();
    LCD_write_string(0x0000, "书山有路勤为径；");
	LCD_write_string(0x0100, "学海无涯苦作舟。");
	LCD_write_string(0x0200, "莫愁前路无知己；");
	LCD_write_string(0x0300, "天下谁能不识君。");

	while(1)
	{	

	}
}

