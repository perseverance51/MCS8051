//CCP/PCA���PWM(6λ+7λ+8λ)�Ĳ��Գ���
#include "reg51.h"
#include "intrins.h"

#define FOSC    11059200L

typedef unsigned char BYTE;
typedef unsigned int WORD;

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
sfr P6M1 = 0xCB;
sfr P6M0 = 0xCC;
sfr P7M1 = 0xE1;
sfr P7M0 = 0xE2;

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

void main()
{
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;

    ACC = P_SW1;			//P10 P11 P37 
    ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=0
    P_SW1 = ACC;                    //(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
    
//  ACC = P_SW1;
//  ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=1 CCP_S1=0
//  ACC |= CCP_S0;                  //(P3.4/ECI_2, P3.5/CCP0_2, P3.6/CCP1_2, P3.7/CCP2_2)
//  P_SW1 = ACC;  			//P35 P36 P37
 
//  ACC = P_SW1;
//  ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=1
//  ACC |= CCP_S1;                  //(P2.4/ECI_3, P2.5/CCP0_3, P2.6/CCP1_3, P2.7/CCP2_3)
//  P_SW1 = ACC;  				//P25  P26  P27

    CCON = 0;                       //��ʼ��PCA���ƼĴ���
                                    //PCA��ʱ��ֹͣ
                                    //���CF��־
                                    //���ģ���жϱ�־
    CL = 0;                         //��λPCA�Ĵ���
    CH = 0;
    CMOD = 0x00;                    //����PCAʱ��Դ,f=SYSclk/12/256
                                    //��ֹPCA��ʱ������ж�
    PCA_PWM0 = 0x00;                //PCAģ��0������8λPWM
    CCAP0H = CCAP0L = 0x20;         //PWM0��ռ�ձ�Ϊ87.5% ((100H-20H)/100H)
    CCAPM0 = 0x42;                  //PCAģ��0Ϊ8λPWMģʽ
//P10
		PCA_PWM1 = 0x40;                //PCAģ��1������7λPWM:SYSclk/12/128
    CCAP1H = CCAP1L = 0x20;         //PWM1��ռ�ձ�Ϊ75% ((80H-20H)/80H)
    CCAPM1 = 0x42;                  //PCAģ��1Ϊ7λPWMģʽ
//P37
    PCA_PWM2 = 0x80;                //PCAģ��2������6λPWMf=SYSclk/12/64
    CCAP2H = CCAP2L = 0x20;         //PWM2��ռ�ձ�Ϊ50% ((40H-20H)/40H)
    CCAPM2 = 0x42;                  //PCAģ��2Ϊ6λPWMģʽ

    CR = 1;                         //PCA��ʱ����ʼ����

    while (1);
}
