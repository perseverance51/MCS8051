/*-------------------------------
	洗衣机模拟控制程序
--------------------------------*/
#include <REG52.H>

#define uchar unsigned char
#define uint  unsigned int
/* 指示灯状态*/
#define qiang_ledon qiang_led=1;
#define qiang_ledoff qiang_led=0;
#define biao_ledon biao_led=1;
#define biao_ledoff biao_led=0;
#define ruo_ledon ruo_led=1;
#define ruo_ledoff ruo_led=0;
#define piao_ledon piao_led=1;
#define piao_ledoff piao_led=0;
#define sgan_ledon sgan_led=1;
#define sgan_ledoff sgan_led=0;
#define jin_ledon jin_led=1;
#define jin_ledoff jin_led=0;
#define pai_ledon pai_led=1;
#define pai_ledoff pai_led=0;

unsigned char code Tab[11]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F}; //共阴数码管显示0～9的段码表
unsigned char int_time;        //记录中断次数
unsigned char second;        //储存秒
uint t=0,tt=0;
/* 按键引脚定义 */
   sbit k1=P0^0;
   sbit k2=P0^1;
   sbit k3=P0^2;
/* 指示灯引脚定义 */
   sbit sg=P0^3;
   sbit hk1=P0^4;
   sbit hk2=P0^5;
   sbit ledon=P0^7;
   sbit qiang_led=P2^0;
   sbit biao_led=P2^1;
   sbit ruo_led=P2^2;
   sbit piao_led=P2^3;
   sbit sgan_led=P2^4;
   sbit jin_led=P2^5;
   sbit pai_led=P2^6;
/* 电机驱动引脚定义 */
   sbit p30=P3^0;//
   sbit p31=P3^1;//
   sbit beepon=P2^7;//蜂鸣器报警
	 /* 数码管位切换引脚定义 */
   sbit sm1=P3^6;
   sbit sm2=P3^7;
	 
void delay2(void)
   {
  unsigned char m;
  for(m=0;m<200;m++)
        ;
   }

void delay1(int s)
   { 
   int i;
   for(;s>0;s--)
   for(i=0;i<65;i++);
   }
 
void delay(uint i)
   {
   uchar j;
   for(i;i>0;i--)
   for(j=255;j>0;j--);
   }
/* 数码管显示函数 */
void DisplaySecond(unsigned char k)
{
   
   sm1=0;        //P2.6引脚输出低电平， DS6点亮 
   P1=Tab[k/10];          //显示十位
     delay2();
     delay2();
   sm1=1;
   sm2=0;       //P2.7引脚输出低电平， DS7点亮 
   P1=Tab[k%10];         //显示个位
     delay2();
     delay2();
   P3=0xff;     //关闭所有数码管
   P1=1;        //显示个位
     delay2();
     delay2();
}
	/* 蜂鸣器驱动函数 */   
void beep()
   {
  	p30=0;
    p31=0;
   	t=0;
	while(1)
	{
	  beepon ^=1;
      delay(300);
      if(t>=80) break;
	}
	beepon=0;
	biao_led=0;
    }
	 /* 强洗模式 */ 
void f_qiang()
    {	 
	 qiang_ledon;
	 t=0;
	 tt=0;	
     int_time=0;
     second=00;
	 while(1)
    {	
	  if(tt>=400) break; 
      t=0;
	  while(t<80&&tt<=400)
	  {	
	    p31=0;
	    p30=1;	//正转 4s
        DisplaySecond(second);
	    }
	  t=0;  	//反转 4s	
	  while(t<80&&tt<=400)
	  {
	    p30=0;
	    p31=1;
	    DisplaySecond(second);
	    }
     }
	  p30=0;
	  p31=0;	  
     }
/* 标准洗模式 */
void f_biao()
	{
	  qiang_led=0;
      biao_led=1;
	  t=0;
	  tt=0;
      int_time=0;
      second=00;
	 while(1)
     {
	   if(tt>=400) break; 
	   t=0;
	  while(t<60&&tt<=400)
	   {	
          p31=0;
		  p30=1;	//正转3s
          DisplaySecond(second);
       }

		t=0;  	//反转3s
	  while(t<60&&tt<=400)
       {
	      p30=0;
		  p31=1;
		  DisplaySecond(second);
		}
       }
       p30=0;
	   p31=0;
	 }
/* 弱洗模式 */
void f_ruo()
    {
	  qiang_led=0;
	  biao_led=0;
	  ruo_led=1;
	  t=0;
	  tt=0;
      int_time=0;
       second=00;
		while(1)
       {
	   	 if(tt>=400) break; 
		 t=0;
	       while(t<40&&tt<=400)
           {	p31=0;
		        p30=1;	//正转2s
                DisplaySecond(second);
	        }
	     t=0;  	//反转 2s
	       while(t<40&&tt<=400)
            {   p30=0;
	        	p31=1;
		        DisplaySecond(second);
	      	}
         }
		p30=0;
	    p31=0;
       }
