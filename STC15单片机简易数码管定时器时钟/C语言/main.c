/* 使用了宏晶科技的资料及程序   */


/*************	本程序功能说明	**************
【Proteus仿真】STC单片机IO方式控制74HC595驱动8位数码管显示简易数码管时钟

显示效果为: 数码时钟.

******************************************/

#define 	MAIN_Fosc		22118400L	//定义主时钟

#include	"STC15Fxxxx.H"


#define	Timer0_Reload	(65536UL -(MAIN_Fosc / 1000))		//Timer 0 中断频率, 1000次/秒

#define DIS_DOT		0x20
#define DIS_BLACK	0x10
#define DIS_		0x11

u8 code t_display[]={						//标准字库
//	 0    1    2    3    4    5    6    7    8    9   -
	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x40
};	

u8 code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};		//位码


sbit	P_HC595_SER   = P4^0;	//pin 14	SER		data input
sbit	P_HC595_RCLK  = P5^4;	//pin 12	RCLk	store (latch) clock
sbit	P_HC595_SRCLK = P4^3;	//pin 11	SRCLK	Shift data clock

u8 	LED8[8];		//显示缓冲
u8	display_index;	//显示位索引
bit	B_1ms;			//1ms标志

u8	cnt10ms;

u8	hour,minute,second;	//RTC变量
u16	msecond;

void	DisplayRTC(void);
void	RTC(void);

/**********************************************/
void main(void)
{
	u8	i;

	P0M1 = 0;	P0M0 = 0;	//设置为准双向口
	P1M1 = 0;	P1M0 = 0;	//设置为准双向口
	P2M1 = 0;	P2M0 = 0;	//设置为准双向口
	P3M1 = 0;	P3M0 = 0;	//设置为准双向口
	P4M1 = 0;	P4M0 = 0;	//设置为准双向口
	P5M1 = 0;	P5M0 = 0;	//设置为准双向口
	P6M1 = 0;	P6M0 = 0;	//设置为准双向口
	P7M1 = 0;	P7M0 = 0;	//设置为准双向口
	
	display_index = 0;
	
	AUXR = 0x80;	//Timer0 set as 1T, 16 bits timer auto-reload, 
	TH0 = (u8)(Timer0_Reload / 256);
	TL0 = (u8)(Timer0_Reload % 256);
	ET0 = 1;	//Timer0 interrupt enable
	TR0 = 1;	//Tiner0 run
	EA = 1;		//打开总中断
	
	for(i=0; i<8; i++)	LED8[i] = 0x10;	//上电消隐

	hour   = 22;	//初始化时间值
	minute = 10;
	second = 0;
	DisplayRTC();
	cnt10ms = 0;

	while(1)
	{
		if(B_1ms)	//1ms到
		{
			B_1ms = 0;
			if(++msecond >= 100)	//1秒到,实际是1000，Proteus仿真速度太慢，加速10倍
			{
				msecond = 0;
				RTC();
				DisplayRTC();
			}
//			if(msecond == 50)	DisplayRTC();	//小时后的小数点做秒闪，实际是500，Proteus仿真速度太慢，加速10倍

		}
	}
} 

/********************** 显示时钟函数 ************************/
void	DisplayRTC(void)
{
	if(hour >= 10)	LED8[0] = hour / 10;
	else			LED8[0] = DIS_BLACK;
	LED8[1] = hour % 10;
	LED8[2] = 10;//-
	LED8[3] = minute / 10;
	LED8[4] = minute % 10;
		LED8[5] =10;//-
	if(second >= 10)	LED8[6] = second / 10;
	else			LED8[6] =  0;
	LED8[7] = second % 10;
	//if(msecond >= 500)		LED8[7] |= DIS_DOT;	//小数点做秒闪
}

/********************** RTC演示函数 ************************/
void	RTC(void)
{
		if(++second >= 60)
	{
		second = 0;
		if(++minute >= 60)
		{
			minute = 0;
			if(++hour >= 24)	hour = 0;
		}
	}
}


/**************** 向HC595发送一个字节函数 ******************/
void Send_595(u8 dat)
{		
	u8	i;
	for(i=0; i<8; i++)
	{
		dat <<= 1;
		P_HC595_SER   = CY;
		P_HC595_SRCLK = 1;
		P_HC595_SRCLK = 0;
	}
}

/********************** 显示扫描函数 ************************/
void DisplayScan(void)
{	
	Send_595(~T_COM[display_index]);				//输出位码
	Send_595(t_display[LED8[display_index]]);	//输出段码

	P_HC595_RCLK = 1;
	P_HC595_RCLK = 0;							//锁存输出数据
	if(++display_index >= 8)	display_index = 0;	//8位结束回0
}


/********************** Timer0 1ms中断函数 ************************/
void timer0 (void) interrupt TIMER0_VECTOR
{
	TR0 =0;
	TH0 = (u8)(Timer0_Reload / 256);//Proteus仿真，需要填写重装载值。
	TL0 = (u8)(Timer0_Reload % 256);
	DisplayScan();	//1ms扫描显示一位
	B_1ms = 1;		//1ms标志
	TR0 =1;
}


