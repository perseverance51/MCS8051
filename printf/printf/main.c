
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

/*************	����˵��	**************

˫����ȫ˫���жϷ�ʽ�շ�ͨѶ����

ͨ��PC��MCU��������, MCU�յ���ͨ�����ڰ��յ�������ԭ������.

******************************************/

/*************	���س�������	**************/

sbit ledpin = P1^0;
/*************	���ر�������	**************/


/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/


/*************  ����1��ʼ������ *****************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ʹ�ò�����,   BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 9600ul;			//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//�ж����ȼ�, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(����ʹ���ڲ�ʱ��)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//��ʼ������1 USART1,USART2

	//PrintString1("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//SUART1����һ���ַ���
}


/**********************************************/
void main(void)
{
	P1M1 = 0;	
	P1M0 = 0;	//����Ϊ׼˫���
	P2M1 = 0;	
	P2M0 = 0;	//����Ϊ׼˫���
	UART_config();
	EA = 1;
InitMPU6050();
	while (1)
	{
printf("X����ٶ�=%d��\n",GetData(ACCEL_XOUT_H));	//X����ٶ�
		Delay1000ms();	
printf("Y����ٶ�=%d��\n",GetData(ACCEL_YOUT_H));		//Y����ٶ�
			Delay1000ms();
printf("Z����ٶ�=%d��\n",GetData(ACCEL_ZOUT_H));		//Z����ٶ�
		Delay1000ms();	
printf("X����ٶ�=%d��\n",GetData(GYRO_XOUT_H));	//X����ٶ�
		Delay1000ms();	
printf("Y����ٶ�=%d��\n",GetData(GYRO_YOUT_H));	//Y����ٶ�
			Delay1000ms();
printf("Z����ٶ�=%d��\n",GetData(GYRO_ZOUT_H));	//Z����ٶ�
		Delay1000ms();
	printf("�¶�=%3.1f�棬\n",Get_temp());	//Z����ٶ�			
			ledpin = !ledpin;
			Delay1000ms();
	}
}



