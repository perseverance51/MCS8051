/**
  ***************************************************************************************
  * 文  件：LCD1602模块实现源文件
  ***************************************************************************************
  */
 
#include "reg52.h"  //52系列单片机头文件
#include "delay.h"  //延时函数实现头文件
#include "lcd1602.h"  //LCD1602实现头文件
 
/**
  ***************************************************************************************
  * 函  数：LCD1602判断忙函数，忙则循环等待
  * 参  数：无
  * 返回值：无
  ***************************************************************************************
  */
void LCD1602_isBusy(void)  //LCD1602判断忙函数
{
	unsigned char busy;  //忙标志
	LCD1602_RS = 0;  //RS = 0
	LCD1602_RW = 1;  //RW = 1
	LCD1602_E = 0;  //E = 0
	do{
		LCD1602_E = 1;  //E = 1
		delay_ms(5);  //延时5毫秒
		busy = LCD1602_DATA & 0x80;  //读忙标志
		LCD1602_E = 0;  //E = 0
	}while(busy);  //忙则循环等待
}
 
/**
  ***************************************************************************************
  * 函  数：LCD1602写指令、写显示地址函数
  * 参  数：cmd：要写入的指令或显示地址
  * 返回值：无
  ***************************************************************************************
  */
void LCD1602_WriteCmd(unsigned char cmd)  //LCD1602写指令、写显示地址函数
{
	LCD1602_isBusy();  //判断忙，忙则循环等待
	LCD1602_RS = 0;  //RS = 0
	LCD1602_RW = 0;  //RW = 0
	LCD1602_E = 0;  //E = 0
	LCD1602_DATA = cmd;  //写入指令或显示地址
	LCD1602_E = 1;  //E = 1
	delay_ms(5);  //延时5毫秒
	LCD1602_E = 0;  //E = 0
}
 
/**
  ***************************************************************************************
  * 函  数：LCD1602写数据函数
  * 参  数：num：要写入的8位数据
  * 返回值：无
  ***************************************************************************************
  */
void LCD1602_WriteData(unsigned char num)  //LCD1602写数据函数
{
	LCD1602_isBusy();  //判断忙，忙则循环等待
	LCD1602_RS = 1;  //RS = 1
	LCD1602_RW = 0;  //RW = 0
	LCD1602_E = 0;  //E = 0
	LCD1602_DATA = num;  //写入数据
	LCD1602_E = 1;  //E = 1
	delay_ms(5);  //延时5毫秒
	LCD1602_E = 0;  //E = 0
}
 
/**
  ***************************************************************************************
  * 函  数：LCD1602指定行列位置显示字符函数
  * 参  数：x：行，1~2
  * 参  数：y：列，1~16
  * 参  数：chr：显示的字符，ASCII字符
  * 返回值：无
  ***************************************************************************************
  */
void LCD1602_DispChar (unsigned char x,unsigned char y,unsigned char chr)  //LCD1602指定行x列y位置显示字符chr函数
{
	LCD1602_WriteCmd(0x80+0x40*(x-1)+(y-1));  //显示字符位置：第x行第y列
	LCD1602_WriteData(chr);  //显示指定字符chr
}
 
/**
  ***************************************************************************************
  * 函  数：LCD1602初始化函数
  * 参  数：无
  * 返回值：无
  ***************************************************************************************
  */
void LCD1602_Init(void)  //LCD1602初始化函数
{
	LCD1602_WriteCmd(0x38);  //显示模式：8位数据接口，两行显示，5*7点阵字符
	LCD1602_WriteCmd(0x08);  //显示关，光标关，闪烁关
	LCD1602_WriteCmd(0x01);  //清除DDRAM，清除AC（清屏）
	LCD1602_WriteCmd(0x06);  //读数据后AC自动加1，读数据后画面不动
	LCD1602_WriteCmd(0x0c);  //显示开，光标关，闪烁关
}