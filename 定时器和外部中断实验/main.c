#include <reg51.h>
unsigned char table[]={ 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned char count=0;
sbit LED_B1=P3^6;
sbit LED_A1=P3^7;
sbit fmq=P1^0;
unsigned char counter;
unsigned char NSF=0;

typedef unsigned int u16;          //对数据类型进行声明定义
typedef unsigned char u8;

sbit k1=P3^1;
sbit k2=P3^0;

sbit beep=P1^0;
sbit relay=P1^1;
u8 start=0;
void delayms(unsigned int xms);
//定时器0初始化
void Init(void)
{
    TMOD=0x01;
    TH0=(65536-10000)/256;
    TL0=(65536-10000)%256;
}
/****************************************************************
***************
* 函 数 名 : Int1Init()
* 函数功能 : 设置外部中断 1
* 输 入 : 无
* 输 出 : 无
*****************************************************************
**************/
void Int0Init()
{
		//设置 INT0
		IT0=1;//跳变沿出发方式（下降沿）
		EX0=1;//打开 INT0 的中断允许。
		EA=1;//打开总中断
}
/****************************************************************
***************
* 函 数 名 : Int0() interrupt 0
* 函数功能 : 外部中断 0 的中断函数
* 输 入 : 无
* 输 出 : 无
*****************************************************************
**************/
void Int0() interrupt 0 //外部中断 0 的中断函数
{
//delay(1000); //延时消抖
		ET0=1;//定时器中断使能
	  TR0=1;//启动 T0
    EA=1;
}

//处理定时器0中断
void timer0(void) interrupt 1
{
    TMOD=0x01;
    TH0=(65536-50000)/256;
    TL0=(65536-50000)%256;
    TR0=1;
    ET0=1;
    EA=1;
		beep=1;
	relay=1;
    count++;
    if(count>19)
    {//当定时到1秒时执行花括号里面的语句
        count=0;
        NSF++;		
    }
}



void delayms(unsigned int xms)
{
    unsigned int i,j;
    for(i=xms; i>0; i--)
        for(j=110; j>0; j--);

}


//数码管显示函数
void display1(unsigned char num1,unsigned char num2)
{
    LED_A1=0;
    P0=table[num1];

    delayms(10);
    LED_A1=1;

    LED_B1=0;
    P0=table[num2];

    delayms(10);
    LED_B1=1;
}
void main()
{
	relay =0;
		Int0Init();//外部中断1
	  Init();//定时器初始化，但是不启动
        beep=0;     
        while(1)
        {
            display1(NSF/10,NSF%10);
            delayms(10);
					if(NSF>10)
					{//输出10秒后，关闭定时器。
		ET0=0;//定时器中断使能
	  TR0=0;//启动 T0
						NSF=0;
				beep=0;
				relay=0;
					}
        }
	}