//名称：8×8LED点阵屏仿电梯数字滚动显示
//说明：本例模拟了电梯显示屏上下滚动显示楼层的效果，当目标楼层大于
//当前楼层时，将向上滚动显示，反之则向下滚动显示。
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
	Current_Level = 1;//从1开始显示
	TMOD  = 0x01;	//T0工作方式
	TH0 = -4000/256;//4ms//每个数字有8字节
	TL0 = -4000%256;
	TR0=1;
	IE = 0x82;
	EA =1;
	while(1);	
}

void Led_Dispaly() interrupt 1
{
	uchar i;
	//在停止滚动时，如果有按键按下，则判断目标楼层
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
	
	//上升显示
	if (Current_Level < Dest_Level )
	{
		if ( ++r ==8)
		{//每个数字有8字节
				r =0;
			if( ++x ==4)
			{//每完成x次刷新后后偏
				x = 0;
				if(++offset ==8)
				{
					offset = 0;
					Current_Level++;				
				}
				
			}
		}
		
	}
	//下降显示
	else if(Current_Level > Dest_Level )
	{
		if ( ++r ==8)
		{//每个数字有8字节
			r =0;
			if( ++x ==4)
			{//每完成x次刷新后前偏
				x = 0;
				if(--offset == -8)
				{
					offset = 0;
					Current_Level--;				
				}
				
			}
		}
		
	}
	//停止滚动，保持稳定的刷新显示
	else
	{
		if ( ++r ==8) r =0;		
	}
}