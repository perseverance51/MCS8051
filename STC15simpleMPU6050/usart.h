#ifndef __USART_H
#define __USART_H	

#include "stdio.h"
#include	"config.h"
#include	"STC15Fxxxx.H"


extern  unsigned char idata RX1_Buffer[32];	//���ջ���

extern bit	B_TX1_Busy;	//����æ��־
extern unsigned char 	TX1_Cnt;	//���ͼ���
extern unsigned char	RX1_Cnt;	//���ռ���

void InitSerialPort(void);
//void	UART1_config(u8 brt);	// ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer1��������.
void 	PrintString1(u8 *puts);
char putchar(unsigned char c);
//void UART1_int (void);

#endif