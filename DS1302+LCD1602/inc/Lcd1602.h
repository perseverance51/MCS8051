#ifndef __LCD1602_H__
#define __LCD1602_H__

#include <reg52.h>

#define LCD1602_DB P0

sbit LCD1602_RS = P2^0;
sbit LCD1602_RW = P2^1;
sbit LCD1602_E = P2^2;

/* �ȴ�Һ��׼���� */
extern void LcdWaitReady();
/* �� LCD1602 Һ��д��һ�ֽ����cmd-��д������ֵ */
extern void LcdWriteCmd(unsigned char cmd);
/* �� LCD1602 Һ��д��һ�ֽ����ݣ�dat-��д������ֵ */
extern void LcdWriteDat(unsigned char dat);
/* ������ʾ RAM ��ʼ��ַ���༴���λ�ã�(x,y)-��Ӧ��Ļ�ϵ��ַ����� */
extern void LcdSetCursor(unsigned char x, unsigned char y);
/* ��Һ������ʾ�ַ�����(x,y)-��Ӧ��Ļ�ϵ���ʼ���꣬str-�ַ���ָ�� */
extern void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);
/* �򿪹�����˸Ч�� */
extern void LcdOpenCursor();
/* �رչ����ʾ */
extern void LcdCloseCursor();
/* ��ʼ�� 1602 Һ�� */
extern void InitLcd1602();









#endif