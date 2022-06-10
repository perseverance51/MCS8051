#include <reg51.h>
unsigned char table[]={ 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned char count=0;
sbit LED_B1=P3^6;
sbit LED_A1=P3^7;
sbit fmq=P1^0;
unsigned char counter;
unsigned char NSF=0;

typedef unsigned int u16;          //���������ͽ�����������
typedef unsigned char u8;

sbit k1=P3^1;
sbit k2=P3^0;

sbit beep=P1^0;
sbit relay=P1^1;
u8 start=0;
void delayms(unsigned int xms);
//��ʱ��0��ʼ��
void Init(void)
{
    TMOD=0x01;
    TH0=(65536-10000)/256;
    TL0=(65536-10000)%256;
}
/****************************************************************
***************
* �� �� �� : Int1Init()
* �������� : �����ⲿ�ж� 1
* �� �� : ��
* �� �� : ��
*****************************************************************
**************/
void Int0Init()
{
		//���� INT0
		IT0=1;//�����س�����ʽ���½��أ�
		EX0=1;//�� INT0 ���ж�����
		EA=1;//�����ж�
}
/****************************************************************
***************
* �� �� �� : Int0() interrupt 0
* �������� : �ⲿ�ж� 0 ���жϺ���
* �� �� : ��
* �� �� : ��
*****************************************************************
**************/
void Int0() interrupt 0 //�ⲿ�ж� 0 ���жϺ���
{
//delay(1000); //��ʱ����
		ET0=1;//��ʱ���ж�ʹ��
	  TR0=1;//���� T0
    EA=1;
}

//����ʱ��0�ж�
void timer0(void) interrupt 1
{
    TMOD=0x01;
    TH0=(65536-50000)/256;
    TL0=(65536-50000)%256;
    TR0=1;
    ET0=1;
    EA=1;
		beep=1;
	relay=1;
    count++;
    if(count>19)
    {//����ʱ��1��ʱִ�л�������������
        count=0;
        NSF++;		
    }
}



void delayms(unsigned int xms)
{
    unsigned int i,j;
    for(i=xms; i>0; i--)
        for(j=110; j>0; j--);

}


//�������ʾ����
void display1(unsigned char num1,unsigned char num2)
{
    LED_A1=0;
    P0=table[num1];

    delayms(10);
    LED_A1=1;

    LED_B1=0;
    P0=table[num2];

    delayms(10);
    LED_B1=1;
}
void main()
{
	relay =0;
		Int0Init();//�ⲿ�ж�1
	  Init();//��ʱ����ʼ�������ǲ�����
        beep=0;     
        while(1)
        {
            display1(NSF/10,NSF%10);
            delayms(10);
					if(NSF>10)
					{//���10��󣬹رն�ʱ����
		ET0=0;//��ʱ���ж�ʹ��
	  TR0=0;//���� T0
						NSF=0;
				beep=0;
				relay=0;
					}
        }
	}