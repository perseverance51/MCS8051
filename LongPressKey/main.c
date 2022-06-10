#include <STC89C5xRC.H>
#include <intrins.h>
typedef unsigned int u16;	  // typedef�����Ͷ��壩 �������޷��������������� unsigned int ���������� uint
typedef unsigned char u8;  //�������޷����ַ����������� unsigned char ���������� uchar

#define KEY_MODE 0x01    // ģʽ����
#define KEY_PLUS 0x02     // ��
#define FOSC 11059200L
#define T1MS (65536-FOSC/12/1000)   //1ms timer calculation method in 12T mode

sbit KeyValue=P3^7;
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
unsigned char code smgduan[]= {
//0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90
	0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f
};
//���干���������ʾ0~9
 u8 cnt_plus=0;
u8 Trg,Cont,CNT;
u16  Count=0;

u8 DisplayData[8];
u8 datnum1=0,datnum2=0,datnum3=0,datnum4=0;

void delay(u16 i)
{
	while(i--);	
}
void Timer0Init(void)		//20����@11.0592MHz
{
	 TMOD = 0x01;                    //set timer0 as mode1 (16-bit)
    TL0 = T1MS;                     //initial timer0 low byte
    TH0 = T1MS >> 8;                //initial timer0 high byte
    TR0 = 0;                        //timer0 start running
    ET0 = 1;                        //enable timer0 interrupt
    EA = 1;                         //open global interrupt switch
    CNT = 0;                      //initial counter
}

void KeyRead()
{
    unsigned char ReadData = KeyValue^0x01;   //������㣬����ʱΪ1��û�а�ʱΪ0
    Trg = ReadData & (ReadData ^ Cont);   // ��������Ϊ1
    Cont = ReadData;                     // ��������Ϊ1
}
void KeyProc(void)
{
	
    if (Trg & KEY_MODE) // ������µ���KEY_MODE�������㳣���ⰴ��Ҳû���ã�
    {   //���ǲ���ִ�еڶ��ε� �� �������ɿ��ٰ���
        Count++;         // ִ��+1
    
    if (Cont^KeyValue) // ������ӡ����������Ų���
    {
			TR0=1;//������ʱ��
         if (cnt_plus>50) // 20ms*50 = 1S ���ʱ�䵽
            Count +=9;      // ִ��+10
				 cnt_plus =0;
  }else {
		TR0=0;//������ǳ����͹رն�ʱ��
		cnt_plus=0;
		}
		if(Count>=9999) {  //����0~9���������ʾ�ص�0
                	Count=9999;
            }
}
		}
/*******************************************************************************
* �� �� ��         : DigDisplay
* ��������		   : ����ܶ�̬ɨ�躯����ѭ��ɨ��8���������ʾ
*******************************************************************************/
void DigDisplay()
{
	u8 i;

	for(i=0;i<8;i++)
	{
		switch(i)	 //λѡ��ѡ�����������ܣ�
		{
			case(0):
				LSA=1;LSB=1;LSC=1; break;//��ʾ��7λ
			case(1):
				LSA=0;LSB=1;LSC=1; break;//��ʾ��6λ
			case(2):
				LSA=1;LSB=0;LSC=1; break;//��ʾ��5λ
			case(3):
				LSA=0;LSB=0;LSC=1; break;//��ʾ��4λ
			case(4):
				LSA=1;LSB=1;LSC=0; break;//��ʾ��3λ 
			case(5):
				LSA=0;LSB=1;LSC=0; break;//��ʾ��2λ 
			case(6):
				LSA=1;LSB=0;LSC=0; break;//��ʾ��1λ 
			case(7):
				LSA=0;LSB=0;LSC=0; break;//��ʾ��0λ	
		}
		P0=DisplayData[i];//���Ͷ���
		delay(100); //���һ��ʱ��ɨ��	
		P0=0x00;//����
	}
}

void main() {
	
	Timer0Init();
    while(1) {
      KeyRead();
			KeyProc();
 			DigDisplay();	
			DisplayData[0]=smgduan[Count%1000%100%10];
			DisplayData[1]=smgduan[Count%1000%100/10];
			DisplayData[2]=smgduan[Count%1000/100];
			DisplayData[3]=smgduan[Count/1000];		
    }
}
void tm0_isr() interrupt 1
{  
    TL0 = T1MS;                     //reload timer0 low byte
    TH0 = T1MS >> 8;                //reload timer0 high byte
    if (CNT++ > 20)               //1ms * 1000 -> 1s
    {
        CNT = 0;               //reset counter
			cnt_plus++;       // ��ʱ
         
       
    }
}