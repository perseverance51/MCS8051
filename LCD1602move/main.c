#include <STC89C5xRC.H>
#include "LCD1602.h"

bit flag500ms = 0; //500ms ��ʱ��־
unsigned char T0RH = 0; //T0 ����ֵ�ĸ��ֽ�
unsigned char T0RL = 0; //T0 ����ֵ�ĵ��ֽ�
unsigned char code str1[] = "Perseverance51";
//����ʾ�ĵڶ����ַ�����������һ���ַ����ȳ����϶̵��п��ÿո���
unsigned char code str2[] = "Hello ' World";
/* ���ò����� T0��ms-T0 ��ʱʱ�� */
void ConfigTimer0(unsigned int ms){
    unsigned long tmp; //��ʱ����
    tmp = 11059200 / 12; //��ʱ������Ƶ��
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
void main()
{
	    unsigned char i;
    unsigned char index = 0; //�ƶ�����
    unsigned char  bufMove1[16+sizeof(str1)+16]; //�ƶ���ʾ������ 1
    unsigned char  bufMove2[16+sizeof(str2)+16]; //�ƶ���ʾ������ 2
	EA = 1; //�����ж�
	 ConfigTimer0(60); //���� T0 ��ʱ ���ù�С�ᵼ��Proteus����ʱ��Ļ�кڿ����򣬰������ֵ��Ӧ�ó���51ms����
	//ʵ��Ӳ����Ӷ��壺pdata �ⲿ�洢��
	
    LCD1602_Init();									//��ʼ��LCD1602
	LCD1602_write_com(0x01);					//����ָ��
	//��������ͷһ�����Ϊ�ո�
    for (i=0; i<16; i++){
        bufMove1[i] = ' ';
        bufMove2[i] = ' ';
    }
    //����ʾ�ַ����������������м�λ��
    for (i=0; i<(sizeof(str1)-1); i++){
        bufMove1[16+i] = str1[i];
        bufMove2[16+i] = str2[i];
    }
    //��������βһ��Ҳ���Ϊ�ո�
    for (i=(16+sizeof(str1)-1); i<sizeof(bufMove1); i++){
        bufMove1[i] = ' ';
        bufMove2[i] = ' ';
    }
    while(1)
    {
        if (flag500ms){ //ÿ 500ms �ƶ�һ����Ļ
            flag500ms = 0;
            //�ӻ������������ʾ��һ���ַ���ʾ��Һ����
            LcdShowStr(0, 0, bufMove1+index, 16);
            LcdShowStr(1, 0, bufMove2+index, 16);
            //�ƶ�����������ʵ������
            index++;
            if (index >= (16+sizeof(str1)-1)){
                //��ʼλ�ôﵽ�ַ���β���󼴷��ش�ͷ��ʼ
                index = 0;
            }
        }
    }
}

/* T0 �жϷ���������ʱ 500ms */
void InterruptTimer0() interrupt 1{
    static unsigned char tmr500ms = 0;
   
    TH0 = T0RH; //���¼�������ֵ
    TL0 = T0RL;
    tmr500ms++;
    if (tmr500ms >= 10){
        tmr500ms = 0;
        flag500ms = 1;
    }
    
}