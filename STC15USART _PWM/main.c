
/*************	����˵��	**************

˫����ȫ˫���жϷ�ʽ�շ�ͨѶ����
ע�⣺
ͨ��PC��MCU��������, MCU�յ���ͨ�����ڰ��յ�������ԭ������.

******************************************/


#include	"config.h"
#include "stdio.h"
#include "USART.h"
#include "PWM.h"

void  delay_ms(unsigned int ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 13000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}



//========================================================================
// ����: void main(void)
// ����: ��������
// ����: none.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void main(void)
{
	
			PCA_Init();	//PCA��ʼ��
//	P0M1 = 0;	P0M0 = 0;	//����Ϊ׼˫���
//	P1M1 = 0;	P1M0 = 0;	//����Ϊ׼˫���
//	P2M1 = 0;	P2M0 = 0;	//����Ϊ׼˫���
	P2M1 &= ~(0xe0);	//P2.7 P2.6 P2.5 ����Ϊ�������
	P2M0 |=  (0xe0);
	P3M1 = 0;	P3M0 = 0;	//����Ϊ׼˫���
//	P4M1 = 0;	P4M0 = 0;	//����Ϊ׼˫���
//	P5M1 = 0;	P5M0 = 0;	//����Ϊ׼˫���
//	P6M1 = 0;	P6M0 = 0;	//����Ϊ׼˫���
//	P7M1 = 0;	P7M0 = 0;	//����Ϊ׼˫���
		//	S1_USE_P30P31();
 InitSerialPort(); 		//��ʼ������,������9600��8bit����λ��1ֹͣλ����У��
	EA = 1;	//�������ж�

//	PrintString1("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//SUART1����һ���ַ���

	while (1)
	{
		delay_ms(500);	
		P10 = !P10;
		if(++pwm0 >= PWM_HIGH_MAX)  pwm0 = PWM_HIGH_MIN;
		PWMn_Update(PCA0,pwm0);

		if(++pwm1 >= PWM_HIGH_MAX)	pwm1 = PWM_HIGH_MIN;
		PWMn_Update(PCA1,pwm1);

		if(++pwm2 >= PWM_HIGH_MAX)	pwm2 = PWM_HIGH_MIN;
		PWMn_Update(PCA2,pwm2);
		
		printf("pwm0:%u \t pwm1:%u \t pwm2:%u\r\n",pwm0,pwm1,pwm2);	//SUART1����һ���ַ���
	}
}



