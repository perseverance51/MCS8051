//ST7920 LCD ����
//Author: cdhigh <2021-09-22>
//ST7920�Ļ�ͼ�ṹ�����Ͻ�Ϊ����ԭ�㣬д����ֽڱ�ʾ��ĳ�����꿪ʼ��8���������أ�
//ÿ�� 128/8=16 ���ֽڣ�ÿ���ֽ����Ϊ���λ���ұ�Ϊ���λ������64�С�
//DDRAM��ַ
//0x80  0x81  0x82  0x83  0x84  0x85  0x86  0x87    //��һ�к���λ��
//0x90  0x91  0x92  0x93  0x94  0x95  0x96  0x97    //�ڶ��к���λ��
//0x88  0x89  0x8a  0x8b  0x8c  0x8d  0x8e  0x8f     //�����к���λ��
//0x98  0x99  0x9a  0x9b  0x9c  0x9d  0x9e  0x9f     //�����к���λ��
#ifndef __ST7290_H_
#define __ST7290_H_

#include <STC89C5xRC.H>

#define LCD12864_Port P0

sbit LCD12864_CS  = P2^6;
sbit LCD12864_PSB = P3^2;

sbit RS_Pin = P2 ^ 6;			//�Ĵ���ѡ��λ��RSλ
sbit WR_Pin = P2 ^ 5;			//��дѡ��λ��RW
sbit EN_Pin = P2 ^ 7;			//ʹ���ź�λ��Eλ

#define MODE_8BIT       8  //8bit����ģʽ 
#define MODE_4BIT       4  //4bit����ģʽ
#define MODE_SPI     	1  //����-PSB�ӵ�

//����ʹ�õĽӿ�ģʽ��MODE_8BIT, MODE_4BIT, MODE_SPI
#define LCD_INTERFACE   MODE_SPI

void LCD_clear(void);
void LCD_init(void);
void LCD_write_command(unsigned char command);
void LCD_write_data(unsigned char Data);
void LCD_write_byte(unsigned char byte);
void LCD_write_half_byte(unsigned char half_byte);
void LCD2_spi_write_byte(unsigned char Data);
void LCD2_spi_write_byte_standard(unsigned char Data);
unsigned char LCD_read_data(void);
unsigned char LCD_read_status(void);
unsigned char LCD_read_byte(void);
void LCD_startGraphic(void);
void LCD_endGraphic(void);
void LCD_Inverse_16X16(unsigned int rowCol, unsigned char charNum, unsigned char reverse);
unsigned int LCD_rowCol_to_inter_Xy(unsigned int rowCol);
void LCD_set_text_address(unsigned int rowCol);
void LCD_set_graphic_address(unsigned char x, unsigned char y);
void LCD_write_char(unsigned int rowCol, unsigned int Code);
void LCD_write_string(unsigned int rowCol, const char * p);
void LCD_Draw_Bmp(unsigned char *pImage, unsigned char col, unsigned char row, unsigned char x0, unsigned char y0);

#if LCD_INTERFACE != MODE_SPI
void LCD_write_dot(unsigned char x, unsigned char y);
#endif

#define BYTE_BIT(bitno) (1 << (bitno))
#define TEST_BIT(value, bitno) ((1 << (bitno)) & (value))
#define SET_BIT(value, bitno)  ((value) |= (1 << (bitno)))
#define CLR_BIT(value, bitno)  ((value) &= ~(1 << (bitno)))

//�����ֽڴճ���������
//Ϊ�˸��õ�����ʹ��X/Y��ROW/COL��ʹ��ROW/COL�Ķ���һ�����Ͳ�����X/YΪ�����ֽڲ���
#define ROW_COL(r, c) (((r) << 8) | (c & 0xff))

#endif

