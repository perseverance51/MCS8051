/*********************************************************************
 *
 *                           TM1638_drive.h
 *
 *********************************************************************
 * 描    述: TM1638驱动函数头文件
 * 开发平台: KEIL C51
 * 公    司: 			
 * 网    址: 
 * 作者			日期			注释
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 飞云创意电子		18/10/26		原始文件
 
 ********************************************************************/

#ifndef TM1638_DRIVE_H
#define TM1638_DRIVE_H

#include<stc15series.h>

#define K1S1   1
#define K1S2   2
#define K1S3   3
#define K1S4   4
#define K1S5   5
#define K1S6   6
#define K1S7   7
#define K1S8   8

#define K2S1   9
#define K2S2   10
#define K2S3   11
#define K2S4   12
#define K2S5   13
#define K2S6   14
#define K2S7   15
#define K2S8   16

#define K3S1   17
#define K3S2   18
#define K3S3   19
#define K3S4   20
#define K3S5   21
#define K3S6   22
#define K3S7   23
#define K3S8   24

#define SEG9      0
#define SEG10     1
#define SEG9SEG10 2

//TM1638模块引脚定义
sbit	DIO=P2^0;
sbit	CLK=P2^1;
sbit	STB=P2^2;

 //共阴数码管段码
extern unsigned char code Num_tab[16];

//TM1638低8段位码  【SEG1-SEG8】
extern unsigned char code WeiL_tab[8];

//TM1638高2段位码  【SEG9-SEG10】
extern unsigned char code WeiH_tab[8];

void TM1638_W_DATA(unsigned char DATA);
void TM1638_W_CMD(unsigned char cmd);
void Write_DATA(unsigned char add,unsigned char DATA);
void init_TM1638(void);
unsigned char Read_Onekey(void);
void WriteSEG9_LED(unsigned char sta);
void WriteSEG10_LED(unsigned char sta);
void Write_LED(unsigned char sta9,unsigned char sta10);



#endif 