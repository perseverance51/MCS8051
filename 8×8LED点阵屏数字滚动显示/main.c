//���ƣ�8��8LED�������µ������ֹ�����ʾ
//˵��������ģ���˵�����ʾ�����¹�����ʾ¥���Ч������Ŀ��¥�����
//��ǰ¥��ʱ�������Ϲ�����ʾ����֮�����¹�����ʾ��
#include <reg51.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
uchar code Table[]=
{
	0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C,//0
	0x18,0x38,0x78,0x18,0x18,0x18,0x18,0x18,//1
	0x00,0x7E,0x02,0x02,0x7E,0x40,0x40,0x7E,//2
	0x00,0x7E,0x02,0x02,0x7E,0x02,0x02,0x7E,//3
	0x00,0x48,0x48,0x48,0x7E,0x08,0x08,0x08,//4
	0x00,0x7E,0x40,0x40,0x7E,0x02,0x02,0x7E,//5
	0x00,0x7C,0x40,0x40,0x7C,0x44,0x44,0x7C,//6
	0x00,0x7C,0x04,0x04,0x04,0x04,0x04,0x04,//7
	0x00,0x7C,0x44,0x44,0x7C,0x44,0x44,0x7C,//8
	0x00,0x7E,0x42,0x42,0x7E,0x02,0x02,0x7E,//9
	
};
uint r= 0;
char offset = 0;
uchar Current_Level = 1,Dest_Level=1,x=0,t =0;

void main()
{
	 P3 = 0x80;
	Current_Level = 1;//��1��ʼ��ʾ
	TMOD  = 0x01;	//T0������ʽ
	TH0 = -4000/256;//4ms//ÿ��������8�ֽ�
	TL0 = -4000%256;
	TR0=1;
	IE = 0x82;
	EA =1;
	while(1);	
}

void Led_Dispaly() interrupt 1
{
	uchar i;
	//��ֹͣ����ʱ������а������£����ж�Ŀ��¥��
	if (P1 != 0xff && Current_Level == Dest_Level )
	{
	if (P1 ==0xfe)Dest_Level =5;
	if (P1 ==0xfd)Dest_Level =4;
	if (P1 ==0xfb)Dest_Level =3;
	if (P1 ==0xf7)Dest_Level =2;
	if (P1 ==0xef)Dest_Level =1;
	}
	TH0 = -4000/256;//4ms
	TL0 = -4000%256;
	P3 =_crol_(P3,1);
	i = Current_Level *8 + r + offset;
	P0 = ~Table[i];
	
	//������ʾ
	if (Current_Level < Dest_Level )
	{
		if ( ++r ==8)
		{//ÿ��������8�ֽ�
				r =0;
			if( ++x ==4)
			{//ÿ���x��ˢ�º��ƫ
				x = 0;
				if(++offset ==8)
				{
					offset = 0;
					Current_Level++;				
				}
				
			}
		}
		
	}
	//�½���ʾ
	else if(Current_Level > Dest_Level )
	{
		if ( ++r ==8)
		{//ÿ��������8�ֽ�
			r =0;
			if( ++x ==4)
			{//ÿ���x��ˢ�º�ǰƫ
				x = 0;
				if(--offset == -8)
				{
					offset = 0;
					Current_Level--;				
				}
				
			}
		}
		
	}
	//ֹͣ�����������ȶ���ˢ����ʾ
	else
	{
		if ( ++r ==8) r =0;		
	}
}