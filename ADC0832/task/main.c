#include <reg52.h>
#include <intrins.h>

/*已知数据类型重新定义*/
#define uchar unsigned char
#define uint unsigned int


/*引脚定义*/
//LCD1602
#define LCD1602_DB P0	//LCD1602数据总线
sbit LCD1602_RS = P2^0;	 //RS端
sbit LCD1602_RW = P2^1;	 //RW端
sbit LCD1602_EN = P2^2;	 //EN端

//ADC0832
sbit cs = P1^0;//片选使能，低电平有效
sbit clk = P1^1;//芯片时钟输入
sbit dio = P1^2;//数据输入DI与输出DO

//延时2us
void Delay_2us(void)
{
	_nop_();
	_nop_();
}
/*=================================================
*函数名称：Read_Busy
*函数功能：判断1602液晶忙，并等待
=================================================*/
void Read_Busy()
{
	uchar busy;
	LCD1602_DB = 0xff;//复位数据总线
	LCD1602_RS = 0;	  //拉低RS
	LCD1602_RW = 1;	  //拉高RW读
	do
	{
		LCD1602_EN = 1;//使能EN
		busy = LCD1602_DB;//读回数据
		LCD1602_EN = 0;	 //拉低使能以便于下一次产生上升沿
	}while(busy & 0x80); //判断状态字BIT7位是否为1，为1则表示忙，程序等待
}
/*=================================================
*函数名称：LCD1602_Write_Cmd
*函数功能：写LCD1602命令
*调用：Read_Busy();
*输入：cmd:要写的命令
=================================================*/
void LCD1602_Write_Cmd(uchar cmd)
{
	Read_Busy();	 //判断忙，忙则等待
	LCD1602_RS = 0;
	LCD1602_RW = 0;	//拉低RS、RW操作时序情况1602课件下中文使用说明基本操作时序章节
	LCD1602_DB = cmd;//写入命令
	LCD1602_EN = 1;	 //拉高使能端 数据被传输到LCD1602内
	LCD1602_EN = 0;	 //拉低使能以便于下一次产生上升沿
}
/*=================================================
*函数名称：LCD1602_Write_Dat
*函数功能：写LCD1602数据
*调用：Read_Busy();
*输入：dat：需要写入的数据
=================================================*/
void LCD1602_Write_Dat(uchar dat)
{
	Read_Busy();
	LCD1602_RS = 1;
	LCD1602_RW = 0;
	LCD1602_DB = dat;
	LCD1602_EN = 1;
	LCD1602_EN = 0;
}

/*=================================================
*函数名称：LCD1602_Dis_Str
*函数功能：在指定位置显示字符串
*调用：LCD1602_Write_Cmd(); LCD1602_Write_Dat();
*输入：x：要显示的横坐标取值0-40，y：要显示的行坐标取值0-1（0为第一行,1为第二行）
		*str：需要显示的字符串
=================================================*/
void LCD1602_Dis_Str(uchar x, uchar y, uchar *str)
{
	if(y) x |= 0x40;
	x |= 0x80;
	LCD1602_Write_Cmd(x);
	while(*str != '\0')
	{
		LCD1602_Write_Dat(*str++);
	}
}
/*=================================================
*函数名称：Init_LCD1602
*函数功能：1602初始化
*调用：	LCD1602_Write_Cmd();
=================================================*/
void Init_LCD1602()
{
	LCD1602_Write_Cmd(0x38); //	设置16*2显示，5*7点阵，8位数据接口
	LCD1602_Write_Cmd(0x0c); //开显示
	LCD1602_Write_Cmd(0x06); //读写一字节后地址指针加1
	LCD1602_Write_Cmd(0x01); //清除显示
}
/*****************************************
函数简介：获取ADC0832数据
函数名称：ADC_read_data(bit channel)
参数说明：ch为入口参数，ch=0选择通道0，ch=1选择通道1
函数返回：返回读取到的二进制ADC数据，格式为unsigned char
		  当返回一直0时，转换数据有误
******************************************/
uchar ADC_read_data(bit channel)
{
	uchar i,dat0=0,dat1=0;
  //------第1次下降沿之前di置高，启动信号---------

	cs=0;			//片选信号置低，启动AD转换芯片
	clk=0;			//时钟置低平
	
	dio=1;  		//开始信号为高电平
	Delay_2us();
	clk=1;			//产生一个正脉冲,在时钟上升沿，输入开始信号（DI=1）
	Delay_2us();
  	clk=0;			//第1个时钟下降沿
	dio=1;
	
	Delay_2us();
	clk=1;		    // 第2个下降沿输入DI=1，表示双通道单极性输入
	Delay_2us();	 
  //------在第2个下降沿，模拟信号输入模式选择（1：单模信号，0：双模差分信号）---------		
	clk=0;	
	dio=channel;         // 第3个下降沿,设置DI，选择通道;
	Delay_2us();
	clk=1;
	Delay_2us();

   //------在第3个下降沿，模拟信号输入通道选择（1：通道CH1，0：通道CH0）------------	
	
	clk=0;
	dio=1;          //第四个下降沿之前，DI置高，准备接收数 
	Delay_2us();	
	for(i=0;i<8;i++)                 //第4~11共8个下降沿读数据（MSB->LSB）
	{
		clk=1;
		Delay_2us();
		clk=0;
		Delay_2us();
		dat0=dat0<<1|dio;
	}
	for(i=0;i<8;i++)                 //第11~18共8个下降沿读数据（LSB->MSB）
	{
		dat1=dat1|((uchar)(dio)<<i);
		clk=1;
		Delay_2us();
		clk=0;
		Delay_2us();
	} 
	cs=1;				  
	return (dat0==dat1)?dat0:0;	    //判断dat0与dat1是否相等
}

void main()
{
	uchar adc = 0;
	uint voltage = 0;
	uchar LED_buffer1[16] = {"    ADC:        "};//ADC显示缓冲
	uchar LED_buffer2[16] = {"Voltage:      V "};//电压显示缓冲
	Init_LCD1602();//1602初始化
	while(1)
	{
		adc = ADC_read_data(0);//读取adc值
		voltage = adc * 500.0 / 255;//将adc值换算成电压
		
		LED_buffer1[9] = adc / 100 + '0';		//取adc百位,整型转ASCII码，补0
		LED_buffer1[10] = adc % 100 / 10 + '0';	//取adc十位
		LED_buffer1[11] = adc % 10 + '0';		//取adc个位

		LED_buffer2[9] = voltage / 100 + '0';		//取电压个位
		LED_buffer2[10] = '.';//小数点
		LED_buffer2[11] = voltage / 10 % 10 + '0';	//取电压十分位
		LED_buffer2[12] = voltage % 10 + '0';	//取电压百分位
		
		
		LCD1602_Dis_Str(0, 0, &LED_buffer1[0]);	//显示字符串
		LCD1602_Dis_Str(0, 1, &LED_buffer2[0]);	//显示字符串
	}
}