#include <REGX52.H>
#include <intrins.h>

typedef unsigned char   uchar;
typedef unsigned int    uint;
sbit LED = P1^0;
//uchar const sequence[8] = {0x02,0x06,0x04,0x0c,0x08,0x09,0x01,0x03};
uchar const sequence[] = {0x06,0x0c,0x09,0x03};
//0010,0b0110,0100,0b1100,1000,0b1001,0001,0b0011
#define out_port  (P2)
sbit  interruptPin = P3^2;
#define HIGH  (1)
#define LOW (0)
#define  Speed 120//�ٶ�
#define FOSC 12000000L
#define T1MS (65536-FOSC/12/1000)   //1ms timer calculation method in 12T mode
static volatile unsigned char keyFlag = 0  ;//����״̬
static volatile unsigned long currentMillis = 0;
//unsigned long previousMillis;        // ������һ�ص�ʱ���
void fanzhuan();
void zhengzhuan();
void Stop();

void exint0() interrupt 0           //(location at 0003H)
{
     keyFlag++;
		keyFlag%=3;
}
/* Timer0 interrupt routine */
void tm0_isr() interrupt 1
{
    TL0 = T1MS;                     //reload timer0 low byte
    TH0 = T1MS >> 8;                //reload timer0 high byte
	currentMillis ++;

}

void Timer0Init(void)		//1����@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x18;		//���ö�ʱ��ʼֵ
	TH0 = 0xFC;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
}

void delay(unsigned long interval)
{
    unsigned long previousMillis = currentMillis  ;
	while(interval>0)
	{
    if (currentMillis - previousMillis >=1 )
		{
			interval --;
		} 
	}		
}
/*
void delay(unsigned int t)
{
  unsigned int count = 0;
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
*/

void main() {
	Timer0Init();
	  IT0 = 1;                        //set INT0 int type (1:Falling 0:Low level)
    EX0 = 1;                        //enable INT0 interrupt
    EA = 1;                         //open global interrupt switch
	   out_port = 0x00;
	while(1)
 {
	 if(keyFlag)keyFlag==1? zhengzhuan():fanzhuan(); else  Stop();			
 } 
}

void Stop(){
  //���������д�������ƴ���
     out_port = 0X0f;
     LED= LOW;
  }

void zhengzhuan(){ 
				static unsigned char i=0;
				i = i<4 ? i+1 : 0;
         out_port = sequence[i];
					LED = !LED;
delay(Speed);//����ֵ�������100����������ʱ��ᶶ�� .

      }
    
void fanzhuan(){
	static unsigned char k=0;
	k = k>0 ? k-1 : 3;	
	   out_port = sequence[k];
     LED= !LED;
delay(Speed);//����ֵ�������100����������ʱ��ᶶ�� .

     }  
