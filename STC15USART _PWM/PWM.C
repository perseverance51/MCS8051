#include "PWM.h"


volatile u16		CCAP0_tmp,PWM0_high,PWM0_low;
volatile u16		CCAP1_tmp,PWM1_high,PWM1_low;
volatile u16		CCAP2_tmp,PWM2_high,PWM2_low;
volatile u16	pwm0,pwm1,pwm2;

//========================================================================
// 函数: void PWMn_SetHighReg(unsigned int high)
// 描述: 更新占空比数据。
// 参数: high: 	占空比数据，即PWM输出高电平的PCA时钟脉冲个数。
// 返回: 无
// 版本: VER1.0
// 日期: 2013-5-15
// 备注: 
//========================================================================
void PWMn_Update(u8 PCA_id, u16 pwm)
{
	if(pwm > PWM_HIGH_MAX)	pwm = PWM_HIGH_MAX;	//如果写入大于最大占空比数据，强制为最大占空比。
	if(pwm < PWM_HIGH_MIN)	pwm = PWM_HIGH_MIN;	//如果写入小于最小占空比数据，强制为最小占空比。

	if(PCA_id == PCA0)
	{
		CR = 0;						//停止PCA一会， 一般不会影响PWM。
		PWM0_high = pwm;			//数据在正确范围，则装入占空比寄存器。
		PWM0_low = PWM_DUTY - pwm;	//计算并保存PWM输出低电平的PCA时钟脉冲个数。
		CR = 1;						//启动PCA。
	}
	else if(PCA_id == PCA1)
	{
		CR = 0;						//停止PCA。
		PWM1_high = pwm;			//数据在正确范围，则装入占空比寄存器。
		PWM1_low = PWM_DUTY - pwm;	//计算并保存PWM输出低电平的PCA时钟脉冲个数。
		CR = 1;						//启动PCA。
	}
	else if(PCA_id == PCA2)
	{
		CR = 0;						//停止PCA。
		PWM2_high = pwm;			//数据在正确范围，则装入占空比寄存器。
		PWM2_low = PWM_DUTY - pwm;	//计算并保存PWM输出低电平的PCA时钟脉冲个数。
		CR = 1;						//启动PCA。
	}
}

//========================================================================
// 函数: void	PCA_Init(void)
// 描述: PCA初始化程序.
// 参数: none
// 返回: none.
// 版本: V1.0, 2013-11-22
//========================================================================
void	PCA_Init(void)
{
	CR = 0;
	AUXR1 = (AUXR1 & ~(3<<4)) | PCA_P24_P25_P26_P27;	//切换IO口
	CCAPM0 = (PCA_Mode_HighPulseOutput | ENABLE);	//16位软件定时、高速脉冲输出、中断模式
	CCAPM1 = (PCA_Mode_HighPulseOutput | ENABLE);
	CCAPM2 = (PCA_Mode_HighPulseOutput | ENABLE);

	CH = 0;
	CL = 0;
	CMOD  = (CMOD  & ~(7<<1)) | PCA_Clock_1T;			//选择时钟源
	PPCA  = 1;	// 高优先级中断

	pwm0 = (PWM_DUTY / 4 * 1);	//给PWM一个初值
	pwm1 = (PWM_DUTY / 4 * 2);
	pwm2 = (PWM_DUTY / 4 * 3);

	PWMn_Update(PCA0,pwm0);
	PWMn_Update(PCA1,pwm1);
	PWMn_Update(PCA2,pwm2);

	CR    = 1;	// 运行PCA定时器
}
//======================================================================

//========================================================================
// 函数: void	PCA_Handler (void) interrupt 7
// 描述: PCA中断处理程序.
// 参数: None
// 返回: none.
// 版本: V1.0, 2012-11-22
//========================================================================
void	PCA_Handler (void) interrupt 7
{
	if(CCF0)		//PCA模块0中断
	{
		CCF0 = 0;		//清PCA模块0中断标志
		if(P25)	CCAP0_tmp += PWM0_high;	//输出为高电平，则给影射寄存器装载高电平时间长度
		else	CCAP0_tmp += PWM0_low;	//输出为低电平，则给影射寄存器装载低电平时间长度
		CCAP0L = (u8)CCAP0_tmp;			//将影射寄存器写入捕获寄存器，先写CCAP0L
		CCAP0H = (u8)(CCAP0_tmp >> 8);	//后写CCAP0H
	}

	if(CCF1)	//PCA模块1中断
	{
		CCF1 = 0;		//清PCA模块1中断标志
		if(P26)	CCAP1_tmp += PWM1_high;	//输出为高电平，则给影射寄存器装载高电平时间长度
		else	CCAP1_tmp += PWM1_low;	//输出为低电平，则给影射寄存器装载低电平时间长度
		CCAP1L = (u8)CCAP1_tmp;			//将影射寄存器写入捕获寄存器，先写CCAP0L
		CCAP1H = (u8)(CCAP1_tmp >> 8);	//后写CCAP0H
	}

	if(CCF2)	//PCA模块2中断
	{
		CCF2 = 0;		//清PCA模块1中断标志
		if(P27)	CCAP2_tmp += PWM2_high;	//输出为高电平，则给影射寄存器装载高电平时间长度
		else	CCAP2_tmp += PWM2_low;	//输出为低电平，则给影射寄存器装载低电平时间长度
		CCAP2L = (u8)CCAP2_tmp;			//将影射寄存器写入捕获寄存器，先写CCAP0L
		CCAP2H = (u8)(CCAP2_tmp >> 8);	//后写CCAP0H
	}
}