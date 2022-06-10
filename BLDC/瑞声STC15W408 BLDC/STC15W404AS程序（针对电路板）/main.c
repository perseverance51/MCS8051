/******************************************/
//
// 该程序工作的主频是35MHz
// STC15W408AS
// 作者：瑞生 
// 博客：http://www.rationmcu.com
//
/******************************************/

#include <reg51.h>
#include <intrins.h>
#define uchar unsigned char
#define uint  unsigned int



/* STC15W408AS寄存器补充 */
sfr AUXR1 = 0XA2;
sfr CCON = 0XD8;
sfr CMOD = 0XD9;
sfr CCAPM0 = 0XDA;
sfr CCAPM1 = 0XDB;
sfr CCAPM2 = 0XDC;

sfr CL     = 0xE9;
sfr CCAP0L = 0xEA;
sfr CCAP1L = 0xEB;
sfr CCAP2L = 0xEC;
sfr CH     = 0xF9;
sfr CCAP0H = 0xFA;
sfr CCAP1H = 0xFB;
sfr CCAP2H = 0xFC;

sbit CF    = CCON^7;
sbit CR    = CCON^6;
sbit CCF2  = CCON^2;
sbit CCF1  = CCON^1;
sbit CCF0  = CCON^0;

sbit PPCA = IP^7;

sfr P2M1 = 0X95;
sfr P2M0 = 0X96;

sfr CMPCR1 = 0XE6;
sfr CMPCR2 = 0XE7;

sfr AUXR = 0X8E;
sfr TH2 = 0XD6;
sfr TL2 = 0XD7;
sfr P4 = 0xc0;
sfr P5 = 0xc8;

sfr PCA_PWM0 = 0XF2;
sfr PCA_PWM1 = 0XF3;
sfr PCA_PWM2 = 0XF4;

sfr P1ASF = 0X9D;
sfr ADC_CONTR = 0XBC;
sfr PCON2 = 0X97;

sfr SPSTAT      =   0xCD;   //
sfr SPCTL       =   0xCE;   //
sfr SPDAT       =   0xCF;   //
//按键控制 启动 加速 减速 停转
sbit ADD = P1^2;
sbit DEL = P5^5;

sbit PWM2_L = P3^2;
sbit PWM1_L = P3^3;
sbit PWM0_L = P3^6;


#define DISABLE_CMP_INT CMPCR1&=0X7F // 关闭比较器
#define ENABLE_CMP_INT CMPCR1|=0X80 // 打开比较器

/*************************/

unsigned char Step=0; // 换相步骤
unsigned char PWM_Value=0; // 决定PWM占空比的值
unsigned char QD_Sign=0; // 0 没有启动 1启动

void Delay1ms()		//@35MHz
{
	unsigned char i, j;

	i = 35;
	j = 8;
	do
	{
		while (--j);
	} while (--i);
}

void delay_ms(unsigned int ms)
{
	while(ms)
	{
		Delay1ms();
		ms=ms-1;
	}
}

void Delay1us()		//@35MHz
{
	unsigned char i;

	_nop_();
	i = 6;
	while (--i);
}

