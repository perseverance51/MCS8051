//#include <reg51.h>
#include <intrins.h>
#include <stdio.h> 	       // 为使用KEIL自带的库函数printf而加入
#include <STC15F2K60S2.H>
#include <string.h> 
#define uchar unsigned char
#define uint  unsigned int
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};

void delay(uchar x);  //x*0.14MS
void delay1(int ms);


sbit IRIN = P3^2;         //红外接收器数据线


uchar IRCOM[7];

sbit LCD_RS = P2^6;             
sbit LCD_RW = P2^5;
sbit LCD_EN = P2^7;
//sfr AUXR  = 0x8E; 
sfr BRT = 0x9C; //S2 Baud-Rate Timer 

uchar code  cdis1[ ] = {" Red Control "};
uchar code  cdis2[ ] = {"  IR-CODE: --H  "};

void UART_init(void)
{		  
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR &= 0xFB;		//定时器时钟12T模式
	T2L = 0xE8;		//设置定时初始值
	T2H = 0xFF;		//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	TI=1;			// 很关键，使用printf函数时必须有此命令
} 


/*******************************************************************/
main()
{
    IE = 0x81;                 //允许总中断中断,使能 INT0 外部中断
    TCON = 0x01;               //触发方式为脉冲负边沿触发    
    IRIN=1;                    //I/O口初始化   
   
	UART_init();
	 delay1(10);                 //延时
	 printf ("stc408as\n"); 
   while(1)	;
} //end main
/**********************************************************/
void IR_IN() interrupt 0 using 0
{
  unsigned char j,k,N=0;
     EX0 = 0;   
	 delay(15);
	 if (IRIN==1) 
     { EX0 =1;
	   return;
	  } 
                           //确认IR信号出现
  while (!IRIN)            //等IR变为高电平，跳过9ms的前导低电平信号。
    {delay(1);}

 for (j=0;j<4;j++)         //收集四组数据
 { 
  for (k=0;k<8;k++)        //每组数据有8位
  {
   while (IRIN)            //等 IR 变为低电平，跳过4.5ms的前导高电平信号。
     {delay(1);}
    while (!IRIN)          //等 IR 变为高电平
     {delay(1);}
     while (IRIN)           //计算IR高电平时长
      {
    delay(1);
    N++;           
    if (N>=30)
	 { EX0=1;
	 return;}                  //0.14ms计数过长自动离开。
      }                        //高电平计数完毕                
     IRCOM[j]=IRCOM[j] >> 1;                  //数据最高位补“0”
     if (N>=8) {IRCOM[j] = IRCOM[j] | 0x80;}  //数据最高位补“1”
     N=0;
  }//end for k
 }//end for j
   
   if (IRCOM[2]!=~IRCOM[3])
   { EX0=1;
     return; }

   IRCOM[5]=IRCOM[2] & 0x0F;     //取键码的低四位
   IRCOM[6]=IRCOM[2] >> 4;       //右移4次，高四位变为低四位

   if(IRCOM[5]>9)
    { IRCOM[5]=IRCOM[5];}
   else
	  IRCOM[5]=IRCOM[5];

   if(IRCOM[6]>9)
    { IRCOM[6]=IRCOM[6];}
   else
	  IRCOM[6]=IRCOM[6];
	printf("RECE_IR DATA:%02bx-%02bx-%02bx-%02bx \n", IRCOM[0], IRCOM[1],IRCOM[2], IRCOM[3]); 
 //  printf ("rece_ir:%s\n",IRCOM); 
			memset(IRCOM, 0, sizeof(IRCOM));          //清空数组   
     EX0 = 1; 
} 


/**********************************************************/
void delay(unsigned char x)    //x*0.14MS
{
 unsigned char i;
  while(x--)
 {
  for (i = 0; i<13; i++) {}
 }
}

/**********************************************************/
void delay1(int ms)
{
 unsigned char y;
  while(ms--)
 {
  for(y = 0; y<250; y++)
  {
   _nop_();
   _nop_();
   _nop_();
   _nop_();
  }
 }
}

