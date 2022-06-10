#include "reg52.h"
#include <math.h>
#define ADC_PORT P2 //ADC0832���Ӷ˿�
//5�ֲ�
#define W_SINE 0
#define W_SQU 1
#define W_TRI 2
#define W_TRA 3
#define W_SAW 4
#define PI 3.1415926f //Բ����

//�����ͷ�д��
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

sbit ADC_CS_WR=P3^0; //��ͼ��ADC0832ʹ�ܿ�
//5��LED����P0��
sbit LED0=P0^0;
sbit LED1=P0^1;
sbit LED2=P0^2;
sbit LED3=P0^3;
sbit LED4=P0^4;
//5������
sbit KEY0=P1^0;
sbit KEY1=P1^1;
sbit KEY2=P1^2;
sbit KEY3=P1^3;
sbit KEY4=P1^4;
//ȫ�ֱ���
u8 volatile mode;//ģʽ�����Ҳ�����
u16 freq;//Ƶ��
u8 time;//�ƴβ���
u8 AM;//����


void Timer0Init(void);
void scanKey(void);
void sys_init(void);

void sine_wave(u8 location);//���Ҳ�
void squ_wave(u8 location);//������
void tri_wave(u8 location);//������
void tra_wave(u8 location);//�ݡ���
void saw_wave(u8 location);//�⡭��

void Timer0Init(void)		//100΢��@12.000MHz
{
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x02;		//���ö�ʱ��ģʽ
	TL0 = 0x9C;		//���ö�ʱ��ֵ
	TH0 = 0x9C;		//���ö�ʱ����ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	EA = 1;         //�����ж�
	ET0 = 1;        //����ʱ��0�ж�
}

void scanKey(void)//����ɨ��
{
	if(KEY0==0)
	{
		mode=0;
		P0=0xFF;//�����е�
		LED0=0;
	}
	
	if(KEY1==0)
	{
		mode=1;
		P0=0xFF;//�����е�
		LED1=0;
	}
	
	if(KEY2==0)
	{
		mode=2;
		P0=0xFF;//�����е�
		LED2=0;
	}
	
	if(KEY3==0)
	{
		mode=3;
		P0=0xFF;//�����е�
		LED3=0;
	}
	
	if(KEY4==0)
	{
		mode=4;
		P0=0xFF;//�����е�
		LED4=0;
	}
}

void sys_init(void)//ϵͳ��ʼ��
{
	time=0;
	ADC_CS_WR=0;
	ADC_PORT=0;
	mode=0;//Ĭ��������Ҳ�
	freq=100;//Ĭ��Ƶ��100Hz
	AM=255;//������
}
void sine_wave(u8 location)//������Ҳ�
{
	double x=(double)location/50*PI;//��0-100������0-2��(pai��û���Ǹ����ţ�������(�s�n�t))
	u8 y=(sin(x)*(AM/2)+(AM/2));//���y����������0-254����ΪADC��Χ0-AM���о����оƬ�����
	ADC_PORT=y;
}

void squ_wave(u8 location)//������
{
	if(location<50)
		ADC_PORT=AM;
	else
		ADC_PORT=0x00;
}

void tri_wave(u8 location)//������
{
	//Ϊ�˼򻯣��ڵ��������V����
	u8 y;
	if(location<50)
		y=(50-location)*AM/50;
	else
		y=(location-50)*AM/50;
	ADC_PORT=y;
}
void tra_wave(u8 location)//�ݡ���
{
	u8 y;
	if(location<25)
		y=0xFF;
	else if(location<50)
		y=(50-location)*AM/25;
	else if(location<75)
		y=0;
	else
		y=(location-75)*AM/25;
	ADC_PORT=y;
}
void saw_wave(u8 location)//�⡭��
{
	ADC_PORT=location*AM/100;
	//�ã�100-location��Ҳ�Ա�ɷ�����
}

void Timer0Work() interrupt 1 //�жϷ�����
{
	switch(mode)
	{
		case W_SINE:sine_wave((u8)(time*freq/100)%100);break;//���������λ��
		case W_SQU:squ_wave((u8)((time*freq/100)%100));break;
		case W_TRI:tri_wave((u8)((time*freq/100)%100));break;
		case W_TRA:tra_wave((u8)((time*freq/100)%100));break;
		case W_SAW:saw_wave((u8)((time*freq/100)%100));break;
	}
	time++;
	if(time>=100)//����100��
		time=0;
}

int main(void)
{
	sys_init();
	Timer0Init();
	while(1)
	{
		scanKey();
	}
}

