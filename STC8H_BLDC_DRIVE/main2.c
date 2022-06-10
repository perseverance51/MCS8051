
//BLDC无刷直流电机驱动(无HALL)
//测试工作频率为11.0592MHz
//本例程实现如下功能: 通过3组PWM通道控制无霍尔马达运转
//本例程仅适用57BL02马达在24V无负载条件下演示

#include "stc8h.h"
#include "intrins.h"

typedef unsigned char   u8;
typedef unsigned int    u16;

typedef struct TIM_struct
{
    volatile unsigned char CR1;     /*!< control register 1 */
    volatile unsigned char CR2;     /*!< control register 2 */
    volatile unsigned char SMCR;    /*!< Synchro mode control register */
    volatile unsigned char ETR;     /*!< external trigger register */
    volatile unsigned char IER;     /*!< interrupt enable register*/
    volatile unsigned char SR1;     /*!< status register 1 */
    volatile unsigned char SR2;     /*!< status register 2 */
    volatile unsigned char EGR;     /*!< event generation register */
    volatile unsigned char CCMR1;   /*!< CC mode register 1 */
    volatile unsigned char CCMR2;   /*!< CC mode register 2 */
    volatile unsigned char CCMR3;   /*!< CC mode register 3 */
    volatile unsigned char CCMR4;   /*!< CC mode register 4 */
    volatile unsigned char CCER1;   /*!< CC enable register 1 */
    volatile unsigned char CCER2;   /*!< CC enable register 2 */
    volatile unsigned char CNTRH;   /*!< counter high */
    volatile unsigned char CNTRL;   /*!< counter low */
    volatile unsigned char PSCRH;   /*!< prescaler high */
    volatile unsigned char PSCRL;   /*!< prescaler low */
    volatile unsigned char ARRH;    /*!< auto-reload register high */
    volatile unsigned char ARRL;    /*!< auto-reload register low */
    volatile unsigned char RCR;     /*!< Repetition Counter register */
    volatile unsigned char CCR1H;   /*!< capture/compare register 1 high */
    volatile unsigned char CCR1L;   /*!< capture/compare register 1 low */
    volatile unsigned char CCR2H;   /*!< capture/compare register 2 high */
    volatile unsigned char CCR2L;   /*!< capture/compare register 2 low */
    volatile unsigned char CCR3H;   /*!< capture/compare register 3 high */
    volatile unsigned char CCR3L;   /*!< capture/compare register 3 low */
    volatile unsigned char CCR4H;   /*!< capture/compare register 3 high */
    volatile unsigned char CCR4L;   /*!< capture/compare register 3 low */
    volatile unsigned char BKR;     /*!< Break Register */
    volatile unsigned char DTR;     /*!< dead-time register */
    volatile unsigned char OISR;    /*!< Output idle register */
}TIM_TypeDef;

TIM_TypeDef *TIM1,*TIM2;

#define TIM1_BaseAddress    0xFEC0
#define TIM2_BaseAddress    0xFEE0

#define TRUE        1
#define FALSE       0

#define RV09_CH     6

#define TIM1_Period     ((u16)280)
#define TIM1_STPulse    ((u16)245)

#define START       0x1A
#define RUN         0x1B
#define STOP        0x1C
#define IDLE        0x1D

