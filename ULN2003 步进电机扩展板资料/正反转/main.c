/*******************************************************
ULN2003����5V���ٲ����������
Target:STC89C52RC-40C
Crystal:12MHz

Platform:51&avr��Ƭ����Сϵͳ��+ULN2003������������׼�

*******************************************************
���߷�ʽ:
IN1 ---- P10
IN2 ---- P11
IN3 ---- P12
IN4 ---- P13
+   ---- +5V
-   ---- GND
*********************/
#include<reg52.h>
#define uchar unsigned char
#define uint  unsigned int
#define MotorData P1                    //����������ƽӿڶ���
uchar phasecw[4] ={0x03,0x06,0x0c,0x09};//��ת �����ͨ���� D-C-B-A
uchar phaseccw[4]={0x03,0x09,0x0c,0x06};//��ת �����ͨ���� A-B-C-D

uchar key1_down,key2_down,key3_down,key1_flag,key2_flag,key3_flag;//��������
sbit key1 = P3^0;//��������
sbit key2 = P3^1;

//ms��ʱ����
void Delay_xms(uint t)
{
   uchar k;
   while(t--)
   {
    for(k=0; k<125; k++)
    { }
}
	 }
//˳ʱ��ת��
void MotorCW(void)
{
 uchar i;
 for(i=0;i<4;i++)
  {
   MotorData=phasecw[i];
   Delay_xms(200);//ת�ٵ���
  }
}
//��ʱ��ת��
void MotorCCW(void)
{
 uchar i;
 for(i=0;i<4;i++)
  {
   MotorData=phaseccw[i];
   Delay_xms(200);//ת�ٵ���
  }
}


  //�����ⲿ�ж�    
void Init_P( )
{      
      EA=1; //��ȫ���ж�      
      EX0=1; //���ⲿ�ж�0      
      IT0=1; //�ⲿ�ж�0���ش���     
      IT1=0; //�ⲿ�ж�1��ƽ����     
}  
      //�ⲿ�ж�0���� 
void interint0() interrupt 0 //using **     
{     
   	key1_flag = 0;
		key2_flag = 0;
		key3_flag = 1;
  
      
} 
void key_scan()
{
	/************����1*************/
	if(key1 == 0)
	{
		Delay_xms(100);
		if(key1 == 0)key1_down = 1;	
	}
	if(key1_down == 1 && key1 == 1)
	{
		key1_flag = 1;
		key2_flag = 0;
		key3_flag = 0;
		key1_down = 0;	
	}
	/************����2*************/
	if(key2 == 0)
	{
		Delay_xms(100);
		if(key2 == 0)key2_down = 1;	
	}
	if(key2_down == 1 && key2 == 1)
	{
		key1_flag = 0;
		key2_flag = 1;
		key3_flag = 0;
		key2_down = 0;	
	}
	}

/************************������****************************/
main()
{ 
	Init_P( );
   	while(1)
    {  
      	key_scan();
	if(key1_flag == 1)MotorCW();       //�����ת
	if(key2_flag == 1)MotorCCW();		//�����ת
  if(key3_flag == 1) P1 = 0x0f;			//���ֹͣ
	}
}
