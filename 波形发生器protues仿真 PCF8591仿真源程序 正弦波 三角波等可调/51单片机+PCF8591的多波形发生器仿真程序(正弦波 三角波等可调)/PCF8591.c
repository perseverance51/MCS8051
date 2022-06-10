#include "PCF8591.h"
#include "delay.h"


sbit SCL=P2^1;  //����ʱ�������  
sbit SDA=P2^0;  //�������������
uchar flag=0;

void start()//��ʼ�ź�  
{  
    SDA=1;  
    delay();  
    SCL=1;  
    delay();  
    SDA=0;  
    delay();  
}  
  
void stop() //ֹͣ�ź�  
{  
    SDA=0;  
    delay();  
    SCL=1;  
    delay();  
    SDA=1;  
    delay();  
}  
  
  
void respons()//Ӧ��    �൱��һ�����ܵ���ʱ����  
{  
    uchar i;  
    SCL=1;  
    delay();  
    while((SDA==1)&&(i<250))  
        i++;  
    SCL=0;  
    delay();  
}  
  
void init() //��ʼ��  
{  
    SDA=1;  
    delay();  
    SCL=1;  
    delay();      
}  
  
void write_byte(uchar date) //дһ�ֽ�����  
{  
    uchar i,temp;  
    temp=date;  
    for(i=0;i<8;i++)  
    {  
        temp=temp<<1; //����һλ �Ƴ���һλ��CY��  
        SCL=0;          //ֻ����scl=0ʱsda�ܱ仯ֵ  
        delay();  
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
  
  
  
void write_add(uchar control,uchar date)  
{  
    start();  
    write_byte(PCF8591);    //10010000  ǰ��λ�̶� ��������λȫ�����ӵ��� ���Զ���0 ���һλ��д ����Ϊ�͵�ƽ  
    respons();  
    write_byte(control);  
    respons();  
    write_byte(date);  
    respons();  
    stop();  
  
}  

