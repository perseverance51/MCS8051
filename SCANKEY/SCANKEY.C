#include<STC15.H>
#include<intrins.h>

#define uchar unsigned char
#define uint unsigned int
#define KeyBus P6    //定义矩阵键盘接口

uchar Key_res =0; 
uchar i=0;
uchar temp=0XFF;     //串行发送临时传递参数
uchar temp1=0XFF;    //串行接收临时传递参数 
uchar busy;     //正忙标志(数据正在传输)
code uchar A[10]={0X3F,0X06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F};  //无点，数码管驱动数组

void GPIO(void);    //IO口初始化函数
void Delay5ms();		//@11.0592MHz
void UartInit(void);		//9600bps@11.0592MHz 串行口1初始化函数
uchar Key16_Scan(void);//矩阵键盘扫描函数子程序
void Key16_Function(void); //矩阵键盘服务(功能)子函数
void SendData(void);  //发送串口数据 (串行发送采用查询方式)

void main(void)
{
	GPIO();     //IO口初始化函数
    UartInit(); //串行口1初始化函数
	ES=1;       //允许串行口1中断
	EA=1;       //总中断允许控制位
	while(1) 
	{
		Key_res =0; 
		i=Key16_Scan();   //矩阵键盘扫描函数子程序
        switch(i)
			{
			  case  1: temp=0X00;break;
			  case  2: temp=0X01;break;
		      case  3: temp=0X02;break;
		      case  4: temp=0X03;break;
			  case  5: temp=0X04;break;
			  case  6: temp=0X05;break;
			  case  7: temp=0X06;break;
			  case  8: temp=0X07;break;
			  case  9: temp=0X08;break;
			  case 10: temp=0X09;break;
			  case 11: temp=0X0A;break;
			  case 12: temp=0X0B;break;
			  case 13: temp=0X0C;break;
			  case 14: temp=0X0D;break;
			  case 15: temp=0X0E;break;
			  case 16: temp=0X0F;break;
			  default: break;
			}
        SendData();  //发送串口数据
	}
}

//-----------------------------------IO口初始化函数-----------------------------//
void GPIO(void)
{
	P0M1=0;
	P0M0=0;
	P1M1=0;
	P1M0=0;
	P2M1=0;
	P2M0=0;
	P3M1=0;
	P3M0=0;
	P4M1=0;
	P4M0=0;
	P5M1=0;
	P5M0=0;
}
//------------------------------------软件延时---------------------------------------//
void Delay5ms()		//@11.0592MHz
{
	unsigned char i, j;
	i = 54;
	j = 199;
	do
	{
	  while (--j);
	} while (--i);
}
//-------------------------------------串行口1初始化函数-----------------------------//
void UartInit(void)		//9600bps@11.0592MHz
{
	SCON = 0x50;		 //8位数据,可变波特率
	AUXR |= 0x40;		 //定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		 //串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		 //设定定时器1为16位自动重装方式
	TL1 = 0xE0;		   //设定定时初值
	TH1 = 0xFE;		   //设定定时初值
	ET1 = 0;		     //禁止定时器1中断
	TR1 = 1;		     //启动定时器1
}


//---------------------------------------矩阵键盘扫描函数子程序-------------------------//

uchar Key16_Scan(void)
{
	uchar X_temp = 0,Y_temp = 0;
	uchar Key_down = 0;
	
	KeyBus = 0XFF;	
	KeyBus = 0X0F;	//高4位置0，低4位置1，此时有按键按下时，低四位的某一位会被拉低，由此定位按下的按键在第几列
	Y_temp = KeyBus&0X0F;
	
	if(Y_temp!=0X0F)	//如果检测到某列有按键按下（有按键按下时，低四位会有一位被拉低）
	{
		Delay5ms();    //按键消抖(不知道有无必要)
		KeyBus = 0X0F; 	
	    Y_temp = KeyBus&0X0F;//将KeyBus低四位的按键信息赋值给Y_temp
		if(Y_temp!=0X0F)
		{
			KeyBus = 0X0F;
			Y_temp = KeyBus&0X0F;  //将KeyBus低四位的按键信息赋值给Y_temp,反应列信息
			KeyBus = 0XF0;
			X_temp = KeyBus&0XF0;  //将KeyBus高四位的按键信息赋值给X_temp,反应行信息
			if(Key_down == 0)      //按键抬起检测，	
			{
			  switch(Y_temp)    //判断列
			  {
				  case 0X07: Key_res=1; break;
				  case 0X0B: Key_res=2; break;
				  case 0X0D: Key_res=3; break;
				  case 0X0E: Key_res=4; break;
				  default: return 0;    //无按键按下，返回0
			  }
			 switch(X_temp)    //判断行
			  {
				 case 0X70: return Key_res+12;break;
			     case 0XB0: return Key_res+ 8;break;
                 case 0XD0: return Key_res+ 4;break;
                 case 0XE0: return Key_res;   break;
                 default: return 0;	 //无按键按下，返回0	
			 }
		 }
	 }
	}
	else
		Key_down = 0;	//按键被松开
	if(Key_res)
		Key_down = 1;	//标志按键被按下，防止重入
	return Key_res;
}
    

