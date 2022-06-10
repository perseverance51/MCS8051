//��CCP/PCA��16λ����ģʽ�������ȵĳ���
//��Ƭ���ͺţ�STC15W408AS
//����Ƶ��һ��Ϊ11.0592MHz

#include "reg51.h"
#include "intrins.h"
#include <stdio.h> 	       // Ϊʹ��KEIL�Դ��Ŀ⺯��printf������
#define FOSC    11059200L

typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long DWORD;
BYTE cnt;                           //�洢PCA��ʱ�������
DWORD count0;                       //��¼��һ�εĲ���ֵ
DWORD count1;                       //��¼���εĲ���ֵ
DWORD length;                       //�洢�źŵ�ʱ�䳤��(count1 - count0)

sfr P0M1 = 0x93;
sfr P0M0 = 0x94;
sfr P1M1 = 0x91;
sfr P1M0 = 0x92;
sfr P2M1 = 0x95;
sfr P2M0 = 0x96;
sfr P3M1 = 0xb1;
sfr P3M0 = 0xb2;
sfr P4M1 = 0xb3;
sfr P4M0 = 0xb4;
sfr P5M1 = 0xC9;
sfr P5M0 = 0xCA;

sfr P_SW1       = 0xA2;             //���蹦���л��Ĵ���1

#define CCP_S0 0x10                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5

sfr CCON        =   0xD8;           //PCA���ƼĴ���
sbit CCF0       =   CCON^0;         //PCAģ��0�жϱ�־
sbit CCF1       =   CCON^1;         //PCAģ��1�жϱ�־
sbit CR         =   CCON^6;         //PCA��ʱ�����п���λ
sbit CF         =   CCON^7;         //PCA��ʱ�������־
sfr CMOD        =   0xD9;           //PCAģʽ�Ĵ���
sfr CL          =   0xE9;           //PCA��ʱ�����ֽ�
sfr CH          =   0xF9;           //PCA��ʱ�����ֽ�
sfr CCAPM0      =   0xDA;           //PCAģ��0ģʽ�Ĵ���
sfr CCAP0L      =   0xEA;           //PCAģ��0����Ĵ��� LOW
sfr CCAP0H      =   0xFA;           //PCAģ��0����Ĵ��� HIGH
sfr CCAPM1      =   0xDB;           //PCAģ��1ģʽ�Ĵ���
sfr CCAP1L      =   0xEB;           //PCAģ��1����Ĵ��� LOW
sfr CCAP1H      =   0xFB;           //PCAģ��1����Ĵ��� HIGH
sfr CCAPM2      =   0xDC;           //PCAģ��2ģʽ�Ĵ���
sfr CCAP2L      =   0xEC;           //PCAģ��2����Ĵ��� LOW
sfr CCAP2H      =   0xFC;           //PCAģ��2����Ĵ��� HIGH
sfr PCA_PWM0    =   0xf2;           //PCAģ��0��PWM�Ĵ���
sfr PCA_PWM1    =   0xf3;           //PCAģ��1��PWM�Ĵ���
sfr PCA_PWM2    =   0xf4;           //PCAģ��2��PWM�Ĵ���

sfr AUXR        =   0x8E;   //0000,0000 �����Ĵ���
sfr T2H         =   0xD6;   //0000,0000 T2���ֽ�
sfr T2L         =   0xD7;   //0000,0000 T2���ֽ�
sbit EPCA       =   IE^6;           //PCA�ж�����λ

/*------------------------------------------------
                 ���ڳ�ʼ��������������9600
------------------------------------------------*/
 void UART_init(void)
{
		SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR &= 0xFB;		//��ʱ��ʱ��12Tģʽ
	T2L = 0xE8;		//���ö�ʱ��ʼֵ
	T2H = 0xFF;		//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	TI=1;			// �ܹؼ���ʹ��printf����ʱ�����д�����
	ES = 1; //���������ж�
	EA=1;			   //�������ж�
}
//UART1 ���ʹ�������
void UART1_SendData(char dat)
{
    ES=0;           //�ش����ж�
    SBUF=dat;           
    while(TI!=1);   //�ȴ����ͳɹ�
    TI=0;           //��������жϱ�־
    ES=1;           //�������ж�
}
 
