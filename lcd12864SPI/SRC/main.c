/*  ˵����
	//����и����������룬�����ں��ֺ��油��"\xfd"
	//���߸�Keil����������벹��
	//http://www.stcisp.com/keilfdfix/keilfdfix.zip
	//Keil�����װ·��C:\Keil_v5\C51\BIN


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
    LCD_write_string(0x0000, "��ɽ��·��Ϊ����");
	LCD_write_string(0x0100, "ѧ�����Ŀ����ۡ�");
	LCD_write_string(0x0200, "Ī��ǰ·��֪����");
	LCD_write_string(0x0300, "����˭�ܲ�ʶ����");

	while(1)
	{	

	}
}