void delay_us(unsigned int us)
{
	while(us)
	{
		Delay1us();
		us=us-1;
	}
}
// 换相序列函数
void StepXL(void) 
{
 switch(Step)
  {
   case 0:  // AB
			CCAP0H=PWM_Value;CCAP1H=0;CCAP2H=0; // 打开A相的高端
			PWM0_L=0;PWM2_L=0;PWM1_L=1; // 打开B相的低端
			ADC_CONTR = 0XED; // 选择P1.5作为ADC输入 即c相电压
			CMPCR1&=0xDF; // PIE=0，关闭上升沿中断
			CMPCR1|=0x10; // NIE=1，使能下降沿中断
			break;
   case 1:  // AC
			CCAP0H=PWM_Value;CCAP1H=0;CCAP2H=0; // 打开A相的高端
			PWM0_L=0;PWM1_L=0;PWM2_L=1; // 打开C相的低端
			ADC_CONTR = 0XEC; // 选择P1.4作为ADC输入 即B相电压
			CMPCR1&=0xEF; // NIE=0 ,关闭下降沿中断
			CMPCR1|=0x20; // PIE=1，使能上升沿中断
	 
      break;
   case 2:  // BC
			CCAP0H=0;CCAP2H=0;CCAP1H=PWM_Value; // 打开B相的高端
			PWM0_L=0;PWM1_L=0;PWM2_L=1; // 打开C相的低端
			ADC_CONTR = 0XEB; // 选择P1.3作为ADC输入 即a相电压
			CMPCR1&=0xDF; // PIE=0，关闭上升沿中断
			CMPCR1|=0x10; // NIE=1，使能下降沿中断
      break;
   case 3:  // BA
      CCAP0H=0;CCAP2H=0;CCAP1H=PWM_Value; // 打开B相的高端
			PWM1_L=0;PWM2_L=0;PWM0_L=1; // 打开A相的低端
			ADC_CONTR = 0XED; // 选择P1.5作为ADC输入 即c相电压
			CMPCR1&=0xEF; // NIE=0 ,关闭下降沿中断
			CMPCR1|=0x20; // PIE=1，使能上升沿中断
			
      break;
   case 4: // CA
      CCAP0H=0;CCAP1H=0;CCAP2H=PWM_Value; // 打开C相的高端
			PWM1_L=0;PWM2_L=0;PWM0_L=1; // 打开A相的低端
			ADC_CONTR = 0XEC; // 选择P1.4作为ADC输入 即B相电压
			CMPCR1&=0xDF; // PIE=0，关闭上升沿中断
			CMPCR1|=0x10; // NIE=1，使能下降沿中断
      break;
   case 5: // CB
      CCAP0H=0;CCAP1H=0;CCAP2H=PWM_Value;// 打开C相的高端
      PWM0_L=0;PWM2_L=0;PWM1_L=1; // 打开B相的低端
			ADC_CONTR = 0XEB; // 选择P1.3作为ADC输入 即a相电压
			CMPCR1&=0xEF; // NIE=0 ,关闭下降沿中断
			CMPCR1|=0x20; // PIE=1，使能上升沿中断
	 
      break;
	 default:break;
  }	
}


/*****************串口配置函数**********************/
void serial_open(void)
{
	SCON = 0X50;//工作在串口模式
	AUXR |= 0X04;//
	TL2 = 0X71;// 9600 @35MHz
	TH2 = 0Xfc;
	AUXR|=0X10;// 
}

/******************串口发送数据函数********************/
 void senddata(uchar data_buf)
 {
 	SBUF = data_buf;
 	while(!TI);
 	TI = 0;
 }

