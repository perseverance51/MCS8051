#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include <reg51.h>    //51��Ƭ��ͷ�ļ�


//�����    138������ 
sbit LSA=P2^2;	
sbit LSB=P2^3;	
sbit LSC=P2^4;
//LED��
sbit LED=P2^0;

typedef unsigned int uint;	  // typedef�����Ͷ��壩 �������޷��������������� unsigned int ���������� uint
typedef unsigned char uchar;  //�������޷����ַ����������� unsigned char ���������� uchar

extern uint;    //�������ⲿ����ʹ��
extern uchar;   //�������ⲿ����ʹ��
extern void SEGDisplay(void);   // extern���ⲿ�ģ� �����˺������ⲿ����

#endif
