//#include <STC15F2K60S2.H>
      //��������Լ���51��Ƭ��оƬ���޸ġ�
//#include <STC89C5xRC.H>
#include <STC12C5A60S2.H>

#include "intrins.h"//�����ʱ��������������nop()����Ҫ��Ӵ�ͷ�ļ�
#include <stdio.h> 	       // Ϊʹ��KEIL�Դ��Ŀ⺯��printf������

sbit INIR = P3^2;                //��������Լ��������ţ�ע��Ҫ���жϹ��ܡ�
bit InFrared_Way = 0;
unsigned char Data[4] = {0};

void InFrared_Init(void);
void UartInit(void)		//
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0xE0;		//���ö�ʱ��ʼֵ
	TH1 = 0xFE;		//���ö�ʱ��ʼֵ
	ET1 = 0;		//��ֹ��ʱ��%d�ж�
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	TI=1;			// �ܹؼ���ʹ��printf����ʱ�����д�����
}
void Delay1000ms()		//@35MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 133;
	j = 255;
	k = 40;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void main(void)
{
//       TMOD = 0x01;
//       TR0 = 0;
//       IT0 = 1;
//       EX0 = 1;
		UartInit();
       EA = 1;
	Delay1000ms();
		printf ("STC12��Ƭ�� \n"); 
	Delay1000ms();
		printf ("Start waite IR_DATE \n"); 
       while (1)
       {
//              if (InFrared_Way == 1)
//              {
//                     EX0 = 0;
//                     TR0 = 1;
//                     InFrared_Init();
//                     TR0 = 1;
//                     EX0 = 1;
//                     InFrared_Way = 0;
//              }
       }
}

void Int_0(void) interrupt 0
{//�ⲿ�ж�0
       InFrared_Way  = 1;
}

void InFrared_Init(void)
{
       unsigned char i, j;     
       TH0 = 0;
       TL0 = 0;
       while (INIR == 0 && TH0 <= 35);
       if (INIR == 1)
       {
              while (INIR == 1 && TH0 <= 55);
              for (i = 0; i < 4; i++)
              {
                     for (j = 0; j < 8; j++)
                     {
                            TH0 = 0;
                            TL0 = 0;
                            while (INIR == 0 && TH0 <= 3);
                            while (INIR == 1);
                            Data[i] >>= 1;
                            if (TH0 >= 7)
                            {
                                   Data[i] |= 0x80;
                            }
                     }
              }
							printf ("RECE_DATA: %s \n",Data); 
       }
}
