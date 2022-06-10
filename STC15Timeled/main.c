#include "reg51.h"
typedef unsigned char u8;
typedef unsigned int u16;
//-----------------------------------------------
#define FOSC 11059200L
#define T5MS (65536-5*FOSC/1000) //1Tģʽ��110592MHz
//#define T5MS (65536-5*FOSC/12/1000) //12Tģʽ��110592MHz
sfr AUXR = 0x8e; //Auxiliary register
sbit P10 = P1^0;
bit	B_5ms;			//5ms��־
 unsigned int 	msecond;		//��ʱ
//-----------------------------------------------
/* Timer0 interrupt routine */
void tm0_isr() interrupt 1 using 1
{
		B_5ms = 1;
}
void Timer0Init(void)		//5����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	// AUXR &= 0x7f; //��ʱ��0Ϊ12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
//	TL0 = 0x00;		//���ö�ʱ��ʼֵ
//	TH0 = 0x28;		//���ö�ʱ��ʼֵ
		 TL0 = T5MS; //��ʼ����ʱֵ
		 TH0 = T5MS >> 8;
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1; //ʹ�ܶ�ʱ��0�ж�
	EA = 1;
}

//-----------------------------------------------
/* main program */
void main()
{
	Timer0Init();
	while(1)
	{
	if(B_5ms)	//5ms�����־λ
		{
			B_5ms = 0;
			if(++msecond >= 200)	//��������1S
			{
				msecond = 0;
				 P10 = ! P10; //�����Կ�ȡ��
			}
		}
	}
}