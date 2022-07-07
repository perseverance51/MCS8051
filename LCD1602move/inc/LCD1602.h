#ifndef __LCD1602_H__
#define __LCD1602_H__

//#include <reg52.h>
#include <STC89C5xRC.H>


//LCD1602 IO设置
#define LCD1602_PORT P0
sbit LCD1602_RS = P2^0;
sbit LCD1602_RW = P2^1;
sbit LCD1602_EN = P2^2;

//函数或者变量声明
extern void LCD1602_delay_ms(unsigned int n);
extern void LCD1602_write_com(unsigned char com);
extern void LCD1602_printc(unsigned char dat);
//extern void LCD1602_prints(unsigned char *s);
extern void LCD1602_Init();
extern void LCD1602_set_position(bit hang, unsigned char x);           //LCD光标定位到x处
//extern void LcdSetCursor(unsigned char x, unsigned char y);
extern void LcdShowStr(unsigned char x, unsigned char y,
        unsigned char *str, unsigned char len);

#endif