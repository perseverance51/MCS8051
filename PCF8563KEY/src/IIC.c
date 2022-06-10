#include <reg52.h>
#include <intrins.h>
#include "IIC.h"

//extern unsigned char   pcftmp[7];
//定义变量

SYSTEMTIME PCF8563Buffer;
bit Flag_Time_Refresh = 1;

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
	_nop_();
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

void iic_send_add_byte(unsigned char add,unsigned char bat) //向某个地址发送某数据
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

void ReadData1(unsigned char address,unsigned char count,unsigned char * buff) /*多字节*/
{
    unsigned char i;
    iic_start();
    iic_send_byte(0xa2); /*写命令*/
    iic_ack();
    iic_send_byte(address); /*写地址*/
    iic_ack();
    iic_start();
    iic_send_byte(0xa3); /*读命令*/
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
输入数据<-pcf8563
********************************************/
/*
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
*/
/********************************************
内部函数，输出ACK ,每个字节传输完成，输出ack=0,结束读书据，ack=1;
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
内部函数，等待ACK
********************************************/
/*
void WaitACK()
{
		unsigned char errtime=20;
   		SDA=1;
   		delay();  //读ACK
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
			iic_send_byte(0xa2); //写命令
		WriteACK(0);
       iic_send_byte(0x02);//获取秒
		WriteACK(0);
         iic_start();
       iic_send_byte(0xa3); //读命令
				WriteACK(0);
        	ReadValue = Readbyte();
       
		ReadValue  = ((ReadValue >> 4) & 0x07) * 10 + (ReadValue & 0x0f);//秒
	if(ReadValue != PCF8563Buffer.Second)		//跟上一次状态不同，置位刷新标志位
		Flag_Time_Refresh = 1;
		Time->Second = ReadValue;//获取秒
			WriteACK(0);
	
     ReadValue  =Readbyte();//获取分有效值
     Time->Minute = ((ReadValue >> 4) & 0x07) * 10 + (ReadValue & 0x0f);
			WriteACK(0); 
	
        ReadValue = Readbyte();//获取时有效值
				Time->Hour = ((ReadValue >> 4) & 0x03) * 10 + (ReadValue & 0x0f);
         	WriteACK(0); 
		
				ReadValue = 0x03f&Readbyte();//获取日期有效值
        Time->Day =(ReadValue/16*10)  + (ReadValue%16);//获取日期	
				WriteACK(0);
	
					ReadValue = Readbyte();//获取周
        Time->Week =0x07&ReadValue;//获取周有效值
				WriteACK(0);
        
				ReadValue  =0x1f&Readbyte();//获取月有效值
       Time->Month =(ReadValue/16*10)  + (ReadValue%16);
				WriteACK(0);      
				ReadValue  =0xff&Readbyte();//获取年

				Time->Year  = (ReadValue/16*10)  + (ReadValue%16);;
          WriteACK(1);
     	   iic_stop();

}
