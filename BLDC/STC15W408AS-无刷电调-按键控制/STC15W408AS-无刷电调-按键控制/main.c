
/*---------------------------------------------------------------------*/
/* --- STC MCU International Limited ----------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: 13922805190 --------------------------------------------*/
/* --- Fax: 0513-55012956,55012947,55012969 ---------------------------*/
/* --- Tel: 0513-55012928,55012929,55012966 ---------------------------*/
/* --- Web: www.GXWMCU.com   www.stcmcu.com ---------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了宏晶科技的资料及程序   */
/*---------------------------------------------------------------------*/


/*************	功能说明	**************

本程序试验使用STC15W401AS-35I-SOP16<RMB1.6>来驱动航模用的无传感器无刷三相直流马达.

本程序参考自网上的代码(作者: 瑞生), 改良而来.

电路图见文件 "BLDC-V10-实验电路.pdf".

控制信号由P3.2输入正脉冲信号, 间隔5~20ms, 脉冲宽度1.000~1.610ms.

1.160ms开始启动, 1.610ms为最高速度, 分辨率为2us.

本程序仅仅是简单控制, 软件没有处理 过0延时30度切换 过流检测.

由于过0检测部分有RC滤波, 所以改变电容值可以大约的对应在最高速时延时30度的时间.

有意者可自行完善电路和程序.

******************************************/

#define MAIN_Fosc		24000000L	//定义主时钟

#include "STC15Fxxxx.H"

//CMPCR1
#define	CMPEN	0x80	//1: 允许比较器, 0: 禁止,关闭比较器电源
#define	CMPIF	0x40	//比较器中断标志, 包括上升沿或下降沿中断, 软件清0
#define	PIE		0x20	//1: 比较结果由0变1, 产生上升沿中断
#define	NIE		0x10	//1: 比较结果由1变0, 产生下降沿中断
#define	PIS		0x08	//输入正极性选择, 0: 选择外部P5.5做正输入,           1: 由ADCIS[2:0]所选择的ADC输入端做正输入.
#define	NIS		0x04	//输入负极性选择, 0: 选择内部BandGap电压BGv做负输入, 1: 选择外部P5.4做输入.
#define	CMPOE	0x02	//1: 允许比较结果输出到P1.2, 0: 禁止.
#define	CMPRES	0x01	//比较结果, 1: CMP+电平高于CMP-,  0: CMP+电平低于CMP-,  只读

//CMPCR2
#define	INVCMPO	0x80	//1: 比较器输出取反,  0: 不取反
#define	DISFLT	0x40	//1: 关闭0.1uF滤波,   0: 允许
#define	LCDTY	0x00	//0~63, 比较结果变化延时周期数

sbit SWK = P3^3; //外部按键
sbit LED = P5^5; //LED


sbit PWM2_L = P3^4;
sbit PWM1_L = P3^5;
sbit PWM0_L = P3^6;


u8	Step;
u8	PWM_Value; // 决定PWM占空比的值
u16	RxPulseWide; //
bit	B_RxOk;
bit	B_RUN;
u8	PWW_Set;
u8	Rx_cnt;
u8	TimeOut;	//堵转超时

#define DISABLE_CMP_INT CMPCR1 &= ~0X40		// 关闭比较器中断
#define ENABLE_CMP_INT  CMPCR1 |= 0X40		// 打开比较器中断

/*************************/

void	Delay_n_ms(u8 dly)
{
	u16	j;
	do
	{
		j = MAIN_Fosc / 13000;	//延时1ms, 主程序在此节拍下运行
		while(--j)	;
	}while(--dly);
}


void delay_us(u8 us)
{
	do
	{
		NOP(20);	//@24MHz
	}
	while(--us);
}
 void StepXL(void) // 换相序列函数
{
 switch(Step)
  {
	case 0:  //AB
		CCAP1H = PWM_Value;CCAP2H=0;CCAP0H=0;//打开B相 负极
		PWM0_L=1;PWM1_L =0;PWM2_L=0;//打开A相的高
		ADC_CONTR = 0XED;	CMPCR1 = 0xAC;// 选择P1.5作为ADC输入 即c相电压 	上升沿中断
		break;
	case 1:  //AC
		CCAP2H = PWM_Value;CCAP0H=0;CCAP1H=0;//打开C相 负极
		PWM0_L=1;PWM1_L =0;PWM2_L=0;//打开A相的高
		ADC_CONTR = 0XEC;	CMPCR1 = 0x9C;// 选择P1.4作为ADC输入 即B相电压	下降沿中断
		break;
	case 2:  //BC
		CCAP2H = PWM_Value;CCAP0H=0;CCAP1H=0;//打开C相 负极
		PWM0_L=0;PWM1_L =1;PWM2_L=0;//打开B相的高
		ADC_CONTR = 0XEB;	CMPCR1 = 0xAC;// 选择P1.3作为ADC输入 即a相电压	上升沿中断
		break;
	case 3:  //BA
		CCAP0H = PWM_Value;CCAP1H=0;CCAP2H=0;//打开A相 负极
		PWM0_L=0;PWM1_L =1;PWM2_L=0;//打开B相的高
		ADC_CONTR = 0XED;	CMPCR1 = 0x9C;// 选择P1.5作为ADC输入 即c相电压  下降沿中断
		break;
	case 4:  //CA
		CCAP0H = PWM_Value;CCAP1H=0;CCAP2H=0;//打开A相 负极
		PWM0_L=0;PWM1_L =0;PWM2_L=1;//打开C相的高
		ADC_CONTR = 0XEC;	CMPCR1 = 0xAC;// 选择P1.4作为ADC输入 即B相电压	上升沿中断
		break;
	case 5:  //CB
		CCAP1H = PWM_Value;CCAP2H=0;CCAP0H=0;//打开B相 负极
		PWM0_L=0;PWM1_L =0;PWM2_L=1;//打开C相的高
		ADC_CONTR = 0XEB;	CMPCR1 = 0x9C;// 选择P1.3作为ADC输入 即a相电压	下降沿中断
		break;


	default:
		break;
  }	
}

