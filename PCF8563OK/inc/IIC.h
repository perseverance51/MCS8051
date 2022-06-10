#ifndef __IIC_H__
#define __IIC_H__

#include "IIC.h"
#include <reg52.h>
#include <intrins.h>



sbit SDA=P3^1;   //SDA口设置
sbit SCL=P3^0;   //SCL口设置

//unsigned char  year,month,date,week,hour,minute,second; 


void delay();    //略微延时  6us约>4.7us
void iic_start();   //启动信号	
void iic_stop();  //停止信号
void iic_ack();   //应答信号
void iic_send_byte(unsigned char bat);  //发送数据
unsigned char iic_rev();  //接受数据
void iic_send_add_byte(unsigned char add,unsigned char bat); //向某个地址发送某数据
unsigned char iic_rec_add_byte(unsigned char add); //从某个地址读出数据
void ReadData1(unsigned char address,unsigned char count,unsigned char * buff); /*多字节*/
//unsigned char ReadData(unsigned char address); //单字节
void WriteACK( unsigned char ack);
//void WaitACK();
//void  ReadRTC(void);


#endif

