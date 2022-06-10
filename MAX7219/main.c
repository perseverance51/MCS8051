#include <reg52.h>
#include <intrins.h>
#include <string.h>
#include<DS1302.h>

typedef unsigned char u8;
typedef unsigned int u16;

sbit DIN = P2^0;     //数据串出引脚
sbit CS = P2^1;     //片选端
sbit CLK = P2^2;    //移位时钟端
/***外部中断和定时T0中断标志位****/
u8 volatile State = 1;
u8 volatile flags = 1; //1秒定时标志

unsigned char Date_buff[3] = {0, 0,0};//日期数据存储
unsigned char Time_buff[3] = {0,0,0};//时间数据存储
//显示的数字，数值为10的时候，显示“-”

/****定时器T0初始化*****/
void TimeInit()
{
    EA = 1;                       //打开中断总开关
    EX0=1; //开外部中断0
    IT0=1; //外部中断0边沿触发
    ET0 = 1;                      //打开T0中断开关
    TMOD = 0x01;    //设置定时器工作在模式1
    TH0=(65536-5000)/256;  //高四位寄存器的值；//5000us=5ms
    TL0=(65536-5000)%256;  //低四位寄存器的值；
    TR0 = 1;               //允许定时器T0工作
}
void UpDate();
void 	DisTime();
void DisDate();
void delayms(unsigned int x)
{
    unsigned char i;
    while(x--)
    {
        for(i = 0; i < 120; i++);
    }
}
//写数据函数
void write_data(unsigned char addr,unsigned char dat)
{
    unsigned char i;
    CS = 0;        // 先写地址，片选置低，串行数据加载到移位寄存器
    for(i = 0; i < 8; i++) //
    {
        CLK = 0;     //时钟上升沿数据移入内部移位寄存器
        addr <<= 1;    //待发送的地址，每次左移一次，高位在前发送，
        DIN = CY;    //数据移位后，如果有溢出，则可以从进位位CY中获得溢出的数据位
        CLK = 1;
        _nop_();
        _nop_();
        CLK = 0;    //下降沿时数据从DOUT移出
    }
    for(i = 0; i < 8; i++)
    {
        CLK = 0;   //
        dat <<= 1;   //发送数据
        DIN = CY;
        CLK = 1;
        _nop_();
        _nop_();
        CLK = 0;
    }
    CS = 1;      //CS上升沿，数据锁存
}
//初始化函数
void init_max7221(void)
{
    write_data(0x09,0xff);    //编码模式
    write_data(0x0a,0x07);    //亮度控制
    write_data(0x0b,0x07);    //扫描数码管的位数
    write_data(0x0c,0x01);     //工作模式
}
void main(void)
{

    unsigned char i;
    TimeInit();                   //定时器中断初始化
    //	Init_DS1302();//将1302初始化,Proteus仿真时需要将初始注释掉，否则时间不会读取系统时间。
    init_max7221();    // 初始化
    delayms(2);
    for(i = 0; i < 8; i++)
    {
        write_data(i+1,10);   // 显示-位
    }
    delayms(1000);
    while(1) {
        if(flags == 1)            //判断1秒定时标志
        {
            UpDate();
            flags = 0;            //标志位清零
        }
        if(State)
            DisTime();
        else DisDate();
    }
}
void UpDate() {
//unsigned char second,minute,hour,day,month,year;      //分别储存苗、分、小时，日，月，年
    unsigned char ReadValue;   //储存从1302读取的数据
    memset(Time_buff, 0, sizeof(Time_buff));//需要string.h头文件
    ReadValue = ReadSet1302(0x81);   //从秒寄存器读数据
    Time_buff[0]=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);//将读出数据转化

    ReadValue = ReadSet1302(0x83);  //从分寄存器读
    Time_buff[1]=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化

    ReadValue = ReadSet1302(0x85);  //从时寄存器读
    Time_buff[2]=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化

    ReadValue = ReadSet1302(0x87);   //从日寄存器读数据
    Date_buff[2]=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化

    ReadValue = ReadSet1302(0x89);  //从月寄存器读
    Date_buff[1]=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化

    ReadValue = ReadSet1302(0x8d);  //从年寄存器读
    Date_buff[0]=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化
}
void 	DisTime() {

    write_data(1,Time_buff[2]/10);   // 显示时高位
    write_data(2,Time_buff[2]%10);    // 显示时低位
    write_data(3,10);    // 显示-位
    write_data(4,Time_buff[1]/10);   // 显示时高位
    write_data(5,Time_buff[1]%10);    // 显示时低位
    write_data(6,10);    // 显示-
    write_data(7,Time_buff[0]/10);   // 显示时高位
    write_data(8,Time_buff[0]%10);    // 显示时低位
}

void DisDate() {
    write_data(1,Date_buff[0]/10);   // 显示年高位
    write_data(2,Date_buff[0]%10);    // 显示年低位
    write_data(3,10);    // 显示-位
    write_data(4,Date_buff[1]/10);   // 显示月高位
    write_data(5,Date_buff[1]%10);    // 显示月低位
    write_data(6,10);    // 显示-
    write_data(7,Date_buff[2]/10);   // 显示日高位
    write_data(8,Date_buff[2]%10);    // 显示日低位

}
/****中断服务函数*****/
void InterruptTimer0() interrupt 1
{
    static volatile u8 cnt = 0;  //用来记录中断次数，设置的中断定时为1ms，中断200次即一秒钟

    TH0=(65536-5000)/256;  //高四位寄存器的值；//5000us=5ms
    TL0=(65536-5000)%256;  //低四位寄存器的值；
    cnt++;                                     //中断次数计数值加1
    if(cnt>=200)                                 //中断次数达到200次即为1秒
    {
        cnt = 0;                               //清零，重新记录中断次数
        flags = 1;                             //设置1秒定时标志位1
    }
}
/****中断服务函数*****/
void interint0() interrupt 0
{   //外部中断0处理
    State = !State;
}
