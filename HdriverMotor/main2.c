#include <reg52.h>

//��������
void DelayMs(int i);
void motor_z();
void motor_f();
void motor_off();

//A B C D��˿ڶ���
sbit A1=P2^1; 
sbit B1=P2^2;
sbit C1=P2^3;
sbit D1=P2^4;

//�����˿ڶ���
sbit key1=P3^0; //��ת
sbit key2=P3^1;	//��ת
sbit key3=P3^2;	//ֹͣ
sbit key4=P3^3; //

//ͨ�緽ʽ
#define Coil_A1   {A1=1;B1=0;C1=0;D1=0;}
#define Coil_B1   {A1=0;B1=1;C1=0;D1=0;}
#define Coil_C1   {A1=0;B1=0;C1=1;D1=0;}
#define Coil_D1   {A1=0;B1=0;C1=0;D1=1;}
#define Coil_OFF  {A1=0;B1=0;C1=0;D1=0;}

unsigned char Speed;
//�����ת
void motor_z();
//���ֹͣ
void motor_off();
//�����ת
void motor_f();
//����ɨ��
void keyscan();
void DelayMS(unsigned int t);

void main()
{
 
  Speed=200; 
  while(1)  
  {         
     keyscan();
  }
}

//�����ת
void motor_z()
{
     Coil_A1                 
     DelayMs(Speed);        
	                         
     Coil_B1
     DelayMs(Speed);
		
     Coil_C1
     DelayMs(Speed);
		
     Coil_D1
     DelayMs(Speed);
}

//�����ת
void motor_f()
{
     Coil_D1
     DelayMs(Speed);
	
     Coil_C1
     DelayMs(Speed);
	
     Coil_B1
     DelayMs(Speed);
	
     Coil_A1
     DelayMs(Speed);
}

//���ֹͣ
void motor_off()
{
    Coil_OFF
}

//��ʱ

void DelayMS(unsigned int t)
{
 unsigned char k;
   while(t--)
   {
    for(k=0; k<125; k++);
}	
}

//����ɨ��
void keyscan()
{
  static int a=0;
	static int b=0;
	
	if(key1==0)
	{ 
	  DelayMs(100);	
		if(key1==0)
		{
			a=~a;
			while(!key1);
		}
	}				
	
	if(a)
	{
	  motor_z();	
	}
	
	
	if(key2==0)
	{ 
	  DelayMs(100);	
		if(key2==0)
		{
			b=~b;
			while(!key2);
		}
	}				
	
	if(b)
	{
	  motor_f();	
	}
	
	
	if(key3==0)
	{ 
	  DelayMs(100);	
		if(key3==0)
		{
			a=0;
			b=0;
			while(!key3);
		}
	}				
	
	if(a==0&&b==0)
	{
	  motor_off();	
	}
	
}
