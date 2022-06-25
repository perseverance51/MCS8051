//https:	//shop366226707.taobao.com
//QQ:	3065674908

#ifndef _CONFIG_H
#define _CONFIG_H

//����������Ʋ����������ת����ת����1������1����ֹͣ
//�ϵ�ʱ������������������ʾ�ٶ���С��1���Ӽ���λ����ͨ���������ʾ������������õ�˫���ķ�ʽ
//�������ת��һ��9����ͨ����������ת�� 
//�������ʾ��λ��LED��ʾ��ת����ת��ֹͣ

/* ͨ��ͷ�ļ� */
#include <reg52.h>

/*�궨��*/
#define SpeedShow P0   		//�������ʾ�ӿ�
#define MotorDriver P2   	//������������ӿ�

/* ���������ض��� */
typedef unsigned char uchar;      // 8λ�з���������
typedef unsigned int 	uint;       //16λ�з���������
typedef unsigned long ulong;      //32λ�з���������

			
/* IO���ŷ��䶨�� */
sbit KEY1  = P1^2;  //����1����������1
sbit KEY2  = P1^3;  //����2����������2
sbit KEY3  = P1^4;  //����3����������3
sbit KEY4  = P3^5;  //����4����������4
sbit KEY5  = P3^6;  //����4����������4

sbit FWD_LED = P1^0;  		//��תָʾ��
sbit REV_LED = P1^1;  		//��תָʾ��

#endif
