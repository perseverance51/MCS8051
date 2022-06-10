#ifndef __LCD12864_H__
#define __LCD12864_H__

#include <reg52.h>

/*****Һ����ʾ���Ķ˿ڶ���*****/
#define data_ora P0 /*Һ����������*/
sbit LCDMcs=P2^4 ;    /*Ƭѡ1*/
sbit LCDScs=P2^3 ;    /*Ƭѡ2*/

//sbit RESET=P2^3 ;  /*��λ�ź�*/
sbit LCDDi=P2^2 ;     /*����/ָ�� ѡ��*/
sbit LCDRW=P2^1 ;     /*��/д ѡ��*/
sbit LCDEnable=P2^0 ;     /*��/д ʹ��*/


#define LCDLCDDisp_Off   0x3e
#define LCDLCDDisp_On    0x3f
#define Page_Add     0xb8//ҳ��ַ
#define LCDCol_Add     0x40//�е�ַ
#define Start_Line     0xC0//�е�ַ

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