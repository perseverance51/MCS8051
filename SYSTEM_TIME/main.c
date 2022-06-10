/*
���ö�ʱ��0ʵ��51��Ƭ�����ϵͳ�ϵ��ۼ�����ʱ��
u32:�����͵ı������ֵ��4294967295���룬
������������49��17ʱ2��47�룬Ȼ�������0��ʼ������ʱ

*/
//#include <reg51.h>
#include <STC89C5xRC.H>
#include <intrins.h>

#define  interval  1000 //������ʱʱ����
sbit RCK=P2^1;//����74HC595�洢�Ĵ���ʱ������:ST_CP
sbit SCK=P2^2;//����74HC595��λ�Ĵ���ʱ������:SH_CP
sbit SER=P2^3;//����74HC595������������:DS
sbit led=P2^0;
 
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;
 
u32 sysRunmillis;	//ϵͳ����ʱ����������浥Ƭ�����ϵ縴λ�������е�ʱ�䣬��λ�Ǻ��롣����ֵ�ɶ�ʱ��T0���ж���Ӧ�Ӻ�������
u8 hour,min,sec;	//��ǰϵͳ����ʱ�任���Сʱ��������������
u8 strTime[]={0,0,0,0,0,0,0,0};	//����ʱ����ÿ1λ�ϵ����֣�0~9�����ָ�����-����ֵ��10
 
u8 DigCode[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40};//����������ܶ���
 
//��ȫ�ֱ���sysRunmillis����ĺ�����ת����ʱ�����ʽ����ʱ�����ÿһλ���֣�0~9������������strTime�
void millis2time()
{
	u32 t;
  t = sysRunmillis/1000ul;	  //����ת��Ϊ��
	hour = t/3600%24;			  //����Сʱ��	
	min = (t%3600)/60;		  //�����
	sec = t%60;				  //������
	//���´���ʱ���֡���ÿ1λ����ʱ������ֵС��10������0ռλ��
	if(hour<10)
	{
		strTime[0]=0;
		strTime[1]=hour;
	}
	else
	{
		strTime[0]=hour/10;
		strTime[1]=hour%10;
	}
	if(min<10)
	{
		strTime[3]=0;
		strTime[4]=min;
	}
	else
	{
		strTime[3]=min/10;
		strTime[4]=min%10;
	}
	if(sec<10)
	{
		strTime[6]=0;
		strTime[7]=sec;
	}
	else
	{
		strTime[6]=sec/10;
		strTime[7]=sec%10;
	}
}
 
//��74HC95���������λѡ�Ͷ�������
void _74HC595SendByte(u8 wei,u8 duan)
{
	u8 i;
	RCK=0;
	for(i=0;i<8;i++)
	{
		SCK=0;
		_nop_();
		SER=wei>>7;
		SCK=1;
		_nop_();
		wei<<=1;
	}
	for(i=0;i<8;i++)
	{
		SCK=0;
		_nop_();
		SER=duan>>7;
		SCK=1;
		_nop_();
		duan<<=1;
	}
	RCK=1;
}
 
//��ʼ����ʱ��T0
void Timer0Init(void)		//1����@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	//������ֵ��ÿ1�����ж�1��
	TL0 = 0x18;		//���ö�ʱ��ʼֵ
	TH0 = 0xFC;		//���ö�ʱ��ʼֵ
//    EA = 1;
//    ET0 = 1;//T0������ж���λ
	IE |= 0x82;//����ǰ��2��
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
}


 
void Timer0Interrupt() interrupt 1
{
	//��ͳ51��Ƭ��û��16λ�Զ�����ģʽ,Proteus����ʱ��Ҫ��������2��
	TL0 = 0x20;		//���ö�ʱ��ʼֵ
	TH0 = 0xD1;		//���ö�ʱ��ʼֵ
    sysRunmillis++;	//ÿ1�����1
}
 
void delayms(u16 ms)   //������ʱ����
{
    u32 temp;
	temp = sysRunmillis ;
    while(sysRunmillis - temp < ms );
}
 
void main()
{
	u8 i;
	u32 previousMillis = 0;//��һ��ʱ��
	u32 currentMillis;//��ǰʱ��
	Timer0Init();
	strTime[2]=10;
	strTime[5]=10;

	while(1)
	{
		currentMillis = sysRunmillis;
		millis2time();
		for(i=0;i<8;i++)
		{//�������ʾ
			_74HC595SendByte(~(0x01<<i),DigCode[strTime[i]]);
		}
		if (currentMillis - previousMillis >= interval) {//ÿ��1�뷭תһ��
    previousMillis = currentMillis;
			led=!led;
		}
	}
}