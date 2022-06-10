//#include <STC12C5A60S2.H>
//#include <STC8.H>
//#include <STC15F2K60S2.H>
#include "STC15Wxx.h"
#include "intrins.h"
#include <stdio.h> 	       // Ϊʹ��KEIL�Դ��Ŀ⺯��printf������
bit busy;
void UartInit(void)		//9600bps@16MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0x5F;		//���ö�ʱ��ʼֵ
	T2H = 0xFE;		//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	TI=1;			// �ܹؼ���ʹ��printf����ʱ�����д�����
}
/*----------------------------
UART �жϷ������
-----------------------------*/
//void Uart() interrupt 4 using 1
//{
// if (RI)
// {
// RI = 0; //���RIλ
// P0 = SBUF; //P0��ʾ��������
////P22 = RB8; //P2.2��ʾУ��λ
// }
// if (TI)
// {
// TI = 0; //���TIλ
// busy = 0; //��æ��־
// }
//}
void Delay1000ms()		//@16MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 61;
	j = 204;
	k = 243;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void main()
{
	UartInit();
//	EA =1;
	 while(1)
		 	{
				printf("perseverance51 \n"); 
				Delay1000ms();
				printf ("hello world! \n"); 
	}
}