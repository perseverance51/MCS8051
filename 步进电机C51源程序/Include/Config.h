//https:	//shop366226707.taobao.com
//QQ:	3065674908

#ifndef _CONFIG_H
#define _CONFIG_H

//五个按键控制步进电机：正转，反转，加1挡，减1档，停止
//上电时电机启动，数码管上显示速度最小档1，加减档位均能通过数码管显示出来，电机采用单双八拍方式
//步进电机转速一共9档，通过按键调节转速 
//数码管显示档位，LED显示正转，反转，停止

/* 通用头文件 */
#include <reg52.h>

/*宏定义*/
#define SpeedShow P0   		//数码管显示接口
#define MotorDriver P2   	//步进电机驱动接口

/* 数据类型重定义 */
typedef unsigned char uchar;      // 8位有符号整型数
typedef unsigned int 	uint;       //16位有符号整型数
typedef unsigned long ulong;      //32位有符号整型数

			
/* IO引脚分配定义 */
sbit KEY1  = P1^2;  //按键1的输入引脚1
sbit KEY2  = P1^3;  //按键2的输入引脚2
sbit KEY3  = P1^4;  //按键3的输入引脚3
sbit KEY4  = P3^5;  //按键4的输入引脚4
sbit KEY5  = P3^6;  //按键4的输入引脚4

sbit FWD_LED = P1^0;  		//正转指示灯
sbit REV_LED = P1^1;  		//反转指示灯

#endif
