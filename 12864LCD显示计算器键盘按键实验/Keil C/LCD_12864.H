#include <reg51.h>
#include <intrins.h>

#ifndef __LCD_12864_H
#define __LCD_12864_H


#define uchar unsigned char
#define uint unsigned int
#define LCD_DB_PORT P0 //液晶 DB0~DB7
#define LCD_START_ROW 0xC0 //起始行
#define LCD_PAGE 0xB8 //页指令
#define LCD_COL 0x40 //列指令

 bit LCD_Check_Busy();
 void LCD_Write_Command(uchar c);
 void LCD_Write_Data(uchar d );
extern void LCD_Initialize();
extern void Common_Show(uchar P,uchar L,uchar W,uchar *r);
extern void Display_A_char(uchar P,uchar L,uchar *M);
extern void Display_A_WORD(uchar P,uchar L,uchar *M);
extern void Display_A_WORD_String(uchar P,uchar L,uchar C,uchar *M);



#endif  