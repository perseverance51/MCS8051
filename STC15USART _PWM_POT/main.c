
/*************	功能说明	**************

双串口全双工中断方式收发通讯程序。
注意：
通过PC向MCU发送数据, MCU收到后通过串口把收到的数据原样返回.

******************************************/
#include	"config.h"
#include "stdio.h"
#include "USART.h"
#include "PWM.h"
#include "ADC.h"

//	#define	Cal_MODE 	0	 //每次测量只读1次ADC. 分辨率0.01V
	#define	Cal_MODE 	1	//每次测量连续读16次ADC 再平均计算. 分辨率0.01V
//typedef 	unsigned char	u8;
//typedef 	unsigned int	u16;
//typedef 	unsigned long	u32;

volatile bit	B_4ms;			//1ms标志

volatile u16	msecond;		//计时
u16	Bandgap;	//

//void  delay_ms(unsigned int ms)
//{
//     unsigned int i;
//	 do{
//	      i = MAIN_Fosc / 13000;
//		  while(--i)	;   //14T per loop
//     }while(--ms);
//}
/********************** 模拟量映射函数 ************************/
long map( long x,  long in_min,  long in_max,  long out_min,  long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//========================================================================
// 函数: void main(void)
// 描述: 主函数。
// 参数: none.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-11-28
// 备注: 
//========================================================================
void main(void)
{
			u8 i;
	int	Duty ;
	int	j;
	
			PCA_Init();	//PCA初始化
//	P0M1 = 0;	P0M0 = 0;	//设置为准双向口
//	P1M1 = 0;	P1M0 = 0;	//设置为准双向口
	P1M1 = 0x00; P1M0 = 0x00; 
	P1M1 |= (1<<3);		// 把ADC口设置为高阻输入
	P1M0 &= ~(1<<3);
	P1ASF = (1<<3);		//P1.3做ADC
	ADC_CONTR = 0xE0;	//90T, ADC power on
//	P2M1 = 0;	P2M0 = 0;	//设置为准双向口
	P2M1 &= ~(0xe0);	//P2.7 P2.6 P2.5 设置为推挽输出
	P2M0 |=  (0xe0);
	P3M1 = 0;	P3M0 = 0;	//设置为准双向口
//	P4M1 = 0;	P4M0 = 0;	//设置为准双向口
//	P5M1 = 0;	P5M0 = 0;	//设置为准双向口
//	P6M1 = 0;	P6M0 = 0;	//设置为准双向口
//	P7M1 = 0;	P7M0 = 0;	//设置为准双向口
		//	S1_USE_P30P31();
	Timer0Init();//定时器0初始化
 InitSerialPort(); 		//初始化串口,波特率9600，8bit数据位，1停止位，无校验
	EA = 1;	//允许总中断

//	PrintString1("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//SUART1发送一个字符串

	while (1)
	{
		P10 = !P10;
		if(pwm0 >= PWM_HIGH_MAX)  pwm0 = PWM_HIGH_MAX;
		
//		if(pwm1 >= PWM_HIGH_MAX)	pwm1 = PWM_HIGH_MAX;
//		PWMn_Update(PCA1,pwm1);

//		if(pwm2 >= PWM_HIGH_MAX)	pwm2 = PWM_HIGH_MAX;
//		PWMn_Update(PCA2,pwm2);
	if(B_4ms)	//5ms到达标志位
		{
			B_4ms = 0;
			if(++msecond >= 250)	//计数到达1S
			{
				msecond = 0;
			#if (Cal_MODE == 0)
			//=================== 只读1次ADC, 10bit ADC. 分辨率0.01V ===============================
				P1ASF = 0;
				Get_ADC10bitResult(1);	//改变P1ASF后先读一次并丢弃结果, 让内部的采样电容的电压等于输入值.
				Bandgap = Get_ADC10bitResult(1);	//读P1.1的电压为基准ADC, P1ASF=0, 0通道的电压值
	
				P1ASF = 1<<3;
				j = Get_ADC10bitResult(3);	//读外部电压ADC
			//	j = (u16)((u32)j * 507 / Bandgap);	//计算外部电压, Bandgap为5.07V, 测电压分辨率0.01V
					printf("\t ADC =%d \t\n",j);	
				pwm0 = (u16)map(j, 0, 1023,80, 5920);//将映射的值赋值给pwm0
				 
				printf("\t pwm0 =%u \t\n",pwm0 );
				#endif
			//==========================================================================

			//===== 连续读16次ADC 再平均计算. 分辨率0.01V =========
			#if (Cal_MODE == 1)
				P1ASF = 0;
				Get_ADC10bitResult(1);	//改变P1ASF后先读一次并丢弃结果, 让内部的采样电容的电压等于输入值.
				for(j=0, i=0; i<16; i++)
				{
					j += Get_ADC10bitResult(1);	//读P11内部基准ADC, P1ASF=0, 读1通道
				}
				Bandgap = j >> 4;	//16次平均
				P1ASF = (1<<3);		//P1.3做ADC
				for(j=0, i=0; i<16; i++)
				{
					j += Get_ADC10bitResult(3);	//读外部电压ADC
				}
				j = j >> 4;	//16次平均
				pwm0= (u16)map(j, 0, 1023, 80, 5920);//将映射的值赋值给pwm0
				//pwm0 = j;
					printf("\t pwm0 =%d \t\n",j);	
				//j = (u16)((u32)j * 507 / Bandgap);	//计算外部电压, Bandgap为5.07V, 测电压分辨率0.01V
			#endif		
			//==========================================================================
	
			Duty=map(pwm0, 80, 5920, 0, 100);//将pwm0转换为占空比串口输出
			PWMn_Update(PCA1,pwm0);//PCA0--P25,PCA1--P26
			printf("占空比:%u%% \r\n",Duty);
			}
			}

	//	printf("pwm0:%u \t pwm1:%u \t pwm2:%u\r\n",pwm0,pwm1,pwm2);	//SUART1发送一个字符串
	}
}

/********************** Timer0 1ms中断函数 ************************/
void timer0 (void) interrupt 1
{
	B_4ms = 1;		//1ms标志
}
