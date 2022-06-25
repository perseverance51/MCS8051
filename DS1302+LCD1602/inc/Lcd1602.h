#ifndef __LCD1602_H__
#define __LCD1602_H__

#include <reg52.h>

#define LCD1602_DB P0

sbit LCD1602_RS = P2^0;
sbit LCD1602_RW = P2^1;
sbit LCD1602_E = P2^2;

/* 等待液晶准备好 */
extern void LcdWaitReady();
/* 向 LCD1602 液晶写入一字节命令，cmd-待写入命令值 */
extern void LcdWriteCmd(unsigned char cmd);
/* 向 LCD1602 液晶写入一字节数据，dat-待写入数据值 */
extern void LcdWriteDat(unsigned char dat);
/* 设置显示 RAM 起始地址，亦即光标位置，(x,y)-对应屏幕上的字符坐标 */
extern void LcdSetCursor(unsigned char x, unsigned char y);
/* 在液晶上显示字符串，(x,y)-对应屏幕上的起始坐标，str-字符串指针 */
extern void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);
/* 打开光标的闪烁效果 */
extern void LcdOpenCursor();
/* 关闭光标显示 */
extern void LcdCloseCursor();
/* 初始化 1602 液晶 */
extern void InitLcd1602();









#endif