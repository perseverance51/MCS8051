/*-----------------------------------------------
  名称：矩阵键盘依次输入控制 使用行列逐级扫描 中断方式

  内容：如计算器输入数据形式相同 从右至左 使用行列扫描方法
        中断方式可以有效提供cpu工作效率，在有按键动作时才扫描，平时不进行扫描工作
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
//#include<intrins.h> 

#define DataPort P0 //定义数据端口 程序中遇到DataPort 则用P0 替换
#define KeyPort  P1
#define 	MAIN_Fosc			12000000uL	//定义主时钟
unsigned char T0RH = 0; //T0 重载值的高字节
unsigned char T0RL = 0; //T0 重载值的低字节

sbit LATCH1=P2^2;//定义锁存使能端口 段锁存
sbit LATCH2=P2^3;//                 位锁存
sbit P32 =P3^2;
bit KeyPressFlag;//定义按键标志位
unsigned char keycode;
unsigned char code DuanMa[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 显示段码值0~9

void DelayMs(unsigned char t); //ms级延时
void Display();//数码管显示函数
void KeyScan(void);//键盘扫描
unsigned char KeyPro(void);
void Init_Timer0(void);//定时器初始化
void Init_INT0(void);  //外部中断0初始化和
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{
	KeyPort =0xf0;
	Init_Timer0();
	Init_INT0();
while (1) KeyPort = 0xf0;       //主循环
}

/*------------------------------------------------
 mS延时函数
------------------------------------------------*/
void delay(unsigned int xms)	// 延迟函数
{
	unsigned int i,j;
	for(i=xms;i>0;i--)
		for(j=112;j>0;j--);
}
/*数码管显示函数*/
void Display()
{
      static unsigned char k=0;
	   DataPort=0x00;   //清空数据，防止有交替重影
       LATCH1=0;
			 LATCH2=1;
       DataPort=DuanMa[keycode/10]; //取位码 
		delay(5);
		LATCH1=1;//关掉上面显示的十位显示
       LATCH2=1;     //位锁存
       LATCH2=0;//打开个位显示
	
	   DataPort=0x00;   //清空数据，防止有交替重影
       DataPort=DuanMa[keycode%10];  //取显示数据，段码
			delay(5);
		k = k<2 ? k+1 : 0;
}
/*------------------------------------------------
                    定时器初始化子程序
------------------------------------------------*/
void Init_Timer0(void)
{
	unsigned long tmp; //临时变量
 TMOD |= 0x01;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响		     
    tmp = MAIN_Fosc / 12; //定时器计数频率
    tmp = (tmp * 50) / 1000; //计算所需的计数值50ms
    tmp = 65536 - tmp; //计算定时器重载值
    tmp = tmp + 12; //补偿中断响应延时造成的误差
    T0RH = (unsigned char)(tmp>>8); //定时器重载值拆分为高低字节
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0; //清零 T0 的控制位
    TMOD |= 0x01; //配置 T0 为模式 1
    TH0 = T0RH; //加载 T0 重载值
    TL0 = T0RL;

 EA=1;            //总中断打开
 ET0=1;           //定时器中断打开
 TR0=1;           //定时器开关打开
}
/*------------------------------------------------
                 定时器中断子程序
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
 TH0=(65536-50000)/256;		  //重新赋值 50ms
 TL0=(65536-50000)%256;
 Display();       // 调用数码管扫描
}
/*------------------------------------------------
按键扫描函数，返回扫描键值
------------------------------------------------*/
void KeyScan(void)  //键盘扫描函数，使用行列逐级扫描法
{
		unsigned char t,key;
/**********************第一行矩阵*******************/

	KeyPort = 0xfe;	//	1111 1110 让P3.0口输出低
	t = KeyPort;
	t=t&0xf0;	// 1111 0000 屏蔽后四位

	if(t != 0xf0) // 说明按键有被按住
	{
		delay(10);
		t = KeyPort;
		t = t&0xf0;
		if( t != 0xf0)
		{
			t = KeyPort;
			switch(t)
			{
				case 0xee:
					key = 1;   // 此时为 1110 1110  S1按键被按下
					break;
				case 0xde:
					key = 2;   // 此时为 1101 1110  S2按键被按下
					break;
				case 0xbe:
					key = 3;   // 此时为 1011 1110  S3按键被按下
					break;
				case 0x7e:
					key = 4;   // 此时为 0111 1110  S4按键被按下
					break;		
			}
			// 判断按键是否被松开
			while( t != 0xf0){
			 	t = KeyPort;
				t = t&0xf0;
			}
			keycode = key;		
		}
	
	}

/**********************第二行矩阵*******************/

	KeyPort = 0xfd;	//	1111 1101 让P3.1口输出低
	t = KeyPort;
	t=t&0xf0;	// 1111 0000 屏蔽后四位
	if(t != 0xf0) // 说明按键有被按住
	{
		delay(10);
		t = KeyPort;
		t = t&0xf0;
		if( t != 0xf0)
		{
			t = KeyPort;
			switch(t)
			{
				case 0xed:
					key = 5;   // 此时为 1110 1101  S5按键被按下
					break;
				case 0xdd:
					key = 6;   // 此时为 1101 1101  S6按键被按下
					break;
				case 0xbd:
					key = 7;   // 此时为 1011 1101  S7按键被按下
					break;
				case 0x7d:
					key = 8;   // 此时为 0111 1101  S8按键被按下
					break;
		
			}
			// 判断按键是否被松开
			while( t != 0xf0){
			 	t = KeyPort;
				t = t&0xf0;
			}
						keycode = key;	
		}
	
	}

/**********************第三行矩阵*******************/

	KeyPort = 0xfb;	//	1111 1011 让P3.2口输出低
	t = KeyPort;
	t=t&0xf0;	// 1111 0000 屏蔽后四位

	if(t != 0xf0) // 说明按键有被按住
	{
		delay(10);
		t = KeyPort;
		t = t&0xf0;
		if( t != 0xf0)
		{
			t = KeyPort;
			switch(t)
			{
				case 0xeb:
					key = 9;   // 此时为 1110 1011  S9按键被按下
					break;
				case 0xdb:
					key = 10;   // 此时为 1101 1011  S10按键被按下
					break;
				case 0xbb:
					key = 11;   // 此时为 1011 1011 S11按键被按下
					break;
				case 0x7b:
					key = 12;   // 此时为 0111 1011  S12按键被按下
					break;		
			}
			// 判断按键是否被松开
			while( t != 0xf0){
			 	t = KeyPort;
				t = t&0xf0;
			}
			keycode = key;		
		}	
	}

/**********************第四行矩阵*******************/

	KeyPort = 0xf7;	//	1111 0111 让P1.3口输出低
	t = KeyPort;
	t = t&0xf0;	// 1111 0000 屏蔽后四位
	if(t != 0xf0) // 说明按键有被按住
	{
		delay(10);
		t = KeyPort;
		t = t&0xf0;
		if( t != 0xf0)
		{
			t = KeyPort;
			switch(t)
			{
				case 0xe7:
					key = 13;   // 此时为 1110 0111  S13按键被按下
					break;
				case 0xd7:
					key = 14;   // 此时为 1101 0111  S14按键被按下
					break;
				case 0xb7:
					key = 15;   // 此时为 1011 0111  S15按键被按下
					break;
				case 0x77:
					key = 16;   // 此时为 0111 0111  S16按键被按下
					break;	
			}
			// 判断按键是否被松开
			while( t != 0xf0){
			 	t = KeyPort;
				t = t&0xf0;
			}
			keycode = key;	//	把按键的值发给数组，让其对应的数字打印再数码管上	
		}
	}
}

/*------------------------------------------------
           外部中断0初始化
------------------------------------------------*/
void Init_INT0(void)
{
  EA=1;          //全局中断开
  EX0=1;         //外部中断0开
  IT0=1;         //边沿触发
}

/*------------------------------------------------
                 外部中断0程序
------------------------------------------------*/
void int0() interrupt 0 //在中断服务函数中进行键盘扫描
{
	 KeyScan();
}