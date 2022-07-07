#include "LCD1602.h"

//****************************************************
//MS延时函数(12M晶振下测试)
//****************************************************
void LCD1602_delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}

//****************************************************
//写指令
//****************************************************
void LCD1602_write_com(unsigned char com)
{
	LCD1602_RS = 0;
	LCD1602_delay_ms(1);
	LCD1602_EN = 1;
	LCD1602_PORT = com;
	LCD1602_delay_ms(1);
	LCD1602_EN = 0;
}

//****************************************************
//写数据
//****************************************************
void LCD1602_printc(unsigned char dat)
{
	LCD1602_RS = 1;
	LCD1602_delay_ms(1);	
	LCD1602_PORT = dat;
	LCD1602_EN = 1;
	LCD1602_delay_ms(1);
	LCD1602_EN = 0;
}

//****************************************************
//连续写字符
//****************************************************
/*
void LCD1602_prints(unsigned char *s)
{
	while(*s>0)
	{
		LCD1602_printc(*s);
		s++;
	}
}
*/
//****************************************************
//LCD光标定位到x处
//****************************************************

void LCD1602_set_position(bit hang, unsigned char x)          
{
  if(hang)LCD1602_write_com(0x80 + 0x40 + x);
	else LCD1602_write_com(0x80 + x);
}

//****************************************************
//LCD初始化
//****************************************************
void LCD1602_Init()
{
	LCD1602_EN = 0;
	LCD1602_RW = 0;						//设置为写状态
	LCD1602_write_com(0x38);			//显示模式设定
	LCD1602_write_com(0x0c);			//开关显示、光标有无设置、光标闪烁设置
	LCD1602_write_com(0x06);			//写一个字符后指针加一
	LCD1602_write_com(0x01);			//清屏指令
}

/* 设置显示 RAM 起始地址，亦即光标位置，(x,y)-对应屏幕上的字符坐标 */
/*
void LcdSetCursor(unsigned char x, unsigned char y){
    unsigned char addr;
    if (y == 0){ //由输入的屏幕坐标计算显示 RAM 的地址
        addr = 0x00 + x; //第一行字符地址从 0x00 起始
    }else{
        addr = 0x40 + x; //第二行字符地址从 0x40 起始
    }
    LCD1602_write_com(addr | 0x80); //设置 RAM 地址
}
*/
/* 在液晶上显示字符串，(x,y)-对应屏幕上的起始坐标，
str-字符串指针，len-需显示的字符长度 */
void LcdShowStr(unsigned char x, unsigned char y,
        unsigned char *str, unsigned char len){
    LCD1602_set_position(x, y); //设置起始地址
    while (len--){ //连续写入 len 个字符数据
       LCD1602_printc(*str++); //先取 str 指向的数据，然后 str 自加 1
    }
}
