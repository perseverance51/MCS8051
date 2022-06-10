#include <reg52.h>
#include <intrins.h>
#include "IIC.h"

//extern unsigned char   pcftmp[7];
//�������

SYSTEMTIME PCF8563Buffer;
bit Flag_Time_Refresh = 1;

void delay()    //��΢��ʱ  6usԼ>4.7us
{
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}

void iic_start()   //�����ź�
{

    SDA=1;
	_nop_();
    SCL=1;
    delay();
    SDA=0;
    delay();
    SCL=0;

}

void iic_stop()  //ֹͣ�ź�
{
    SDA=0;
    SCL=1;
    delay();
    SDA=1;
    delay();
    SCL=0;
}

void iic_ack()   //Ӧ���ź�
{
    unsigned char i=0;
    SCL=1;
    delay();
    while((SDA==1)&&(i<255))
        i++;
    SCL=0;
    delay();
}

void iic_send_byte(unsigned char bat)  //��������
{
    unsigned char i,temp;
    temp = bat;
    for(i=0; i<=7; i++)
    {
        temp=temp<<1;
        SCL=0;
        SDA=CY;
        delay();
        SCL=1;
        delay();
    }
    SCL=0;
    delay();
    SDA=1;
    delay();
}

unsigned char iic_rev()  //��������
{
    unsigned char temp,i;
    SCL=0;
    delay();
    SDA=1;
    for(i=0; i<=7; i++)
    {
        SCL=1;
        delay();
        temp=(temp<<1)|SDA;
        SCL=0;
        delay();
    }
    delay();
    return temp;
}

void iic_send_add_byte(unsigned char add,unsigned char bat) //��ĳ����ַ����ĳ����
{
    iic_start();
    iic_send_byte(0xa2);
    WriteACK(0);
    iic_send_byte(add);
    WriteACK(0);
    iic_send_byte(bat);
	 WriteACK(1);
//    iic_ack();
    iic_stop();
}

unsigned char iic_rec_add_byte(unsigned char add) //��ĳ����ַ��������
{
    unsigned char temp;
    iic_start();
    iic_send_byte(0xa2);
    iic_ack();
    iic_send_byte(add);
    iic_ack();
    iic_start();
    iic_send_byte(0xa3);
    iic_ack();
    temp=iic_rev();
    iic_stop();
    return temp;
}
/********************************************
�ڲ�����.��������
����:B
********************************************/
unsigned char Readbyte()
{
    unsigned char i,bytedata=0;
    SDA=1;
    _nop_();
    for(i=0; i<8; i++)
    {
        SCL=1;
        delay();
        bytedata<<=1;
        if(SDA==1)bytedata=bytedata|0x01;
        delay();
        SCL=0;
        delay();
    }
    return (bytedata);
}

void ReadData1(unsigned char address,unsigned char count,unsigned char * buff) /*���ֽ�*/
{
    unsigned char i;
    iic_start();
    iic_send_byte(0xa2); /*д����*/
    iic_ack();
    iic_send_byte(address); /*д��ַ*/
    iic_ack();
    iic_start();
    iic_send_byte(0xa3); /*������*/
    iic_ack();
    for(i=0; i<count; i++)
    {
        buff[i]=Readbyte();
        if(i<count-1)
            WriteACK(0);
    }
    WriteACK(1);
    iic_stop();
}
/********************************************
��������<-pcf8563
********************************************/
/*
unsigned char ReadData(unsigned char address) //���ֽ�
{
	unsigned char rdata;
      	 iic_start();
	  iic_send_byte(0xa2); //д����
	   iic_send_byte(address); //д��ַ
	   iic_ack();
	     	 iic_start();
	   iic_send_byte(0xa3); //������
	   iic_ack();
	    rdata=Readbyte();
	    WriteACK(1);
	   iic_stop();
	    return(rdata);
}
*/
/********************************************
�ڲ����������ACK ,ÿ���ֽڴ�����ɣ����ack=0,��������ݣ�ack=1;
********************************************/
void WriteACK( unsigned char ack)
{
    SDA=ack;
    delay();
    SCL=1;
    delay();
    SCL=0;
}
/********************************************
�ڲ��������ȴ�ACK
********************************************/
/*
void WaitACK()
{
		unsigned char errtime=20;
   		SDA=1;
   		delay();  //��ACK
   		SCL=1;
   		delay();
   		while(SDA)
   		{
				errtime--;
      			if(!errtime)
					 iic_stop();
   		}
   		SCL=0;
   		delay();
}
*/


void  ReadRTC(SYSTEMTIME *Time)
{
		unsigned char ReadValue;
         iic_start();
			iic_send_byte(0xa2); //д����
		WriteACK(0);
       iic_send_byte(0x02);//��ȡ��
		WriteACK(0);
         iic_start();
       iic_send_byte(0xa3); //������
				WriteACK(0);
        	ReadValue = Readbyte();
       
		ReadValue  = ((ReadValue >> 4) & 0x07) * 10 + (ReadValue & 0x0f);//��
	if(ReadValue != PCF8563Buffer.Second)		//����һ��״̬��ͬ����λˢ�±�־λ
		Flag_Time_Refresh = 1;
		Time->Second = ReadValue;//��ȡ��
			WriteACK(0);
	
     ReadValue  =Readbyte();//��ȡ����Чֵ
     Time->Minute = ((ReadValue >> 4) & 0x07) * 10 + (ReadValue & 0x0f);
			WriteACK(0); 
	
        ReadValue = Readbyte();//��ȡʱ��Чֵ
				Time->Hour = ((ReadValue >> 4) & 0x03) * 10 + (ReadValue & 0x0f);
         	WriteACK(0); 
		
				ReadValue = 0x03f&Readbyte();//��ȡ������Чֵ
        Time->Day =(ReadValue/16*10)  + (ReadValue%16);//��ȡ����	
				WriteACK(0);
	
					ReadValue = Readbyte();//��ȡ��
        Time->Week =0x07&ReadValue;//��ȡ����Чֵ
				WriteACK(0);
        
				ReadValue  =0x1f&Readbyte();//��ȡ����Чֵ
       Time->Month =(ReadValue/16*10)  + (ReadValue%16);
				WriteACK(0);      
				ReadValue  =0xff&Readbyte();//��ȡ��

				Time->Year  = (ReadValue/16*10)  + (ReadValue%16);;
          WriteACK(1);
     	   iic_stop();

}
