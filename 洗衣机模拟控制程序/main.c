/*-------------------------------
	ϴ�»�ģ����Ƴ���
--------------------------------*/
#include <REG52.H>

#define uchar unsigned char
#define uint  unsigned int
/* ָʾ��״̬*/
#define qiang_ledon qiang_led=1;
#define qiang_ledoff qiang_led=0;
#define biao_ledon biao_led=1;
#define biao_ledoff biao_led=0;
#define ruo_ledon ruo_led=1;
#define ruo_ledoff ruo_led=0;
#define piao_ledon piao_led=1;
#define piao_ledoff piao_led=0;
#define sgan_ledon sgan_led=1;
#define sgan_ledoff sgan_led=0;
#define jin_ledon jin_led=1;
#define jin_ledoff jin_led=0;
#define pai_ledon pai_led=1;
#define pai_ledoff pai_led=0;

unsigned char code Tab[11]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F}; //�����������ʾ0��9�Ķ����
unsigned char int_time;        //��¼�жϴ���
unsigned char second;        //������
uint t=0,tt=0;
/* �������Ŷ��� */
   sbit k1=P0^0;
   sbit k2=P0^1;
   sbit k3=P0^2;
/* ָʾ�����Ŷ��� */
   sbit sg=P0^3;
   sbit hk1=P0^4;
   sbit hk2=P0^5;
   sbit ledon=P0^7;
   sbit qiang_led=P2^0;
   sbit biao_led=P2^1;
   sbit ruo_led=P2^2;
   sbit piao_led=P2^3;
   sbit sgan_led=P2^4;
   sbit jin_led=P2^5;
   sbit pai_led=P2^6;
/* ����������Ŷ��� */
   sbit p30=P3^0;//
   sbit p31=P3^1;//
   sbit beepon=P2^7;//����������
	 /* �����λ�л����Ŷ��� */
   sbit sm1=P3^6;
   sbit sm2=P3^7;
	 
void delay2(void)
   {
  unsigned char m;
  for(m=0;m<200;m++)
        ;
   }

void delay1(int s)
   { 
   int i;
   for(;s>0;s--)
   for(i=0;i<65;i++);
   }
 
void delay(uint i)
   {
   uchar j;
   for(i;i>0;i--)
   for(j=255;j>0;j--);
   }
/* �������ʾ���� */
void DisplaySecond(unsigned char k)
{
   
   sm1=0;        //P2.6��������͵�ƽ�� DS6���� 
   P1=Tab[k/10];          //��ʾʮλ
     delay2();
     delay2();
   sm1=1;
   sm2=0;       //P2.7��������͵�ƽ�� DS7���� 
   P1=Tab[k%10];         //��ʾ��λ
     delay2();
     delay2();
   P3=0xff;     //�ر����������
   P1=1;        //��ʾ��λ
     delay2();
     delay2();
}
	/* �������������� */   
void beep()
   {
  	p30=0;
    p31=0;
   	t=0;
	while(1)
	{
	  beepon ^=1;
      delay(300);
      if(t>=80) break;
	}
	beepon=0;
	biao_led=0;
    }
	 /* ǿϴģʽ */ 
void f_qiang()
    {	 
	 qiang_ledon;
	 t=0;
	 tt=0;	
     int_time=0;
     second=00;
	 while(1)
    {	
	  if(tt>=400) break; 
      t=0;
	  while(t<80&&tt<=400)
	  {	
	    p31=0;
	    p30=1;	//��ת 4s
        DisplaySecond(second);
	    }
	  t=0;  	//��ת 4s	
	  while(t<80&&tt<=400)
	  {
	    p30=0;
	    p31=1;
	    DisplaySecond(second);
	    }
     }
	  p30=0;
	  p31=0;	  
     }
/* ��׼ϴģʽ */
void f_biao()
	{
	  qiang_led=0;
      biao_led=1;
	  t=0;
	  tt=0;
      int_time=0;
      second=00;
	 while(1)
     {
	   if(tt>=400) break; 
	   t=0;
	  while(t<60&&tt<=400)
	   {	
          p31=0;
		  p30=1;	//��ת3s
          DisplaySecond(second);
       }

		t=0;  	//��ת3s
	  while(t<60&&tt<=400)
       {
	      p30=0;
		  p31=1;
		  DisplaySecond(second);
		}
       }
       p30=0;
	   p31=0;
	 }
/* ��ϴģʽ */
void f_ruo()
    {
	  qiang_led=0;
	  biao_led=0;
	  ruo_led=1;
	  t=0;
	  tt=0;
      int_time=0;
       second=00;
		while(1)
       {
	   	 if(tt>=400) break; 
		 t=0;
	       while(t<40&&tt<=400)
           {	p31=0;
		        p30=1;	//��ת2s
                DisplaySecond(second);
	        }
	     t=0;  	//��ת 2s
	       while(t<40&&tt<=400)
            {   p30=0;
	        	p31=1;
		        DisplaySecond(second);
	      	}
         }
		p30=0;
	    p31=0;
       }
