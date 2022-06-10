/*----------------------------------------------------------   
 * 项目名称:

     串口通信驱动程序

 * 功能描述:

     1、串口初始化  
	 2、查询法接收一个字节 
	 3、串口发送一个字节
	 4、串口发送一个字符串 

 * 配置说明:
     MCU:             STC15W408AS
     开发板:       	  STC15W408AS开发板
     晶振:      	  外部时钟：16MHz
     扩展模块:  	  -
     软件:            Keil.C51.V9.01

 * 备注:
     - 将开发板的串口与电脑串口连接

----------------------------------------------------------*/   
#ifndef _USART_H_
#define _USART_H_

#include	"config.h"
#define FOSC 16000000L //系统时钟（晶振）频率
#define BAUD 9600
extern bit busy;//清忙标志


extern unsigned char *pchar;

void InitSerialPort(void);					//串口初始化设置
unsigned char ReceiveByte(void);			//串口查询法接收一个字节
char putchar(unsigned char c);			//串口发送一个字节
//void SendString(unsigned char *pstr);		//串口发送一个字符串

#endif

//-------------------------THE END------------------------*/   

