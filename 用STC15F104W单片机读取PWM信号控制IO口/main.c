//ת����ע����http://bbs.5imx.com
//оƬ�ͺ�ΪSTC15F100W-STC15F104W,��д����ʱ��ѡ��IRCƵ��12M.
//STC15F100WоƬ��P3.2��Ϊģ�ͽ��ջ�PWM�ź������.
//������ɸ��ݸ�����������жϱ���a��ֵ���ﵽ��Ӧ��Ҫ��.
//����a��ֵ��λΪ΢�룺1000us=1ms,1500=1.5ms,2000=2.0ms

#include<reg51.h>
sfr AUXR = 0x8E;
unsigned int a0=0;
unsigned int a1=0;
//sbit led1=P3^3;
sbit led2=P3^4;
sbit led1=P3^5;
bit HL;
                   
void pulseIn0();
void pulseIn1();
 
 
void Timer0Init(void)
{
  AUXR &= 0x7F;
  TMOD &= 0xF0;
  TL0 = 0x00;
  TH0 = 0x00;
  TF0 = 0;
  TR0 = 0;
}
 
 
void main()
{   
  EA   = 1;
  //INT0 = 1;
  EX0  = 0; 
  IT0  = 0;  
  EX1  = 0; 
  IT1  = 0;
  Timer0Init();
  while(1){  
    pulseIn1();
    if(a0>980&&a0<1020)led2=0;  else led2=1;
    pulseIn0();
    if(a1>1480&&a1<1520)led1=0; else led1=1;
  }
}
 
 //�ⲿ�ж�0
void INT_0 (void) interrupt 0  using 2
{
   HL = INT0;
   if(HL==1)TR0 = 1;
   if(HL==0){
     TR0 = 0;
     a0   = TH0;
     a0   = a0*256+TL0;
     TL0 = 0x00;
     TH0 = 0x00; 
     EX0  = 0; 
   }
}
�ⲿ�ж�1
void INT_1 (void) interrupt 2  using 2
{
   HL = INT1;
   if(HL==1)TR0 = 1;
   if(HL==0){
     TR0 = 0;
     a1   = TH0;
     a1  = a1*256+TL0;
     TL0 = 0x00;
     TH0 = 0x00; 
     EX1  = 0; 
   }
}
void pulseIn0(){
  while(INT0==1);
  EX0  = 1; 
}
void pulseIn1(){
  while(INT1==1);
  EX1  = 1; 
}