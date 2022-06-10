

#include "stdio.h"
#include "config.h"
//#include "STC15Fxxxx.H"
#include "MPU6050.H"

//#include "usart.h"
#include "delay.h"
#define		BaudRate1		9600L
#define		UART1_BUF_LENGTH	32
#define	RX1_Lenth		32			//���ڽ��ջ��峤��
#define	Timer1_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 1 ��װֵ�� ��Ӧ300KHZ

u8	TX1_Cnt;	//���ͼ���
u8	RX1_Cnt;	//���ռ���
bit	B_TX1_Busy;	//����æ��־

u8 	idata RX1_Buffer[UART1_BUF_LENGTH];	//���ջ���

void InitSerialPort(void)
{ 
	SCON = 0x50;			//8λ���ݣ��ɱ䲨����
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TH1 = (u8)(Timer1_Reload >> 8);
	TL1 = (u8)Timer1_Reload;
	
	ET1 = 0;		//��ֹ��ʱ��%d�ж�
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
			ES=1; 				//�����жϿ���,���ò�ѯ��ʱ���ô��ж�      
    	REN=1;      			//����Ϊ������ʽ1�������������
    	SM0=0;					//SM0 SM1���ڹ�����ʽѡ��01��8λ�첽�շ����������ɶ�ʱ������
    	SM1=1;
	
}
//��дputchar����,�������printf���ڴ�ӡ���
 char putchar(unsigned char c)
{
	     SBUF = c; 					//��������
     while(!TI); 					//�ȴ��������
     TI=0; 							//���㷢�ͱ�־λ;
	return c;
}
void	UART1_config(u8 brt)	// ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer1��������.
{
	/*********** ������ʹ�ö�ʱ��2 *****************/
	if(brt == 2)
	{
		AUXR |= 0x01;		//S1 BRT Use Timer2;
		//Timer1_Reload;
	}

	/*********** ������ʹ�ö�ʱ��1 *****************/
	else
	{
		TR1 = 0;
		AUXR &= ~0x01;		//S1 BRT Use Timer1;
		AUXR |=  (1<<6);	//Timer1 set as 1T mode
		TMOD &= ~(1<<6);	//Timer1 set As Timer
		TMOD &= ~0x30;		//Timer1_16bitAutoReload;
		TH1 = (u8)((65536UL - (MAIN_Fosc / 4) / BaudRate1) / 256);
		TL1 = (u8)((65536UL - (MAIN_Fosc / 4) / BaudRate1) % 256);
		ET1 = 0;	//��ֹ�ж�
		INT_CLKO &= ~0x02;	//�����ʱ��
		TR1  = 1;
		
	}
	/*************************************************/
		//SCON = 0x50;			//8λ���ݣ��ɱ䲨����
	//SCON = (SCON & 0x3f) | 0x40;	//UART1ģʽ, 0x00: ͬ����λ���, 0x40: 8λ����,�ɱ䲨����, 0x80: 9λ����,�̶�������, 0xc0: 9λ����,�ɱ䲨����
//	PS  = 1;	//�����ȼ��ж�
	ES  = 1;	//�����ж�
	REN = 1;	//�������
	P_SW1 &= 0x3f;
	P_SW1 |= 0x80;		//UART1 switch to, 0x00: P3.0 P3.1, 0x40: P3.6 P3.7, 0x80: P1.6 P1.7 (����ʹ���ڲ�ʱ��)
	PCON2 |=  (1<<4);	//�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE

	B_TX1_Busy = 0;
	TX1_Cnt = 0;
	RX1_Cnt = 0;
}
	void main(void)
	{

	P0M1 = 0;	P0M0 = 0;	//����Ϊ׼˫���
	P1M1 = 0;	P1M0 = 0;	//����Ϊ׼˫���
	P2M1 = 0;	P2M0 = 0;	//����Ϊ׼˫���
	P3M1 = 0;	P3M0 = 0;	//����Ϊ׼˫���
	P4M1 = 0;	P4M0 = 0;	//����Ϊ׼˫���
	P5M1 = 0;	P5M0 = 0;	//����Ϊ׼˫���
	P6M1 = 0;	P6M0 = 0;	//����Ϊ׼˫���
	P7M1 = 0;	P7M0 = 0;	//����Ϊ׼˫���
	UART1_config(1);	// ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer1��������.
		InitSerialPort();
	//PrintString1("STC15W408AS UART1 Test Prgramme!\r\n");	//SUART1����һ���ַ���
	EA = 1;	//�������ж�
	while (1)
	{
		delay_ms(500);
		printf("perseverance51 \r\n");	//SUART1����һ���ַ���
		P10 = !P10;
	}
}


