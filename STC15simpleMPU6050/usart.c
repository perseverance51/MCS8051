#include "usart.h"

#define		BaudRate1		9600L
//#define		UART1_BUF_LENGTH	32
#define	RX1_Lenth		32			//���ڽ��ջ��峤��
#define	Timer1_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 1 ��װֵ�� ��Ӧ300KHZ


unsigned char	TX1_Cnt;	//���ͼ���
unsigned char	RX1_Cnt;	//���ռ���
 unsigned char idata RX1_Buffer[32];
bit	B_TX1_Busy;	//����æ��־
	//========================================================================
// ����: void UART1_int (void) interrupt UART1_VECTOR
// ����: UART1�жϺ�����
// ����: nine.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void UART1_int (void) interrupt UART1_VECTOR
{
	if(RI)
	{
		RI = 0;
		RX1_Buffer[RX1_Cnt] = SBUF;		//����һ���ֽ�
		if(++RX1_Cnt >= RX1_Lenth)	RX1_Cnt = 0;	//�����������
	}

	if(TI)
	{
		TI = 0;
		B_TX1_Busy = 0;		//�������æ��־
	}
}

//extern bit	B_TX1_Busy;	//����æ��־
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
		EA = 1;	//�������ж�
}
//========================================================================
// ����: void PrintString1(u8 *puts)
// ����: ����1�����ַ���������
// ����: puts:  �ַ���ָ��.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void PrintString1(u8 *puts)	//����һ���ַ���
{
    for (; *puts != 0;	puts++)   	//����ֹͣ��0����
	{
		SBUF = *puts;
		B_TX1_Busy = 1;
		while(B_TX1_Busy);
	}
}

//��дputchar����,�������printf���ڴ�ӡ���
 char putchar(unsigned char c)
{
	     SBUF = c; 					//��������
     while(!TI); 					//�ȴ��������
     TI=0; 							//���㷢�ͱ�־λ;
	return c;
}

