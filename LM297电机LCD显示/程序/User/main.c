/**************************************************************
51单片机+L298驱动步进电机+L297测速+LCD1602显示+按键调试+Proteus仿真
**************************************************************/

#include <reg51.h>
#include <stdio.h>

//宏定义
#define uchar unsigned char
#define uint unsigned int
	
//定义提供脉冲的管脚
sbit CLOCK = P0^1;
sbit CW = P0^0;//控制电机方向

//LCD显示用
sbit RS=P2^0;
sbit RW=P2^1;
sbit E=P2^2;


sbit s_add = P3^2;	//速度加
sbit s_mus = P3^3;	//速度减
sbit forward = P3^4;	//正转
sbit reversal = P3^5;	//反转

uchar SpeedChar[]="SPEED(n/min):";
uchar StateChar[]="RUN STATE:";
uchar STATE_CW[]="CW ";
uchar STATE_CCW[]="CCW";

//当前速度
uint speed = 0;
//设置速度标志位
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
		EA=1;//打开总中断	
		TMOD=0x01; //定时器工作方式
		TH0=(65535-1000)/256; //初值约1ms
		TL0=(65535-1000)%256; 
		ET0=1;	//打开定时器中断
		TR0=1;	//打开定时器
		lcd_int();
		display();
		//计算示初始速度
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
	lcd_w_cmd(0x80);	   //设置显示地址（第一行）
	for(i=0;SpeedChar[i]!='\0';i++)		//此处显示SpeedChar[]字符串，字符串结束符为‘0’
	{
		lcd_w_dat(SpeedChar[i]);
	}
	
	lcd_w_cmd(0xC0);  //更改显示位置用
	for(i=0;StateChar[i]!='\0';i++)		//此处显示SpeedChar[]字符串，字符串结束符为‘0’
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
			for(i=0;STATE_CW[i]!='\0';i++)		//此处显示Current_Time[]字符串，字符串结束符为‘0’
			{
				lcd_w_dat(STATE_CW[i]);
			}
		}
		if(CW==1)
		{
			for(i=0;STATE_CCW[i]!='\0';i++)		//此处显示Current_Time[]字符串，字符串结束符为‘0’
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
		for(i=0;str[i]!='\0';i++)		//此处显示Current_Time[]字符串，字符串结束符为‘0’
		{
			lcd_w_dat(str[i]);
		}
}

//函数名：lcd_int
//函数功能：lcd初始化
void lcd_int()
{
	lcd_w_cmd(0x3c);	//设置工作方式
	lcd_w_cmd(0x0c);	  //设置显示状态
	lcd_w_cmd(0x01);	 //清屏
	lcd_w_cmd(0x06);	 //设置输入方式
	lcd_w_cmd(0x80);	 //设置初始显示位置
}
//函数名：lcd_w_cmd
//函数功能：写命令
//形式参数：命令字已经存入com单元中
void lcd_w_cmd(unsigned char com)
{
	unsigned char i;
	do
	{						 //查lcd忙状态
	i=lcd_r_start();		  //调用读状态字函数
	i&=0x80;				   //“与”操作屏蔽掉低7位
	delay(2);
	}while(i!=0);				//lcd忙，继续查询，否则退出循环 
	RW=0;
	delay(1);
	RS=0;						 //RW=0，RS=0，写lcd命令字
	delay(1);
	E=1;						  //E端时序以0或1高低波动
	delay(1);
	P1=com;						  //将com中的命令字写入lcd数据口
	delay(1);
	E=0;
	delay(1);
	RW=1;
	delay(2);
}
//函数名：lcd_w_dat
//函数功能：写数据
void lcd_w_dat(unsigned char dat)
{
	unsigned char i;
	do
	{						   //查忙操作
	i=lcd_r_start();		   //调用读状态字函数
	i&=0x80;				   //“与”操作屏蔽掉低7位
	delay(2);
	}while(i!=0);			   //lcd忙，继续查询，否则退出循环 
	RW=0;
	delay(1);
	RS=1;						 //RW=0，RS=1，写lcd命令字
	delay(1);
	E=1;						  //E端时序以0或1高低波动
	delay(1);
	P1=dat;						   //将dat中的显示数据写入lcd数据口
	delay(1);
	E=0;
	delay(1);
	RW=1;
	delay(2);
}
//函数名：lcd_r_start
//函数功能：读状态字
unsigned char lcd_r_start()
{
	unsigned char s;
	RW=1;			  //RW=1,RS=0,读lcd状态
	delay(1);
	RS=0;
	delay(1);
	E=1;			   //E端时序以0或1高低波动
	delay(1);
	s=P1;				 //从lcd的数据口读状态  
	delay(1);
	E=0;
	delay(1);
	RW=0;
	delay(1);
	return(s);			 //返回值s读取状态忙或空闲
}
void Time() 	interrupt 1
{
		static unsigned int count=0;
		TH0=(65535-1000)/256;  //重装初值
		TL0=(65535-1000)%256;	 //重装初值
		count++;
		//8个脉冲转一圈
		if(count>=set_speed)
		{
				count = 0;
				CLOCK = ~CLOCK;
		}

}