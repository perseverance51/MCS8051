#ifndef _LCD1602_H
#define _LCD1602_H
		int LCD_init();    //������ʼ������
    int write_com(unsigned char);//����д�����
    int write_date(unsigned char);//����д���ݺ���
    int delay(unsigned char);//�����ӳٺ���
		void LCD_send(unsigned char,unsigned char,unsigned char[],unsigned char);
#endif