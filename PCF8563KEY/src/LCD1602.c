
#include "LCD1602.h"

//void Delay_ms(unsigned int a)
//{
//    unsigned int b,c;
//    for(c=0; c<a; c++)
//        for(b=0; b<123; b++);
//}

void LCD1602_WriteCom(unsigned char com)          //д������
{

    LCD1602_RS=0;
		Delay_ms(1);
	 LCD1602_E=1;
    LCD1602_DATAPINS = com;        //����4λ�Ľ����ǽӵ�P0�ڵĸ���λ�����Դ��͸���λ���ø�
    Delay_ms(1);
    LCD1602_E=0;
    
}

void LCD1602_WriteData(unsigned char dat)                        //д������
{

    LCD1602_RS=1;
    Delay_ms(1);
    LCD1602_DATAPINS=dat;        
    LCD1602_E=1;
    Delay_ms(1);
    LCD1602_E=0;

}
//xλ�ã�yָ���У�sд���ַ�������
void  LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s)
{
    if (y == 0)
    {
        LCD1602_WriteCom(0x80 + x);     //��ʾ��һ��
    }
    else
    {
        LCD1602_WriteCom(0xC0 + x);      //��ʾ�ڶ���
    }
    while(*s)
    {
        LCD1602_WriteData(*s);
        s ++;
    }

}
void LCD1602_Init()                                                  //LCD��ʼ���ӳ���
{
	    LCD1602_E=0;
		LCD1602_RW=0;
    LCD1602_WriteCom(0x38);         //��8λ����תΪ4λ����
    LCD1602_WriteCom(0x0c);  //����ʾ����ʾ���
    LCD1602_WriteCom(0x06);  //дһ��ָ���1
    LCD1602_WriteCom(0x01);  //����
}

