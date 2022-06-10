
/*************	����˵��	**************

˫����ȫ˫���жϷ�ʽ�շ�ͨѶ����
ע�⣺
ͨ��PC��MCU��������, MCU�յ���ͨ�����ڰ��յ�������ԭ������.

******************************************/
#include	"config.h"
#include "stdio.h"
#include "USART.h"
#include "PWM.h"
#include "ADC.h"

//	#define	Cal_MODE 	0	 //ÿ�β���ֻ��1��ADC. �ֱ���0.01V
	#define	Cal_MODE 	1	//ÿ�β���������16��ADC ��ƽ������. �ֱ���0.01V
//typedef 	unsigned char	u8;
//typedef 	unsigned int	u16;
//typedef 	unsigned long	u32;

volatile bit	B_4ms;			//1ms��־

volatile u16	msecond;		//��ʱ
u16	Bandgap;	//

//void  delay_ms(unsigned int ms)
//{
//     unsigned int i;
//	 do{
//	      i = MAIN_Fosc / 13000;
//		  while(--i)	;   //14T per loop
//     }while(--ms);
//}
/********************** ģ����ӳ�亯�� ************************/
long map( long x,  long in_min,  long in_max,  long out_min,  long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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
			u8 i;
	int	Duty ;
	int	j;
	
			PCA_Init();	//PCA��ʼ��
//	P0M1 = 0;	P0M0 = 0;	//����Ϊ׼˫���
//	P1M1 = 0;	P1M0 = 0;	//����Ϊ׼˫���
	P1M1 = 0x00; P1M0 = 0x00; 
	P1M1 |= (1<<3);		// ��ADC������Ϊ��������
	P1M0 &= ~(1<<3);
	P1ASF = (1<<3);		//P1.3��ADC
	ADC_CONTR = 0xE0;	//90T, ADC power on
//	P2M1 = 0;	P2M0 = 0;	//����Ϊ׼˫���
	P2M1 &= ~(0xe0);	//P2.7 P2.6 P2.5 ����Ϊ�������
	P2M0 |=  (0xe0);
	P3M1 = 0;	P3M0 = 0;	//����Ϊ׼˫���
//	P4M1 = 0;	P4M0 = 0;	//����Ϊ׼˫���
//	P5M1 = 0;	P5M0 = 0;	//����Ϊ׼˫���
//	P6M1 = 0;	P6M0 = 0;	//����Ϊ׼˫���
//	P7M1 = 0;	P7M0 = 0;	//����Ϊ׼˫���
		//	S1_USE_P30P31();
	Timer0Init();//��ʱ��0��ʼ��
 InitSerialPort(); 		//��ʼ������,������9600��8bit����λ��1ֹͣλ����У��
	EA = 1;	//�������ж�

//	PrintString1("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//SUART1����һ���ַ���

	while (1)
	{
		P10 = !P10;
		if(pwm0 >= PWM_HIGH_MAX)  pwm0 = PWM_HIGH_MAX;
		
//		if(pwm1 >= PWM_HIGH_MAX)	pwm1 = PWM_HIGH_MAX;
//		PWMn_Update(PCA1,pwm1);

//		if(pwm2 >= PWM_HIGH_MAX)	pwm2 = PWM_HIGH_MAX;
//		PWMn_Update(PCA2,pwm2);
	if(B_4ms)	//5ms�����־λ
		{
			B_4ms = 0;
			if(++msecond >= 250)	//��������1S
			{
				msecond = 0;
			#if (Cal_MODE == 0)
			//=================== ֻ��1��ADC, 10bit ADC. �ֱ���0.01V ===============================
				P1ASF = 0;
				Get_ADC10bitResult(1);	//�ı�P1ASF���ȶ�һ�β��������, ���ڲ��Ĳ������ݵĵ�ѹ��������ֵ.
				Bandgap = Get_ADC10bitResult(1);	//��P1.1�ĵ�ѹΪ��׼ADC, P1ASF=0, 0ͨ���ĵ�ѹֵ
	
				P1ASF = 1<<3;
				j = Get_ADC10bitResult(3);	//���ⲿ��ѹADC
			//	j = (u16)((u32)j * 507 / Bandgap);	//�����ⲿ��ѹ, BandgapΪ5.07V, ���ѹ�ֱ���0.01V
					printf("\t ADC =%d \t\n",j);	
				pwm0 = (u16)map(j, 0, 1023,80, 5920);//��ӳ���ֵ��ֵ��pwm0
				 
				printf("\t pwm0 =%u \t\n",pwm0 );
				#endif
			//==========================================================================

			//===== ������16��ADC ��ƽ������. �ֱ���0.01V =========
			#if (Cal_MODE == 1)
				P1ASF = 0;
				Get_ADC10bitResult(1);	//�ı�P1ASF���ȶ�һ�β��������, ���ڲ��Ĳ������ݵĵ�ѹ��������ֵ.
				for(j=0, i=0; i<16; i++)
				{
					j += Get_ADC10bitResult(1);	//��P11�ڲ���׼ADC, P1ASF=0, ��1ͨ��
				}
				Bandgap = j >> 4;	//16��ƽ��
				P1ASF = (1<<3);		//P1.3��ADC
				for(j=0, i=0; i<16; i++)
				{
					j += Get_ADC10bitResult(3);	//���ⲿ��ѹADC
				}
				j = j >> 4;	//16��ƽ��
				pwm0= (u16)map(j, 0, 1023, 80, 5920);//��ӳ���ֵ��ֵ��pwm0
				//pwm0 = j;
					printf("\t pwm0 =%d \t\n",j);	
				//j = (u16)((u32)j * 507 / Bandgap);	//�����ⲿ��ѹ, BandgapΪ5.07V, ���ѹ�ֱ���0.01V
			#endif		
			//==========================================================================
	
			Duty=map(pwm0, 80, 5920, 0, 100);//��pwm0ת��Ϊռ�ձȴ������
			PWMn_Update(PCA1,pwm0);//PCA0--P25,PCA1--P26
			printf("ռ�ձ�:%u%% \r\n",Duty);
			}
			}

	//	printf("pwm0:%u \t pwm1:%u \t pwm2:%u\r\n",pwm0,pwm1,pwm2);	//SUART1����һ���ַ���
	}
}

/********************** Timer0 1ms�жϺ��� ************************/
void timer0 (void) interrupt 1
{
	B_4ms = 1;		//1ms��־
}
