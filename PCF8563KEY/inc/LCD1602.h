#ifndef __LCD1602_H__
#define __LCD1602_H__

#include <reg52.h>
 #include "delay.h"

#define LCD1602_DATAPINS P1

sbit LCD1602_E=P2^2;
sbit LCD1602_RW=P2^1;
sbit LCD1602_RS=P2^0;

//void Delay1ms(unsigned int a);
void LCD1602_WriteCom(unsigned char com);          //д������
void LCD1602_WriteData(unsigned char dat);                       //д������
void LCD1602_Init();  //LCD��ʼ���ӳ���
void  LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s);



#endif