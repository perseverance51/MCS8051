/**************************************************************
51��Ƭ��+L298�����������+L297����+LCD1602��ʾ+��������+Proteus����
**************************************************************/

#include <reg51.h>
#include <stdio.h>

//�궨��
#define uchar unsigned char
#define uint unsigned int
	
//�����ṩ����Ĺܽ�
sbit CLOCK = P0^1;
sbit CW = P0^0;//���Ƶ������

//LCD��ʾ��
sbit RS=P2^0;
sbit RW=P2^1;
sbit E=P2^2;


sbit s_add = P3^2;	//�ٶȼ�
sbit s_mus = P3^3;	//�ٶȼ�
sbit forward = P3^4;	//��ת
sbit reversal = P3^5;	//��ת

uchar SpeedChar[]="SPEED(n/min):";
uchar StateChar[]="RUN STATE:";
uchar STATE_CW[]="CW ";
uchar STATE_CCW[]="CCW";

//��ǰ�ٶ�
uint speed = 0;
//�����ٶȱ�־λ
uint set_speed = 30;


void lcd_int();
void lcd_w_cmd(unsigned char com);
void lcd_w_dat(unsigned char dat);
unsigned char lcd_r_start();
void display();
void display_speed();
void dis_state();
void delay(uint j)
{
		uint x;
		for(x=0;x<j;x++);
}

void key_scan()
{
		if(forward==0)
		{
				delay(1000);
				if(forward==0)
				{
						CW=1;
						dis_state();
				}
				while(!forward);
		}
		if(reversal==0)
		{
				delay(1000);
				if(reversal==0)
				{
						CW=0;
						dis_state();
				}
				while(!reversal);
		}
		
		if(s_add==0)
		{
				delay(1000);
				if(s_add==0)
				{
					if(set_speed>10)
						set_speed -=2;
						speed=(60000/(8*set_speed));
						display_speed();
						while(!s_add);
				}
		}
		
		if(s_mus==0)
		{
				delay(1000);
				if(s_mus==0)
				{
						if(set_speed<50)
						set_speed +=2;
						speed=(60000/(8*set_speed));
						display_speed();
						while(!s_mus);
				}		
		}

}


void main()
{
		EA=1;//�����ж�	
		TMOD=0x01; //��ʱ��������ʽ
		TH0=(65535-1000)/256; //��ֵԼ1ms
		TL0=(65535-1000)%256; 
		ET0=1;	//�򿪶�ʱ���ж�
		TR0=1;	//�򿪶�ʱ��
		lcd_int();
		display();
		//����ʾ��ʼ�ٶ�
		speed=(60000/(8*set_speed));
		display_speed();
		dis_state();
		while(1){
				key_scan();
		}

}

void display()
{
	
	uchar i=0;
	lcd_w_cmd(0x80);	   //������ʾ��ַ����һ�У�
	for(i=0;SpeedChar[i]!='\0';i++)		//�˴���ʾSpeedChar[]�ַ������ַ���������Ϊ��0��
	{
		lcd_w_dat(SpeedChar[i]);
	}
	
	lcd_w_cmd(0xC0);  //������ʾλ����
	for(i=0;StateChar[i]!='\0';i++)		//�˴���ʾSpeedChar[]�ַ������ַ���������Ϊ��0��
	{
		lcd_w_dat(StateChar[i]);
	}
}


void dis_state()
{
		uchar i;
		lcd_w_cmd(0xCB);
		if(CW==0)
		{
			for(i=0;STATE_CW[i]!='\0';i++)		//�˴���ʾCurrent_Time[]�ַ������ַ���������Ϊ��0��
			{
				lcd_w_dat(STATE_CW[i]);
			}
		}
		if(CW==1)
		{
			for(i=0;STATE_CCW[i]!='\0';i++)		//�˴���ʾCurrent_Time[]�ַ������ַ���������Ϊ��0��
			{
				lcd_w_dat(STATE_CCW[i]);
			}
		}
}
void display_speed()
{
		uchar str[3]={""}; 
		uchar i;
		lcd_w_cmd(0x8d);
		sprintf(str,"%u  ",speed);
		for(i=0;str[i]!='\0';i++)		//�˴���ʾCurrent_Time[]�ַ������ַ���������Ϊ��0��
		{
			lcd_w_dat(str[i]);
		}
}

//��������lcd_int
//�������ܣ�lcd��ʼ��
void lcd_int()
{
	lcd_w_cmd(0x3c);	//���ù�����ʽ
	lcd_w_cmd(0x0c);	  //������ʾ״̬
	lcd_w_cmd(0x01);	 //����
	lcd_w_cmd(0x06);	 //�������뷽ʽ
	lcd_w_cmd(0x80);	 //���ó�ʼ��ʾλ��
}
//��������lcd_w_cmd
//�������ܣ�д����
//��ʽ�������������Ѿ�����com��Ԫ��
void lcd_w_cmd(unsigned char com)
{
	unsigned char i;
	do
	{						 //��lcdæ״̬
	i=lcd_r_start();		  //���ö�״̬�ֺ���
	i&=0x80;				   //���롱�������ε���7λ
	delay(2);
	}while(i!=0);				//lcdæ��������ѯ�������˳�ѭ�� 
	RW=0;
	delay(1);
	RS=0;						 //RW=0��RS=0��дlcd������
	delay(1);
	E=1;						  //E��ʱ����0��1�ߵͲ���
	delay(1);
	P1=com;						  //��com�е�������д��lcd���ݿ�
	delay(1);
	E=0;
	delay(1);
	RW=1;
	delay(2);
}
//��������lcd_w_dat
//�������ܣ�д����
void lcd_w_dat(unsigned char dat)
{
	unsigned char i;
	do
	{						   //��æ����
	i=lcd_r_start();		   //���ö�״̬�ֺ���
	i&=0x80;				   //���롱�������ε���7λ
	delay(2);
	}while(i!=0);			   //lcdæ��������ѯ�������˳�ѭ�� 
	RW=0;
	delay(1);
	RS=1;						 //RW=0��RS=1��дlcd������
	delay(1);
	E=1;						  //E��ʱ����0��1�ߵͲ���
	delay(1);
	P1=dat;						   //��dat�е���ʾ����д��lcd���ݿ�
	delay(1);
	E=0;
	delay(1);
	RW=1;
	delay(2);
}
//��������lcd_r_start
//�������ܣ���״̬��
unsigned char lcd_r_start()
{
	unsigned char s;
	RW=1;			  //RW=1,RS=0,��lcd״̬
	delay(1);
	RS=0;
	delay(1);
	E=1;			   //E��ʱ����0��1�ߵͲ���
	delay(1);
	s=P1;				 //��lcd�����ݿڶ�״̬  
	delay(1);
	E=0;
	delay(1);
	RW=0;
	delay(1);
	return(s);			 //����ֵs��ȡ״̬æ�����
}
void Time() 	interrupt 1
{
		static unsigned int count=0;
		TH0=(65535-1000)/256;  //��װ��ֵ
		TL0=(65535-1000)%256;	 //��װ��ֵ
		count++;
		//8������תһȦ
		if(count>=set_speed)
		{
				count = 0;
				CLOCK = ~CLOCK;
		}

}