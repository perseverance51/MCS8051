/****************

  �������������������������*/
#ifndef __USART_DEBUG_H__
	#define __USART_DEBUG_H__
void UART_init(void);			         // ����1��ʼ����9600 /11.0592MHZ
void UART_Send_Byte(unsigned char dat);
void UART_Send_Str(char *s);	         // �����ַ���
void UART_Send_Num(unsigned long dat);	 // ������ֵ
void UART_Send_StrNum(char *inf,unsigned long dat);	  // �����ַ���+��ֵ
//void UART_Send_Hex(unsigned int hex);				  // ����16����(������Χ)
//void UART_Send_binary(unsigned char dat);			  // ����2����
# endif