
#include "LCD1602.h"

void Delay1us(unsigned int a)
{        
	unsigned int b,c;
	for(c=a;c>0;c--)                
	for(b=110;b>0;b--);
}

void LCD1602_WriteCom(unsigned char com)          //写入命令
{
        LCD1602_E=0;
        LCD1602_RS=0;
        LCD1602_RW=0;
        LCD1602_DATAPINS = com;        //由于4位的接线是接到P0口的高四位，所以传送高四位不用改
        Delay1us(10);
        LCD1602_E=1;
        Delay1us(10);
        LCD1602_E=0;
        Delay1us(10);
        LCD1602_DATAPINS=com<<4;
        Delay1us(10);
        LCD1602_E=1;
        Delay1us(10);
        LCD1602_E=0;
}

void LCD1602_WriteData(unsigned char dat)                        //写入数据
{
        LCD1602_E=0;
        LCD1602_RS=1;
        LCD1602_RW=0;
        LCD1602_DATAPINS=dat;        //由于4位的接线是接到P0口的高四位，所以传送高四位不用改
        Delay1us(10);
        LCD1602_E=1;
        Delay1us(10);
        LCD1602_E=0;
        LCD1602_DATAPINS=dat<<4;
        Delay1us(10);
        LCD1602_E=1;
        Delay1us(10);
        LCD1602_E=0;
}
//x自动行，y指定位置，s写入字符串函数
void  LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s)
 {    
     if (y == 0)
     {    
          LCD1602_WriteCom(0x80 + x);     //表示第一行
     }
     else
     {     
         LCD1602_WriteCom(0xC0 + x);      //表示第二行
     }       
     while(*s)
     {    
          LCD1602_WriteData(*s);    
          s ++;    
     }

 }
void LCD1602_Init()                                                  //LCD初始化子程序
{
        LCD1602_WriteCom(0x32);         //将8位总线转为4位总线
        LCD1602_WriteCom(0x28);         //在四位线下的初始化
        LCD1602_WriteCom(0x0c);  //开显示不显示光标
        LCD1602_WriteCom(0x06);  //写一个指针加1
        LCD1602_WriteCom(0x01);  //清屏
        LCD1602_WriteCom(0x80);  //设置数据指针起点
}

