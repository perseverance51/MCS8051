
/**********************************************
利用红外接收头(VS1838)获取遥控按键码
******************************************************************/
//#include<reg52.h>    //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include <STC15F2K60S2.H>

#include <stdio.h>//用于printf打印，需要加载此头文件
#include <stdarg.h> 

sbit IR=P3^2;  //红外接口，需要接到带外部中断引脚上

/*------------------------------------------------
                全局变量声明
------------------------------------------------*/

unsigned char  irtime;//红外用全局变量

bit irpro_ok,irok;
unsigned char IRcord[4];
unsigned char irdata[33];

/*------------------------------------------------
                  函数声明
------------------------------------------------*/

void Ir_work(void);
void Ircordpro(void);

/*------------------------------------------------
                 串口初始化函数
------------------------------------------------*/
void init_com(void)
{
		SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR &= 0xFB;		//定时器时钟12T模式
	T2L = 0xE8;		//设置定时初始值
	T2H = 0xFF;		//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	TI=1;			// 很关键，使用printf函数时必须有此命令
	
//	TMOD|=0x20;		   //设T1为方式1，GATE=1；
//	SCON=0x50;
//	TH1=0xFD;
//	TL1=0xFD;
//	TR1=1;			   //开启定时器1
	EA=1;			   //开启总中断

}

/*------------------------------------------------
                  定时器0中断处理
------------------------------------------------*/

void tim0_isr (void) interrupt 1 using 1
{
  irtime++;  //用于计数2个下降沿之间的时间
}

/*------------------------------------------------
                  外部中断0中断处理
------------------------------------------------*/
void EX0_ISR (void) interrupt 0 //外部中断0服务函数
{
  	static volatile unsigned char  i =0 ;             //接收红外信号处理
	static  volatile bit startflag = 1;                //是否开始处理标志位
	TR0 = 1;//打开定时器0，计算高电平的时间（引导码）
	if(startflag)                         
	{
		if(irtime<63 && irtime>=33)//引导码 TC9012的头码，9ms+4.5ms
			i=0;
		irdata[i]=irtime;//存储每个电平的持续时间，用于以后判断是0还是1
		irtime=0;
		i++;
		if(i==33)
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

/*------------------------------------------------
                定时器0初始化
------------------------------------------------*/
void TIM0init(void)//定时器0初始化
{

  TMOD|=0x02;//定时器0工作方式2，TH0是重装值，TL0是初值
  TH0=0x00; //重载值
  TL0=0x00; //初始化值
  ET0=1;    //开中断
  TR0=0;    //不开启定时器0
}
/*------------------------------------------------
                  外部中断0初始化
------------------------------------------------*/
void EX0init(void)
{
 IT0 = 1;   //指定外部中断0下降沿触发，INT0 (P3.2)
 EX0 = 1;   //使能外部中断
 EA = 1;    //开总中断
}
/*------------------------------------------------
                  键值处理
------------------------------------------------*/

void Ir_work(void)//红外键值散转程序
{
	//发送红外码
	//这里转义后的码由上位机解码显示，也可以自己上传显示，默认X，也就是只要是X则上位机自动解码，其他值上位机则显示上传的值
	//02bx输出以16进制的格式输出整数类型的数值域宽为2，右对齐，不足的用字符0替代
	printf("RECE_IR DATA:%02bx-%02bx-%02bx-%02bx \n", IRcord[0], IRcord[1], IRcord[2], IRcord[3]); 

	irpro_ok=0;//清空完成标志
	
}
/*------------------------------------------------
                红外码值处理
------------------------------------------------*/
void Ircordpro(void)//红外码值处理函数
{ 
	unsigned char i, j, k;
	unsigned char cord,value;
	
	k=1;
	IRcord[0] = 0; 
	IRcord[1] = 0;
	IRcord[2] = 0;
	IRcord[3] = 0;
	for(i=0;i<4;i++)      //处理4个字节
	{
		for(j=1;j<=8;j++) //处理1个字节8位
		{
			cord=irdata[k];
			if(cord>7)//大于某值为1，这个和晶振有绝对关系，这里使用12M计算，此值可以有一定误差
				value|=0x80;
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
                    主函数
------------------------------------------------*/
void main(void)
{
	init_com();
	EX0init(); //初始化外部中断
	TIM0init();//初始化定时器
	while(1)//主循环
	{
		if(irok)                        //如果接收好了进行红外处理
		{   
			EA = 0;
			Ircordpro();
			EA = 1;
			irok=0;
		}
		
		if(irpro_ok)                   //如果处理好后进行工作处理，如按对应的按键后显示对应的数字等
		{
			EA = 0;    //关总中断
			Ir_work();
			EA = 1;    //开启总中断，等待下一次接收

		}
	}
}
  
  