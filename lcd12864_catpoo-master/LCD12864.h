#ifndef __LCD12864_H__
#define __LCD12864_H__

#include <reg52.h>

/*****液晶显示器的端口定义*****/
#define data_ora P0 /*液晶数据总线*/
sbit LCDMcs=P2^4 ;    /*片选1*/
sbit LCDScs=P2^3 ;    /*片选2*/

//sbit RESET=P2^3 ;  /*复位信号*/
sbit LCDDi=P2^2 ;     /*数据/指令 选择*/
sbit LCDRW=P2^1 ;     /*读/写 选择*/
sbit LCDEnable=P2^0 ;     /*读/写 使能*/


#define LCDLCDDisp_Off   0x3e
#define LCDLCDDisp_On    0x3f
#define Page_Add     0xb8//页地址
#define LCDCol_Add     0x40//列地址
#define Start_Line     0xC0//行地址

 void LCDdelay(unsigned int t);
 void CheckState();
 void write_com(unsigned char cmdcode);
 void init_lcd();
 void write_data(unsigned char LCDDispdata);
 void Clr_Scr();
 void hz_LCDDisp16(unsigned char column,unsigned char page, unsigned char code *hzk);
 void LeftDisplay();
 void RightDisplay();
#endif