#define TIM1_OCMODE_MASK        ((u8)0x70)
#define TIM1_OCCE_ENABLE        ((u8)0x80)
#define TIM1_OCCE_DISABLE       ((u8)0x00)
#define TIM1_OCMODE_TIMING      ((u8)0x00)
#define TIM1_OCMODE_ACTIVE      ((u8)0x10)
#define TIM1_OCMODE_INACTIVE    ((u8)0x20)
#define TIM1_OCMODE_TOGGLE      ((u8)0x30)
#define TIM1_FORCE_INACTIVE     ((u8)0x40)
#define TIM1_FORCE_ACTIVE       ((u8)0x50)
#define TIM1_OCMODE_PWMA        ((u8)0x60)
#define TIM1_OCMODE_PWMB        ((u8)0x70)
#define CC1_POLARITY_HIGH       ((u8)0x02)
#define CC1N_POLARITY_HIGH      ((u8)0x08)
#define CC2_POLARITY_HIGH       ((u8)0x20)
#define CC2N_POLARITY_HIGH      ((u8)0x80)
#define CC1_POLARITY_LOW        ((u8)~0x02)
#define CC1N_POLARITY_LOW       ((u8)~0x08)
#define CC2_POLARITY_LOW        ((u8)~0x20)
#define CC2N_POLARITY_LOW       ((u8)~0x80)
#define CC1_OCENABLE            ((u8)0x01)
#define CC1N_OCENABLE           ((u8)0x04)
#define CC2_OCENABLE            ((u8)0x10)
#define CC2N_OCENABLE           ((u8)0x40)
#define CC1_OCDISABLE           ((u8)~0x01)
#define CC1N_OCDISABLE          ((u8)~0x04)
#define CC2_OCDISABLE           ((u8)~0x10)
#define CC2N_OCDISABLE          ((u8)~0x40)
#define CC3_POLARITY_HIGH       ((u8)0x02)
#define CC3N_POLARITY_HIGH      ((u8)0x08)
#define CC4_POLARITY_HIGH       ((u8)0x20)
#define CC4N_POLARITY_HIGH      ((u8)0x80)
#define CC3_POLARITY_LOW        ((u8)~0x02)
#define CC3N_POLARITY_LOW       ((u8)~0x08)
#define CC4_POLARITY_LOW        ((u8)~0x20)
#define CC4N_POLARITY_LOW       ((u8)~0x80)
#define CC3_OCENABLE            ((u8)0x01)
#define CC3N_OCENABLE           ((u8)0x04)
#define CC4_OCENABLE            ((u8)0x10)
#define CC4N_OCENABLE           ((u8)0x40)
#define CC3_OCDISABLE           ((u8)~0x01)
#define CC3N_OCDISABLE          ((u8)~0x04)
#define CC4_OCDISABLE           ((u8)~0x10)
#define CC4N_OCDISABLE          ((u8)~0x40)

void UART_INIT();
void DelayXus(unsigned char delayTime);
void DelayXms( unsigned char delayTime);
unsigned int ADC_Convert(u8 ch);
void PWM_Init(void);
void SPEED_ADJ();
unsigned char RD_HALL();
void MOTOR_START();
void MOTOR_STOP();
unsigned char KEY_detect();

unsigned char Timer0_cnt=0xb0;
unsigned int HA=0;
unsigned int Motor_speed;
unsigned char Motor_sta = IDLE;
unsigned char BRK_occur=0;
unsigned int TIM2_CAP1_v=0;
unsigned int CAP1_avg=0;
unsigned char CAP1_cnt=0;
unsigned long CAP1_sum=0;

