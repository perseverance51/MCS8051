/*******************************************************
ULN2003驱动5V减速步进电机程序
Target:STC89C52RC-40C
Crystal:12MHz

Platform:51&avr单片机最小系统板+ULN2003步进电机驱动套件

*******************************************************
接线方式:
IN1 ---- P10
IN2 ---- P11
IN3 ---- P12
IN4 ---- P13
+   ---- +5V
-   ---- GND
*********************/
#include<reg52.h>
#define uchar unsigned char
#define uint  unsigned int
#define MotorData P1                    //步进电机控制接口定义
uchar phasecw[4] ={0x03,0x06,0x0c,0x09};//正转 电机导通相序 D-C-B-A
uchar phaseccw[4]={0x03,0x09,0x0c,0x06};//反转 电机导通相序 A-B-C-D

uchar key1_down,key2_down,key3_down,key1_flag,key2_flag,key3_flag;//变量定义
sbit key1 = P3^0;//按键定义
sbit key2 = P3^1;

//ms延时函数
void Delay_xms(uint t)
{
   uchar k;
   while(t--)
   {
    for(k=0; k<125; k++)
    { }
}
	 }
//顺时针转动
void MotorCW(void)
{
 uchar i;
 for(i=0;i<4;i++)
  {
   MotorData=phasecw[i];
   Delay_xms(200);//转速调节
  }
}
//逆时针转动
void MotorCCW(void)
{
 uchar i;
 for(i=0;i<4;i++)
  {
   MotorData=phaseccw[i];
   Delay_xms(200);//转速调节
  }
}


  //开启外部中断    
void Init_P( )
{      
      EA=1; //开全局中断      
      EX0=1; //开外部中断0      
      IT0=1; //外部中断0边沿触发     
      IT1=0; //外部中断1电平触发     
}  
      //外部中断0处理 
void interint0() interrupt 0 //using **     
{     
   	key1_flag = 0;
		key2_flag = 0;
		key3_flag = 1;
  
      
} 
void key_scan()
{
	/************按键1*************/
	if(key1 == 0)
	{
		Delay_xms(100);
		if(key1 == 0)key1_down = 1;	
	}
	if(key1_down == 1 && key1 == 1)
	{
		key1_flag = 1;
		key2_flag = 0;
		key3_flag = 0;
		key1_down = 0;	
	}
	/************按键2*************/
	if(key2 == 0)
	{
		Delay_xms(100);
		if(key2 == 0)key2_down = 1;	
	}
	if(key2_down == 1 && key2 == 1)
	{
		key1_flag = 0;
		key2_flag = 1;
		key3_flag = 0;
		key2_down = 0;	
	}
	}

/************************主程序****************************/
main()
{ 
	Init_P( );
   	while(1)
    {  
      	key_scan();
	if(key1_flag == 1)MotorCW();       //电机正转
	if(key2_flag == 1)MotorCCW();		//电机反转
  if(key3_flag == 1) P1 = 0x0f;			//电机停止
	}
}
