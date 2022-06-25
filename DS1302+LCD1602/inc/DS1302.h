#ifndef __DS1302_H__
#define __DS1302_H__

#include <reg52.h>

//DS1302 IO设置
sbit DS1302_CE = P1^0;
sbit DS1302_CK = P1^1;
sbit DS1302_IO = P1^2;

typedef struct __SYSTEMTIME__ { //日期时间结构体定义
    unsigned int year; //年
    unsigned char mon; //月
    unsigned char day; //日
    unsigned char hour; //时
    unsigned char min; //分
    unsigned char sec; //秒
    unsigned char week; //星期
}SYSTEMTIME;
//函数或者变量声明
/* 发送一个字节到 DS1302 通信总线上 */
extern void DS1302ByteWrite(unsigned char dat);
/* 由 DS1302 通信总线上读取一个字节 */
extern unsigned char DS1302ByteRead();
/* 用单次写操作向某一寄存器写入一个字节，reg-寄存器地址，dat-待写入字节 */
extern void DS1302SingleWrite(unsigned char reg, unsigned char dat);
/* 用单次读操作从某一寄存器读取一个字节，reg-寄存器地址，返回值-读到的字节 */
extern unsigned char DS1302SingleRead(unsigned char reg);
/* 用突发模式连续写入 8 个寄存器数据，dat-待写入数据指针 */
extern void DS1302BurstWrite(unsigned char *dat);
/* 用突发模式连续读取 8 个寄存器的数据，dat-读取数据的接收指针 */
extern void DS1302BurstRead(unsigned char *dat);
/* 获取实时时间，即读取 DS1302 当前时间并转换为时间结构体格式 */
extern void GetRealTime(SYSTEMTIME *time);
/* 设定实时时间，时间结构体格式的设定时间转换为数组并写入 DS1302 */
extern void SetRealTime(SYSTEMTIME *time);
/* DS1302 初始化，如发生掉电则重新设置初始时间 */
extern void InitDS1302();
extern SYSTEMTIME sTime;




#endif
