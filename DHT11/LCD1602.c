#include <LCD1602.h>
#include "reg51.h" 
		sbit RS = P2^6;
    sbit RW = P2^5;
    sbit EN = P2^7;
		
void LCD_send(unsigned char i,unsigned char j,unsigned char str[],unsigned char n)
{
	//输入：显示行数i,列数j，输出字符，输出字符长度n
	unsigned char x=0;
	if(i==1)
	{write_com(0x80+j);}
	else
	{write_com(0x80+0x40+j);}
    for(x=0;x<n;x++)
    {
         write_date(str[x]);
        delay(150);
    }
}
		
int LCD_init()//初始化函数体
{
    EN = 0;
    write_com(0X38);//设置16*2显示，5*7点阵，8位数据接口
    write_com(0X0C);//设置开显示，不显示光标
    write_com(0X06);//写一个字符时，整屏右移
    write_com(0X01);//显示清零
    return 0;
}

int write_com(unsigned char com)//写命令的函数体
{
    RS = 0;
    RW = 0;
    P0 = com;
    delay(5);
    EN = 1;
    delay(5);
    EN = 0;
    return 0;
}

int write_date(unsigned char data1)//写数据的函数体
{
    RS = 1;
    RW = 0;
    P0 = data1;
    delay(5);
    EN = 1;
    delay(5);
    EN = 0;
    return 0;
}

int delay(unsigned char xms)
{
    unsigned char x,y;
    for(x=xms;x>0;x--)
        for(y=110;y>0;y--);
    return 0;
}