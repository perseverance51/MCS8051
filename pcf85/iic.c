#include <reg52.h>
#include <intrins.h>
#include "IIC.h"

void delay()    //略微延时  6us约>4.7us
{
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}

void iic_start()   //启动信号
{

    SDA=1;
    SCL=1;
    delay();
    SDA=0;
    delay();
    SCL=0;

}

void iic_stop()  //停止信号
{
    SDA=0;
    SCL=1;
    delay();
    SDA=1;
    delay();
    SCL=0;
}

void iic_ack()   //应答信号
{
    unsigned char i=0;
    SCL=1;
    delay();
    while((SDA==1)&&(i<255))
        i++;
    SCL=0;
    delay();
}

void iic_send_byte(unsigned char bat)  //发送数据
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

unsigned char iic_rev()  //接受数据
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

void iic_send_add_byte(unsigned char add,unsigned char bat) //向某个地址发送某数据
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

unsigned char iic_rec_add_byte(unsigned char add) //从某个地址读出数据
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
//	 WriteACK(1);//proteus仿真时添加，必须添加此应答信号
    iic_stop();
    return temp;
}
/********************************************
内部函数.输入数据
出口:B
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

void ReadData1(unsigned char address,unsigned char count,unsigned char * buff) /*多字节读取*/
{
    unsigned char i;
    iic_start();
    iic_send_byte(0xa2); /*写命令*/
//    iic_ack();
	WriteACK(0);/*应答信号拉低，表示接收成功，Proteus仿真必须要有这个信号*/
    iic_send_byte(address); /*写地址*/
//    iic_ack();
	WriteACK(0);
    iic_start();
    iic_send_byte(0xa3); /*读命令*/
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
输入数据<-pcf8563
********************************************/
unsigned char ReadData(unsigned char address) //单字节
{
    unsigned char rdata;
    iic_start();
    iic_send_byte(0xa2); //写命令
    iic_send_byte(address); //写地址
    iic_ack();
    iic_start();
    iic_send_byte(0xa3); //读命令
    iic_ack();
    rdata=Readbyte();
    WriteACK(1);
    iic_stop();
    return(rdata);
}

/********************************************
内部函数，输出ACK ,每个字节传输完成，输出ack=0,结束读数据，ack=1;
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
内部函数，等待ACK
********************************************/
void WaitACK()
{
    unsigned char errtime=20;
    SDA=1;
    delay(); /*读ACK*/
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
    iic_send_byte(0xa2); //写命令
    WriteACK(0);
    iic_send_byte(0x02);
    WriteACK(0);
    iic_start();
    iic_send_byte(0xa3); //读命令
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