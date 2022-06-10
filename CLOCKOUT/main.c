#include "reg51.h"
typedef unsigned char BYTE;
typedef unsigned int WORD;
#define FOSC 16000000L
//-----------------------------------------
sfr CLK_DIV = 0x97; //时钟分频寄存器
sfr AUXR = 0x8E; //时钟分频寄存器
//-----------------------------------------
void main()
{
	//AUXR |= 0x00;
//	CLK_DIV = 0x40;//0100,0000 P5.4输出频率为SYSclk = 16MHz
	//	CLK_DIV = 0x42;//0100,0010 P5.4输出频率为SYSclk/4 = 4MHz
//	CLK_DIV = 0x43;//0100,0011 P5.4输出频率为SYSclk = 2MHz
 //	CLK_DIV = 0xc2; //1000,0000 P5.4输出频率为SYSclk/4/4 =1MHz
 //CLK_DIV = 0x80; //1000,0000 P5.4输出频率为SYSclk/2 =8MHz
//	CLK_DIV = 0x83; //1000,0011 P5.4输出频率为SYSclk/2/8 =1M

// CLK_DIV = 0xC0; //1100,0000 P5.4输出频率为SYSclk/4 = 4MHz
			CLK_DIV = 0xC4;//1100,0111 P5.4输出频率为SYSclk/4/16 = 250KHz
//		CLK_DIV = 0xC5;//1100,0111 P5.4输出频率为SYSclk/4/32 = 125KHz
	//CLK_DIV = 0xC6;//1100,0111 P5.4输出频率为SYSclk/4/64 = 62.5KHz
//	CLK_DIV = 0xC7;//1100,0111 P5.4输出频率为SYSclk/4/128 = 31.25KHz
//CLK_DIV = 0x00; //主时钟不对外输出时钟
 while (1); //程序终止
}