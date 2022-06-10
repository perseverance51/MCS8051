/******************************************/
//
// �ó���������Ƶ��35MHz
// STC15W408AS
// ���ߣ����� 
// ���ͣ�http://www.rationmcu.com
//
/******************************************/

#include <reg51.h>
#include <intrins.h>
#define uchar unsigned char
#define uint  unsigned int



/* STC15W408AS�Ĵ������� */
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
//�������� ���� ���� ���� ͣת
sbit ADD = P1^2;
sbit DEL = P5^5;

sbit PWM2_L = P3^2;
sbit PWM1_L = P3^3;
sbit PWM0_L = P3^6;


#define DISABLE_CMP_INT CMPCR1&=0X7F // �رձȽ���
#define ENABLE_CMP_INT CMPCR1|=0X80 // �򿪱Ƚ���

/*************************/

unsigned char Step=0; // ���ಽ��
unsigned char PWM_Value=0; // ����PWMռ�ձȵ�ֵ
unsigned char QD_Sign=0; // 0 û������ 1����

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
// �������к���
void StepXL(void) 
{
 switch(Step)
  {
   case 0:  // AB
			CCAP0H=PWM_Value;CCAP1H=0;CCAP2H=0; // ��A��ĸ߶�
			PWM0_L=0;PWM2_L=0;PWM1_L=1; // ��B��ĵͶ�
			ADC_CONTR = 0XED; // ѡ��P1.5��ΪADC���� ��c���ѹ
			CMPCR1&=0xDF; // PIE=0���ر��������ж�
			CMPCR1|=0x10; // NIE=1��ʹ���½����ж�
			break;
   case 1:  // AC
			CCAP0H=PWM_Value;CCAP1H=0;CCAP2H=0; // ��A��ĸ߶�
			PWM0_L=0;PWM1_L=0;PWM2_L=1; // ��C��ĵͶ�
			ADC_CONTR = 0XEC; // ѡ��P1.4��ΪADC���� ��B���ѹ
			CMPCR1&=0xEF; // NIE=0 ,�ر��½����ж�
			CMPCR1|=0x20; // PIE=1��ʹ���������ж�
	 
      break;
   case 2:  // BC
			CCAP0H=0;CCAP2H=0;CCAP1H=PWM_Value; // ��B��ĸ߶�
			PWM0_L=0;PWM1_L=0;PWM2_L=1; // ��C��ĵͶ�
			ADC_CONTR = 0XEB; // ѡ��P1.3��ΪADC���� ��a���ѹ
			CMPCR1&=0xDF; // PIE=0���ر��������ж�
			CMPCR1|=0x10; // NIE=1��ʹ���½����ж�
      break;
   case 3:  // BA
      CCAP0H=0;CCAP2H=0;CCAP1H=PWM_Value; // ��B��ĸ߶�
			PWM1_L=0;PWM2_L=0;PWM0_L=1; // ��A��ĵͶ�
			ADC_CONTR = 0XED; // ѡ��P1.5��ΪADC���� ��c���ѹ
			CMPCR1&=0xEF; // NIE=0 ,�ر��½����ж�
			CMPCR1|=0x20; // PIE=1��ʹ���������ж�
			
      break;
   case 4: // CA
      CCAP0H=0;CCAP1H=0;CCAP2H=PWM_Value; // ��C��ĸ߶�
			PWM1_L=0;PWM2_L=0;PWM0_L=1; // ��A��ĵͶ�
			ADC_CONTR = 0XEC; // ѡ��P1.4��ΪADC���� ��B���ѹ
			CMPCR1&=0xDF; // PIE=0���ر��������ж�
			CMPCR1|=0x10; // NIE=1��ʹ���½����ж�
      break;
   case 5: // CB
      CCAP0H=0;CCAP1H=0;CCAP2H=PWM_Value;// ��C��ĸ߶�
      PWM0_L=0;PWM2_L=0;PWM1_L=1; // ��B��ĵͶ�
			ADC_CONTR = 0XEB; // ѡ��P1.3��ΪADC���� ��a���ѹ
			CMPCR1&=0xEF; // NIE=0 ,�ر��½����ж�
			CMPCR1|=0x20; // PIE=1��ʹ���������ж�
	 
      break;
	 default:break;
  }	
}


/*****************�������ú���**********************/
void serial_open(void)
{
	SCON = 0X50;//�����ڴ���ģʽ
	AUXR |= 0X04;//
	TL2 = 0X71;// 9600 @35MHz
	TH2 = 0Xfc;
	AUXR|=0X10;// 
}

/******************���ڷ������ݺ���********************/
 void senddata(uchar data_buf)
 {
 	SBUF = data_buf;
 	while(!TI);
 	TI = 0;
 }

// PWM��ʼ������
void PWM_Init(void)
{
	PWM0_L=0;
	PWM1_L=0;
	PWM2_L=0;

	CMOD=0X0C; //ѡ��ϵͳʱ��/6Ϊʱ��Դ����Ƶ��=35M/6/256=22.8K
	CL=0;			// PCA����������
	CH=0;
	
	PCA_PWM0 = 0X00;
	CCAP0H=0;    // ��ʼ��ռ�ձ�Ϊ0% H��ֵװ�ص�L��
	CCAP0L=0;
	CCAPM0=0x42;	// ����ΪPWMģʽ
	
	PCA_PWM1 = 0X00;
	CCAP1H=0;    // ��ʼ��ռ�ձ�Ϊ0%
	CCAP1L=0;
	CCAPM1=0x42;	// ����ΪPWMģʽ
	
	PCA_PWM2 = 0X00;
	CCAP2H=0;    // ��ʼ��ռ�ձ�Ϊ0%
	CCAP2L=0;
	CCAPM2=0x42;	// ����ΪPWMģʽ
	
	CR = 1;
}
// ADC��ʼ������
void ADC_Init(void)
{
	P1ASF = 0X38; // ��ͨP1.3 P1.4 P1.5��AD�����
	ADC_CONTR=0XE0; // ��ADC��Դ �������ת���ٶ�
	IE|=0X20; // EADC=1 ����ADC�ж�
}

