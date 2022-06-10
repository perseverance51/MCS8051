/************************************************
文件：PCF8563.h
用途：PCF8563函数头文件
注意：系统时钟8M
************************************************/
#include "config.h"
unsigned char syserr;
unsigned char *week_list[7]={
	 				 		    "  Sunday ",
			 					"  Monday ",
			 					" Tuesday ",
								"Wednesday",
			 					"Thursday ",
			 					" Friday  ",
			 					"Saturday "
};
unsigned char write_buff[8],read_buff[8],time_tmp[]="00:00:00",data_tmp[]="2008.01.01",*week_tmp;
unsigned char time[7]={
		 	  		   0x40/*秒*/,
					   0x59/*分*/,
					   0x23/*时*/,
					   0x29/*天*/,
					   0x05/*星期*/,
					   0x02/*月/世纪*/,
					   0x08/*年*/
};
/*************************************************************************
** 函数名称: clear(unsigned char *p,unsigned char num)
** 功能描述: 清除指定区域指定长度的数据
** 输　入: unsigned char *p :起始地址   unsigned char num :零数据长度
** 输出	 : 
** 全局变量: 
** 调用模块: 
** 说明：
** 注意：
**************************************************************************/
void clear(unsigned char *p,unsigned char num)
{
 for(;num>0;num--)
 	{
	 *p=0;
	 p++;
	}
}
/*************************************************************************
** 函数名称: PCF8536_wt(unsigned int add,unsigned char data)
** 功能描述: 向PCF8563指定地址写入一条数据
** 输　入: unsigned int add    ：高八位为器件地址，低八位为内部寄存器地址
   		   unsigned char data  ：需要写入的数据
** 输出	 : 
** 全局变量: 
** 调用模块: 
** 说明：
** 注意：
**************************************************************************/
void PCF8536_wt(unsigned int add,unsigned char data)
{
 unsigned char t;
 t=add>>8;
 t<<=1;
 i2cstart();
 if(i2cwt(W_ADD_COM+t)==SLAW)
 	{
	 i2cwt(add);
	 i2cwt(data);
	}
 else syserr=ERR_SLAW;
 i2cstop();
}
/*************************************************************************
** 函数名称: PCF8536_wt_p(unsigned int add,unsigned char *p,unsigned char num)
** 功能描述: 向PCF8563地址连续的寄存器写入系列数据
** 输　入: unsigned int add    ：高八位为器件地址，低八位为内部寄存器地址
   		   unsigned char *p    ：需要写入的数据域的起始地址
		   unsigned char num   ：写入数据的个数
** 输出	 : 
** 全局变量: 
** 调用模块: 
** 说明：写入数据区域为地址连续的寄存器
** 注意：
**************************************************************************/
void PCF8536_wt_p(unsigned int add,unsigned char *p,unsigned char num)
{
 unsigned char t;
 t=add>>8;
 t<<=1;
 i2cstart();
 if(i2cwt(W_ADD_COM+t)==SLAW)
 	{
	 i2cwt(add);
	 for(;num>0;num--)
	 	{
		 i2cwt(*p);
		 p++;
		 _NOP();
		}
	}
 else syserr=ERR_SLAW;
 i2cstop();
}
/*************************************************************************
** 函数名称: PCF8536_rd(unsigned int add,unsigned char *p,unsigned char num)
** 功能描述: 读PCF8563
** 输　入: unsigned int add    ：高八位为器件地址，低八位为内部寄存器地址
   		   unsigned char *p    ：读出的数据存放地址的起始地址
		   unsigned char num   ：读出数据的个数
** 输出	 : 
** 全局变量: 
** 调用模块: 
** 说明：
** 注意：
**************************************************************************/
void PCF8536_rd(unsigned int add,unsigned char *p,unsigned char num)
{
 unsigned char t;
 t=add>>8;
 t<<=1;
 i2cstart();
 if(i2cwt(W_ADD_COM+t)==SLAW)
 	{
	 i2cwt(add);
	}
 else syserr=ERR_SLAW;
 
 i2cstart();
 if(i2cwt(R_ADD_COM+t)==SLAW)
 	{
	 i2cwt(add);
	}
 else syserr=ERR_SLAW;
 
 for(;num>0;num--)
 	{
	 *p=i2crd();
	 p++;
	} 
}
/*************************************************************************
** 函数名称: PCF8563_init(void)
** 功能描述: PCF8563初始化
** 输　入: 
** 输出	 : 
** 全局变量: 
** 调用模块: 
** 说明：
** 注意：
**************************************************************************/
void PCF8563_init(void)
{
 clear(write_buff,8);
 clear(read_buff,8);
 
 PCF8536_wt(0x00,0x20);//写寄存器1，停止计时
 //PCF8536_wt_p(0x02,time,7);//设定时间
 PCF8536_wt(0x00,0x00);//写寄存器1，开始计时
}
/*************************************************************************
** 函数名称: Updata_time(void)
** 功能描述: 刷新时间寄存器及相关数组内容
** 输　入: 
** 输出	 : 
** 全局变量: 
** 调用模块: 
** 说明：
** 注意：
**************************************************************************/
/*void Updata_time(void)
{
 PCF8536_rd(0x02,time,7);//读取时间
 
 time[0]=time[0]&0x7F;//秒0x02
 time[1]=time[1]&0x7F;//分0x03
 time[2]=time[2]&0x3F;//时0x04
 time[3]=time[3]&0x3F;//日0x05
 time[4]=time[4]&0x07;//week 0x06
 time[5]=time[5]&0x1F;//月0x07
 //处理时间BCD转10进制
 time_tmp[7]=(time[0]&0x0F)+0x30;
 time_tmp[6]=(time[0]>>4)+0x30;//秒
 time_tmp[4]=(time[1]&0x0F)+0x30;
 time_tmp[3]=(time[1]>>4)+0x30;//分
 time_tmp[1]=(time[2]&0x0F)+0x30;
 time_tmp[0]=(time[2]>>4)+0x30;//时
 //处理日期BCD转10进制
 data_tmp[9]=(time[3]&0x0F)+0x30;
 data_tmp[8]=(time[3]>>4)+0x30;//日
 data_tmp[6]=(time[5]&0x0F)+0x30;
 data_tmp[5]=(time[5]>>4)+0x30;//月
 data_tmp[3]=(time[6]&0x0F)+0x30;
 data_tmp[2]=(time[6]>>4)+0x30;//年
 //处理星期
 week_tmp=week_list[time[4]&0x0F];
}*/