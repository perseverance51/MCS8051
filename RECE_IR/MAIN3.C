//#include "STC15W4K.H"
//#include <STC15F2K60S2.H>
#include <STC12C5A60S2.H>
#include "intrins.h"//如果延时函数当中所用了nop()就需要添加此头文件
#include <stdio.h> 	       // 为使用KEIL自带的库函数printf而加入
#include <string.h> 

sbit IR=P3^2;  //红外接口标志         //红外接收器数据线 注意要带中断功能。
unsigned char  irtime;//红外用全局变量

bit irpro_ok,irok; //处理OK，接收OK

bit InFrared_Way = 0;
unsigned char ir_flag;
unsigned char Data[4] ;
unsigned char   ircode[4];//红外接收
unsigned int  HighTime ,LowTime;

bit ir_read(unsigned char *pbuf);
void delay(unsigned int x)    //x*0.14MS
{
 unsigned char i;
  while(x--)
 {
  for (i = 0; i<13; i++) {}
 }
}


/*------------------------------------------------
                  定时器0中断处理
------------------------------------------------*/

void tim0_isr (void) interrupt 1 using 1
{
	  
}
	
/*------------------------------------------------
                定时器0初始化
------------------------------------------------*/
void TIM0init(void)//定时器0初始化
{  
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;
	TMOD |= 0x01;//设置定时器0的工作方式1
  TR0 = 0;   //关闭定时器0  

}
 
/*------------------------------------------------
                红外码值处理
------------------------------------------------*/

/*------------------------------------------------
                  外部中断0初始化
------------------------------------------------*/
void EX0init(void)
{
	IR = 1;
 IT0 = 1;   //指定外部中断0下降沿触发，INT0 (P3.2)
 EX0 = 1;   //打开外部中断
 EA = 1;    //开总中断
}
void EX0_ISR(void) interrupt 0 //外部中断0服务函数
{
//	EX0 = 0;//关闭外部中断
//	TH0 = 0;//定时器的初值清零，
//	TL0 = 0;
//	TR0 = 1;//打开定时器0，计算低电平的时间
//	while(IR==0);
//	TR0 = 0;
//	LowTime = TH0*256 + TL0;//低电平的时间 us
//	if((LowTime > 8500)&&(LowTime < 9500)!=1)//如果不是9ms，则退出
//	{
//	EX1 = 1;
//		return;
//	}
//	TH0 = 0;///定时器的初值清零，
//	TL0 = 0;
//	TR0 = 1;//打开定时器0，计算高电平的时间（引导码）
//	while(IR);
//	TR0 = 0;
//	HighTime = TH0*256 + TL0;//高电平的时间 us
//	if((HighTime > 4000)&& (HighTime < 5000))
//	{
//		
//	}
InFrared_Way  = 1;
}  
void InFrared_Init(void)
{
     //  unsigned char i, j;  //定义4个数据，每个数据8个位，i表示数据，j表示位。
   //   unsigned int  HighTime,LowTime;
				TH0 = 0;
       TL0 = 0;    //定时器1清0
			 TR0 = 1; //允许定时器0计时。
			while(IR ==0);
				TR0 = 0;
	LowTime = TH0*256 + TL0;//低电平的时间 us
	if((LowTime > 8500)&&(LowTime < 9500)!=1)//如果不是9ms，则退出
	{
	EX1 = 1;
		return;
	}
	TH0 = 0;///定时器的初值清零，
	TL0 = 0;
	TR0 = 1;//打开定时器0，计算高电平的时间（引导码）
	while(IR);
	TR0 = 0;//关闭定时器，取值
	HighTime = TH0*256 + TL0;//高电平的时间 us
	if((HighTime > 4000)&& (HighTime < 5000))//如果引导码正常，等待4.5ms高电平
	{
		ir_flag = ir_read(ircode);//读数据
	}
	

//              }
       
	EX0 = 1 ;
}
bit ir_read(unsigned char *pbuf)
{
//	unsigned char cnt;
	unsigned char i,j;
	unsigned char temp = 0;
	              for (i = 0; i < 4; i++)//开始接收数据。
              {//接收一个字节，8bit
                     for (j = 0; j < 8; j++)//接收4组8位数据。
                     {
								temp = temp >> 1;
											 TH0 = 0;
											 TL0 = 0;
											 TR0 =1;
											 while(IR==0);//等待低电平结束
											 TR0 = 0;//关闭定时器，取值
											LowTime = TH0*256 + TL0;//数据的低电平的时间 us 
											 
											 TH0 = 0;
											 TL0 = 0;
											 TR0 =1;
											 while(IR);//等待低电平结束
											 	TR0 = 0;//关闭定时器，取值
										HighTime = TH0*256 + TL0;//数据的高电平的时间 us
											 if((LowTime < 360) ||(LowTime > 680) )return 0;//560us以内
											 
												if((HighTime > 500) &&(HighTime < 600) )return 0;//560us以内
										temp = temp & 0x7f;
											 	if((HighTime > 1500) &&(HighTime < 1800) )return 0;//560us以内
										temp = temp |0x80;
										 }
									 pbuf[i]  = temp;
									 temp = 0;
									 }
										 if((pbuf[0]+pbuf[1]) == 255) //效验数据正确性
											if((pbuf[2]+pbuf[3]) == 255) return 1;
												
											else return 0;
}
void UART_init(void)
{		  
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//定时器时钟1T模式
	BRT = 0xDC;		//设置定时重载值
	AUXR |= 0x01;		//串口1使用独立波特率发射器为波特率发生器
	AUXR |= 0x10;		//启动独立波特率发射器
	TI=1;			// 很关键，使用printf函数时必须有此命令
} 

void main()
{	

	 EX0init(); //初始化外部中断
	TIM0init();
	UART_init();	 // 初始化串口
	//printf_String() ;
	 while(1)//主循环
   {
   if (InFrared_Way == 1)
     {
                     EX0 = 0;//关闭外部中断。                    
                     InFrared_Init();// 初始化
                     TR0 = 1;  //开始定时器0计时。
                     EX0 = 1;//打开外部中断
                     InFrared_Way = 0; //清零InFrared_Way ，已经结束红外接收。
								 	printf("RECE_IR DATA:%02bx-%02bx-%02bx-%02bx \n", Data[0], Data[1], Data[2], Data[3]); 
				
					//strcpy(IRcord, "");
					memset(Data, 0, sizeof(Data));          //清空数组
      }
       }
}



		 
		 
		 
		 