/*----------------------------------------------------------   
 * ��Ŀ����:

     ����ͨ����������

 * ��������:

     1�����ڳ�ʼ��  
	 2����ѯ������һ���ֽ� 
	 3�����ڷ���һ���ֽ�
	 4�����ڷ���һ���ַ��� 

 * ����˵��:
     MCU:             STC15W408AS
     ������:       	  STC15W408AS������
     ����:      	  �ⲿʱ�ӣ�16MHz
     ��չģ��:  	  -
     ���:            Keil.C51.V9.01

 * ��ע:
     - ��������Ĵ�������Դ�������

----------------------------------------------------------*/   
#ifndef _USART_H_
#define _USART_H_

#include	"config.h"
#define FOSC 16000000L //ϵͳʱ�ӣ�����Ƶ��
#define BAUD 9600
extern bit busy;//��æ��־


extern unsigned char *pchar;

void InitSerialPort(void);					//���ڳ�ʼ������
unsigned char ReceiveByte(void);			//���ڲ�ѯ������һ���ֽ�
char putchar(unsigned char c);			//���ڷ���һ���ֽ�
//void SendString(unsigned char *pstr);		//���ڷ���һ���ַ���

#endif

//-------------------------THE END------------------------*/   