void ADC_ISR() interrupt 5
{
	ADC_CONTR&=0xEF; // ��ADCת����־
	ADC_CONTR|=0X08; // ����ADCת��
}

// �Ƚ�����������
void CMP_Init(void)
{
	CMPCR1=0X0c; // P5.4��Ϊ�Ƚ����ķ�������ˣ�ADC������Ϊ������� 
	CMPCR2=0x10;// ��ʱ���˲�
}
// �Ƚ����жϺ���
void CMP_ISR(void) interrupt 21
{
	CMPCR1 &=0XBF; // ���������жϱ�־λ
	TH0=0x00; // ����ʱ����ֵ
	TL0=0X00;
	if(Step<5)Step++;
	else Step=0;
	StepXL();
}

// �����������
unsigned char QiDong(void)
{
	unsigned int timer = 300,i;
	DISABLE_CMP_INT;
	PWM_Value = 20; // ռ�ձ�=20/256=7.8%
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
			QD_Sign=1; // ��־�Ѿ�����
			return(1);
		}
		if(Step<5)Step++;
		else Step=0;
		StepXL();
	}
}
// ��ʱ��0��ʼ������
void T0_Iint(void)
{
	TMOD=0X01; // T0������16λ����ģʽ
	TH0=0x00; //
	TL0=0X00;
	ET0=1;// ����ET0�ж�
}

// ��ʱ��0�жϺ���(������붨ʱ��0�жϣ�������ͣת�����ڱ���)
void T0_ISR(void) interrupt 1
{
	PWM0_L=0;PWM1_L=0;PWM2_L=0;
	CCAP0H=0;CCAP1H=0;CCAP2H=0;  // ռ�ձ�Ϊ0
	TR0=0;
	EA=0;
	DISABLE_CMP_INT;
	TH0=0x00; //
	TL0=0X00;
	QD_Sign=0; // ��־û������
	
	QiDong(); // �ٴ�����
	ENABLE_CMP_INT; // �򿪱Ƚ����ж�
	EA=1; // �����ж�
	TR0=1; // �򿪶�ʱ��0
}

/**********************************************/
void main(void)
{
	uchar rec=0; // ���ڽ�������
//	uchar i;

	PWM_Init(); // ��ʼ��PWM
	ADC_Init(); // ��ʼ��ADC
	CMP_Init(); // ��ʼ���Ƚ���
	serial_open(); // �򿪴���
	T0_Iint(); // ��ʱ��0��ʼ��(������붨ʱ��0�жϣ�������ͣת�����ڱ���)
	
	while (1)
	{
		// ��������յ�����(�ⲿ���Ǵ��ڿ��� ���� ���� ���� ͣת)
		if(RI) 
		{
			rec=SBUF; // �ѽ��յ������ݸ���rec
			RI=0; // �崮�ڽ��ձ�־
			
			if(rec==0x22) // ����
			{
				if(PWM_Value<250)
				{
					PWM_Value++; // ����ռ�ձ�
				}
			}
			else if(rec==0x33) // ����
			{
				if(PWM_Value>10)
				{
					PWM_Value--; // ��Сռ�ձ�
				}
			}
			else if(rec==0x11) // �������
			{
				QiDong(); // ����
				ENABLE_CMP_INT; // �򿪱Ƚ����ж�
				EA=1; // �����ж�
				TR0=1; // �򿪶�ʱ��0
			}
			else if(rec==0x44) // ͣת
			{
				CCAP0H=0;CCAP1H=0;CCAP2H=0;  // ռ�ձ�Ϊ0
				PWM0_L=0;PWM1_L=0;PWM2_L=0;
				EA=0; // �����ж�
				DISABLE_CMP_INT; // �رȽ����ж�
			}
			senddata(SBUF);
		}
		// �˲����ǰ������� ���� ���� ���� ͣת
		if(ADD==0) // ���������Ӽ��ٰ���
		{
			delay_ms(20);
			if(ADD==0)
			{
				if(QD_Sign==0)// ���û������
				{
					QiDong(); // ����
					ENABLE_CMP_INT; // �򿪱Ƚ����ж�
					EA=1; // �����ж�
					TR0=1; // �򿪶�ʱ��0
				}
				else if(PWM_Value<250)
				{
					PWM_Value++; // ����ռ�ձ�
				}
			}
		}
		else if(DEL==0) // �����˼��ٰ���
		{
			delay_ms(20);
			if(DEL==0)
			{
				if(PWM_Value>10) 
				{
					PWM_Value--; // ��Сռ�ձ�
				}
				else // ͣת
				{
					CCAP0H=0;CCAP1H=0;CCAP2H=0;  // ռ�ձ�Ϊ0
					PWM0_L=0;PWM1_L=0;PWM2_L=0;
					TR0=0;
					EA=0;
					DISABLE_CMP_INT;
					TH0=0x00; //
					TL0=0X00;
					QD_Sign=0; // ��־û������
				}
			}
		}
	}
}