void main(void)
{
	 	
    unsigned int temp=0;
    unsigned int ADC_result=0;
	
    P_SW2= 0x80;
    P1 = 0x00;
    P0M1 = 0x0C;
    P0M0 = 0x01;
    P1M1 = 0xc0;
    P1M0 = 0x3F;
    P2M1 = 0x00;
    P2M0 = 0x38;
    P3M1 = 0x88;
    P3M0 = 0x02;

    ET0=1;
    TR0=0;
    ADCCFG = 0x0f;
    ADC_CONTR = 0x80;

    PWMA_ENO = 0x3F;                            //PWMA输出使能
    PWMB_ENO = 0x00;                            //PWMB输出使能
    PWMA_PS  = 0x00;                            //PWMA pin 选择
    PWMB_PS  = 0xD5;                            //PWMB pin 选择

    /**********************************************************
    输出比较模式 PWMx_duty = [CCRx/(ARR + 1)]*100
    ***********************************************************/
    /************PWMB  BMF输入 ****************/
    //////////// 时基单元 ////////////

    TIM2-> PSCRL = 15;
    TIM2-> ARRH = 0xff;                         //自动重装载寄存器，计数器 overflow 点
    TIM2-> ARRL = 0xff;
    TIM2-> CCR4H = 0x00;
    TIM2-> CCR4L = 0x05;
    //////////// 通道配置 ////////////
    TIM2-> CCMR1 = 0xf3;                        //通道模式配置
    TIM2-> CCMR2 = 0xf1;
    TIM2-> CCMR3 = 0xf1;
    TIM2-> CCMR4 = 0x70;
    TIM2-> CCER1 = 0x11;
    TIM2-> CCER2 = 0x11;
    //////////// 模式配置 ////////////
    TIM2-> CR2 = 0xf0;
    TIM2-> CR1 = 0x81;
    TIM2-> SMCR = 0x44;
    //////////// 使能 & 中断配置 ////////////
    TIM2-> BKR = 0x80;                          //主输出使能
    TIM2-> IER = 0x02;                          //使能中断
    /************PWMA 控制马达换相 ****************/
    //////////// 时基单元 ////////////
    TIM1-> PSCRH = 0x00;                        //预分频寄存器
    TIM1-> PSCRL = 0x00;
    TIM1-> ARRH = (u8)(TIM1_Period >> 8);
    TIM1-> ARRL = (u8)(TIM1_Period);
    //////////// 通道配置 ////////////
    TIM1-> CCMR1 = 0x70;                        //通道模式配置
    TIM1-> CCMR2 = 0x70;
    TIM1-> CCMR3 = 0x70;
    TIM1-> CCER1 = 0x11;                        //配置通道输出使能和极性
    TIM1-> CCER2 = 0x01;                        //配置通道输出使能和极性
    TIM1-> OISR = 0xAA;                         //配置 MOE=0 时各通道输出电平
    //////////// 模式配置 ////////////
    TIM1-> CR1 = 0xA0;
    TIM1-> CR2 = 0x24;
    TIM1-> SMCR = 0x20;
    TIM1-> BKR = 0x0c;
    //////////// 使能 & 中断配置 ////////////
    TIM1-> CR1 |= 0x01;                         //使能计数器
    EA = 1;

    UART_INIT();

    while (1)
    {
        switch(Motor_sta)
        {
            case START:
                MOTOR_START();
                Motor_sta = RUN;
                for(temp = TIM1_STPulse; temp > ADC_result; temp--) //开环启动
                {
                    ADC_result = (ADC_Convert(RV09_CH)/4);
                    TIM1-> CCR1H = (u8)(temp >> 8);
                    TIM1-> CCR1L = (u8)(temp);
                    TIM1-> CCR2H = (u8)(temp >> 8);
                    TIM1-> CCR2L = (u8)(temp);
                    TIM1-> CCR3H = (u8)(temp >> 8);
                    TIM1-> CCR3L = (u8)(temp);
                    DelayXms(10);
                }
                break;
            case RUN:
                SPEED_ADJ();    //马达调速
                if((BRK_occur == TRUE))
                Motor_sta = STOP;
                break;
            case STOP:
                MOTOR_STOP();
                Motor_sta = IDLE;
                break;
            case IDLE:
                if(KEY_detect()==1)
                Motor_sta = START;              //启动马达
                BRK_occur = FALSE;
                Motor_speed = 0;
                CAP1_avg = 0;
                CAP1_cnt = 0;
                CAP1_sum = 0;
                break;
        }
    }
}

