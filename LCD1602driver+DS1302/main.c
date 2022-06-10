#include <STC89C5xRC.H>
#include "LCD1602.h"
#include "DS1302.h"

unsigned char KEY_NUM = 0;
bit Flag_KEY_Set = 0;
unsigned char KEY2_Count = 0;
sbit Buzzer = P3^7;

//按键 IO设置
sbit KEY1 = P3^2;
sbit KEY2 = P3^3;
sbit KEY3 = P3^4;
sbit KEY4 = P3^5;
//****************************************************
//显示实时时钟
//****************************************************
void Display_RTCC()
{
	unsigned char *weeklist[7]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
	LCD1602_write_com(0x80);							//指针设置到第一行第1列
	
	LCD1602_prints("20");
	LCD1602_printc(DS1302Buffer.Year/10 + 0x30);	 //显示Year高位
	LCD1602_printc(DS1302Buffer.Year%10 + 0x30);	 //显示Year低位
	LCD1602_printc('-');
	LCD1602_printc(DS1302Buffer.Month/10 + 0x30);	 //显示Month高位
	LCD1602_printc(DS1302Buffer.Month%10 + 0x30);	 //显示Month低位
	LCD1602_printc('-');
	LCD1602_printc(DS1302Buffer.Day/10 + 0x30);		 //显示Day高位
	LCD1602_printc(DS1302Buffer.Day%10 + 0x30);		 //显示Day低位

	LCD1602_write_com(0x80+13);							 //指针设置到第一行第14列
	LCD1602_prints(weeklist[DS1302Buffer.Week -1]);
/*
	switch(DS1302Buffer.Week-1)
	{
		case 1:
				LCD1602_prints("Mon");				 //显示星期一
				break;
		case 2:
				LCD1602_prints("Tue");				 //显示星期二
				break;
		case 3:
				LCD1602_prints("Wed");				 //显示星期三
				break;
		case 4:
				LCD1602_prints("Thu");				 //显示星期四
				break;
		case 5:
				LCD1602_prints("Fri");				 //显示星期五
				break;
		case 6:
				LCD1602_prints("Sat");				 //显示星期六
				break;
		case 7:
				LCD1602_prints("Sun");				 //显示星期日
				break;
		default : break;						
	}		
*/

	LCD1602_write_com(0xc0);				 //指针设置到第二行第1列
	LCD1602_printc(DS1302Buffer.Hour/10 + 0x30);	 //显示Hour高位
	LCD1602_printc(DS1302Buffer.Hour%10 + 0x30);	 //显示Hour低位
	LCD1602_printc(':');
	LCD1602_printc(DS1302Buffer.Minute/10 + 0x30);	 //显示Minute高位
	LCD1602_printc(DS1302Buffer.Minute%10 + 0x30);	 //显示Minute低位
	LCD1602_printc(':');
	LCD1602_printc(DS1302Buffer.Second/10 + 0x30);	 //显示Second高位
	LCD1602_printc(DS1302Buffer.Second%10 + 0x30);	 //显示Second低位	
}
//****************************************************
//蜂鸣器程序
//****************************************************
void Buzzer_Di()
{
	Buzzer = 0;
	LCD1602_delay_ms(3);
	Buzzer = 1;
	LCD1602_delay_ms(3);
}
//****************************************************
//按键扫描程序
//****************************************************
void Scan_Key()
{
	if( KEY1 == 0 )	 					//按键1扫描
	{
		LCD1602_delay_ms(10);					//延时去抖
		if( KEY1 == 0 )
		{
			Buzzer_Di();	//蜂鸣器响声
			while(KEY1 == 0);			//等待松手
			KEY_NUM = 1;
		}
	}

	if( KEY2 == 0 )						//按键2扫描
	{
		LCD1602_delay_ms(10);					//延时去抖
		if( KEY2 == 0 )
		{
			Buzzer_Di();
			while(KEY2 == 0);			//等待松手
			KEY_NUM = 2;
		}
	}

	if( KEY3 == 0 )						//按键2扫描
	{
		LCD1602_delay_ms(10);					//延时去抖
		if( KEY3 == 0 )
		{
			Buzzer_Di();
			while(KEY3 == 0);			//等待松手
			KEY_NUM = 3;
		}
	}

	if( KEY4 == 0 )						//按键2扫描
	{
		LCD1602_delay_ms(10);					//延时去抖
		if( KEY4 == 0 )
		{
			Buzzer_Di();
			while(KEY4 == 0);			//等待松手
			KEY_NUM = 4;
		}
	}
}

