#ifndef __DS1302_H__
#define __DS1302_H__

#include <reg52.h>

//DS1302 IO����
sbit DS1302_CE = P1^0;
sbit DS1302_CK = P1^1;
sbit DS1302_IO = P1^2;

typedef struct __SYSTEMTIME__ { //����ʱ��ṹ�嶨��
    unsigned int year; //��
    unsigned char mon; //��
    unsigned char day; //��
    unsigned char hour; //ʱ
    unsigned char min; //��
    unsigned char sec; //��
    unsigned char week; //����
}SYSTEMTIME;
//�������߱�������
/* ����һ���ֽڵ� DS1302 ͨ�������� */
extern void DS1302ByteWrite(unsigned char dat);
/* �� DS1302 ͨ�������϶�ȡһ���ֽ� */
extern unsigned char DS1302ByteRead();
/* �õ���д������ĳһ�Ĵ���д��һ���ֽڣ�reg-�Ĵ�����ַ��dat-��д���ֽ� */
extern void DS1302SingleWrite(unsigned char reg, unsigned char dat);
/* �õ��ζ�������ĳһ�Ĵ�����ȡһ���ֽڣ�reg-�Ĵ�����ַ������ֵ-�������ֽ� */
extern unsigned char DS1302SingleRead(unsigned char reg);
/* ��ͻ��ģʽ����д�� 8 ���Ĵ������ݣ�dat-��д������ָ�� */
extern void DS1302BurstWrite(unsigned char *dat);
/* ��ͻ��ģʽ������ȡ 8 ���Ĵ��������ݣ�dat-��ȡ���ݵĽ���ָ�� */
extern void DS1302BurstRead(unsigned char *dat);
/* ��ȡʵʱʱ�䣬����ȡ DS1302 ��ǰʱ�䲢ת��Ϊʱ��ṹ���ʽ */
extern void GetRealTime(SYSTEMTIME *time);
/* �趨ʵʱʱ�䣬ʱ��ṹ���ʽ���趨ʱ��ת��Ϊ���鲢д�� DS1302 */
extern void SetRealTime(SYSTEMTIME *time);
/* DS1302 ��ʼ�����緢���������������ó�ʼʱ�� */
extern void InitDS1302();
extern SYSTEMTIME sTime;




#endif