/* Ưϴģʽ */
void f_piao()
	{
	   qiang_ledoff;
	   biao_ledoff;
	   piao_ledon;
	   t=0;
	   tt=0;
       int_time=0;
       second=00;
		 while(1)
          {
	   	   if(tt>=400) break; 
		   t=0;
	         while(t<40&&tt<=400)
               {	p31=0;
	                p30=1;	//��ת2s
                    DisplaySecond(second);
	           }
		   t=0;  	//��ת2s
	         while(t<40&&tt<=400)
               {    p30=0;
		            p31=1;
	             	DisplaySecond(second);
		       }
             }
	  p30=0;
      p31=0;
	  piao_ledoff;
	  }
/* ��ˮָʾ�ƺ��� */
void pai_leds()
     {
	    p30=0;
	    p31=0;
   	    pai_led=1;	//��ˮ������ �ȴ���������2 
	    while(hk2);
	    if(hk2==0)  //����պ�
	    pai_led=0;	//��ˮ����
     }
/* ��ˮָʾ�ƺ��� */
void jin()
    {
	    p30=0;
	    p31=0;
	    jin_ledon; 
 	    while(hk1);
	    if(hk1==0)
	    jin_ledoff;
     }
/* ˦��ָʾ�ƺ��� */
void sgan()
    {
	    sgan_ledon;	 //˦�ɵ���
 	    t=0;
        int_time=0;
        second=00;
	     while(t<=400)
            {	
			    p31=0;
		        p30=1;	
                DisplaySecond(second);
	         }
        sgan_ledoff;
    }

void sgan1()
    {
	   sgan_ledon;
 	   t=0;
       int_time=0;
       second=00;
	     while(t<=400)
            {	
			   p31=0;
	           p30=1;	
               DisplaySecond(second);
	         }
        piao_ledoff;
        sgan_ledoff;
        beep();
    	t=401;
    }

/* ϴ��ģʽ1 */
void f_an1()
   {  
	qiang_ledon; //����1����
	jin();  //����ǿϴ
	f_qiang(); 	//ѭ��20S
	delay1(4000);//��ʱ4s
	f_biao();  //ѭ��20S
	delay1(4000);//��ʱ4s
	f_ruo();  //ѭ��20S
	ruo_ledoff;	
	pai_leds();
	delay1(1000); //��ʱ1S ����˦��
	sgan(); //˦��20s
	sgan_ledoff;
	ruo_ledoff;
	piao_ledoff;
	jin();
	delay1(1000); //��ʱ1S ����Ưϴ
	f_piao();  //ѭ��20S
	sgan_ledoff;
	piao_ledoff;
	pai_leds();
	delay1(1000); //��ʱ1S ����˦��
	sgan1();//����˦��20S
	delay(500);//��ʱ	
	beep();
   }
/* ϴ��ģʽ2 */
void f_an2()
   {  
	biao_ledon; //����1����
	jin();
	f_biao();  //ѭ��20S
	delay1(4000); //��ʱ4S
	f_ruo();  //ѭ��20S
	ruo_ledoff;
	pai_leds();
	delay1(1000); //��ʱ1S ����˦��
	sgan(); //˦��20s
	ruo_ledoff;
	piao_ledoff;
	jin();
	delay1(1000); //��ʱ1S ����Ưϴ
	f_piao();  //ѭ��20S
	sgan_led=0;
	pai_leds();
	delay1(1000); //��ʱ1S ����˦��
	sgan();//����˦��20s
	delay(500);//��ʱ5s
	piao_led=0;
	beep(); 
   }
/* ϴ��ģʽ3 */
void f_an3()
  {  
	ruo_led=1; //����1����
	jin();
	f_ruo();  //ѭ��20S
	ruo_ledoff;
	pai_leds();
	delay1(1000); //��ʱ1S ����˦��
	sgan(); //˦��20S
	ruo_ledoff;
	piao_ledoff;
	jin();
	delay1(1000); //��ʱ1S ����Ưϴ
	f_piao();  //ѭ��20S
	sgan_led=0;
	pai_leds();
	delay1(1000); //��ʱ1S ����˦��
	sgan();//����˦��20s
	delay(500);//��ʱ5s
	piao_led=0;
	p30=0;
	beep();
   }


void main(void)
{	 
  TMOD=0x01;       //��ʽ1 16λ������
  TH0=-50000/256;   //�趨��ֵ
  TL0=-50000%256;
  EA=1;               //�����ж�
  ET0=1;             //����ʱ��0�ж�
  TR0=1;             //������ʱ��0
  P2=0;
  int_time=0;     //�жϴ�����ʼ��
  second=00;	   //���ʼ��
	 while(1)
    {	
	  if(k1==0)
	{	 
		delay(10);
		if(k1==0) 
	    {
			f_an1();
		}
	}
	if(k2==0)
	{
		delay(10);
		if(k2==0)
		{
			f_an2();
		}
	}
	if(k3==0)
	{
	    delay(10);
	   if(k3==0)
	    {
	 	   f_an3();
	    }
	}

 	if(sg==0)
	{
		delay(10);
		if(sg==0)
		{
		   sgan1();
		}
	}
}
//	return 0;
}  


void time0() interrupt 1 using 1
   {  
     int_time ++;        //ÿ��һ���ж�,�жϴ���int_time�Լ�1
    if(int_time==20)   //��20���ж�,��1���ӽ���һ�μ��������
      {
        int_time=0;    //�жϴ�����0
        second++;    //���1
      }	            
   TH0 =-50000/256;   //50000  =50ms�ж�һ��
   TL0 =-50000%256;
   t++; 
   tt++;
   }

