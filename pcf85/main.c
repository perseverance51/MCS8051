
#include <reg52.h>
#include "LCD1602.h"
#include "IIC.h"

#define uchar unsigned char
#define uint unsigned int
	
uchar flags = 1; //1秒定时标志
static uchar cnt = 0;  //用来记录中断次数，设置的中断定时为1ms，中断200次即一秒钟
unsigned char *weeklist[7]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

unsigned char write_buff[8],read_buff[8];
//-----------延时用于稳定-------------
void delay_kk(uint k)
{
    while(k--);
}
//-----------时间预设定值---------
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

//----------------PCF8563初始化-----------------
void pcf_init()
{
    iic_send_add_byte(0x00,0x00);  //启动时钟
}

//----------------BCD转10进制-----------
uchar bcd_dec(uchar bat)
{
//    uchar temp1,temp2,tol;
//    temp1=bat&0x0f;
//    temp2=(bat&0xf0)>>4;
//    tol=temp2*10+temp1;
	 return ( (bat/16*10) + (bat%16) );
    //return tol;
}
//------------LCD1602显示：年,月,日,小时,分钟,秒,
void display_time(uchar sec,min,hour,day,mon,year,unsigned char *week)
{
		LCD_Write_String(1,0,"20");
    LCD1602_WriteData(year/10 +'0');
    LCD1602_WriteData(year%10 +'0');
    LCD1602_WriteData('-');
	  LCD1602_WriteData(mon/10 + 0x30);
    LCD1602_WriteData(mon%10 + 0x30);
    LCD1602_WriteData('-');
    LCD1602_WriteData(day/10 + 0x30);
    LCD1602_WriteData(day%10 + 0x30);
	  LCD_Write_String(13,0,week);

    LCD1602_WriteCom(0xc0 + 4);//第二行第四列
    LCD1602_WriteData(hour/10 + '0');
    LCD1602_WriteData(hour%10 + '0');
    LCD1602_WriteData(':');
    LCD1602_WriteData(min/10 + 0x30);
    LCD1602_WriteData(min%10 + 0x30);
    LCD1602_WriteData(':');
    LCD1602_WriteData(sec/10 + 0x30);
    LCD1602_WriteData(sec%10 + 0x30);
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
    uchar sec,min,hour,day,mon,year,week,sec1,min1,hour1,day1,mon1,year1,*week1;
	  //PCF8563读出的变量和代转换成的十进制变量
   	TimeInit();
			LCD1602_Init();
 //  iic_send_add_byte(0x00,0x20);  // 关闭时钟
//   delay_kk(1000);
//   time_init();    //时钟芯片初始时间设置
//    pcf_init();
    delay_kk(1000);
    while(1)
    {
		if(flags){
	//		ReadData1(0x02,0x07,read_buff) ;/*多字节*/
	//		 ReadRTC(read_buff);
//			sec = 0x7f&read_buff[0];//秒
//			min = 0x7f&read_buff[1] ;
//			hour   = 0x3f&read_buff[2];
//			day   = 0x3f&read_buff[3];
//			week = 0x07&read_buff[4];
//			mon   = 0x1f&read_buff[5];
//			year   = 0xff&read_buff[6]; 

//						switch(week-1){
//						case 0: week1="Sun";break;
//						case 6: week1="Sat";break;
//						case 5: week1="Fri";break;
//						case 4: week1="Thu";break;
//						case 3: week1="Wed";break;
//						case 2: week1="Tue";break;
//						case 1: week1="Mon";break;
//					}	
								flags=0;
			
        sec=0x7f&iic_rec_add_byte(0x02);    //读取秒,屏蔽无效位
        min=0x7f&iic_rec_add_byte(0x03);   //读取分钟
        hour = 0x3f&iic_rec_add_byte(0x04);  //读取小时					
        day=0x3f&iic_rec_add_byte(0x05);   //读取天数
				week=0x07&iic_rec_add_byte(0x06);    //读取星期
        mon=0x1f&iic_rec_add_byte(0x07);  //读取月0x07
        year =0xff&iic_rec_add_byte(0x08);    //读取年0xff&
		    
				sec1=bcd_dec(sec);                   //将读取的BCD码秒转换成十进制秒以便运算
        min1=bcd_dec(min);
        hour1=bcd_dec(hour);
        day1=bcd_dec(day);
		     mon1=bcd_dec(mon);				
       year1=bcd_dec(year);		
			week1 = weeklist[week-1];	

    }
	display_time(sec1,min1,hour1,day1,mon1,year1,week1);   //LCD1602显示时间
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
