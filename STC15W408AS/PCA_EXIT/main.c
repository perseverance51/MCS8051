//��CCP/PCA������չ�ⲿ�жϵĲ��Գ���
//��Ƭ���ͺţ�STC15W408AS
//����Ƶ��һ��Ϊ11.0592MHz


#include "reg51.h"
#include "intrins.h"

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
sfr PCAPWM0     =   0xf2;
sfr PCAPWM1     =   0xf3;

sbit PCA_LED    =   P1^0;           //PCA����LED

void PCA_isr() interrupt 7
{
    CCF0 = 0;                       //���жϱ�־
    PCA_LED = !PCA_LED;             //����LEDȡ��
}

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
    P6M0 = 0x00;
    P6M1 = 0x00;
    P7M0 = 0x00;
    P7M1 = 0x00;

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
    CMOD = 0x00;                    //����PCAʱ��Դ
                                    //��ֹPCA��ʱ������ж�
    CCAPM0 = 0x11;                  //PCAģ��0Ϊ�½��ش���0001 0001
//  CCAPM0 = 0x21;                  //PCAģ��0Ϊ�������ش���0010 0001
//  CCAPM0 = 0x31;                  //PCAģ��0Ϊ������/�½��ش���0011 0001

    CR = 1;                         //PCA��ʱ����ʼ����
    EA = 1;

    while (1);
}

