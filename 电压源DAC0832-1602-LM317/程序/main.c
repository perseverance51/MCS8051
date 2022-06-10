#include <reg51.h>
#include<string.h>

typedef unsigned char u8; 		// ����������u8�򻯴����д
typedef unsigned int u16; 

#define LCD1602_DATA_PORT	 P2		// LCD1602��8λ���ݶ˿�

sbit gLcd1602_E	 = P3^2;		// LCD1602�������ߵ�ʹ���ź�
sbit gLcd1602_RW = P3^1;		// LCD1602�������ߵĶ�дѡ���ź�
sbit gLcd1602_RS = P3^0;		// LCD1602�������ߵ�����/����ѡ���ź�

sbit Key1 = P3^5;
sbit Key2 = P3^7;

float Output_DA = 200;

void delay10ms(void)   
{
    unsigned char a,b,c;
    for(c=1;c>0;c--)
        for(b=38;b>0;b--)
            for(a=130;a>0;a--);
}

void Lcd1602WaitNoBusy(void)  //æ��⺯�����ж�bit7��0������ִ�У�1��ֹ
{
    unsigned char sta = 0;      //

    LCD1602_DATA_PORT = 0xff;
    gLcd1602_RS = 0;
    gLcd1602_RW = 1;
    do
    {
        gLcd1602_E = 1;
        sta = LCD1602_DATA_PORT;
        gLcd1602_E = 0;    //ʹ�ܣ���������ͣ��ͷ�����
    }while(sta & 0x80);
}

void Lcd1602WriteCmd(unsigned char cmd)	  
{
	Lcd1602WaitNoBusy();		// �ȵȴ�LCD1602���ڲ�æ״̬

	gLcd1602_E = 0;     		// ��ֹLCD
	gLcd1602_RS = 0;	   		// ѡ��������ģʽ
	gLcd1602_RW = 0;	   		// ѡ��д��ģʽ	
	LCD1602_DATA_PORT = cmd;    // ��1�ֽ������ַ���8λ�������ݶ˿�
	gLcd1602_E = 1;	          	// ʹ��LED
	gLcd1602_E = 0;				// ��ֹLCD
}

void Lcd1602WriteData(unsigned char dat)			
{
	Lcd1602WaitNoBusy();		// �ȵȴ�LCD1602���ڲ�æ״̬

	gLcd1602_E = 0;     		// ��ֹLCD
	gLcd1602_RS = 1;	   		// ѡ��������ģʽ
	gLcd1602_RW = 0;	   		// ѡ��д��ģʽ	
	LCD1602_DATA_PORT = dat;    // ��1�ֽ������ַ���8λ�������ݶ˿�
	gLcd1602_E = 1;	          	// ʹ��LED
	gLcd1602_E = 0;				// ��ֹLCD
}

void Lcd1602SetCursor(unsigned char x,unsigned char y)  	// ������ʾ
{
    unsigned char addr = 0;

   	switch (y)
	{
		case 0:	 					// ����һ��
			addr = 0x00 + x;		break;	
		case 1:						// ����һ��
			addr = 0x40 + x; 		break;
		default:
									break;
	}
    Lcd1602WriteCmd(addr | 0x80);
}

void Lcd1602ShowStr(unsigned char x, unsigned char y, unsigned char *pStr)     //��ʾ�ַ���
{
    Lcd1602SetCursor(x, y);      //��ǰ�ַ�������
    while (*pStr != '\0')
    {
        Lcd1602WriteData(*pStr++);
    }
}

void Lcd1602Init(void)						
{
 	Lcd1602WriteCmd(0x38);  	// ���������ֲ�ĳ�ʼ��ʱ���ȷ���38H
	delay10ms();					// ��ʱ10ms
	Lcd1602WriteCmd(0x38);  	// ���������ֲ�ĳ�ʼ��ʱ���ȷ���38H
	delay10ms();					// ��ʱ10ms
	Lcd1602WriteCmd(0x38);  	// ���������ֲ�ĳ�ʼ��ʱ���ȷ���38H
	delay10ms();					// ��ʱ10ms
	Lcd1602WriteCmd(0x38);		// ��ʾģʽ����
	Lcd1602WriteCmd(0x08);		// �ر���ʾ
	Lcd1602WriteCmd(0x01);		// ������ͬʱ������ָ�룩
	Lcd1602WriteCmd(0x06);		// ��д��ָ���Զ���1��д1���ַ���������ʾ���ƶ�
	Lcd1602WriteCmd(0x0c);		// ����ʾ������ʾ���
	
	Lcd1602ShowStr(2,0,"NC ADJ POWER ");	
	Lcd1602ShowStr(0,1,"Voltage:    V");	
}

/*************** ��ʾ���� *************************************************/
void Write_Volt(unsigned char hang,unsigned char add,unsigned int date)	   //������ʾ
{
	if(hang==1)   
		Lcd1602WriteCmd(0x80+add);
	else
	Lcd1602WriteCmd(0x80+0x40+add);
	Lcd1602WriteData(0x30+date%1000/100);	 //��ʾ��λ
	Lcd1602WriteData(0x30+date%100/10);		 //��ʾʮλ
	Lcd1602WriteData('.');					 //��ʾ.
	Lcd1602WriteData(0x30+date%10);			 //��ʾ��λ
	Lcd1602WriteData('V');					 //��ʾ��ĸV
}

void Out_Volt(unsigned char Volt)
{	 	
	P0=Volt;
}

void main()
{
	Lcd1602Init();
	while (1)
	{
		Out_Volt(Output_DA);
		
		if (Key1 == 0)	
		{
			delay10ms();		
			if (Key1 == 0)			
			{
				if(Output_DA < 200) Output_DA += 2;
			}		
			while(!Key1);
		}
		if (Key2 == 0)	
		{
			delay10ms();		
			if (Key2 == 0)			
			{
				if(Output_DA >= 2) Output_DA -= 2;
			}			
			while(!Key2);
		}
		
		Write_Volt(0,8,Output_DA/2.0);
	}	  
}