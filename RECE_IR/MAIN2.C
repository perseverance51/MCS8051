//#include "STC15W4K.H"
//#include <STC15F2K60S2.H>
#include <STC12C5A60S2.H>
#include "intrins.h"//如果延时函数当中所用了nop()就需要添加此头文件
#include <stdio.h> 	       // 为使用KEIL自带的库函数printf而加入
#include <string.h> 

sbit IR=P3^2;  //红外接口标志         //红外接收器数据线 注意要带中断功能。
unsigned char  irtime;//红外用全局变量

bit irpro_ok,irok; //处理OK，接收OK

unsigned char IRcord[4];
unsigned char irdata[33];

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
	  irtime++;  //用于计数2个下降沿之间的时间
}
	
/*------------------------------------------------
                定时器0初始化
------------------------------------------------*/
void TIM0init(void)//定时器0初始化
{

//  TMOD=0x02;//定时器0工作方式2，TH0是重装值，TL0是初值
//  TH0=0x00; //装初值
//  TL0=0x00; 
//  ET0=1;    //开中断
//  TR0=1;   
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x02;		//设置定时器模式
	TL0 = 0xF5;		//设置定时初始值
	TH0 = 0xF5;		//设置定时重载值
	TF0 = 0;		//清除TF0标志
	ET0 = 1;
	TR0 = 1;		//定时器0开始计时	
}

//void printf_String(void) 
//{
//	char buf [] = "Test String";
//	char *p = buf;
//	printf ("String %s is at address %p\n",buf,p);
//   //输出：String Test String is at address i：0022    
//} 	
/**********************************************************/
//void IR_IN() interrupt 0 using 0

// printf ("RECE_DATA: %s \n",IRcord); 
/*------------------------------------------------
                红外码值处理
------------------------------------------------*/
void Irpro(void)//红外码值处理函数	分析出那些是1那些是0
{ 
  unsigned char i, j, k;
  unsigned char cord,value;

  k=1;	//前导码没数据，从第二个开始，就是用户码开始。
  for(i=0;i<4;i++)      //处理4个字节
     {
      for(j=1;j<=8;j++) //处理1个字节8位
         {
          cord=irdata[k]; //
          if(cord>7)  //低电平下降沿到下一个下降沿的宽度是0.56+0.565=1.125ms，高电平则是0.56+1.69=2.25ms，同样我们也给出一个范围用于区分它们，可以这样识别 （1.125ms + 2.25ms ）/ 2=1.68ms, 
                       //大于1.68为高，小于1.68为低。
			          //假设使用12M晶振 定时器的单位数值时1us，使用8位定时器自动重装，将得到每个定时周期0.256ms的时长， 1.68/0.256=6.59，约等于7.也就是 定时器0计数次数。
            value|=0x80;//最高位就给1，
			if(j<8)
		    {
			 value>>=1;
			}
           k++;
         }
     IRcord[i]=value;
     value=0;     
     } 
	 irpro_ok=1;//处理完毕标志位置1
}
/*------------------------------------------------
                  外部中断0初始化
------------------------------------------------*/
void EX0init(void)
{
 IT0 = 1;   //指定外部中断0下降沿触发，INT0 (P3.2)
 EX0 = 1;   //打开外部中断
 EA = 1;    //开总中断
}
void EX0_ISR(void) interrupt 0 //外部中断0服务函数
{
  static unsigned char  i;             //接收红外信号处理
  static bit startflag;                //是否开始处理标志位

if(startflag)                         
   {
    if(irtime<63&&irtime>=33)//引导码 TC9012的头码，9ms+4.5ms 头码的持续时间的范围，33(8.448ms)<63(16.128ms)，
                        i=0;
    		irdata[i]=irtime;//存储每个电平的持续时间，用于以后判断是0还是1
    		irtime=0;
    		i++;
   			 if(i==33)	  //33是33位的意思，包括32位的数据和一位的头码。
      			{
	  			 irok=1;
				 i=0;
	  			}
          }
  	 else
		{
		irtime=0;
		startflag=1;
		}

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
    if(irok)                        //如果接收好了进行红外处理
	  {   
	   Irpro();
 	   irok=0;
	  }
		 if(irpro_ok)                   //如果处理好后进行工作处理，如按对应的按键后显示对应的数字等
			  {
				//	printf ("RECE_DATA: %s \n",IRcord); 
					printf("RECE_IR DATA:%02bx-%02bx-%02bx-%02bx \n", IRcord[0], IRcord[1], IRcord[2], IRcord[3]); 
					//strcpy(IRcord, "");
					memset(IRcord, 0, sizeof(IRcord));          //清空数组 
					irpro_ok =0;
  	  }
	   }
}



		 
		 
		 
		 