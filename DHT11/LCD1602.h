#ifndef _LCD1602_H
#define _LCD1602_H
		int LCD_init();    //声明初始化函数
    int write_com(unsigned char);//声明写命令函数
    int write_date(unsigned char);//声明写数据函数
    int delay(unsigned char);//声明延迟函数
		void LCD_send(unsigned char,unsigned char,unsigned char[],unsigned char);
#endif