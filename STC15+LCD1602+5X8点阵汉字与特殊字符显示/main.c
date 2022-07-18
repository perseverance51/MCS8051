
/*************	��������˵��	**************

����LCD1602�ַ���.

��ʾЧ��Ϊ:  DS1302 + LCD1602��ʾʱ��

******************************************/
#include	"STC15Fxxxx.H"

#define T1MS (65536-MAIN_Fosc/12/1000)   //12Tģʽ1ms��ʱ
#define  interval  1000 //������ʱʱ����
/*************	���ر�������	**************/
static volatile unsigned long sysRunmillis = 0;	//ϵͳ����ʱ����������浥Ƭ�����ϵ縴λ�������е�ʱ�䣬��λ�Ǻ��롣����ֵ�ɶ�ʱ��T0���ж���Ӧ�Ӻ�������
unsigned long previousMillis = 0;
unsigned char str[4];//uchartostr����ת�����ַ��� ͬʱ���԰�16����ת��10����
sbit Buzzer = P3^7;
unsigned char code table[]=
{
    0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,        // һ����ʾʱ��ASCII�� 0x00
    0x00,0x00,0x00,0x0e,0x00,0xff,0x00,0x00,        // ������ʾʱ��ASCII�� 0x01
    0x00,0x00,0xff,0x00,0x0e,0x00,0xff,0x00,        // ������ʾʱ��ASCII�� 0x02
    0x00,0x00,0xff,0xf5,0xfb,0xf1,0xff,0x00,        // �ģ���ʾʱ��ASCII�� 0x03
    0x00,0xfe,0x08,0xfe,0x0a,0x0a,0xff,0x00,        // �壬��ʾʱ��ASCII�� 0x04
    0x00,0x04,0x00,0xff,0x00,0x0a,0x11,0x00,        // ������ʾʱ��ASCII�� 0x05
	0x00,0x1f,0x11,0x1f,0x11,0x11,0x1f,0x00,        // �գ���ʾʱ��ASCII�� 0x06
	0x18,0x18,0x07,0x08,0x08,0x08,0x07,0x00,        // �棬��ʾʱ��ASCII�� 0x07
};

void Timer0Init(void);		//1����@12.000MHz
void  delay_ms(unsigned long ms);
void	Initialize_LCD(void);
void	Write_AC(u8 hang,u8 lie);
void	Write_DIS_Data(u8 DIS_Data);
void WriteROM();//LCD1602�ڲ����ݼĴ���д�뺯��
void	ClearLine(u8 row);
u8		BIN_ASCII(u8 tmp);
void 	PutString(u8 row, u8 column, u8 *puts);
void	WriteChar(u8 row, u8 column, u8 dat);
void Buzzer_Di(void);
unsigned char *uchartostr(unsigned num);	//��һ���ֽڵ�����ת��Ϊ�ַ��� ��10����
//========================================================================
// ����: void main(void)
// ����: ��������
// ����: none.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void main(void)
{
	unsigned long currentMillis;//��ǰʱ��
	unsigned char i =0;
	P0M1 = 0;	P0M0 = 0;	//����Ϊ׼˫���
	P1M1 = 0;	P1M0 = 0;	//����Ϊ׼˫���
	P2M1 = 0;	P2M0 = 0;	//����Ϊ׼˫���
	P3M1 = 0;	P3M0 = 0;	//����Ϊ׼˫���
	P4M1 = 0;	P4M0 = 0;	//����Ϊ׼˫���
	P5M1 = 0;	P5M0 = 0;	//����Ϊ׼˫���
	P6M1 = 0;	P6M0 = 0;	//����Ϊ׼˫���
	P7M1 = 0;	P7M0 = 0;	//����Ϊ׼˫���
	Timer0Init();	//1����@12.000MHz
	Initialize_LCD();//LCD��ʼ������
	ClearLine(0);
	ClearLine(1);
	PutString(0,0,"STC15W4K");
	WriteROM();			   // ���Զ���������д��LCD1602�ڲ��洢��
  WriteChar(1, 0, 0);
	WriteChar(1, 1, 1);
	WriteChar(1, 2, 2);
	WriteChar(1, 3, 3);
	WriteChar(1, 4, 4);
	WriteChar(1, 5, 5);
	WriteChar(1, 6, 6);
	WriteChar(1, 7, 7);
	while(1)
	{
		currentMillis = sysRunmillis;	
	if (currentMillis - previousMillis  >= interval)
		{//ÿ��1�뷭תһ��
    previousMillis = sysRunmillis;
		if(++i <= 255){
			PutString(0,9,uchartostr(i));
			WriteChar(0, 14, i);	
			
			PutString(1,9,uchartostr(255 -i));
			WriteChar(1, 14, 255 -i);
			
		}
		
	}
	}

} 

void Timer0Init(void)		//1����@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = T1MS ;		//���ö�ʱ��ʼֵ
	TH0 = T1MS >>8;		//���ö�ʱ��ʼֵ
