#include <reg52.h>
#include "Lcd1602.h"
#include "DS1302.h"

#define 	MAIN_Fosc			11059200uL	//������ʱ��
bit flag200ms = 0; //200ms ��ʱ��־
unsigned char T0RH = 0; //T0 ����ֵ�ĸ��ֽ�
unsigned char T0RL = 0; //T0 ����ֵ�ĵ��ֽ�

/*��Χ0 -50ms*/
void delayms(unsigned int ms){
    unsigned long tmp; //��ʱ����
    tmp = MAIN_Fosc / 12; //��ʱ������Ƶ��
    tmp = (tmp * ms) / 1000; //��������ļ���ֵ
    tmp = 65536 - tmp; //���㶨ʱ������ֵ
    tmp = tmp + 12; //�����ж���Ӧ��ʱ��ɵ����
    T0RH = (unsigned char)(tmp>>8); //��ʱ������ֵ���Ϊ�ߵ��ֽ�
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0; //���� T0 �Ŀ���λ
    TMOD |= 0x01; //���� T0 Ϊģʽ 1
    TH0 = T0RH; //���� T0 ����ֵ
    TL0 = T0RL;
    ET0 = 1; //ʹ�� T0 �ж�
    TR0 = 1; //���� T0
}
/* T0 �жϷ�������ִ�� 200ms ��ʱ */
void InterruptTimer0() interrupt 1{
    static unsigned char tmr200ms = 0;
    TH0 = T0RH; //���¼�������ֵ
    TL0 = T0RL;
    tmr200ms++;
    if (tmr200ms >= 200){ //��ʱ 200ms
        tmr200ms = 0;
        flag200ms = 1;
    }
}

void main(){
    unsigned char psec=0xAA; //�뱸�ݣ���ֵ AA ȷ���״ζ�ȡʱ����ˢ����ʾ
    unsigned char time[8]; //��ǰʱ������
    unsigned char str[10]; //�ַ���ת��������
		unsigned char *weeklist[7]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
		unsigned char week;
    EA = 1; //�����ж�
    delayms(1); //T0 ��ʱ 1ms
    InitDS1302(); //��ʼ��ʵʱʱ��
    InitLcd1602(); //��ʼ��Һ��

    while (1){
        if (flag200ms){ //ÿ 200ms ��ȡ����ʱ��
            flag200ms = 0;
            DS1302BurstRead(time); //��ȡ DS1302 ��ǰʱ��
            if (psec != time[0]){ //��⵽ʱ���б仯ʱˢ����ʾ
                str[0] = '2'; //�����ݵĸ� 2 λ��20
                str[1] = '0';
                str[2] = (time[6] >> 4) + '0'; //���ꡱ��λ����ת��Ϊ ASCII ��
                str[3] = (time[6]&0x0F) + '0'; //���ꡱ��λ����ת��Ϊ ASCII ��
                str[4] = '-'; //������ڷָ���
                str[5] = (time[4] >> 4) + '0'; //���¡�
                str[6] = (time[4]&0x0F) + '0';
                str[7] = '-';
                str[8] = (time[3] >> 4) + '0'; //���ա�
                str[9] = (time[3]&0x0F) + '0';
                str[10] = '\0';
                LcdShowStr(0, 0, str); //��ʾ��Һ���ĵ�һ��
                week = (time[5]&0x0F) ; //�����ڡ�
                LcdShowStr(13, 0, weeklist[week - 1]); //��ʾ��Һ���ĵ�һ��
					/******  LCD1602�ڶ�����ʾ��ʱ������ʾ   ******/
                str[0] = (time[2] >> 4) + '0'; //��ʱ��
                str[1] = (time[2]&0x0F) + '0';
                str[2] = ':'; //���ʱ��ָ���
                str[3] = (time[1] >> 4) + '0'; //���֡�
                str[4] = (time[1]&0x0F) + '0';
                str[5] = ':';
                str[6] = (time[0] >> 4) + '0'; //���롱
                str[7] = (time[0]&0x0F) + '0';
                str[8] = '\0';
                LcdShowStr(4, 1, str); //��ʾ��Һ���ĵڶ���
                psec = time[0]; //�õ�ǰֵ�����ϴ�����
            }
        }
    }
}


