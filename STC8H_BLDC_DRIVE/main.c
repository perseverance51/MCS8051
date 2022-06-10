//测试工作频率为 11.0592MHz
//本例程实现如下功能： 侦测反电动势，驱动无霍尔无刷直流马达运转
//目前使用 MCU 内置单比较器可实现无霍尔 100000 转高速运转，还有提升空间
/******************** include **************************/
#include <STC8.H>

//#include "STC8H8K.h"
/******************** typedef **************************/
typedef unsigned char u8;
typedef unsigned int u16;
/******************** define **************************/
#define TIM1_Period ((u16)200)
#define START 0x1A
#define RUN 0x1B
#define STOP 0x1C
#define IDLE 0x1D
/******************** Variable **************************/
u8 Hall_sta = 0;
u16 Aim_Pulse,Start_Pulse;
u8 Motor_sta = IDLE;
u8 Hall_a=0,Hall_b=0,Hall_c=0;
u8 CMP_ch=0;
u8 BMF_en=0;
u8 BMF=0;
/******************** function **************************/
void DelayXus(u8 delayTime);
void DelayXms(u8 delayTime);
void MOTOR_START(void);
void MOTOR_STOP(void);
u8 RD_BMF(void);
u8 KEY_detect(void);
/******************** main **************************/
void main(void)
{
    P_SW2 = 0x80; //SFR enable
    P0M1 = 0x0F;
    P0M0 = 0x00;
    P1M1 = 0xc1;
    P1M0 = 0x30;
    P2M1 = 0x00;
    P2M0 = 0xff;
    P3M1 = 0xE0;
    P3M0 = 0x13;
    P5M1 = 0x03;
    P5M0 = 0x00;
    RSTCFG = 0x40; //P54 用作 IO
    ADCCFG = 0x0f;
    ADC_CONTR = 0x80;
    CMPCR2 = 0x1f;
    CMPEXCFG = 0x00;
    CMPCR1 = 0xB0;
    PWM_ENO = 0x15; //IO 输出 PWM
    PWM_PS = 0x55;
    TIM1-> PSCRH = 0x00; //预分频寄存器
    TIM1-> PSCRL = 0x02;
    TIM1-> DTR = 0x02; // 死区时间配置
    TIM1-> ARRH = (u8)(TIM1_Period >> 8);
    TIM1-> ARRL = (u8)(TIM1_Period);
    TIM1-> CCMR1 = 0x60; // 通道模式配置
    TIM1-> CCMR2 = 0x60;
    TIM1-> CCMR3 = 0x60;
    TIM1-> CCER1 = 0x11; // 配置通道输出使能和极性
    TIM1-> CCER2 = 0x01; // 配置通道输出使能和极性
    TIM1-> OISR = 0x00; // 配置 MOE=0 时各通道输出电平
    TIM1-> CR1 = 0xA0;
    TIM1-> CR2 = 0x24;
    TIM1-> SMCR = 0x20;
    TIM1-> BKR = 0x0c;
    TIM1-> CR1 |= 0x01; // 使能计数器
    EA = 1;
    Aim_Pulse = 100;
    while (1)
    {
        switch(Motor_sta)
        {
        case START:
            MOTOR_START();
            Motor_sta = RUN;
            break;
        case RUN:
            if(KEY_detect()==1)
                Motor_sta = STOP;
            break;
        case STOP:
            MOTOR_STOP();
            Motor_sta = IDLE;
            break;
        case IDLE:
            if(KEY_detect()==1)
                Motor_sta = START;
            break;
        default:
            break;
        }
    }
}
void CMP_Isr() interrupt 21
{
    CMPCR1 &= ~0x30; //关比较器中断
    if(CMPCR1 & 0x01)
    {
        if((CMPEXCFG&0X03) == 0) {
            Hall_a=1;
        }
        if((CMPEXCFG&0X03) == 1) {
            Hall_b=1;
        }
        if((CMPEXCFG&0X03) == 2) {
            Hall_c=1;
        }
    } else
    {
        if((CMPEXCFG&0X03) == 0) {
            Hall_a=0;
        }
        if((CMPEXCFG&0X03) == 1) {
            Hall_b=0;
        }
        if((CMPEXCFG&0X03) == 2) {
            Hall_c=0;
        }
    }
    BMF= RD_BMF();
    if(BMF_en)
    {
        switch(BMF)
        {
        case 3:
            TIM1-> CCR3H = 0;
            TIM1-> CCR3L = 0;
            TIM1-> CCR1H = (u8)(Aim_Pulse >> 8);
            TIM1-> CCR1L = (u8)(Aim_Pulse);
            TIM1-> CCR2H = 0;
            TIM1-> CCR2L = 0;
            P21=0;
            P23=1;
            P25=0;
            break;
        case 2:
            TIM1-> CCR3H = 0;
            TIM1-> CCR3L = 0;
            TIM1-> CCR1H = (u8)(Aim_Pulse >> 8);
            TIM1-> CCR1L = (u8)(Aim_Pulse);
            TIM1-> CCR2H = 0;
            TIM1-> CCR2L = 0;
            P21=0;
            P23=0;
            P25=1;
            break;
        case 6:
            TIM1-> CCR1H = 0;
            TIM1-> CCR1L = 0;
            TIM1-> CCR2H = (u8)(Aim_Pulse >> 8);
            TIM1-> CCR2L = (u8)(Aim_Pulse);
            TIM1-> CCR3H = 0;
            TIM1-> CCR3L = 0;
            P21=0;
            P23=0;
            P25=1;
            break;
        case 4:
            TIM1-> CCR1H = 0;
            TIM1-> CCR1L = 0;
            TIM1-> CCR2H = (u8)(Aim_Pulse >> 8);
            TIM1-> CCR2L = (u8)(Aim_Pulse);
            TIM1-> CCR3H = 0;
            TIM1-> CCR3L = 0;
            P21=1;
            P23=0;
            P25=0;
            break;
        case 5:
            TIM1-> CCR1H = 0;
            TIM1-> CCR1L = 0;
            TIM1-> CCR2H = 0;
            TIM1-> CCR2L = 0;
            TIM1-> CCR3H = (u8)(Aim_Pulse >> 8);
            TIM1-> CCR3L = (u8)(Aim_Pulse);
            P21=1;
            P23=0;
            P25=0;
            break;
        case 1:
            TIM1-> CCR1H = 0;
            TIM1-> CCR1L = 0;
            TIM1-> CCR2H = 0;
            TIM1-> CCR2L = 0;
            TIM1-> CCR3H = (u8)(Aim_Pulse >> 8);
            TIM1-> CCR3L = (u8)(Aim_Pulse);
            P21=0;
            P23=1;
            P25=0;
            break;
        default :
            break;
        }
    }
    CMP_ch++;
    if(CMP_ch==3) CMP_ch=0;
    CMPEXCFG &= ~0x03;
    CMPEXCFG |= CMP_ch;
    CMPCR1 &= ~0x40; //清比较器中断标志
    CMPCR1 |= 0x30; //开比较器中断
}
u8 RD_BMF()
{
    Hall_sta = 0;
    (Hall_b)? (Hall_sta|=0x01):(Hall_sta&=~0x01);
    (Hall_a)? (Hall_sta|=0x02):(Hall_sta&=~0x02);
    (Hall_c)? (Hall_sta|=0x04):(Hall_sta&=~0x04);
    return Hall_sta;
}
void MOTOR_START()
{
    u16 i,timer=120;
    u8 HA = 3;
    u8 Start_cnt = 0;
    Start_Pulse = 0;
    TIM1-> BKR |= 0x80; // 主输出使能
    while(1)
    {
        for(i=0; i<timer; i++)
        {
            DelayXus(78); //等待
        }
        timer-= timer/25;
        switch(HA)
        {
        case 3:
            TIM1-> CCR1H = (u8)(Start_Pulse >> 8);
            TIM1-> CCR1L = (u8)(Start_Pulse);
            TIM1-> CCR2H = 0;
            TIM1-> CCR2L = 0;
            TIM1-> CCR3H = 0;
            TIM1-> CCR3L = 0;
            P21=0;
            P23=1;
            P25=0;
            HA = 2;
            break;
        case 2:
            TIM1-> CCR1H = (u8)(Start_Pulse >> 8);
            TIM1-> CCR1L = (u8)(Start_Pulse);
            TIM1-> CCR2H = 0;
            TIM1-> CCR2L = 0;
            TIM1-> CCR3H = 0;
            TIM1-> CCR3L = 0;
            P21=0;
            P23=0;
            P25=1;
            HA = 6;
            break;
        case 6:
            TIM1-> CCR1H = 0;
            TIM1-> CCR1L = 0;
            TIM1-> CCR2H = (u8)(Start_Pulse >> 8);
            TIM1-> CCR2L = (u8)(Start_Pulse);
            TIM1-> CCR3H = 0;
            TIM1-> CCR3L = 0;
            P21=0;
            P23=0;
            P25=1;
            HA = 4;
            break;
        case 4:
            TIM1-> CCR1H = 0;
            TIM1-> CCR1L = 0;
            TIM1-> CCR2H = (u8)(Start_Pulse >> 8);
            TIM1-> CCR2L = (u8)(Start_Pulse);
            TIM1-> CCR3H = 0;
            TIM1-> CCR3L = 0;
            P21=1;
            P23=0;
            P25=0;
            HA = 5;
            break;
        case 5:
            TIM1-> CCR1H = 0;
            TIM1-> CCR1L = 0;
            TIM1-> CCR2H = 0;
            TIM1-> CCR2L = 0;
            TIM1-> CCR3H = (u8)(Start_Pulse >> 8);
            TIM1-> CCR3L = (u8)(Start_Pulse);
            P21=1;
            P23=0;
            P25=0;
            HA = 1;
            break;
        case 1:
            TIM1-> CCR1H = 0;
            TIM1-> CCR1L = 0;
            TIM1-> CCR2H = 0;
            TIM1-> CCR2L = 0;
            TIM1-> CCR3H = (u8)(Start_Pulse >> 8);
            TIM1-> CCR3L = (u8)(Start_Pulse);
            P21=0;
            P23=1;
            P25=0;
            HA = 3;
            break;
        default :
            break;
        }
        if(++Start_cnt>80)
        {
            BMF_en=1;
            break;
        }
        else
        {
            if(Start_Pulse<Aim_Pulse) Start_Pulse += 4;
            else Start_Pulse = Aim_Pulse;
        }
    }
}
void MOTOR_STOP()
{
    BMF_en = 0;
    TIM1-> BKR &= ~0x80; //关比较器中断
    P21=0;
    P23=0;
    P25=0;
}
u8 KEY_detect()
{
    if(!P05)
    {
        DelayXms(30);
        if(!P05)
        {
            while(!P05);
            return 1;
        }
    }
    return 0;
}
void DelayXus(u8 delayTime)
{
    int i = 0;
    while( delayTime-- )
    {
        for( i = 0 ; i < 1 ; i++);
    }
}
void DelayXms( u8 delayTime )
{
    int i = 0;
    while( delayTime-- )
    {
        for( i = 0 ; i < 2 ; i++ )
        {
            DelayXus(100);
        }
    }
}