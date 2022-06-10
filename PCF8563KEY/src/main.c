
#include <reg52.h>
#include "LCD1602.h"
#include "IIC.h"
#include "delay.h"
#define uchar unsigned char
#define uint unsigned int
	
unsigned char KEY_NUM = 0;
bit Flag_KEY_Set = 0;
unsigned char KEY2_Count = 0;
sbit Buzzer = P3^7;
uchar flags = 1; //1秒定时标志
static uchar cnt = 0;  //用来记录中断次数，设置的中断定时为1ms，中断200次即一秒钟
unsigned char *weeklist[7]= {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

unsigned char write_buff[8],read_buff[8];


//按键 IO设置
sbit KEY1 = P3^2;
sbit KEY2 = P3^3;
sbit KEY3 = P3^4;
sbit KEY4 = P3^5;

//****************************************************
//蜂鸣器程序
//****************************************************
void Buzzer_Di()
{
	Buzzer = 0;
	Delay_ms(3);
	Buzzer = 1;
	Delay_ms(3);
}

//-----------时间预设定值---------
/*
void time_init()
{
    iic_send_add_byte(0x02,0x30);         //0秒
    iic_send_add_byte(0x03,0x26);         //0分钟
    iic_send_add_byte(0x04,0x11);          //0小时
    iic_send_add_byte(0x05,0x04);  //3号
    iic_send_add_byte(0x06,0x02);  //week
    iic_send_add_byte(0x07,0x01);           //1月  20世纪的
    iic_send_add_byte(0x08,0x22);         //2022年
}
*/
//----------------PCF8563初始化-----------------
void pcf_init()
{
    iic_send_add_byte(0x00,0x00);  //启动时钟
}

//----------------BCD转10进制-----------
//uchar bcd_dec(uchar bat)
//{
//    return ( (bat/16*10) + (bat%16) );
//}
//------------LCD1602显示：年,月,日,小时,分钟,秒,
void display_time()
{
		unsigned char *weeklist[7]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    LCD_Write_String(1,0,"20");
    LCD1602_WriteData(PCF8563Buffer.Year/10 +'0');
    LCD1602_WriteData(PCF8563Buffer.Year%10 +'0');
    LCD1602_WriteData('-');
    LCD1602_WriteData(PCF8563Buffer.Month/10 + 0x30);
    LCD1602_WriteData(PCF8563Buffer.Month%10 + 0x30);
    LCD1602_WriteData('-');
    LCD1602_WriteData(PCF8563Buffer.Day/10 + 0x30);
    LCD1602_WriteData(PCF8563Buffer.Day%10 + 0x30);
    LCD_Write_String(13,0,weeklist[PCF8563Buffer.Week -1]);

    LCD1602_WriteCom(0xc0 + 4);//第二行第四列
    LCD1602_WriteData(PCF8563Buffer.Hour/10 + '0');
    LCD1602_WriteData(PCF8563Buffer.Hour%10 + '0');
    LCD1602_WriteData(':');
    LCD1602_WriteData(PCF8563Buffer.Minute/10 + 0x30);
    LCD1602_WriteData(PCF8563Buffer.Minute%10 + 0x30);
    LCD1602_WriteData(':');
    LCD1602_WriteData(PCF8563Buffer.Second/10 + 0x30);
    LCD1602_WriteData(PCF8563Buffer.Second%10 + 0x30);

}
//****************************************************
//按键扫描程序
//****************************************************
void Scan_Key()
{
	if( KEY1 == 0 )	 					//按键1扫描
	{
		Delay_ms(10);					//延时去抖
		if( KEY1 == 0 )
		{
			Buzzer_Di();	//蜂鸣器响声
			while(KEY1 == 0);			//等待松手
			KEY_NUM = 1;
		}
	}

	if( KEY2 == 0 )						//按键2扫描
	{
		Delay_ms(10);					//延时去抖
		if( KEY2 == 0 )
		{
			Buzzer_Di();
			while(KEY2 == 0);			//等待松手
			KEY_NUM = 2;
		}
	}

	if( KEY3 == 0 )						//按键2扫描
	{
		Delay_ms(10);					//延时去抖
		if( KEY3 == 0 )
		{
			Buzzer_Di();
			while(KEY3 == 0);			//等待松手
			KEY_NUM = 3;
		}
	}

	if( KEY4 == 0 )						//按键2扫描
	{
		Delay_ms(10);					//延时去抖
		if( KEY4 == 0 )
		{
			Buzzer_Di();
			while(KEY4 == 0);			//等待松手
			KEY_NUM = 4;
		}
	}
}
void TimeInit()
{
    EA = 1;                       //打开中断总开关
    ET0 = 1;                      //打开T0中断开关
    TMOD = 0x01;    //设置定时器工作在模式1
    TH0=(65536-5000)/256;  //高四位寄存器的值；//5000us=5ms
    TL0=(65536-5000)%256;  //低四位寄存器的值；
    TR0 = 1;                      //允许定时器T0工作
}
void main()
{
   // uchar sec,min,hour,day,mon,year,week,sec1,min1,hour1,day1,mon1,year1,*week1;
    //PCF8563读出的变量和代转换成的十进制变量
	uchar SetVal;
    TimeInit();
    LCD1602_Init();
//  iic_send_add_byte(0x00,0x20);  // 关闭时钟
//   Delay_ms(1000);
//   time_init();    //时钟芯片初始时间设置
    pcf_init();
    Delay_ms(1000);
    while(1)
    {
        if(flags==1&& Flag_KEY_Set == 0) {
					Flag_Time_Refresh = 0;
					ReadRTC(&PCF8563Buffer);
					display_time();   //LCD1602显示时间
            flags=0;
        }
       Scan_Key();						//按键扫描
						switch( KEY_NUM )
		{
			case 1:						//按键1执行程序
					KEY_NUM = 0;		//清空按键标志
					Flag_KEY_Set = ~Flag_KEY_Set;	//设置按键标志位翻转
					if(Flag_KEY_Set)
					{						
						LCD1602_WriteCom(0x0f);	//LCD1602指针闪烁开0d或0f(带下划线)
						LCD1602_WriteCom(0x80 + 4);	//指针设置到第一行第4列
						KEY2_Count = 0;				//按键S2计数清零
					}
					else
					{
						LCD1602_WriteCom(0x0c);	//退出设置，LCD1602指针闪烁关
					}
					break;
			case 2:						//按键2执行程序
					KEY_NUM = 0;		//清空按键标志
					if(Flag_KEY_Set)			//在设置状态时运行
					{
						KEY2_Count++;
						if(KEY2_Count == 7)
							KEY2_Count = 0;
						switch(KEY2_Count)
						{
							case 0:
									LCD1602_WriteCom(0x80 + 4);	//指针指向年，闪烁
									break;
							case 1:
									LCD1602_WriteCom(0x80 + 7);	//指针指向月，闪烁
									break;
							case 2:
									LCD1602_WriteCom(0x80+ 10);	//指针指向日，闪烁
									break;
							case 3:
									LCD1602_WriteCom(0x80 + 14);	//指针指向星期，闪烁
									break;
							case 4:
									LCD1602_WriteCom(0xc0 + 5);	//指针指向小时，闪烁
									break;
							case 5:
									LCD1602_WriteCom(0xc0 + 8);	//指针指向分钟，闪烁
									break;
							case 6:
									LCD1602_WriteCom(0xc0 + 11);	//指针指向秒钟，闪烁
									break;
							default : break;
						}	
					}					
					break;//切换设置位置
			case 3:						//按键3执行程序
					KEY_NUM = 0;		//清空按键标志
					if(Flag_KEY_Set)			//在设置状态时运行
					{
						switch(KEY2_Count)
						{
							case 0:
									PCF8563Buffer.Year++;
									if(PCF8563Buffer.Year >= 99)
									{
										PCF8563Buffer.Year = 0;
									}
									SetVal=  ((PCF8563Buffer.Year/ 10) << 4) +(PCF8563Buffer.Year % 10);//年
									iic_send_add_byte(0x08, 	SetVal);//年存入必须转BCD码
									display_time();
									
									LCD1602_WriteCom(0x80 + 4);	//指针指向年，闪烁
									break;
							case 1:
									PCF8563Buffer.Month++;
									if(PCF8563Buffer.Month >= 13)
									{
										PCF8563Buffer.Month = 1;
									}
									iic_send_add_byte(0x07, PCF8563Buffer.Month );
									display_time();
	
									LCD1602_WriteCom(0x80 + 7);	//指针指向月，闪烁
									break;
							case 2:
									PCF8563Buffer.Day++;
									if(PCF8563Buffer.Day >= 32)
									{
										PCF8563Buffer.Day = 1;
									}
									iic_send_add_byte( 0x05, PCF8563Buffer.Day );
									display_time();
	
									LCD1602_WriteCom(0x80 + 10);	//指针指向日，闪烁
									break;
							case 3:
									PCF8563Buffer.Week++;
									if(PCF8563Buffer.Week >= 8)
									{
										PCF8563Buffer.Week = 1;
									}
									iic_send_add_byte(0x06, PCF8563Buffer.Week );
									display_time();
	
									LCD1602_WriteCom(0x80 + 14);	//指针指向星期，闪烁
									break;
							case 4:
									PCF8563Buffer.Hour++;
									if(PCF8563Buffer.Hour >= 24)
									{
										PCF8563Buffer.Hour = 0;
									}
									iic_send_add_byte(0x04, PCF8563Buffer.Hour );
									display_time();
	
									LCD1602_WriteCom(0xc0 + 5);	//指针指向小时，闪烁
									break;
							case 5:
									PCF8563Buffer.Minute++;
									if(PCF8563Buffer.Minute >= 60)
									{
										PCF8563Buffer.Minute = 0;
									}
									SetVal =  ((PCF8563Buffer.Minute/ 10) << 4) +(PCF8563Buffer.Minute % 10);//BCD转码，分钟
									iic_send_add_byte( 0x03, SetVal );
									display_time();
	
									LCD1602_WriteCom(0xc0 + 8);	//指针指向分钟，闪烁
									break;
							case 6:
									PCF8563Buffer.Second++;
									if(PCF8563Buffer.Second >= 60)
									{
										PCF8563Buffer.Second = 0;
									}
									SetVal =  ((PCF8563Buffer.Second/ 10) << 4) +(PCF8563Buffer.Second % 10);//BCD转码，秒钟
									iic_send_add_byte(0x02, SetVal);
									display_time();
	
									LCD1602_WriteCom(0xc0 + 11);	//指针指向秒钟，闪烁
									break;
							default : LCD1602_WriteCom(0x0c);	break;//关闭闪烁
						}
					}
					break;
			case 4:						//按键4执行程序
					KEY_NUM = 0;		//清空按键标志
					if(Flag_KEY_Set)			//在设置状态时运行
					{
						switch(KEY2_Count)
						{
							case 0:
									PCF8563Buffer.Year--;
									if(PCF8563Buffer.Year == 255)
									{
										PCF8563Buffer.Year = 99;
									}
									SetVal =  ((PCF8563Buffer.Year/ 10) << 4) +(PCF8563Buffer.Year % 10);//年
									iic_send_add_byte(0x08,SetVal);//年存入必须转BCD码
									display_time();
									
									LCD1602_WriteCom(0x80 + 4);	//指针指向年，闪烁
									break;
							case 1:
									PCF8563Buffer.Month--;
									if(PCF8563Buffer.Month == 0)
									{
										PCF8563Buffer.Month = 12;
									}
									iic_send_add_byte(0x07 , PCF8563Buffer.Month );
									display_time();
	
									LCD1602_WriteCom(0x80 + 6);	//指针指向月，闪烁
									break;
							case 2:
									PCF8563Buffer.Day--;
									if(PCF8563Buffer.Day == 0)
									{
										PCF8563Buffer.Day = 31;
									}
									iic_send_add_byte( 0x05, PCF8563Buffer.Day );
									display_time();
	
									LCD1602_WriteCom(0x80 + 10);	//指针指向日，闪烁
									break;
							case 3:
									PCF8563Buffer.Week--;
									if(PCF8563Buffer.Week == 0)
									{
										PCF8563Buffer.Week = 7;
									}
									iic_send_add_byte( 0x06, PCF8563Buffer.Week );
							display_time();   //LCD1602显示时间
	
									LCD1602_WriteCom(0x80 + 14);	//指针指向星期，闪烁
									break;
							case 4:
									PCF8563Buffer.Hour--;
									if(PCF8563Buffer.Hour == 255)
									{
										PCF8563Buffer.Hour = 23;
									}
									iic_send_add_byte(0x04, PCF8563Buffer.Hour );
									display_time();  //LCD1602显示时间
	
									LCD1602_WriteCom(0xc0 + 5);	//指针指向小时，闪烁
									break;
							case 5:
									PCF8563Buffer.Minute--;
									if(PCF8563Buffer.Minute == 255)
									{
										PCF8563Buffer.Minute = 59;
									}
									SetVal =  ((PCF8563Buffer.Minute/ 10) << 4) +(PCF8563Buffer.Minute % 10);//BCD转码，分钟
									iic_send_add_byte( 0x03, SetVal );
									display_time();  //LCD1602显示时间
	
									LCD1602_WriteCom(0xc0 + 8);	//指针指向分钟，闪烁
									break;
							case 6:
									PCF8563Buffer.Second--;
									if(PCF8563Buffer.Second == 255)
									{
										PCF8563Buffer.Second = 59;
									}
									SetVal =  ((PCF8563Buffer.Second/ 10) << 4) +(PCF8563Buffer.Second % 10);//BCD转码，分钟
									iic_send_add_byte(0x02, SetVal);
								
									display_time();
	
									LCD1602_WriteCom(0xc0 + 11);	//指针指向秒钟，闪烁
									break;
							default : break;
						}
					}
					break;
			default : break;
		}	
    }
}
void InterruptTimer0() interrupt 1
{

    TH0=(65536-5000)/256;  //高四位寄存器的值；//5000us=5ms
    TL0=(65536-5000)%256;  //低四位寄存器的值；
    cnt++;                                     //中断次数计数值加1
    if(cnt>=200)                                 //中断次数达到200次即为1秒
    {
        cnt = 0;                               //清零，重新记录中断次数
        flags = 1;                             //设置1秒定时标志位1
    }
}
