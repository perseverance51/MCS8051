#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include <reg51.h>    //51单片机头文件


//数码管    138译码器 
sbit LSA=P2^2;	
sbit LSB=P2^3;	
sbit LSC=P2^4;
//LED灯
sbit LED=P2^0;

typedef unsigned int uint;	  // typedef（类型定义） 用来给无符号整型类型数据 unsigned int 定义新名字 uint
typedef unsigned char uchar;  //用来给无符号字符型类型数据 unsigned char 定义新名字 uchar

extern uint;    //声明给外部函数使用
extern uchar;   //声明给外部函数使用
extern void SEGDisplay(void);   // extern（外部的） 声明此函数给外部调用

#endif
