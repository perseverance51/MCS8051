#include<reg52.h>
//#include<intrins.h>
sbit LED = P2^0;
#define FOSC 12000000uL
#define T1MS (65536-FOSC/12/1000)   //12Tģʽ1ms��ʱ
static volatile unsigned long currentMillis = 0;

void Timer0Init(void)		//1����@12.000MHz
{

	TMOD = 0x01;   //set timer0 as mode1 (16-bit)
	TL0 = T1MS;		//���ö�ʱ��ʼֵ����8λ
	TH0 = T1MS >> 8;  		//���ö�ʱ��ʼֵ����8λ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;     //enable timer0 interrupt
  EA = 1;      //open global interrupt switch
}

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1
{
    TL0 = T1MS;                     //reload timer0 low byte
    TH0 = T1MS >> 8;                //reload timer0 high byte
	currentMillis ++;
}
//��ʱ����
void delay(unsigned long interval)
{
    unsigned long previousMillis = currentMillis  ;
	unsigned long temp = interval*0.02;//����
	while(interval>=temp)
	{
  	if (currentMillis - previousMillis >=1 )
		{
			interval--;
			previousMillis++;
		} 
	}		
}

void main() {
	Timer0Init();
	while(1)
 {
	 	LED = !LED ;
 	 delay(3000);
 } 
}