//	TL0 = 0x18;		//���ö�ʱ��ʼֵ
//	TH0 = 0xFC;		//���ö�ʱ��ʼֵ
	
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;     //enable timer0 interrupt
  EA = 1;      //open global interrupt switch
}

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1
{
//	TL0 = 0x18;		//���ö�ʱ��ʼֵ
//	TH0 = 0xFC;		//���ö�ʱ��ʼֵ
    TL0 = T1MS;                     //reload timer0 low byte
    TH0 = T1MS >> 8;                //reload timer0 high byte
	sysRunmillis ++;
}


//========================================================================
// ����: void  delay_ms(u8 ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��, ����ֻ֧��1~255ms. �Զ���Ӧ��ʱ��.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void  delay_ms(unsigned long ms)
{
	unsigned long	temp = sysRunmillis ;
    while(sysRunmillis - temp < ms );
//     unsigned int i;
//	 do{
//	      i = MAIN_Fosc / 13000;
//		  while(--i)	;   //14T per loop
//     }while(--ms);
}




/************* LCD1602��س���	*****************************************************/
//8λ���ݷ��ʷ�ʽ	LCD1602		��׼����	������д	2014-2-21

#define LineLength	16		//16x2

/*************	Pin define	*****************************************************/

sfr		LCD_BUS = 0x80;	//P0--0x80, P1--0x90, P2--0xA0, P3--0xB0

sbit	LCD_B7  = LCD_BUS^7;	//D7 -- Pin 14		LED- -- Pin 16 
sbit	LCD_B6  = LCD_BUS^6;	//D6 -- Pin 13		LED+ -- Pin 15
sbit	LCD_B5  = LCD_BUS^5;	//D5 -- Pin 12		Vo   -- Pin 3
sbit	LCD_B4  = LCD_BUS^4;	//D4 -- Pin 11		VDD  -- Pin 2
sbit	LCD_B3  = LCD_BUS^3;	//D3 -- Pin 10		VSS  -- Pin 1
sbit	LCD_B2  = LCD_BUS^2;	//D2 -- Pin  9
sbit	LCD_B1  = LCD_BUS^1;	//D1 -- Pin  8
sbit	LCD_B0  = LCD_BUS^0;	//D0 -- Pin  7

sbit	LCD_ENA	= P2^2;	//Pin 6
sbit	LCD_RW	= P2^1;	//Pin 5	//LCD_RS   R/W   DB7--DB0        FOUNCTION
sbit	LCD_RS	= P2^0;	//Pin 4	//	0		0	  INPUT      write the command to LCD model
								//	0		1     OUTPUT     read BF and AC pointer from LCD model
								//	1		0     INPUT      write the data to LCD  model
								//	1		1     OUTPUT     read the data from LCD model
/*
total 2 lines, 16x2= 32
first line address:  0~15
second line address: 64~79

*/

#define C_CLEAR			0x01		//clear LCD
#define C_HOME 			0x02		//cursor go home
#define C_CUR_L			0x04		//cursor shift left after input
#define C_RIGHT			0x05		//picture shift right after input
#define C_CUR_R			0x06		//cursor shift right after input
#define C_LEFT 			0x07		//picture shift left after input
#define C_OFF  			0x08		//turn off LCD
#define C_ON   			0x0C		//turn on  LCD
#define C_FLASH			0x0D		//turn on  LCD, flash 
#define C_CURSOR		0x0E		//turn on  LCD and cursor
#define C_FLASH_ALL		0x0F		//turn on  LCD and cursor, flash
#define C_CURSOR_LEFT	0x10		//single cursor shift left
#define C_CURSOR_RIGHT	0x10		//single cursor shift right
#define C_PICTURE_LEFT	0x10		//single picture shift left
#define C_PICTURE_RIGHT	0x10		//single picture shift right
#define C_BIT8			0x30		//set the data is 8 bits
#define C_BIT4			0x20		//set the data is 4 bits
#define C_L1DOT7		0x30		//8 bits,one line 5*7  dots
#define C_L1DOT10		0x34		//8 bits,one line 5*10 dots
#define C_L2DOT7		0x38		//8 bits,tow lines 5*7 dots
#define C_4bitL2DOT7	0x28		//4 bits,tow lines 5*7 dots
#define C_CGADDRESS0	0x40		//CGRAM address0 (addr=40H+x)
#define C_DDADDRESS0	0x80		//DDRAM address0 (addr=80H+x)


#define	LCD_DelayNop()	NOP(15)

#define		LCD_BusData(dat)	LCD_BUS = dat


//========================================================================
// ����: void	CheckBusy(void)
// ����: ���æ����
// ����: none.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void	CheckBusy(void)
{
	u16	i;
	for(i=0; i<5000; i++)	{if(!LCD_B7)	break;}		//check the LCD busy or not. With time out
//	while(LCD_B7);			//check the LCD busy or not. Without time out
}

