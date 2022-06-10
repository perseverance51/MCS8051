/***********************************************************
�����ܣ�ADC�ɼ���ѹ�źţ�0-5V��3.3V��
***********************************************************/
#include "reg51.h"
#include "intrins.h"
#include <stdio.h> 	       // Ϊʹ��KEIL�Դ��Ŀ⺯��printf������

sfr ADC_CONTR = 0XBC;
sfr ADC_RES   = 0XBD;
sfr ADC_LOW2  = 0XBE;
sfr P1ASF     = 0X9D;
sfr AUXR = 0x8E; //�����Ĵ���
typedef unsigned char BYTE;
typedef unsigned int WORD;
 
#define ADC_POWER   0X80
#define ADC_FLAG    0X10
#define ADC_START   0X08
#define ADC_SPEEDLL 0X00
#define ADC_SPEEDL  0X20
#define ADC_SPEEDH  0X40
#define ADC_SPEEDHH 0X60
 
void InitADC();
void Delay(WORD n);
WORD GetADCResult(BYTE ch);
void DisplayADCResult(BYTE Channel);//ѡ��0-7ͨ������һ����Ϊת������
 
 void delay_ms(WORD t)//���ö�ʱ����Ϊ��ʱ
{
  WORD count = 0;
  TMOD |= 0x01;
  TH0 = 0xFC;
  TL0 = 0x66;
  TR0 = 1;
  while (1)
  {
    if (TF0 == 1)
    {
      count++;
      TF0 = 0;
      TH0 = 0xFC;
      TL0 = 0x66;
    }
    if (count >= t)
    {
      break;
    }
  }
}

 /*----------------------------
��ʼ������
----------------------------*/
void InitUart()
{
//����������ö�ʱ��1
	TMOD = 0x20;	// 0010 0000 ��ʱ��1�����ڷ�ʽ2��8λ�Զ���װ��ʽ��
	TH1  = 0xFD;	// �����ʣ�9600 /11.0592MHZ
	TL1  = 0xFD;	// �����ʣ�9600 /11.0592MHZ
	TR1  = 1;
	//����������ö�����
	AUXR = 0x00;             // �ܹؼ���8051��ʱ���ٶȣ�ʹ�ö�ʱ��1��Ϊ�����ʷ�������S1ST2=0
	SCON = 0x50; 	// 0101 0000 SM0.SM1=01(���ձ��8λͨ�ţ�,REN=1��������ܣ�
	TI=1;			// �ܹؼ���ʹ��printf����ʱ�����д�����
}
void main()
{
	 InitUart();   //Init UART, use to show ADC result
	InitADC();
	while(1)
	{
		delay_ms(1000);
		DisplayADCResult(1);		
	}
}
//ƽ���˲�//
#define N 100
float AD_average(BYTE Channel)
{
	float value=0;
	BYTE count;
	for(count=0;count<100;count++)
	{
	value+=GetADCResult(Channel);   //100�β���
	}
//value/=202.5;
// value/=205;
// value/=2048;
// value/=512;
 value/=100;		   //��ƽ��
 value=value*5.0/1024;//val/1024*5
 return value;
}

void DisplayADCResult(BYTE Channel)
{
	float ADResult;
//	WORD Result;
	ADResult=AD_average(Channel)*100;//��ѹֵ�Ŵ�100��
//	Result=(int)ADResult;
printf("��ѹֵ = %.4f \n",ADResult); //����������С�����4λ
		
}
 
WORD GetADCResult(BYTE ch)
{
	ADC_CONTR=ADC_POWER|ADC_SPEEDLL|ch|ADC_START;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	while(!(ADC_CONTR&ADC_FLAG));
	ADC_CONTR&=~ADC_FLAG;
 
	return ADC_RES*4+ADC_LOW2;
}
 
void InitADC()
{
	P1ASF = 0XFF;
	ADC_RES = 0;
	ADC_CONTR = ADC_POWER|ADC_SPEEDLL;
	Delay(2);
}
 
void Delay(WORD t)
{
	WORD x;
	while(t--)
	{
		x = 5000;
		while(x--);
	}
}
 
 