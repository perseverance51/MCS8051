
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
#include "stdio.h"
//#include "string.h"
//#include "stdarg.h"	 

#include	"config.h"
#include	"USART1.h"
#include	"delay.h"
#include	"mpu.h"

/*************	功能说明	**************

双串口全双工中断方式收发通讯程序。

通过PC向MCU发送数据, MCU收到后通过串口把收到的数据原样返回.

******************************************/

/*************	本地常量声明	**************/

sbit ledpin = P1^0;
/*************	本地变量声明	**************/


/*************	本地函数声明	**************/



/*************  外部函数和变量声明 *****************/


/*************  串口1初始化函数 *****************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 9600ul;			//波特率, 一般 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//中断优先级, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//初始化串口1 USART1,USART2

	//PrintString1("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//SUART1发送一个字符串
}


/**********************************************/
void main(void)
{
	P1M1 = 0;	
	P1M0 = 0;	//设置为准双向口
	P2M1 = 0;	
	P2M0 = 0;	//设置为准双向口
	UART_config();
	EA = 1;
InitMPU6050();
	while (1)
	{
printf("X轴加速度=%d，\n",GetData(ACCEL_XOUT_H));	//X轴加速度
		Delay1000ms();	
printf("Y轴加速度=%d，\n",GetData(ACCEL_YOUT_H));		//Y轴加速度
			Delay1000ms();
printf("Z轴加速度=%d，\n",GetData(ACCEL_ZOUT_H));		//Z轴加速度
		Delay1000ms();	
printf("X轴加速度=%d，\n",GetData(GYRO_XOUT_H));	//X轴角速度
		Delay1000ms();	
printf("Y轴加速度=%d，\n",GetData(GYRO_YOUT_H));	//Y轴角速度
			Delay1000ms();
printf("Z轴加速度=%d，\n",GetData(GYRO_ZOUT_H));	//Z轴角速度
		Delay1000ms();
	printf("温度=%3.1f℃，\n",Get_temp());	//Z轴角速度			
			ledpin = !ledpin;
			Delay1000ms();
	}
}



