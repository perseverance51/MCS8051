//用CCP/PCA功能实现16位定时器的测试程序
//单片机型号：STC15W408AS
//工作频率一般为11.0592MHz

#include "reg51.h"
#include "intrins.h"

#define FOSC    11059200L
#define T100Hz  (FOSC / 12 / 100)

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


sfr P_SW1       = 0xA2;             //外设功能切换寄存器1

#define CCP_S0 0x10                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5

sfr CCON        =   0xD8;           //PCA控制寄存器
sbit CCF0       =   CCON^0;         //PCA模块0中断标志
sbit CCF1       =   CCON^1;         //PCA模块1中断标志
sbit CR         =   CCON^6;         //PCA定时器运行控制位
sbit CF         =   CCON^7;         //PCA定时器溢出标志
sfr CMOD        =   0xD9;           //PCA模式寄存器
sfr CL          =   0xE9;           //PCA定时器低字节
sfr CH          =   0xF9;           //PCA定时器高字节
sfr CCAPM0      =   0xDA;           //PCA模块0模式寄存器
sfr CCAP0L      =   0xEA;           //PCA模块0捕获寄存器 LOW
sfr CCAP0H      =   0xFA;           //PCA模块0捕获寄存器 HIGH
sfr CCAPM1      =   0xDB;           //PCA模块1模式寄存器
sfr CCAP1L      =   0xEB;           //PCA模块1捕获寄存器 LOW
sfr CCAP1H      =   0xFB;           //PCA模块1捕获寄存器 HIGH
sfr PCAPWM0     =   0xf2;
sfr PCAPWM1     =   0xf3;

sbit PCA_LED    =   P1^0;           //PCA测试LED

BYTE cnt;
WORD value;

void PCA_isr() interrupt 7		//PCA中断
{
    CCF0 = 0;                       //清中断标志
    CCAP0L = value;	
    CCAP0H = value >> 8;            //更新比较值
    value += T100Hz;	//(11059200 / 12 / 100) = 96
    if (cnt-- == 0)
    {
        cnt = 100;                  //记数100次
        PCA_LED = !PCA_LED;         //每秒闪烁一次
    }
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

    CCON = 0;                       //初始化PCA控制寄存器
                                    //PCA定时器停止
                                    //清除CF标志
                                    //清除模块中断标志
    CL = 0;                         //复位PCA寄存器
    CH = 0;
    CMOD = 0x00;                    //设置PCA时钟源，SYSclk/12
                                    //禁止PCA定时器溢出中断
    value = T100Hz;			//(FOSC / 12 / 100)
    CCAP0L = value;
    CCAP0H = value >> 8;            //初始化PCA模块0
		
    value += T100Hz;
    CCAPM0 = 0x49;                  //允许比较器功能，使能比较CCF0中断。0100 1001

    CR = 1;                         //PCA定时器开始工作
    EA = 1;
    cnt = 0;

    while (1);
}