void main()
{
    LCD1602_Init();									//初始化LCD1602
    LCD1602_delay_ms(1);
    LCD1602_set_position(0,0);//指针设置到第一行第1列
    LCD1602_prints("Hello World!");
    LCD1602_set_position(0,13);//指针设置到第一行第13列
    LCD1602_prints("STC");
	  LCD1602_delay_ms(500);
  	LCD1602_write_com(0x01);					//清屏指令
		DS1302_Init();									//初始化DS1302
	
    while(1)
    {
			DS1302_GetTime(&DS1302Buffer);							//获取当前RTCC值
	if(Flag_Time_Refresh==1&& Flag_KEY_Set == 0)			//数据更新时才刷新LCD
		{
			Flag_Time_Refresh = 0;
			Display_RTCC();				//显示实时时钟		
		}
			Scan_Key();						//按键扫描
		switch( KEY_NUM )
		{
			case 1:						//按键1执行程序
					KEY_NUM = 0;		//清空按键标志
					Flag_KEY_Set = ~Flag_KEY_Set;	//设置按键标志位翻转
					if(Flag_KEY_Set)
					{						
						LCD1602_write_com(0x0d);	//LCD1602指针闪烁开0d或0f(带下划线)
						LCD1602_write_com(0x80+ 3);	//指针设置到第一行第4列
						KEY2_Count = 0;				//按键S2计数清零
					}
					else
					{
						LCD1602_write_com(0x0c);	//LCD1602指针闪烁关
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
									LCD1602_write_com(0x80+ 3);	//指针指向年，闪烁
									break;
							case 1:
									LCD1602_write_com(0x80+ 6);	//指针指向月，闪烁
									break;
							case 2:
									LCD1602_write_com(0x80+ 9);	//指针指向日，闪烁
									break;
							case 3:
									LCD1602_write_com(0x80 + 15);	//指针指向星期，闪烁
									break;
							case 4:
									LCD1602_write_com(0xc0 + 1);	//指针指向小时，闪烁
									break;
							case 5:
									LCD1602_write_com(0xc0 + 4);	//指针指向分钟，闪烁
									break;
							case 6:
									LCD1602_write_com(0xc0 + 7);	//指针指向秒钟，闪烁
									break;
							default : break;
						}	
					}					
					break;
			case 3:						//按键3执行程序
					KEY_NUM = 0;		//清空按键标志
					if(Flag_KEY_Set)			//在设置状态时运行
					{
						switch(KEY2_Count)
						{
							case 0:
									DS1302Buffer.Year++;
									if(DS1302Buffer.Year >= 100)
									{
										DS1302Buffer.Year = 0;
									}
									DS1302_SetTime( DS1302_YEAR , DS1302Buffer.Year );

									Display_RTCC();
									
									LCD1602_write_com(0x80+ 3);	//指针指向年，闪烁
									break;
							case 1:
									DS1302Buffer.Month++;
									if(DS1302Buffer.Month >= 13)
									{
										DS1302Buffer.Month = 1;
									}
									DS1302_SetTime( DS1302_MONTH , DS1302Buffer.Month );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 6);	//指针指向月，闪烁
									break;
							case 2:
									DS1302Buffer.Day++;
									if(DS1302Buffer.Day >= 32)
									{
										DS1302Buffer.Day = 1;
									}
									DS1302_SetTime( DS1302_DAY , DS1302Buffer.Day );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 9);	//指针指向日，闪烁
									break;
							case 3:
									DS1302Buffer.Week++;
									if(DS1302Buffer.Week >= 8)
									{
										DS1302Buffer.Week = 1;
									}
									DS1302_SetTime( DS1302_WEEK , DS1302Buffer.Week );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 15);	//指针指向星期，闪烁
									break;
							case 4:
									DS1302Buffer.Hour++;
									if(DS1302Buffer.Hour >= 24)
									{
										DS1302Buffer.Hour = 0;
									}
									DS1302_SetTime( DS1302_HOUR , DS1302Buffer.Hour );
									Display_RTCC();
	
									LCD1602_write_com(0xc0 + 1);	//指针指向小时，闪烁
									break;
							case 5:
									DS1302Buffer.Minute++;
									if(DS1302Buffer.Minute >= 60)
									{
										DS1302Buffer.Minute = 0;
									}
									DS1302_SetTime( DS1302_MINUTE , DS1302Buffer.Minute );
									Display_RTCC();
	
									LCD1602_write_com(0xc0 + 4);	//指针指向分钟，闪烁
									break;
							case 6:
									DS1302Buffer.Second++;
									if(DS1302Buffer.Second >= 60)
									{
										DS1302Buffer.Second = 0;
									}
									DS1302_SetTime( DS1302_SECOND , DS1302Buffer.Second);
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 0x40 + 7);	//指针指向秒钟，闪烁
									break;
							default : break;
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
									DS1302Buffer.Year--;
									if(DS1302Buffer.Year == 255)
									{
										DS1302Buffer.Year = 99;
									}
									DS1302_SetTime( DS1302_YEAR , DS1302Buffer.Year );
									Display_RTCC();
									
									LCD1602_write_com(0x80+ 3);	//指针指向年，闪烁
									break;
							case 1:
									DS1302Buffer.Month--;
									if(DS1302Buffer.Month == 0)
									{
										DS1302Buffer.Month = 12;
									}
									DS1302_SetTime( DS1302_MONTH , DS1302Buffer.Month );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 6);	//指针指向月，闪烁
									break;
							case 2:
									DS1302Buffer.Day--;
									if(DS1302Buffer.Day == 0)
									{
										DS1302Buffer.Day = 31;
									}
									DS1302_SetTime( DS1302_DAY , DS1302Buffer.Day );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 9);	//指针指向日，闪烁
									break;
							case 3:
									DS1302Buffer.Week--;
									if(DS1302Buffer.Week == 0)
									{
										DS1302Buffer.Week = 7;
									}
									DS1302_SetTime( DS1302_WEEK , DS1302Buffer.Week );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 15);	//指针指向星期，闪烁
									break;
							case 4:
									DS1302Buffer.Hour--;
									if(DS1302Buffer.Hour == 255)
									{
										DS1302Buffer.Hour = 23;
									}
									DS1302_SetTime( DS1302_HOUR , DS1302Buffer.Hour );
									Display_RTCC();
	
									LCD1602_write_com(0xc0 + 1);	//指针指向小时，闪烁
									break;
							case 5:
									DS1302Buffer.Minute--;
									if(DS1302Buffer.Minute == 255)
									{
										DS1302Buffer.Minute = 59;
									}
									DS1302_SetTime( DS1302_MINUTE , DS1302Buffer.Minute );
									Display_RTCC();
	
									LCD1602_write_com(0xc0 + 4);	//指针指向分钟，闪烁
									break;
							case 6:
									DS1302Buffer.Second--;
									if(DS1302Buffer.Second == 255)
									{
										DS1302Buffer.Second = 59;
									}
									DS1302_SetTime( DS1302_SECOND , DS1302Buffer.Second);
									Display_RTCC();
	
									LCD1602_write_com(0xc0 + 7);	//指针指向秒钟，闪烁
									break;
							default : break;
						}
					}
					break;
			default : break;
		}				
//        LCD1602_set_position(1,0);				 //指针设置到第二行第1列
//        LCD1602_prints("perseverance");
//        LCD1602_set_position(1,14);//指针设置到第二行第14列
//        LCD1602_prints("51");
    }
}