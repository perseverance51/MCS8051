/************************************************
文件：delay.h
用途：延时函数头文件
注意：系统时钟8M
************************************************/
#ifndef __DELAY_H__
#define __DELAY_H__

extern void delay_1us(void);
extern void delay_nus(unsigned int n);
extern void delay_1ms(void);
extern void delay_nms(unsigned int n);

#endif