// PWM初始化函数
void PWM_Init(void)
{
	PWM0_L=0;
	PWM1_L=0;
	PWM2_L=0;

	CMOD=0X0C; //选择系统时钟/6为时钟源，即频率=35M/6/256=22.8K
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
// ADC初始化函数
void ADC_Init(void)
{
	P1ASF = 0X38; // 开通P1.3 P1.4 P1.5的AD输入口
	ADC_CONTR=0XE0; // 打开ADC电源 设置最大转换速度
	IE|=0X20; // EADC=1 允许ADC中断
}

void ADC_ISR() interrupt 5
{
	ADC_CONTR&=0xEF; // 清ADC转换标志
	ADC_CONTR|=0X08; // 启动ADC转换
}

// 比较器启动函数
void CMP_Init(void)
{
	CMPCR1=0X0c; // P5.4作为比较器的反相输入端，ADC引脚作为正输入端 
	CMPCR2=0x10;// 个时钟滤波
}
// 比较器中断函数
void CMP_ISR(void) interrupt 21
{
	CMPCR1 &=0XBF; // 需软件清除中断标志位
	TH0=0x00; // 给定时器初值
	TL0=0X00;
	if(Step<5)Step++;
	else Step=0;
	StepXL();
}

// 电机启动函数
unsigned char QiDong(void)
{
	unsigned int timer = 300,i;
	DISABLE_CMP_INT;
	PWM_Value = 20; // 占空比=20/256=7.8%
	Step=0;
	StepXL();
	delay_ms(5);

	while(1)
	{
		for(i=0;i<timer; i++) 
		{
			delay_us(50);  //
		} 
		timer-= timer/15+1;
		if(timer < 25) 
		{
			QD_Sign=1; // 标志已经启动
			return(1);
		}
		if(Step<5)Step++;
		else Step=0;
		StepXL();
	}
}
// 定时器0初始化函数
void T0_Iint(void)
{
	TMOD=0X01; // T0工作于16位计数模式
	TH0=0x00; //
	TL0=0X00;
	ET0=1;// 允许ET0中断
}

// 定时器0中断函数(如果进入定时器0中断，代表电机停转，用于保护)
void T0_ISR(void) interrupt 1
{
	PWM0_L=0;PWM1_L=0;PWM2_L=0;
	CCAP0H=0;CCAP1H=0;CCAP2H=0;  // 占空比为0
	TR0=0;
	EA=0;
	DISABLE_CMP_INT;
	TH0=0x00; //
	TL0=0X00;
	QD_Sign=0; // 标志没有启动
	
	QiDong(); // 再次启动
	ENABLE_CMP_INT; // 打开比较器中断
	EA=1; // 打开总中断
	TR0=1; // 打开定时器0
}

/**********************************************/
void main(void)
{
	uchar rec=0; // 串口接收数据
//	uchar i;

	PWM_Init(); // 初始化PWM
	ADC_Init(); // 初始化ADC
	CMP_Init(); // 初始化比较器
	serial_open(); // 打开串口
	T0_Iint(); // 定时器0初始化(如果进入定时器0中断，代表电机停转，用于保护)
	
	while (1)
	{
		// 如果串口收到数据(这部分是串口控制 启动 加速 减速 停转)
		if(RI) 
		{
			rec=SBUF; // 把接收到的数据给了rec
			RI=0; // 清串口接收标志
			
			if(rec==0x22) // 加速
			{
				if(PWM_Value<250)
				{
					PWM_Value++; // 增加占空比
				}
			}
			else if(rec==0x33) // 减速
			{
				if(PWM_Value>10)
				{
					PWM_Value--; // 减小占空比
				}
			}
			else if(rec==0x11) // 电机启动
			{
				QiDong(); // 启动
				ENABLE_CMP_INT; // 打开比较器中断
				EA=1; // 打开总中断
				TR0=1; // 打开定时器0
			}
			else if(rec==0x44) // 停转
			{
				CCAP0H=0;CCAP1H=0;CCAP2H=0;  // 占空比为0
				PWM0_L=0;PWM1_L=0;PWM2_L=0;
				EA=0; // 关总中断
				DISABLE_CMP_INT; // 关比较器中断
			}
			senddata(SBUF);
		}
		// 此部分是按键控制 启动 加速 减速 停转
		if(ADD==0) // 按下了增加加速按键
		{
			delay_ms(20);
			if(ADD==0)
			{
				if(QD_Sign==0)// 如果没有启动
				{
					QiDong(); // 启动
					ENABLE_CMP_INT; // 打开比较器中断
					EA=1; // 打开总中断
					TR0=1; // 打开定时器0
				}
				else if(PWM_Value<250)
				{
					PWM_Value++; // 增加占空比
				}
			}
		}
		else if(DEL==0) // 按下了减速按键
		{
			delay_ms(20);
			if(DEL==0)
			{
				if(PWM_Value>10) 
				{
					PWM_Value--; // 减小占空比
				}
				else // 停转
				{
					CCAP0H=0;CCAP1H=0;CCAP2H=0;  // 占空比为0
					PWM0_L=0;PWM1_L=0;PWM2_L=0;
					TR0=0;
					EA=0;
					DISABLE_CMP_INT;
					TH0=0x00; //
					TL0=0X00;
					QD_Sign=0; // 标志没有启动
				}
			}
		}
	}
}