void TIM0_ISR() interrupt 1
{
    if(Motor_sta == START)
    {
        if(Timer0_cnt<0xe0) Timer0_cnt++;
        TH0=Timer0_cnt;

        switch(HA%6)
        {
        case 0:
            TIM1-> CCMR3 &= ~TIM1_OCMODE_MASK;
            TIM1-> CCMR3 |= TIM1_FORCE_INACTIVE;
            TIM1-> CCMR1 &= ~TIM1_OCMODE_MASK;
            TIM1-> CCMR1 |= TIM1_OCMODE_PWMB;
            break;
        case 1:
            TIM1-> CCER1 &= CC2N_POLARITY_LOW;
            TIM1-> CCER2 |= CC3N_POLARITY_HIGH;
             break;
        case 2:
            TIM1-> CCMR1 &= ~TIM1_OCMODE_MASK;
            TIM1-> CCMR1 |= TIM1_FORCE_INACTIVE;
            TIM1-> CCMR2 &= ~TIM1_OCMODE_MASK;
            TIM1-> CCMR2 |= TIM1_OCMODE_PWMB;
            break;
        case 3:
            TIM1-> CCER1 |= CC1N_POLARITY_HIGH;
            TIM1-> CCER2 &= CC3N_POLARITY_LOW;
            break;
        case 4:
            TIM1-> CCMR2 &= ~TIM1_OCMODE_MASK;
            TIM1-> CCMR2 |= TIM1_FORCE_INACTIVE;
            TIM1-> CCMR3 &= ~TIM1_OCMODE_MASK;
            TIM1-> CCMR3 |= TIM1_OCMODE_PWMB;
            break;
        case 5:
            TIM1-> CCER1 &= CC1N_POLARITY_LOW;
            TIM1-> CCER1 |= CC2N_POLARITY_HIGH;
            break;
        }
        HA++;
    }

    if(Motor_sta == RUN)
    {
        TR0=0;
        switch(RD_HALL())
        {
        case 3:
            TIM1-> CCMR3 &= ~TIM1_OCMODE_MASK;
            TIM1-> CCMR3 |= TIM1_FORCE_INACTIVE;
            TIM1-> CCMR1 &= ~TIM1_OCMODE_MASK;
            TIM1-> CCMR1 |= TIM1_OCMODE_PWMB;
            break;
        case 1:
            TIM1-> CCER1 &= CC2N_POLARITY_LOW;
            TIM1-> CCER2 |= CC3N_POLARITY_HIGH;
            break;
        case 5:
            TIM1-> CCMR1 &= ~TIM1_OCMODE_MASK;
            TIM1-> CCMR1 |= TIM1_FORCE_INACTIVE;
            TIM1-> CCMR2 &= ~TIM1_OCMODE_MASK;
            TIM1-> CCMR2 |= TIM1_OCMODE_PWMB;
            break;
        case 4:
            TIM1-> CCER1 |= CC1N_POLARITY_HIGH;
            TIM1-> CCER2 &= CC3N_POLARITY_LOW;
            break;
        case 6:
            TIM1-> CCMR2 &= ~TIM1_OCMODE_MASK;
            TIM1-> CCMR2 |= TIM1_FORCE_INACTIVE;
            TIM1-> CCMR3 &= ~TIM1_OCMODE_MASK;
            TIM1-> CCMR3 |= TIM1_OCMODE_PWMB;
            break;
        case 2:
            TIM1-> CCER1 &= CC1N_POLARITY_LOW;
            TIM1-> CCER1 |= CC2N_POLARITY_HIGH;
            break;
        }
    }
}

void PWMA_ISR() interrupt 26
{
    if((TIM1->SR1 & 0x20))
    {
        P00=0;
        CAP1_sum += TIM2_CAP1_v;
        CAP1_cnt++;
        if(CAP1_cnt==128)
        {
            CAP1_cnt=0;
            CAP1_avg = (CAP1_sum>>7);
            CAP1_sum = 0;
            Motor_speed = 5000000/CAP1_avg;
        }
        TIM1->SR1 &=~0x20;                      //清零
    }
    if((TIM1->SR1 & 0x80))                      //BRK
    {
        BRK_occur = TRUE;
        TIM1->SR1 &=~0x80;                      //清零
    }
}

