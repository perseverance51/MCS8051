
/*************	本程序功能说明	**************

驱动LCD1602字符屏.

显示效果为:  DS1302 + LCD1602显示时间

******************************************/
#include	"STC15Fxxxx.H"

#define T1MS (65536-MAIN_Fosc/12/1000)   //12T模式1ms定时
#define  interval  1000 //设置延时时间间隔
/*************	本地变量声明	**************/
static volatile unsigned long sysRunmillis = 0;	//系统运行时间计数，保存单片机从上电复位以来运行的时间，单位是毫秒。该数值由定时器T0的中断响应子函数更新
unsigned long previousMillis = 0;
unsigned char str[4];//uchartostr函数转换的字符串 同时可以把16进制转成10进制
sbit Buzzer = P3^7;
unsigned char code table[]=
{
    0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,        // 一，显示时的ASCII码 0x00
    0x00,0x00,0x00,0x0e,0x00,0xff,0x00,0x00,        // 二，显示时的ASCII码 0x01
    0x00,0x00,0xff,0x00,0x0e,0x00,0xff,0x00,        // 三，显示时的ASCII码 0x02
    0x00,0x00,0xff,0xf5,0xfb,0xf1,0xff,0x00,        // 四，显示时的ASCII码 0x03
    0x00,0xfe,0x08,0xfe,0x0a,0x0a,0xff,0x00,        // 五，显示时的ASCII码 0x04
    0x00,0x04,0x00,0xff,0x00,0x0a,0x11,0x00,        // 六，显示时的ASCII码 0x05
	0x00,0x1f,0x11,0x1f,0x11,0x11,0x1f,0x00,        // 日，显示时的ASCII码 0x06
	0x18,0x18,0x07,0x08,0x08,0x08,0x07,0x00,        // ℃，显示时的ASCII码 0x07
};

void Timer0Init(void);		//1毫秒@12.000MHz
void  delay_ms(unsigned long ms);
void	Initialize_LCD(void);
void	Write_AC(u8 hang,u8 lie);
void	Write_DIS_Data(u8 DIS_Data);
void WriteROM();//LCD1602内部数据寄存器写入函数
void	ClearLine(u8 row);
u8		BIN_ASCII(u8 tmp);
void 	PutString(u8 row, u8 column, u8 *puts);
void	WriteChar(u8 row, u8 column, u8 dat);
void Buzzer_Di(void);
unsigned char *uchartostr(unsigned num);	//将一个字节的数据转换为字符串 或10进制
//========================================================================
// 函数: void main(void)
// 描述: 主函数。
// 参数: none.
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
//========================================================================
void main(void)
{
	unsigned long currentMillis;//当前时间
	unsigned char i =0;
	P0M1 = 0;	P0M0 = 0;	//设置为准双向口
	P1M1 = 0;	P1M0 = 0;	//设置为准双向口
	P2M1 = 0;	P2M0 = 0;	//设置为准双向口
	P3M1 = 0;	P3M0 = 0;	//设置为准双向口
	P4M1 = 0;	P4M0 = 0;	//设置为准双向口
	P5M1 = 0;	P5M0 = 0;	//设置为准双向口
	P6M1 = 0;	P6M0 = 0;	//设置为准双向口
	P7M1 = 0;	P7M0 = 0;	//设置为准双向口
	Timer0Init();	//1毫秒@12.000MHz
	Initialize_LCD();//LCD初始化函数
	ClearLine(0);
	ClearLine(1);
	PutString(0,0,"STC15W4K");
	WriteROM();			   // 将自定义字形码写入LCD1602内部存储器
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
		{//每隔1秒翻转一次
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

void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = T1MS ;		//设置定时初始值
	TH0 = T1MS >>8;		//设置定时初始值
//	TL0 = 0x18;		//设置定时初始值
//	TH0 = 0xFC;		//设置定时初始值
	
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;     //enable timer0 interrupt
  EA = 1;      //open global interrupt switch
}

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1
{
//	TL0 = 0x18;		//设置定时初始值
//	TH0 = 0xFC;		//设置定时初始值
    TL0 = T1MS;                     //reload timer0 low byte
    TH0 = T1MS >> 8;                //reload timer0 high byte
	sysRunmillis ++;
}


//========================================================================
// 函数: void  delay_ms(u8 ms)
// 描述: 延时函数。
// 参数: ms,要延时的ms数, 这里只支持1~255ms. 自动适应主时钟.
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
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




/************* LCD1602相关程序	*****************************************************/
//8位数据访问方式	LCD1602		标准程序	梁工编写	2014-2-21

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
// 函数: void	CheckBusy(void)
// 描述: 检测忙函数
// 参数: none.
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
//========================================================================
void	CheckBusy(void)
{
	u16	i;
	for(i=0; i<5000; i++)	{if(!LCD_B7)	break;}		//check the LCD busy or not. With time out
//	while(LCD_B7);			//check the LCD busy or not. Without time out
}

//========================================================================
// 函数: void IniSendCMD(u8 cmd)
// 描述: 初始化写命令(不检测忙)
// 参数: cmd: 要写的命令.
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
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
// 函数: void Write_CMD(u8 cmd)
// 描述: 写命令(检测忙)
// 参数: cmd: 要写的命令.
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
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
// 函数: void Write_DIS_Data(u8 dat)
// 描述: 写显示数据(检测忙)
// 参数: dat: 要写的数据.
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
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

unsigned char *uchartostr(unsigned num)	//将一个字节的数据转换为字符串 或10进制
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
// 函数: void Initialize_LCD(void)
// 描述: 初始化函数
// 参数: none.
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
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
// 函数: void ClearLine(u8 row)
// 描述: 清除1行
// 参数: row: 行(0或1)
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
//========================================================================
void ClearLine(u8 row)
{
	u8 i;
	Write_CMD(((row & 1) << 6) | 0x80);
	for(i=0; i<LineLength; i++)	Write_DIS_Data(' ');
}

//========================================================================
// 函数: void	WriteChar(u8 row, u8 column, u8 dat)
// 描述: 指定行、列和字符, 写一个字符
// 参数: row: 行(0或1),  column: 第几个字符(0~15),  dat: 要写的字符.
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
//========================================================================
void	WriteChar(u8 row, u8 column, u8 dat)
{
	Write_CMD((((row & 1) << 6) + column) | 0x80);
	Write_DIS_Data(dat);
}

//========================================================================
// 函数: void PutString(u8 row, u8 column, u8 *puts)
// 描述: 写一个字符串，指定行、列和字符串首地址
// 参数: row: 行(0或1),  column: 第几个字符(0~15),  puts: 要写的字符串指针.
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
//========================================================================
void PutString(u8 row, u8 column, u8 *puts)
{
	Write_CMD((((row & 1) << 6) + column) | 0x80);
	for ( ;  *puts != 0;  puts++)		//遇到停止符0结束
	{
		Write_DIS_Data(*puts);
		if(++column >= LineLength)	break;
	}
}
void WriteROM()//LCD1602内部数据寄存器写入函数
{
		unsigned char i;
	Write_CMD(0x40);			 // 操作CGRAM的命令码
	for(i=0;i<64;i++)            // 写入数组中数据  
	{
		Write_DIS_Data(table[i]);
	}
}

//******************** LCD20 Module END ***************************
//****************************************************
//蜂鸣器程序
//****************************************************
void Buzzer_Di(void){
	Buzzer = 0;
	delay_ms(3);
	Buzzer = 1;
	delay_ms(3);
}