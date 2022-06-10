#include <STC89C5xRC.H>
#include "LCD1602.h"


void main()
{
    LCD1602_Init();									//初始化LCD1602
    LCD1602_delay_ms(1);
    LCD1602_set_position(0,0);//指针设置到第一行第1列
    LCD1602_prints("Hello World!");
    LCD1602_set_position(0,13);//指针设置到第一行第13列
    LCD1602_prints("STC");
//	LCD1602_write_com(0x01);					//清屏指令
    while(1)
    {
        LCD1602_set_position(1,0);				 //指针设置到第二行第1列
        LCD1602_prints("perseverance");
        LCD1602_set_position(1,14);//指针设置到第二行第14列
        LCD1602_prints("51");
    }
}