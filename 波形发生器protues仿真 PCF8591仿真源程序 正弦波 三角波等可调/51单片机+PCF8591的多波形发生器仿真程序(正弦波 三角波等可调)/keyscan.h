#ifndef __KEYSCAN_H
#define __KEYSCAN_H
#include <reg52.h>
#define uchar unsigned char

sbit Key1=P2^3;  //Ƶ������
sbit Key2=P2^4;  //Ƶ�ʼ�С
sbit Key3=P2^5;  //��������
sbit Key4=P2^6;  //���ȼ�С
sbit Key5=P2^7;

void keyscan(void);

#endif
