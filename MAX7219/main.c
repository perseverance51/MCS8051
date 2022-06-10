#include <reg52.h>
#include <intrins.h>
#include <string.h>
#include<DS1302.h>

typedef unsigned char u8;
typedef unsigned int u16;

sbit DIN = P2^0;     //���ݴ�������
sbit CS = P2^1;     //Ƭѡ��
sbit CLK = P2^2;    //��λʱ�Ӷ�
/***�ⲿ�жϺͶ�ʱT0�жϱ�־λ****/
u8 volatile State = 1;
u8 volatile flags = 1; //1�붨ʱ��־

unsigned char Date_buff[3] = {0, 0,0};//�������ݴ洢
unsigned char Time_buff[3] = {0,0,0};//ʱ�����ݴ洢
//��ʾ�����֣���ֵΪ10��ʱ����ʾ��-��

/****��ʱ��T0��ʼ��*****/
void TimeInit()
{
    EA = 1;                       //���ж��ܿ���
    EX0=1; //���ⲿ�ж�0
    IT0=1; //�ⲿ�ж�0���ش���
    ET0 = 1;                      //��T0�жϿ���
    TMOD = 0x01;    //���ö�ʱ��������ģʽ1
    TH0=(65536-5000)/256;  //����λ�Ĵ�����ֵ��//5000us=5ms
    TL0=(65536-5000)%256;  //����λ�Ĵ�����ֵ��
    TR0 = 1;               //����ʱ��T0����
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
//д���ݺ���
void write_data(unsigned char addr,unsigned char dat)
{
    unsigned char i;
    CS = 0;        // ��д��ַ��Ƭѡ�õͣ��������ݼ��ص���λ�Ĵ���
    for(i = 0; i < 8; i++) //
    {
        CLK = 0;     //ʱ�����������������ڲ���λ�Ĵ���
        addr <<= 1;    //�����͵ĵ�ַ��ÿ������һ�Σ���λ��ǰ���ͣ�
        DIN = CY;    //������λ����������������Դӽ�λλCY�л�����������λ
        CLK = 1;
        _nop_();
        _nop_();
        CLK = 0;    //�½���ʱ���ݴ�DOUT�Ƴ�
    }
    for(i = 0; i < 8; i++)
    {
        CLK = 0;   //
        dat <<= 1;   //��������
        DIN = CY;
        CLK = 1;
        _nop_();
        _nop_();
        CLK = 0;
    }
    CS = 1;      //CS�����أ���������
}
//��ʼ������
void init_max7221(void)
{
    write_data(0x09,0xff);    //����ģʽ
    write_data(0x0a,0x07);    //���ȿ���
    write_data(0x0b,0x07);    //ɨ������ܵ�λ��
    write_data(0x0c,0x01);     //����ģʽ
}
void main(void)
{

    unsigned char i;
    TimeInit();                   //��ʱ���жϳ�ʼ��
    //	Init_DS1302();//��1302��ʼ��,Proteus����ʱ��Ҫ����ʼע�͵�������ʱ�䲻���ȡϵͳʱ�䡣
    init_max7221();    // ��ʼ��
    delayms(2);
    for(i = 0; i < 8; i++)
    {
        write_data(i+1,10);   // ��ʾ-λ
    }
    delayms(1000);
    while(1) {
        if(flags == 1)            //�ж�1�붨ʱ��־
        {
            UpDate();
            flags = 0;            //��־λ����
        }
        if(State)
            DisTime();
        else DisDate();
    }
}
void UpDate() {
//unsigned char second,minute,hour,day,month,year;      //�ֱ𴢴��硢�֡�Сʱ���գ��£���
    unsigned char ReadValue;   //�����1302��ȡ������
    memset(Time_buff, 0, sizeof(Time_buff));//��Ҫstring.hͷ�ļ�
    ReadValue = ReadSet1302(0x81);   //����Ĵ���������
    Time_buff[0]=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);//����������ת��

    ReadValue = ReadSet1302(0x83);  //�ӷּĴ�����
    Time_buff[1]=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //����������ת��

    ReadValue = ReadSet1302(0x85);  //��ʱ�Ĵ�����
    Time_buff[2]=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //����������ת��

    ReadValue = ReadSet1302(0x87);   //���ռĴ���������
    Date_buff[2]=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //����������ת��

    ReadValue = ReadSet1302(0x89);  //���¼Ĵ�����
    Date_buff[1]=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //����������ת��

    ReadValue = ReadSet1302(0x8d);  //����Ĵ�����
    Date_buff[0]=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //����������ת��
}
void 	DisTime() {

    write_data(1,Time_buff[2]/10);   // ��ʾʱ��λ
    write_data(2,Time_buff[2]%10);    // ��ʾʱ��λ
    write_data(3,10);    // ��ʾ-λ
    write_data(4,Time_buff[1]/10);   // ��ʾʱ��λ
    write_data(5,Time_buff[1]%10);    // ��ʾʱ��λ
    write_data(6,10);    // ��ʾ-
    write_data(7,Time_buff[0]/10);   // ��ʾʱ��λ
    write_data(8,Time_buff[0]%10);    // ��ʾʱ��λ
}

void DisDate() {
    write_data(1,Date_buff[0]/10);   // ��ʾ���λ
    write_data(2,Date_buff[0]%10);    // ��ʾ���λ
    write_data(3,10);    // ��ʾ-λ
    write_data(4,Date_buff[1]/10);   // ��ʾ�¸�λ
    write_data(5,Date_buff[1]%10);    // ��ʾ�µ�λ
    write_data(6,10);    // ��ʾ-
    write_data(7,Date_buff[2]/10);   // ��ʾ�ո�λ
    write_data(8,Date_buff[2]%10);    // ��ʾ�յ�λ

}
/****�жϷ�����*****/
void InterruptTimer0() interrupt 1
{
    static volatile u8 cnt = 0;  //������¼�жϴ��������õ��ж϶�ʱΪ1ms���ж�200�μ�һ����

    TH0=(65536-5000)/256;  //����λ�Ĵ�����ֵ��//5000us=5ms
    TL0=(65536-5000)%256;  //����λ�Ĵ�����ֵ��
    cnt++;                                     //�жϴ�������ֵ��1
    if(cnt>=200)                                 //�жϴ����ﵽ200�μ�Ϊ1��
    {
        cnt = 0;                               //���㣬���¼�¼�жϴ���
        flags = 1;                             //����1�붨ʱ��־λ1
    }
}
/****�жϷ�����*****/
void interint0() interrupt 0
{   //�ⲿ�ж�0����
    State = !State;
}