void PWMB_ISR() interrupt 27
{
    unsigned char ccr_tmp=0;

    if((TIM2->SR1 & 0X02))
    {
        ccr_tmp = TIM2-> CCR1H;
        if(ccr_tmp>1)                           //软件滤波
        {
            TIM2_CAP1_v = ccr_tmp;
            TIM2_CAP1_v = (TIM2_CAP1_v<<8) + TIM2->CCR1L;
            if(Motor_sta == RUN)                //换向delay计时
            {
                TR0=1;
                TH0 = 256-(TIM2_CAP1_v>>9);
            }
        }
        TIM2->SR1 &=~0X02;
    }
}

void UART_INIT()
{
    SCON = 0x50;                                //8位可变波特率
    AUXR = 0x40;                                //定时器1为1T模式
    TMOD = 0x20;                                //定时器1为模式0(16位自动重载)

    TL1 = 254;
    TH1 = 254;
//  ET1 = 0;
    TR1 = 1;
}

void DelayXus(unsigned char delayTime)
{
    int i = 0;
    while( delayTime--)
    {
        for( i = 0 ; i < 1 ; i++);
    }
}

void DelayXms( unsigned char delayTime )
{
    int i = 0;
    while( delayTime--)
    {
        for( i = 0 ; i < 2 ; i++)
        {
            DelayXus(100);
        }
    }
}

unsigned int ADC_Convert(u8 ch)
{
    u16 res=0;

    ADC_CONTR &= ~0x0f;
    ADC_CONTR |= ch;
    ADC_CONTR |= 0x40;
    DelayXus(1);
    while (!(ADC_CONTR & 0x20));
    ADC_CONTR &= ~0x20;

    res = ADC_RES;
    res = (res<<2)+(ADC_RESL>>6);

    if (res < 360) res=360;
    if (res > 900) res=900;

    return res;
}

void SPEED_ADJ()
{
    u16 ADC_result;

    ADC_result = (ADC_Convert(RV09_CH)/4);      //调速旋钮ADC采样
    TIM1-> CCR1H = (u8)(ADC_result >> 8);       //计数器比较值
    TIM1-> CCR1L = (u8)(ADC_result);
    TIM1-> CCR2H = (u8)(ADC_result >> 8);
    TIM1-> CCR2L = (u8)(ADC_result);
    TIM1-> CCR3H = (u8)(ADC_result >> 8);
    TIM1-> CCR3L = (u8)(ADC_result);
}

unsigned char RD_HALL()                         //读霍尔传感器
{
    unsigned char Hall_sta = 0;

    DelayXus(40);
    (P17)? (Hall_sta|=0x01) : (Hall_sta&=~0x01);
    (P54)? (Hall_sta|=0x02) : (Hall_sta&=~0x02);
    (P33)? (Hall_sta|=0x04) : (Hall_sta&=~0x04);

    return Hall_sta;
}

void MOTOR_START()
{
    TIM1-> CCR1H = (u8)(TIM1_STPulse >> 8);     //计数器比较值
    TIM1-> CCR1L = (u8)(TIM1_STPulse);
    TIM1-> CCR2H = (u8)(TIM1_STPulse >> 8);
    TIM1-> CCR2L = (u8)(TIM1_STPulse);
    TIM1-> CCR3H = (u8)(TIM1_STPulse >> 8);
    TIM1-> CCR3L = (u8)(TIM1_STPulse);
    TIM1-> BKR |= 0x80;                         //主输出使能相当于总开关
    TIM1-> IER = 0x00;                          //使能中断
    TR0 = 1;

    while (HA < 6*20);

    TIM1-> IER = 0xa0;                          //使能中断
}

void MOTOR_STOP()
{
    TIM1-> BKR &= ~0x80;
    TIM1-> IER &= ~0x20;
}

unsigned char KEY_detect()
{
    if(!P37)
    {
        DelayXms(10);
        if(!P37)
        {
            return 1;
        }
        else return 0;
    }
    else if(!P03)
    {
        DelayXms(10);
        if(!P03)
        {
            return 2;
        }
        else return 0;
    }
    else return 0;
}
