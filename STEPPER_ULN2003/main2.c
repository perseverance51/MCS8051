#include<reg51.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int 
//四相八拍转动数组
uchar code FFW[8]={0x02,0x06,0x04,0x0c,0x08,0x09,0x01,0x03//315,270,225......360(0)
};//八拍
uchar code REV[8]={0x01,0x09,0x08,0x0c,0x04,0x06,0x02,0x03//45,90,145......360(0)
};
uchar key1_down,key2_down,key3_down,key1_flag,key2_flag,key3_flag;//变量定义
sbit key1 = P3^0;//按键定义
sbit key2 = P3^1;
sbit key3 = P3^2;
/************************按键程序****************************/
/****************按键延迟函数100MS**********/
void delay()
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 2;
	j = 13;
	k = 237;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
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
		delay();
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
		delay();
		if(key2 == 0)key2_down = 1;	
	}
	if(key2_down == 1 && key2 == 1)
	{
		key1_flag = 0;
		key2_flag = 1;
		key3_flag = 0;
		key2_down = 0;	
	}
	/************按键3************
	if(key3 == 0)
	{
		delay();
		if(key3 == 0)key3_down = 1;	
	}
	if(key3_down == 1 && key3 == 1)
	{
		key1_flag = 0;
		key2_flag = 0;
		key3_flag = 1;
		key3_down = 0;	
	}
	*/
}
/****************转动延迟函数**********/
void delayB(uint t)
{                           
   uchar k;
   while(t--)
   {
    for(k=0; k<125; k++)
    { }
   }
}
/************************电机正转****************************/
void motor_ffw()
{ 
   	uchar i;
	for (i=0; i<8; i++)       //一个周期转30度
    {
       	P1 = FFW[i];          //取数据
        delayB(100);            //调节转速
    }
 }
/************************电机反转****************************/
void motor_rev()
{
 	uchar i;
	for (i=0; i<8; i++)     //一个周期转30度
    {
       	P1 = REV[i];          //取数据
        delayB(100);            //调节转速
    }
}
/************************主程序****************************/
main()
{ 
	Init_P( );
   	while(1)
    {  
      	key_scan();
	if(key1_flag == 1)motor_ffw();       //电机正转
	if(key2_flag == 1)motor_rev();		//电机反转
  if(key3_flag == 1) P1 = 0x0f;			//电机停止
	}
}
