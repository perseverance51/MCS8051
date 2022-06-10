
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
uchar flags = 1; //1�붨ʱ��־
static uchar cnt = 0;  //������¼�жϴ��������õ��ж϶�ʱΪ1ms���ж�200�μ�һ����
unsigned char *weeklist[7]= {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

unsigned char write_buff[8],read_buff[8];


//���� IO����
sbit KEY1 = P3^2;
sbit KEY2 = P3^3;
sbit KEY3 = P3^4;
sbit KEY4 = P3^5;

//****************************************************
//����������
//****************************************************
void Buzzer_Di()
{
	Buzzer = 0;
	Delay_ms(3);
	Buzzer = 1;
	Delay_ms(3);
}

//-----------ʱ��Ԥ�趨ֵ---------
/*
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
*/
//----------------PCF8563��ʼ��-----------------
void pcf_init()
{
    iic_send_add_byte(0x00,0x00);  //����ʱ��
}

//----------------BCDת10����-----------
//uchar bcd_dec(uchar bat)
//{
//    return ( (bat/16*10) + (bat%16) );
//}
//------------LCD1602��ʾ����,��,��,Сʱ,����,��,
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

    LCD1602_WriteCom(0xc0 + 4);//�ڶ��е�����
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
//����ɨ�����
//****************************************************
void Scan_Key()
{
	if( KEY1 == 0 )	 					//����1ɨ��
	{
		Delay_ms(10);					//��ʱȥ��
		if( KEY1 == 0 )
		{
			Buzzer_Di();	//����������
			while(KEY1 == 0);			//�ȴ�����
			KEY_NUM = 1;
		}
	}

	if( KEY2 == 0 )						//����2ɨ��
	{
		Delay_ms(10);					//��ʱȥ��
		if( KEY2 == 0 )
		{
			Buzzer_Di();
			while(KEY2 == 0);			//�ȴ�����
			KEY_NUM = 2;
		}
	}

	if( KEY3 == 0 )						//����2ɨ��
	{
		Delay_ms(10);					//��ʱȥ��
		if( KEY3 == 0 )
		{
			Buzzer_Di();
			while(KEY3 == 0);			//�ȴ�����
			KEY_NUM = 3;
		}
	}

	if( KEY4 == 0 )						//����2ɨ��
	{
		Delay_ms(10);					//��ʱȥ��
		if( KEY4 == 0 )
		{
			Buzzer_Di();
			while(KEY4 == 0);			//�ȴ�����
			KEY_NUM = 4;
		}
	}
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
   // uchar sec,min,hour,day,mon,year,week,sec1,min1,hour1,day1,mon1,year1,*week1;
    //PCF8563�����ı����ʹ�ת���ɵ�ʮ���Ʊ���
	uchar SetVal;
    TimeInit();
    LCD1602_Init();
//  iic_send_add_byte(0x00,0x20);  // �ر�ʱ��
//   Delay_ms(1000);
//   time_init();    //ʱ��оƬ��ʼʱ������
    pcf_init();
    Delay_ms(1000);
    while(1)
    {
        if(flags==1&& Flag_KEY_Set == 0) {
					Flag_Time_Refresh = 0;
					ReadRTC(&PCF8563Buffer);
					display_time();   //LCD1602��ʾʱ��
            flags=0;
        }
       Scan_Key();						//����ɨ��
						switch( KEY_NUM )
		{
			case 1:						//����1ִ�г���
					KEY_NUM = 0;		//��հ�����־
					Flag_KEY_Set = ~Flag_KEY_Set;	//���ð�����־λ��ת
					if(Flag_KEY_Set)
					{						
						LCD1602_WriteCom(0x0f);	//LCD1602ָ����˸��0d��0f(���»���)
						LCD1602_WriteCom(0x80 + 4);	//ָ�����õ���һ�е�4��
						KEY2_Count = 0;				//����S2��������
					}
					else
					{
						LCD1602_WriteCom(0x0c);	//�˳����ã�LCD1602ָ����˸��
					}
					break;
			case 2:						//����2ִ�г���
					KEY_NUM = 0;		//��հ�����־
					if(Flag_KEY_Set)			//������״̬ʱ����
					{
						KEY2_Count++;
						if(KEY2_Count == 7)
							KEY2_Count = 0;
						switch(KEY2_Count)
						{
							case 0:
									LCD1602_WriteCom(0x80 + 4);	//ָ��ָ���꣬��˸
									break;
							case 1:
									LCD1602_WriteCom(0x80 + 7);	//ָ��ָ���£���˸
									break;
							case 2:
									LCD1602_WriteCom(0x80+ 10);	//ָ��ָ���գ���˸
									break;
							case 3:
									LCD1602_WriteCom(0x80 + 14);	//ָ��ָ�����ڣ���˸
									break;
							case 4:
									LCD1602_WriteCom(0xc0 + 5);	//ָ��ָ��Сʱ����˸
									break;
							case 5:
									LCD1602_WriteCom(0xc0 + 8);	//ָ��ָ����ӣ���˸
									break;
							case 6:
									LCD1602_WriteCom(0xc0 + 11);	//ָ��ָ�����ӣ���˸
									break;
							default : break;
						}	
					}					
					break;//�л�����λ��
			case 3:						//����3ִ�г���
					KEY_NUM = 0;		//��հ�����־
					if(Flag_KEY_Set)			//������״̬ʱ����
					{
						switch(KEY2_Count)
						{
							case 0:
									PCF8563Buffer.Year++;
									if(PCF8563Buffer.Year >= 99)
									{
										PCF8563Buffer.Year = 0;
									}
									SetVal=  ((PCF8563Buffer.Year/ 10) << 4) +(PCF8563Buffer.Year % 10);//��
									iic_send_add_byte(0x08, 	SetVal);//��������תBCD��
									display_time();
									
									LCD1602_WriteCom(0x80 + 4);	//ָ��ָ���꣬��˸
									break;
							case 1:
									PCF8563Buffer.Month++;
									if(PCF8563Buffer.Month >= 13)
									{
										PCF8563Buffer.Month = 1;
									}
									iic_send_add_byte(0x07, PCF8563Buffer.Month );
									display_time();
	
									LCD1602_WriteCom(0x80 + 7);	//ָ��ָ���£���˸
									break;
							case 2:
									PCF8563Buffer.Day++;
									if(PCF8563Buffer.Day >= 32)
									{
										PCF8563Buffer.Day = 1;
									}
									iic_send_add_byte( 0x05, PCF8563Buffer.Day );
									display_time();
	
									LCD1602_WriteCom(0x80 + 10);	//ָ��ָ���գ���˸
									break;
							case 3:
									PCF8563Buffer.Week++;
									if(PCF8563Buffer.Week >= 8)
									{
										PCF8563Buffer.Week = 1;
									}
									iic_send_add_byte(0x06, PCF8563Buffer.Week );
									display_time();
	
									LCD1602_WriteCom(0x80 + 14);	//ָ��ָ�����ڣ���˸
									break;
							case 4:
									PCF8563Buffer.Hour++;
									if(PCF8563Buffer.Hour >= 24)
									{
										PCF8563Buffer.Hour = 0;
									}
									iic_send_add_byte(0x04, PCF8563Buffer.Hour );
									display_time();
	
									LCD1602_WriteCom(0xc0 + 5);	//ָ��ָ��Сʱ����˸
									break;
							case 5:
									PCF8563Buffer.Minute++;
									if(PCF8563Buffer.Minute >= 60)
									{
										PCF8563Buffer.Minute = 0;
									}
									SetVal =  ((PCF8563Buffer.Minute/ 10) << 4) +(PCF8563Buffer.Minute % 10);//BCDת�룬����
									iic_send_add_byte( 0x03, SetVal );
									display_time();
	
									LCD1602_WriteCom(0xc0 + 8);	//ָ��ָ����ӣ���˸
									break;
							case 6:
									PCF8563Buffer.Second++;
									if(PCF8563Buffer.Second >= 60)
									{
										PCF8563Buffer.Second = 0;
									}
									SetVal =  ((PCF8563Buffer.Second/ 10) << 4) +(PCF8563Buffer.Second % 10);//BCDת�룬����
									iic_send_add_byte(0x02, SetVal);
									display_time();
	
									LCD1602_WriteCom(0xc0 + 11);	//ָ��ָ�����ӣ���˸
									break;
							default : LCD1602_WriteCom(0x0c);	break;//�ر���˸
						}
					}
					break;
			case 4:						//����4ִ�г���
					KEY_NUM = 0;		//��հ�����־
					if(Flag_KEY_Set)			//������״̬ʱ����
					{
						switch(KEY2_Count)
						{
							case 0:
									PCF8563Buffer.Year--;
									if(PCF8563Buffer.Year == 255)
									{
										PCF8563Buffer.Year = 99;
									}
									SetVal =  ((PCF8563Buffer.Year/ 10) << 4) +(PCF8563Buffer.Year % 10);//��
									iic_send_add_byte(0x08,SetVal);//��������תBCD��
									display_time();
									
									LCD1602_WriteCom(0x80 + 4);	//ָ��ָ���꣬��˸
									break;
							case 1:
									PCF8563Buffer.Month--;
									if(PCF8563Buffer.Month == 0)
									{
										PCF8563Buffer.Month = 12;
									}
									iic_send_add_byte(0x07 , PCF8563Buffer.Month );
									display_time();
	
									LCD1602_WriteCom(0x80 + 6);	//ָ��ָ���£���˸
									break;
							case 2:
									PCF8563Buffer.Day--;
									if(PCF8563Buffer.Day == 0)
									{
										PCF8563Buffer.Day = 31;
									}
									iic_send_add_byte( 0x05, PCF8563Buffer.Day );
									display_time();
	
									LCD1602_WriteCom(0x80 + 10);	//ָ��ָ���գ���˸
									break;
							case 3:
									PCF8563Buffer.Week--;
									if(PCF8563Buffer.Week == 0)
									{
										PCF8563Buffer.Week = 7;
									}
									iic_send_add_byte( 0x06, PCF8563Buffer.Week );
							display_time();   //LCD1602��ʾʱ��
	
									LCD1602_WriteCom(0x80 + 14);	//ָ��ָ�����ڣ���˸
									break;
							case 4:
									PCF8563Buffer.Hour--;
									if(PCF8563Buffer.Hour == 255)
									{
										PCF8563Buffer.Hour = 23;
									}
									iic_send_add_byte(0x04, PCF8563Buffer.Hour );
									display_time();  //LCD1602��ʾʱ��
	
									LCD1602_WriteCom(0xc0 + 5);	//ָ��ָ��Сʱ����˸
									break;
							case 5:
									PCF8563Buffer.Minute--;
									if(PCF8563Buffer.Minute == 255)
									{
										PCF8563Buffer.Minute = 59;
									}
									SetVal =  ((PCF8563Buffer.Minute/ 10) << 4) +(PCF8563Buffer.Minute % 10);//BCDת�룬����
									iic_send_add_byte( 0x03, SetVal );
									display_time();  //LCD1602��ʾʱ��
	
									LCD1602_WriteCom(0xc0 + 8);	//ָ��ָ����ӣ���˸
									break;
							case 6:
									PCF8563Buffer.Second--;
									if(PCF8563Buffer.Second == 255)
									{
										PCF8563Buffer.Second = 59;
									}
									SetVal =  ((PCF8563Buffer.Second/ 10) << 4) +(PCF8563Buffer.Second % 10);//BCDת�룬����
									iic_send_add_byte(0x02, SetVal);
								
									display_time();
	
									LCD1602_WriteCom(0xc0 + 11);	//ָ��ָ�����ӣ���˸
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

    TH0=(65536-5000)/256;  //����λ�Ĵ�����ֵ��//5000us=5ms
    TL0=(65536-5000)%256;  //����λ�Ĵ�����ֵ��
    cnt++;                                     //�жϴ�������ֵ��1
    if(cnt>=200)                                 //�жϴ����ﵽ200�μ�Ϊ1��
    {
        cnt = 0;                               //���㣬���¼�¼�жϴ���
        flags = 1;                             //����1�붨ʱ��־λ1
    }
}
