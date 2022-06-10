#ifndef __USART_H
#define __USART_H	

#include "stdio.h"
#include	"config.h"
#include	"STC15Fxxxx.H"


extern  unsigned char idata RX1_Buffer[32];	//接收缓冲

extern bit	B_TX1_Busy;	//发送忙标志
extern unsigned char 	TX1_Cnt;	//发送计数
extern unsigned char	RX1_Cnt;	//接收计数

void InitSerialPort(void);
//void	UART1_config(u8 brt);	// 选择波特率, 2: 使用Timer2做波特率, 其它值: 使用Timer1做波特率.
void 	PrintString1(u8 *puts);
char putchar(unsigned char c);
//void UART1_int (void);

#endif