
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* --- QQ:  800003751 ----------------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/


#ifndef	__DELAY_H
#define	__DELAY_H
#include "intrins.h"//用于提供_nop_();函数



void Delay25ms();		//@11.0592MHz

void Delay1ms();	//@22.1184MHz
void Delay25us();
void Delay10us();		//@11.0592MHz
void Delay50us();		//@11.0592MHz
void Delay35us();		//@11.0592MHz
void Delay1000ms();

#endif
