#include <STC89C5xRC.H>
#include "LCD1602.h"


void main()
{
    LCD1602_Init();									//��ʼ��LCD1602
    LCD1602_delay_ms(1);
    LCD1602_set_position(0,0);//ָ�����õ���һ�е�1��
    LCD1602_prints("Hello World!");
    LCD1602_set_position(0,13);//ָ�����õ���һ�е�13��
    LCD1602_prints("STC");
//	LCD1602_write_com(0x01);					//����ָ��
    while(1)
    {
        LCD1602_set_position(1,0);				 //ָ�����õ��ڶ��е�1��
        LCD1602_prints("perseverance");
        LCD1602_set_position(1,14);//ָ�����õ��ڶ��е�14��
        LCD1602_prints("51");
    }
}