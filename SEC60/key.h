#ifndef __KEY_H__  
#define __KEY_H__ 

#define KEY_MODE 0x01    // 模式按键
#define KEY_PLUS 0x02     // 加
#define KEY_MODE 0x01    // 模式按键
#define KEY_PLUS 0x02     // 加

#include <hardware.h>
#include "software.h"
sbit KeyValue=P3^7;

extern uchar Trg;
extern uchar Cont;
extern uint cnt_plus;

//unsigned char code segment[]= {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
//定义共阳数码管显示0~9


void KeyRead();
void KeyProc();

#endif