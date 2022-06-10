#include <reg52.h>
#include <intrins.h>
#include "IIC.h"

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
    unsigned char temp=0,i;
    SCL=0;
    delay();
    SDA=1;
    for(i=0; i<=7; i++)
    {
        SCL=1;
        delay();
			temp<<=1;
			 if(SDA==1)temp = temp|0x01;
			 delay();
        SCL=0;
        delay();
    }
    return temp;
}

void iic_send_add_byte(unsigned char add,unsigned char bat) //��ĳ����ַ����ĳ����
{
    iic_start();
    iic_send_byte(0xa2);
    iic_ack();
    iic_send_byte(add);
    iic_ack();
    iic_send_byte(bat);
    iic_ack();
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
	iic_ack();
//	 WriteACK(1);//proteus����ʱ��ӣ�������Ӵ�Ӧ���ź�
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

void ReadData1(unsigned char address,unsigned char count,unsigned char * buff) /*���ֽڶ�ȡ*/
{
    unsigned char i;
    iic_start();
    iic_send_byte(0xa2); /*д����*/
//    iic_ack();
	WriteACK(0);/*Ӧ���ź����ͣ���ʾ���ճɹ���Proteus�������Ҫ������ź�*/
    iic_send_byte(address); /*д��ַ*/
//    iic_ack();
	WriteACK(0);
    iic_start();
    iic_send_byte(0xa3); /*������*/
//    iic_ack();
	WriteACK(0);
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

/********************************************
�ڲ����������ACK ,ÿ���ֽڴ�����ɣ����ack=0,���������ݣ�ack=1;
********************************************/
void WriteACK( bit ack)
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
void WaitACK()
{
    unsigned char errtime=20;
    SDA=1;
    delay(); /*��ACK*/
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
void  ReadRTC(unsigned char * buff)
{
    iic_start();
    iic_send_byte(0xa2); //д����
    WriteACK(0);
    iic_send_byte(0x02);
    WriteACK(0);
    iic_start();
    iic_send_byte(0xa3); //������
    WriteACK(0);
    buff[0] =Readbyte();
    WriteACK(0);
    buff[1] =Readbyte();
    WriteACK(0);
    buff[2]   =Readbyte();
    WriteACK(0);
    buff[3]   =Readbyte();
    WriteACK(0);
    buff[4]   =Readbyte();
    WriteACK(0);
    buff[5]   =Readbyte();
    WriteACK(0);
    buff[6]   =Readbyte();
    WriteACK(1);
    iic_stop();
}