/* 漂洗模式 */
void f_piao()
	{
	   qiang_ledoff;
	   biao_ledoff;
	   piao_ledon;
	   t=0;
	   tt=0;
       int_time=0;
       second=00;
		 while(1)
          {
	   	   if(tt>=400) break; 
		   t=0;
	         while(t<40&&tt<=400)
               {	p31=0;
	                p30=1;	//正转2s
                    DisplaySecond(second);
	           }
		   t=0;  	//反转2s
	         while(t<40&&tt<=400)
               {    p30=0;
		            p31=1;
	             	DisplaySecond(second);
		       }
             }
	  p30=0;
      p31=0;
	  piao_ledoff;
	  }
/* 排水指示灯函数 */
void pai_leds()
     {
	    p30=0;
	    p31=0;
   	    pai_led=1;	//排水阀灯亮 等待霍尔开关2 
	    while(hk2);
	    if(hk2==0)  //如果闭合
	    pai_led=0;	//排水灯灭
     }
/* 进水指示灯函数 */
void jin()
    {
	    p30=0;
	    p31=0;
	    jin_ledon; 
 	    while(hk1);
	    if(hk1==0)
	    jin_ledoff;
     }
/* 甩干指示灯函数 */
void sgan()
    {
	    sgan_ledon;	 //甩干灯亮
 	    t=0;
        int_time=0;
        second=00;
	     while(t<=400)
            {	
			    p31=0;
		        p30=1;	
                DisplaySecond(second);
	         }
        sgan_ledoff;
    }

void sgan1()
    {
	   sgan_ledon;
 	   t=0;
       int_time=0;
       second=00;
	     while(t<=400)
            {	
			   p31=0;
	           p30=1;	
               DisplaySecond(second);
	         }
        piao_ledoff;
        sgan_ledoff;
        beep();
    	t=401;
    }

/* 洗衣模式1 */
void f_an1()
   {  
	qiang_ledon; //方案1灯亮
	jin();  //进入强洗
	f_qiang(); 	//循环20S
	delay1(4000);//延时4s
	f_biao();  //循环20S
	delay1(4000);//延时4s
	f_ruo();  //循环20S
	ruo_ledoff;	
	pai_leds();
	delay1(1000); //延时1S 进入甩干
	sgan(); //甩干20s
	sgan_ledoff;
	ruo_ledoff;
	piao_ledoff;
	jin();
	delay1(1000); //延时1S 进入漂洗
	f_piao();  //循环20S
	sgan_ledoff;
	piao_ledoff;
	pai_leds();
	delay1(1000); //延时1S 进入甩干
	sgan1();//最终甩干20S
	delay(500);//延时	
	beep();
   }
/* 洗衣模式2 */
void f_an2()
   {  
	biao_ledon; //方案1灯亮
	jin();
	f_biao();  //循环20S
	delay1(4000); //延时4S
	f_ruo();  //循环20S
	ruo_ledoff;
	pai_leds();
	delay1(1000); //延时1S 进入甩干
	sgan(); //甩干20s
	ruo_ledoff;
	piao_ledoff;
	jin();
	delay1(1000); //延时1S 进入漂洗
	f_piao();  //循环20S
	sgan_led=0;
	pai_leds();
	delay1(1000); //延时1S 进入甩干
	sgan();//最终甩干20s
	delay(500);//延时5s
	piao_led=0;
	beep(); 
   }
/* 洗衣模式3 */
void f_an3()
  {  
	ruo_led=1; //方案1灯亮
	jin();
	f_ruo();  //循环20S
	ruo_ledoff;
	pai_leds();
	delay1(1000); //延时1S 进入甩干
	sgan(); //甩干20S
	ruo_ledoff;
	piao_ledoff;
	jin();
	delay1(1000); //延时1S 进入漂洗
	f_piao();  //循环20S
	sgan_led=0;
	pai_leds();
	delay1(1000); //延时1S 进入甩干
	sgan();//最终甩干20s
	delay(500);//延时5s
	piao_led=0;
	p30=0;
	beep();
   }


void main(void)
{	 
  TMOD=0x01;       //方式1 16位计数器
  TH0=-50000/256;   //设定初值
  TL0=-50000%256;
  EA=1;               //开总中断
  ET0=1;             //开定时器0中断
  TR0=1;             //启动定时器0
  P2=0;
  int_time=0;     //中断次数初始化
  second=00;	   //秒初始化
	 while(1)
    {	
	  if(k1==0)
	{	 
		delay(10);
		if(k1==0) 
	    {
			f_an1();
		}
	}
	if(k2==0)
	{
		delay(10);
		if(k2==0)
		{
			f_an2();
		}
	}
	if(k3==0)
	{
	    delay(10);
	   if(k3==0)
	    {
	 	   f_an3();
	    }
	}

 	if(sg==0)
	{
		delay(10);
		if(sg==0)
		{
		   sgan1();
		}
	}
}
//	return 0;
}  


void time0() interrupt 1 using 1
   {  
     int_time ++;        //每来一次中断,中断次数int_time自加1
    if(int_time==20)   //够20次中断,即1秒钟进行一次检测结果采样
      {
        int_time=0;    //中断次数清0
        second++;    //秒加1
      }	            
   TH0 =-50000/256;   //50000  =50ms中断一次
   TL0 =-50000%256;
   t++; 
   tt++;
   }

