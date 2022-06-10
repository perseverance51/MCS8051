#include <LCD1602.h>
#include "reg51.h" 
		sbit RS = P2^6;
    sbit RW = P2^5;
    sbit EN = P2^7;
		
void LCD_send(unsigned char i,unsigned char j,unsigned char str[],unsigned char n)
{
	//���룺��ʾ����i,����j������ַ�������ַ�����n
	unsigned char x=0;
	if(i==1)
	{write_com(0x80+j);}
	else
	{write_com(0x80+0x40+j);}
    for(x=0;x<n;x++)
    {
         write_date(str[x]);
        delay(150);
    }
}
		
int LCD_init()//��ʼ��������
{
    EN = 0;
    write_com(0X38);//����16*2��ʾ��5*7����8λ���ݽӿ�
    write_com(0X0C);//���ÿ���ʾ������ʾ���
    write_com(0X06);//дһ���ַ�ʱ����������
    write_com(0X01);//��ʾ����
    return 0;
}

int write_com(unsigned char com)//д����ĺ�����
{
    RS = 0;
    RW = 0;
    P0 = com;
    delay(5);
    EN = 1;
    delay(5);
    EN = 0;
    return 0;
}

int write_date(unsigned char data1)//д���ݵĺ�����
{
    RS = 1;
    RW = 0;
    P0 = data1;
    delay(5);
    EN = 1;
    delay(5);
    EN = 0;
    return 0;
}

int delay(unsigned char xms)
{
    unsigned char x,y;
    for(x=xms;x>0;x--)
        for(y=110;y>0;y--);
    return 0;
}