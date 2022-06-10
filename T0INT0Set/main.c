 #include <reg52.h>
#define uint unsigned int
#define uchar unsigned char	

sbit a = P2^1;							
sbit b = P2^0;
uchar code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90}; //共阳极段码0-9 

uint num=5;
uchar shi=0;	
void delayms(uint xms)	//延时程序
{		
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}

void display(uint num)				
{
		a=1;	//共阳极数码管高电平有效
		P0=table[num%10];	//循环显示0-9
		delayms(5);	//刷新
		P0=0XFF;			//消隐
		a=0;

		b=1;	//共阳极数码管高电平有效
		P0=table[(num/10)%10];	//循环显示0-9
		delayms(5);			//刷新
		P0=0XFF;		//消隐
		b=0;					
}
//外部中断服务程序
void exint0() interrupt 0 
{
		EX0=0;	//关闭外部请求中断允许位
	TL0 = 0xb0;		//设置定时初始值
	TH0 = 0x3c;		//设置定时初始值
		TR0=1;//开启定时器0中断

}

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1
{
//		EX0 = 0; //关闭INT0中断
	if(shi==20)
	{		//20*50MS定时时间为1秒
		shi=0;	
		if(!num--){
		TR0=0;
		EX0=1;	//启用外部请求中断允许位
		num =5;
	}	
	}
	TL0 = 0xb0;		//设置定时初始值(65536-50000)%256
	TH0 = 0x3c;		//设置定时初始值(65536-50000)/256

 shi++;	
	
}

void main()
{
	TMOD = 0x01; /*定时器工作方式1*/
	EA=1;	//中断总开关
	EX0 = 1; //使能INT0中断
	ET0=1;	//使能定时器0中断
	IT0=1;//外部中断采用下降沿触发
	INT0 = 1;//外部中断引脚置1
	TR0=0;	//定时器0允许中断位关闭
	TF0 = 0;		//清除TF0标志
	while(1)
	{											
		display(num);//调用函数	
	}
}