#define CCP_S0 0x10                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5
void PWM_Init(void)
{
	PWM0_L = 0;
	PWM1_L = 0;
	PWM2_L = 0;
	
	P3M0=0x70;P3M1=0x00;
	P5M0=0x00;P5M1=0x00;

   
//	CMOD = 1 << 1; //选择系统时钟/2为时钟源，即PWM频率=24M/2/256=46.9K
//	CMOD = 5 << 1; //选择系统时钟/4为时钟源，即PWM频率=24M/4/256=23.4K
	CMOD = 6 << 1; //选择系统时钟/6为时钟源，即PWM频率=24M/6/256=15.6K(效果好)

	CL=0;			// PCA计数器清零
	CH=0;

	
	PCA_PWM0 = 0X00;
	CCAP0H=0;    // 初始化占空比为0% H的值装载到L中
	CCAP0L=0;
	CCAPM0=0x42;	// 设置为PWM模式
	
	PCA_PWM1 = 0X00;
	CCAP1H=0;    // 初始化占空比为0%
	CCAP1L=0;
	CCAPM1=0x42;	// 设置为PWM模式
	
	PCA_PWM2 = 0X00;
	CCAP2H=0;    // 初始化占空比为0%
	CCAP2L=0;
	CCAPM2=0x42;	// 设置为PWM模式
	
	CR = 1;
}

void ADC_Init(void)
{
	P1M0=0x00;P1M1=0x1C;
	P1ASF = 0X38; // 开通P1.3 P1.4 P1.5的AD输入口
}
//ADC中断
void CMP_INT(void) interrupt 21
{
	CMPCR1 &= ~0X40; // 需软件清除中断标志位
	if(Step<5)	Step++;	 //换向
	else		Step = 0;
	StepXL();
	TimeOut = 15;	//10ms超时
}

void CMP_Init(void)
{
	CMPCR1 = 0X8C;	// 1000 1100 打开比较器，P5.4作为比较器的反相输入端，ADC引脚作为正输入端 
	CMPCR2 = 60;	// 60个时钟滤波
	P5n_pure_input(0x10);
}

u8 StartMotor(void)	  //启动电机函数
{
	u16 timer,i;
	DISABLE_CMP_INT; // 关比较器中断
	PWM_Value = 70;	// 初始占空比
	Step = 1;StepXL();	// 初始位置
	Delay_n_ms(8);		//启动持续时间（不能太高）
	PWM_Value = 50;	// 初始占空比
	Delay_n_ms(20);	//启动持续时间（不能太高）
	PWM_Value = PWM_Value;// 
	timer = 50;   //
//	return(1);
	while(1)
	{
		for(i=0; i<timer; i++)	delay_us(50);
		timer--;
		if(timer < 35)
        {
            PWM0_L=0;PWM1_L =0;PWM2_L=0;//打开A相的高
            CCAP2H=0;CCAP0H=0;CCAP1H=0;//打开C相 负极
            return(1);//跳出循环
        }  	
		if( Step < 5)	Step++;
		else			Step = 0;
		StepXL();
	}
}



/**********************************************/
void main(void)
{
	u16 kk;
    bit ready_bz=1;
	
	PWM_Init();
	ADC_Init();
	CMP_Init();
	LED = 0; //点亮LED

 ////////////////////////////////
 //开机响3下  【1；2；3】
	Delay_n_ms(250);	
    PWM_Value = 30;	//音量大小	10-100
	Step = 0;
	StepXL();			// 初始位置
	Delay_n_ms(10);

	for(kk=1500; kk>0; kk--)
	{Step = 1,StepXL(),delay_us(10);			// 初始位置;  //
	 Step = 3,StepXL(),delay_us(10);}
	Delay_n_ms(250);	
	for(kk=1500; kk>0; kk--)
	{Step = 1,StepXL(),delay_us(15);			// 初始位置;  //
	 Step = 3,StepXL(),delay_us(15);}
	Delay_n_ms(250);	
	for(kk=1500; kk>0; kk--)
	{Step = 1,StepXL(),delay_us(20);			// 初始位置;  //
	 Step = 3,StepXL(),delay_us(20);}

 //////////////////////////////////////


	PWM_Value = 0;
	RxPulseWide = 1000;
	PWW_Set = 0;
	Rx_cnt  = 0;
	TimeOut = 0;

	EA  = 1; // 打开总中断
	LED = 1; //关闭LED
	
	while (1)
	{
	Delay_n_ms(1);	//延时1ms, 主程序在此节拍下运行

	//
	if(SWK==0)	//马达未运行, 则启动马达
	{
	 LED=0;//LED
	 StartMotor();	// 启动马达
//	 CMPCR1 &= ~0X40;// 需软件清除中断标志位
	 ENABLE_CMP_INT; // 打开比较器中断
	 TimeOut = 0;
	 B_RUN = 1;
	 while(SWK==0)  //正常旋转使用一下程序
		{
	     LED=1;//LED
		 PWM_Value=150;
		}
	}
	else
	{
	 PWM_Value = 0;
	 B_RUN = 0;
	 CCAP0H=0;	CCAP1H=0;	CCAP2H=0;  // 占空比为0
	 PWM0_L=0;	PWM1_L=0;	PWM2_L=0;  // 相序 拉底复位
	 DISABLE_CMP_INT; // 关比较器中断
	}
             
	
	}
}



