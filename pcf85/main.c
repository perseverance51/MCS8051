
#include <reg52.h>
#include "LCD1602.h"
#include "IIC.h"

#define uchar unsigned char
#define uint unsigned int
	
uchar flags = 1; //1�붨ʱ��־
static uchar cnt = 0;  //������¼�жϴ��������õ��ж϶�ʱΪ1ms���ж�200�μ�һ����
unsigned char *weeklist[7]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

unsigned char write_buff[8],read_buff[8];
//-----------��ʱ�����ȶ�-------------
void delay_kk(uint k)
{
    while(k--);
}
//-----------ʱ��Ԥ�趨ֵ---------
void time_init()
{
    iic_send_add_byte(0x02,0x30);         //0��
    iic_send_add_byte(0x03,0x26);         //0����
    iic_send_add_byte(0x04,0x11);          //0Сʱ
    iic_send_add_byte(0x05,0x04);  //3��
	 iic_send_add_byte(0x06,0x02);  //week
    iic_send_add_byte(0x07,0x01);           //1��  20���͵�
    iic_send_add_byte(0x08,0x22);         //2022��
}

//----------------PCF8563��ʼ��-----------------
void pcf_init()
{
    iic_send_add_byte(0x00,0x00);  //����ʱ��
}

//----------------BCDת10����-----------
uchar bcd_dec(uchar bat)
{
//    uchar temp1,temp2,tol;
//    temp1=bat&0x0f;
//    temp2=(bat&0xf0)>>4;
//    tol=temp2*10+temp1;
	 return ( (bat/16*10) + (bat%16) );
    //return tol;
}
//------------LCD1602��ʾ����,��,��,Сʱ,����,��,
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

    LCD1602_WriteCom(0xc0 + 4);//�ڶ��е�����
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
    EA = 1;                       //���ж��ܿ���
    ET0 = 1;                      //��T0�жϿ���
    TMOD = 0x01;    //���ö�ʱ��������ģʽ1
		TH0=(65536-5000)/256;  //����λ�Ĵ�����ֵ��//5000us=5ms
		TL0=(65536-5000)%256;  //����λ�Ĵ�����ֵ��
    TR0 = 1;                      //����ʱ��T0����
}
void main()
{
    uchar sec,min,hour,day,mon,year,week,sec1,min1,hour1,day1,mon1,year1,*week1;
	  //PCF8563�����ı����ʹ�ת���ɵ�ʮ���Ʊ���
   	TimeInit();
			LCD1602_Init();
 //  iic_send_add_byte(0x00,0x20);  // �ر�ʱ��
//   delay_kk(1000);
//   time_init();    //ʱ��оƬ��ʼʱ������
//    pcf_init();
    delay_kk(1000);
    while(1)
    {
		if(flags){
	//		ReadData1(0x02,0x07,read_buff) ;/*���ֽ�*/
	//		 ReadRTC(read_buff);
//			sec = 0x7f&read_buff[0];//��
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
			
        sec=0x7f&iic_rec_add_byte(0x02);    //��ȡ��,������Чλ
        min=0x7f&iic_rec_add_byte(0x03);   //��ȡ����
        hour = 0x3f&iic_rec_add_byte(0x04);  //��ȡСʱ					
        day=0x3f&iic_rec_add_byte(0x05);   //��ȡ����
				week=0x07&iic_rec_add_byte(0x06);    //��ȡ����
        mon=0x1f&iic_rec_add_byte(0x07);  //��ȡ��0x07
        year =0xff&iic_rec_add_byte(0x08);    //��ȡ��0xff&
		    
				sec1=bcd_dec(sec);                   //����ȡ��BCD����ת����ʮ�������Ա�����
        min1=bcd_dec(min);
        hour1=bcd_dec(hour);
        day1=bcd_dec(day);
		     mon1=bcd_dec(mon);				
       year1=bcd_dec(year);		
			week1 = weeklist[week-1];	

    }
	display_time(sec1,min1,hour1,day1,mon1,year1,week1);   //LCD1602��ʾʱ��
	}
}
void InterruptTimer0() interrupt 1
{

		TH0=(65536-5000)/256;  //����λ�Ĵ�����ֵ��//5000us=5ms
		TL0=(65536-5000)%256;  //����λ�Ĵ�����ֵ��
    cnt++;                                     //�жϴ�������ֵ��1
    if(cnt>=200)                                 //�жϴ����ﵽ200�μ�Ϊ1��
    {
        cnt = 0;                               //���㣬���¼�¼�жϴ���
        flags = 1;                             //����1�붨ʱ��־λ1
    }
	}