//-----------------------------------矩阵键盘服务(功能)子函数-------------------//
void Key16_Function(void) 
{
	if(i != 0)    //按键按下
		{
			switch(i)
			{
			  case  1: P1=0XFE;P2=0XFF;break;
			  case  2: P1=0XFD;P2=0XFF;break;
		      case  3: P1=0XFB;P2=0XFF;break;
		      case  4: P1=0XF7;P2=0XFF;break;
			  case  5: P1=0XEF;P2=0XFF;break;
			  case  6: P1=0XDF;P2=0XFF;break;
			  case  7: P1=0XBF;P2=0XFF;break;
			  case  8: P1=0X7F;P2=0XFF;break;
			  case  9: P1=0XFF;P2=0XFE;break;
			  case 10: P1=0XFF;P2=0XFD;break;
			  case 11: P1=0XFF;P2=0XFB;break;
			  case 12: P1=0XFF;P2=0XF7;break;
			  case 13: P1=0XFF;P2=0XEF;break;
			  case 14: P1=0XFF;P2=0XDF;break;
			  case 15: P1=0XFF;P2=0XBF;break;
			  case 16: P1=0XFF;P2=0X7F;break;
			  default:P1=0X00;P2=0X00;break;
			}
	}
}

//------------------------串行接收中断函数 (串行接收采用中断方式)---------------------//
void uart_isr() interrupt 4
{
	if(RI)
	{
	    RI=0;       //清除RI位
        temp1=SBUF;
        switch(temp1)
	   	 {
			   case 0x00:P4=0XFF;P2=0X01;P7=A[0];P2=0X02;P7=A[0];break;
			   case 0x01:P4=0XFE;P2=0X01;P7=A[0];P2=0X02;P7=A[1];break;
			   case 0x02:P4=0XFD;P2=0X01;P7=A[0];P2=0X02;P7=A[2];break;
			   case 0x03:P4=0XFC;P2=0X01;P7=A[0];P2=0X02;P7=A[3];break;
			   case 0x04:P4=0XFB;P2=0X01;P7=A[0];P2=0X02;P7=A[4];break;
		       case 0x05:P4=0XFA;P2=0X01;P7=A[0];P2=0X02;P7=A[5];break;
			   case 0x06:P4=0XF9;P2=0X01;P7=A[0];P2=0X02;P7=A[6];break;
			   case 0x07:P4=0XF8;P2=0X01;P7=A[0];P2=0X02;P7=A[7];break;
			   case 0x08:P4=0XF7;P2=0X01;P7=A[0];P2=0X02;P7=A[8];break;
			   case 0x09:P4=0XF6;P2=0X01;P7=A[0];P2=0X02;P7=A[9];break;
			   case 0x0A:P4=0XF5;P2=0X01;P7=A[1];P2=0X02;P7=A[0];break;
		       case 0x0B:P4=0XF4;P2=0X01;P7=A[1];P2=0X02;P7=A[1];break;
			   case 0x0C:P4=0XF3;P2=0X01;P7=A[1];P2=0X02;P7=A[2];break;
			   case 0x0D:P4=0XF2;P2=0X01;P7=A[1];P2=0X02;P7=A[3];break;
		       case 0x0E:P4=0XF1;P2=0X01;P7=A[1];P2=0X02;P7=A[4];break;
		       case 0x0F:P4=0XF0;P2=0X01;P7=A[1];P2=0X02;P7=A[5];break;
			   default:P4=0XFF;break;
		  }		
   }
	 if(TI)
    {
       TI = 0;         //清除TI位
       busy = 0;       //清零正忙标志
    }

}
//-----------------------发送串口数据(串行发送采用查询方式)-------------------------------//
void SendData(void)
{
    while(busy);          //等待前面的数据发送完成
    busy = 1;             //将正忙标志置1，
	SBUF = temp;          //串行发送
}