//UART1 �����ַ���reentrant
//void UART1_SendString(char *s)
//{
//    while(*s)//����ַ���������
//    {
//        UART1_SendData(*s++);//���͵�ǰ�ַ�
//    }
//}
 
//��дputchar����
char putchar(char c) 
{
    UART1_SendData(c);
    return c;
}

void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 22;
	j = 3;
	k = 227;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void main()
{
		P0M1 = 0x03; 
		P0M0 = 0x00; //P0(˫˫˫˫˫˫�߸�)
//    P1M0 = 0x00;
//    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;
    ACC = P_SW1;
    ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=0
    P_SW1 = ACC;                    //(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
    
//  ACC = P_SW1;
//  ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=1 CCP_S1=0
//  ACC |= CCP_S0;                  //(P3.4/ECI_2, P3.5/CCP0_2, P3.6/CCP1_2, P3.7/CCP2_2)
//  P_SW1 = ACC;  
//  
//  ACC = P_SW1;
//  ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=1
//  ACC |= CCP_S1;                  //(P2.4/ECI_3, P2.5/CCP0_3, P2.6/CCP1_3, P2.7/CCP2_3)
//  P_SW1 = ACC;  

    CCON = 0;                       //��ʼ��PCA���ƼĴ���
                                    //PCA��ʱ��ֹͣ
                                    //���CF��־
                                    //���ģ���жϱ�־
    CL = 0;                         //��λPCA�Ĵ���
    CH = 0;
    CCAP0L = 0;
    CCAP0H = 0;
//    CMOD = 0x09;                    //����PCAʱ��ԴΪϵͳʱ��SYSclk/4,��ʹ��PCA��ʱ����ж�
		CMOD = 0x01;    //SYSclk/12
    CCAPM0 = 0x21;                  //PCAģ��0Ϊ16λ����ģʽ(�����ز���,�ɲ�Ӹߵ�ƽ��ʼ����������),�Ҳ��������ж�
//  CCAPM0 = 0x11;                  //PCAģ��0Ϊ16λ����ģʽ(�½��ز���,�ɲ�ӵ͵�ƽ��ʼ����������),�Ҳ��������ж�
 // CCAPM0 = 0x31;                  //PCAģ��0Ϊ16λ����ģʽ(������/�½��ز���,�ɲ�ߵ�ƽ���ߵ͵�ƽ���),�Ҳ��������ж�

    CR = 1;                         //PCA��ʱ����ʼ����
    EA = 1;

    cnt = 0;
    count0 = 0;
    count1 = 0;
		UART_init();
//	printf("STC15W408AS \n");
//	printf("TRG!!!\n");
    while (1){
//			if(length > 0){
//				CCAPM0 = 0x00; //�رձȽϹ���
//			printf("Pulse_length: %lu\n",length);
//			length = 0 ;
//			Delay500ms();
//			CCAPM0 = 0x21; //�����ȽϹ���
//			}
		}
}

void PCA_isr() interrupt 7
{	
 if (CF)
 {
 CF = 0;
 cnt++; //PCA��ʱ�������+1
 }
 if (CCF0)
 {
 CCF0 = 0;
 count0 = count1; //������һ�εĲ���ֵ
 ((BYTE *)&count1)[3] = CCAP0L; //���汾�εĲ���ֵ
 ((BYTE *)&count1)[2] = CCAP0H;
 ((BYTE *)&count1)[1] = cnt;
 ((BYTE *)&count1)[0] = 0;
 length = count1 - count0; //�������β���Ĳ�ֵ,���õ�ʱ�䳤��
	printf("Pulse_length: %lu\n",length);
 }

}