#include<reg52.h>
#include<intrins.h>
sbit LED = P2^0;
#define FOSC 12000000uL
#define T50MS (65536-FOSC*50/12/1000)   //12T模式50ms定时
unsigned int count; 
/*
void delay(unsigned int ms)
{
	unsigned int cycle = 0;
	TMOD = 0x01;	//设置定时器工作模式为高低八位合并为一个16位寄存器
	TH0 = 0xfc;
	TL0 = 0x67;  
	TR0 = 1;
	while(1)
	{
		if(TF0==1)	//检测定时器0是否溢出
		{
			TF0 = 0;	//若溢出，则重置检测位TF0
			TH0 = 0xfc;
			TL0 = 0x65;
			cycle++;
			if(cycle==ms)
				break;
		}
	}
}
*/
	void Timer0Init(void)		//50毫秒@12.000MHz
{

	TMOD = 0x01;   //set timer0 as mode1 (16-bit)
	TL0 = T50MS;		//设置定时初始值，高8位
	TH0 = T50MS >> 8;  		//设置定时初始值，低8位
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;                        //enable timer0 interrupt
  EA = 1;                         //open global interrupt switch
  count = 19;                      //initial counter
}

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1
{
    TL0 = T50MS;                     //reload timer0 low byte
    TH0 = T50MS >> 8;                //reload timer0 high byte
    if (count-- == 0)               //50ms * 20 -> 1s
    {
      count = 19;               //reset counter
			LED = ~LED;	
    }
}

void main()
{
	Timer0Init();
	LED = 0;	
	while(1);
//	{
//		delay(1077);		
//		LED = ~LED;		
//	}
}

