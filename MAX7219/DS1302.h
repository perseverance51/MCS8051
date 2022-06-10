#ifndef _DS1302_H
#define _DS1302_H

//void delay1ms();
 //void delaynms(unsigned char n);
void delaynus(unsigned char n);
void Write1302(unsigned char dat);
void WriteSet1302(unsigned char Cmd,unsigned char dat);
 unsigned char Read1302(void);
 unsigned char  ReadSet1302(unsigned char Cmd);
 void Init_DS1302(void);

 

#endif