#ifndef _PWM_H_
#define _PWM_H_
#include	"config.h"


/***************************用户宏定义*******************************************************/
//#define 	MAIN_Fosc		16000000UL	//定义主时钟

#define		PWM_DUTY		8000		//定义PWM的周期，数值为PCA所选择的时钟脉冲个数。
#define		PWM_HIGH_MIN	80			//限制PWM输出的最小占空比, 避免中断里重装参数时间不够。
#define		PWM_HIGH_MAX	(PWM_DUTY - PWM_HIGH_MIN)		//限制PWM输出的最大占空比。

/********************************************************************************************/
#define	PCA0			0
#define	PCA1			1
#define	PCA2			2
#define	PCA_Counter		3
#define	PCA_P12_P11_P10_P37	(0<<4)
#define	PCA_P34_P35_P36_P37	(1<<4)
#define	PCA_P24_P25_P26_P27	(2<<4)
#define	PCA_Mode_PWM				0x42
#define	PCA_Mode_Capture			0
#define	PCA_Mode_SoftTimer			0x48
#define	PCA_Mode_HighPulseOutput	0x4c
#define	PCA_Clock_1T	(4<<1)
#define	PCA_Clock_2T	(1<<1)
#define	PCA_Clock_4T	(5<<1)
#define	PCA_Clock_6T	(6<<1)
#define	PCA_Clock_8T	(7<<1)
#define	PCA_Clock_12T	(0<<1)
#define	PCA_Clock_Timer0_OF	(2<<1)
#define	PCA_Clock_ECI	(3<<1)
#define	PCA_Rise_Active	(1<<5)
#define	PCA_Fall_Active	(1<<4)
#define	PCA_PWM_8bit	(0<<6)
#define	PCA_PWM_7bit	(1<<6)
#define	PCA_PWM_6bit	(2<<6)

#define		ENABLE		1
#define		DISABLE		0

extern u16		CCAP0_tmp,PWM0_high,PWM0_low;
extern u16		CCAP1_tmp,PWM1_high,PWM1_low;
extern u16		CCAP2_tmp,PWM2_high,PWM2_low;

extern u16	pwm0,pwm1,pwm2;

void PWMn_Update(u8 PCA_id, u16 pwm);
void	PCA_Init(void);






#endif