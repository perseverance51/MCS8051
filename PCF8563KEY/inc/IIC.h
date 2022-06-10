#ifndef __IIC_H__
#define __IIC_H__

#include "IIC.h"
#include <reg52.h>
#include <intrins.h>



sbit SDA=P3^1;   //SDA������
sbit SCL=P3^0;   //SCL������

//�궨��
#define DS1302_SECOND	0x02
#define DS1302_MINUTE	0x03
#define DS1302_HOUR		0x04 
#define DS1302_DAY		0x05
#define DS1302_MONTH	0x07
#define DS1302_WEEK		0x06
#define DS1302_YEAR		0x08 

//unsigned char  year,month,date,week,hour,minute,second; 
typedef struct __SYSTEMTIME__
{
	unsigned char Second;
	unsigned char Minute;
	unsigned char Hour;
	unsigned char Week;
	unsigned char Day;
	unsigned char Month;
	unsigned char  Year;
}SYSTEMTIME;	//�����ʱ������

void delay();    //��΢��ʱ  6usԼ>4.7us
void iic_start();   //�����ź�	
void iic_stop();  //ֹͣ�ź�
void iic_ack();   //Ӧ���ź�
void iic_send_byte(unsigned char bat);  //��������
unsigned char iic_rev();  //��������
void iic_send_add_byte(unsigned char add,unsigned char bat); //��ĳ����ַ����ĳ����
unsigned char iic_rec_add_byte(unsigned char add); //��ĳ����ַ��������
void ReadData1(unsigned char address,unsigned char count,unsigned char * buff); /*���ֽ�*/
//unsigned char ReadData(unsigned char address); //���ֽ�
void WriteACK( unsigned char ack);
//void WaitACK();
void  ReadRTC(SYSTEMTIME *Time);
extern SYSTEMTIME PCF8563Buffer;
extern bit Flag_Time_Refresh;

#endif

