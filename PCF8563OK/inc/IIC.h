#ifndef __IIC_H__
#define __IIC_H__

#include "IIC.h"
#include <reg52.h>
#include <intrins.h>



sbit SDA=P3^1;   //SDA������
sbit SCL=P3^0;   //SCL������

//unsigned char  year,month,date,week,hour,minute,second; 


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
//void  ReadRTC(void);


#endif