//========================================================================
// ����: void IniSendCMD(u8 cmd)
// ����: ��ʼ��д����(�����æ)
// ����: cmd: Ҫд������.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void IniSendCMD(u8 cmd)
{
	LCD_RW = 0;
	LCD_BusData(cmd);
	LCD_DelayNop();
	LCD_ENA = 1;
	LCD_DelayNop();
	LCD_ENA = 0;
	LCD_BusData(0xff);
}

//========================================================================
// ����: void Write_CMD(u8 cmd)
// ����: д����(���æ)
// ����: cmd: Ҫд������.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void Write_CMD(u8 cmd)
{
	LCD_RS  = 0;
	LCD_RW = 1;
	LCD_BusData(0xff);
	LCD_DelayNop();
	LCD_ENA = 1;
	CheckBusy();			//check the LCD busy or not.
	LCD_ENA = 0;
	LCD_RW = 0;
	
	LCD_BusData(cmd);
	LCD_DelayNop();
	LCD_ENA = 1;
	LCD_DelayNop();
	LCD_ENA = 0;
	LCD_BusData(0xff);
}

//========================================================================
// ����: void Write_DIS_Data(u8 dat)
// ����: д��ʾ����(���æ)
// ����: dat: Ҫд������.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void Write_DIS_Data(u8 dat)
{
	LCD_RS = 0;
	LCD_RW = 1;

	LCD_BusData(0xff);
	LCD_DelayNop();
	LCD_ENA = 1;
	CheckBusy();			//check the LCD busy or not.
	LCD_ENA = 0;
	LCD_RW = 0;
	LCD_RS  = 1;

	LCD_BusData(dat);
	LCD_DelayNop();
	LCD_ENA = 1;
	LCD_DelayNop();
	LCD_ENA = 0;
	LCD_BusData(0xff);
}

unsigned char *uchartostr(unsigned num)	//��һ���ֽڵ�����ת��Ϊ�ַ��� ��10����
{
	unsigned char x2,x1,x0,i;
	x2=num/100;
	x1=num%100/10;
	x0=num%100%10;
	i=0;
	if(x2!=0)
	{
		str[i]=x2+48;
		i++;
	}
	if(x1!=0)
	{
		str[i]=x1+48;
		i++;
	}
	str[i]=x0+48;
	i++;
	str[i]='\0';
	return str;
}

//========================================================================
// ����: void Initialize_LCD(void)
// ����: ��ʼ������
// ����: none.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void Initialize_LCD(void)
{
	LCD_ENA = 0;
	LCD_RS  = 0;
	LCD_RW = 0;

	delay_ms(100);
	IniSendCMD(C_BIT8);		//set the data is 8 bits

	delay_ms(10);
	Write_CMD(C_L2DOT7);		//tow lines 5*7 dots

	delay_ms(6);
	Write_CMD(C_CLEAR);		//clear LCD RAM
	Write_CMD(C_CUR_R);		//Curror Shift Right
	Write_CMD(C_ON);		//turn on  LCD
}



//========================================================================
// ����: void ClearLine(u8 row)
// ����: ���1��
// ����: row: ��(0��1)
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void ClearLine(u8 row)
{
	u8 i;
	Write_CMD(((row & 1) << 6) | 0x80);
	for(i=0; i<LineLength; i++)	Write_DIS_Data(' ');
}

//========================================================================
// ����: void	WriteChar(u8 row, u8 column, u8 dat)
// ����: ָ���С��к��ַ�, дһ���ַ�
// ����: row: ��(0��1),  column: �ڼ����ַ�(0~15),  dat: Ҫд���ַ�.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void	WriteChar(u8 row, u8 column, u8 dat)
{
	Write_CMD((((row & 1) << 6) + column) | 0x80);
	Write_DIS_Data(dat);
}

//========================================================================
// ����: void PutString(u8 row, u8 column, u8 *puts)
// ����: дһ���ַ�����ָ���С��к��ַ����׵�ַ
// ����: row: ��(0��1),  column: �ڼ����ַ�(0~15),  puts: Ҫд���ַ���ָ��.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void PutString(u8 row, u8 column, u8 *puts)
{
	Write_CMD((((row & 1) << 6) + column) | 0x80);
	for ( ;  *puts != 0;  puts++)		//����ֹͣ��0����
	{
		Write_DIS_Data(*puts);
		if(++column >= LineLength)	break;
	}
}
void WriteROM()//LCD1602�ڲ����ݼĴ���д�뺯��
{
		unsigned char i;
	Write_CMD(0x40);			 // ����CGRAM��������
	for(i=0;i<64;i++)            // д������������  
	{
		Write_DIS_Data(table[i]);
	}
}

//******************** LCD20 Module END ***************************
//****************************************************
//����������
//****************************************************
void Buzzer_Di(void){
	Buzzer = 0;
	delay_ms(3);
	Buzzer = 1;
	delay_ms(3);
}