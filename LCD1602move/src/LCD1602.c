#include "LCD1602.h"

//****************************************************
//MS��ʱ����(12M�����²���)
//****************************************************
void LCD1602_delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}

//****************************************************
//дָ��
//****************************************************
void LCD1602_write_com(unsigned char com)
{
	LCD1602_RS = 0;
	LCD1602_delay_ms(1);
	LCD1602_EN = 1;
	LCD1602_PORT = com;
	LCD1602_delay_ms(1);
	LCD1602_EN = 0;
}

//****************************************************
//д����
//****************************************************
void LCD1602_printc(unsigned char dat)
{
	LCD1602_RS = 1;
	LCD1602_delay_ms(1);	
	LCD1602_PORT = dat;
	LCD1602_EN = 1;
	LCD1602_delay_ms(1);
	LCD1602_EN = 0;
}

//****************************************************
//����д�ַ�
//****************************************************
/*
void LCD1602_prints(unsigned char *s)
{
	while(*s>0)
	{
		LCD1602_printc(*s);
		s++;
	}
}
*/
//****************************************************
//LCD��궨λ��x��
//****************************************************

void LCD1602_set_position(bit hang, unsigned char x)          
{
  if(hang)LCD1602_write_com(0x80 + 0x40 + x);
	else LCD1602_write_com(0x80 + x);
}

//****************************************************
//LCD��ʼ��
//****************************************************
void LCD1602_Init()
{
	LCD1602_EN = 0;
	LCD1602_RW = 0;						//����Ϊд״̬
	LCD1602_write_com(0x38);			//��ʾģʽ�趨
	LCD1602_write_com(0x0c);			//������ʾ������������á������˸����
	LCD1602_write_com(0x06);			//дһ���ַ���ָ���һ
	LCD1602_write_com(0x01);			//����ָ��
}

/* ������ʾ RAM ��ʼ��ַ���༴���λ�ã�(x,y)-��Ӧ��Ļ�ϵ��ַ����� */
/*
void LcdSetCursor(unsigned char x, unsigned char y){
    unsigned char addr;
    if (y == 0){ //���������Ļ���������ʾ RAM �ĵ�ַ
        addr = 0x00 + x; //��һ���ַ���ַ�� 0x00 ��ʼ
    }else{
        addr = 0x40 + x; //�ڶ����ַ���ַ�� 0x40 ��ʼ
    }
    LCD1602_write_com(addr | 0x80); //���� RAM ��ַ
}
*/
/* ��Һ������ʾ�ַ�����(x,y)-��Ӧ��Ļ�ϵ���ʼ���꣬
str-�ַ���ָ�룬len-����ʾ���ַ����� */
void LcdShowStr(unsigned char x, unsigned char y,
        unsigned char *str, unsigned char len){
    LCD1602_set_position(x, y); //������ʼ��ַ
    while (len--){ //����д�� len ���ַ�����
       LCD1602_printc(*str++); //��ȡ str ָ������ݣ�Ȼ�� str �Լ� 1
    }
}
