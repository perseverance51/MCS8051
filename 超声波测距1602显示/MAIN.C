#include<reg52.h>
#include<intrins.h>
#define uint  unsigned int
#define uchar unsigned char
#define NOP() {_nop_();_nop_();_nop_();_nop_();}
//------LCD引脚-----
sbit LCD_RS=P2^0;
sbit LCD_RW=P2^1;
sbit LCD_EN=P2^2;
//------超声波引脚-------
sbit Tx=P3^3;		//触发控制信号输入
sbit Rx=P3^2;		//回响信号输出
uchar code table[]={"Distance :"};		//LCD第一行显示
uchar temp_dis[]= {"000.0 cm"};				//LCD第二行
long int t,distance;
uchar cache[4]={0,0,0,0};
//--------延时-------
void delay(uint ms)
{
	uint t;
	while(ms--)
		for(t=0;t<120;t++);
}
//-------读LCD状态-------
uchar read_lcd_state()
{
	uchar state;
	LCD_RS=0;
	LCD_RW=1;
	LCD_EN=1;
	_nop_();
	state=P0;
	LCD_EN=0;
	_nop_();
	return state;
}
//-------忙等待------
void lcd_busy_wait()
{
	while((read_lcd_state() & 0x80)==0x80);
	NOP();
}
//----------LCD写指令----------
void lcd_write_com(uchar com)
{
	lcd_busy_wait();
	LCD_RS=0;		//RS为0时，写指令，RS为1时，写数据
	LCD_RW=0;
	P0=com;
	NOP();
	LCD_EN=1;
	NOP();
	LCD_EN=0;
}
//----------LCD写数据----------
void lcd_write_data(uchar dat)
{
	lcd_busy_wait();
	LCD_RS=1;
	LCD_RW=0;
	P0=dat;
	NOP();					
	LCD_EN=1;
	NOP();
	LCD_EN=0;
}
//-------LCD初始化-------
void lcd_init()
{
	LCD_EN=0;
	lcd_write_com(0x38);			//LCD显示模式设置
	lcd_write_com(0x0c);			//LCD显示开/关及光标设置
	lcd_write_com(0x06);			//当写一个字符后地址指针加1，且光标加1
	lcd_write_com(0x01);			//显示清屏
}
//---------设置液晶显示位置-----------
void set_lcd_pos(uchar p)
{
	lcd_write_com(p|0x80);
}
//---------液晶显示程序----------
void lcd_print(uchar p,uchar *s,uint low)
{
	uint num;
	set_lcd_pos(p);
	for(num=0;num<low;num++)		
	{
		lcd_write_data(s[num]);
		delay(1);	
	}
}
void HC05_Init()
{
	Tx=1;						//触发脉冲
	NOP();NOP();NOP();NOP();
	Tx=0;
	distance=0.175*t;			//距离计算
}
void distance_convert(long int dat)
{
	cache[0]=dat/1000;
	cache[1]=dat/100%10;
	cache[2]=dat/10%10;
	cache[3]=dat%10;
	temp_dis[0]=cache[0]+'0';
	temp_dis[1]=cache[1]+'0';
	temp_dis[2]=cache[2]+'0';
	temp_dis[4]=cache[3]+'0';
}
//------------主程序-----------
void main()
{
	lcd_init();
	delay(5);
	TMOD=0x19;
	EA=1;			//开总中断
	TR0=1;			//启动定时器
	EX0=1;			//开外部中断
	IT0=1;			//设置为下降沿中断方式
	while(1)
	{
		HC05_Init();
		distance_convert(distance);
		lcd_print(0x01,table,10);
		lcd_print(0x44,temp_dis,8);
	}
}
//外部中断0
void int0() interrupt 0
{
	t=(TH0*256 + TL0);	//计算高电平持续的时间,上升沿到来时候开始计时，下降沿到来进入外部中断，关闭计时器，停止计时
	TH0=0;
	TL0=0;
}