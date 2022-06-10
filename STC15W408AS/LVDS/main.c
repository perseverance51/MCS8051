//�ٶ�����оƬ�Ĺ���Ƶ��Ϊ18.432MHz

//ע��:���Ա�ʾ��ʱ,����ISP����ʱ����ѹ��λ���ܹر�

#include "reg51.h"
#include "intrins.h"

sbit ELVD = IE^6;                   //��ѹ����ж�ʹ��λ
sbit P11 = P1^1;                    //���Կ�

#define LVDF    0x20                //PCON.5,��ѹ����жϱ�־λ

//-----------------------------------------

void LVD_ISR() interrupt 6 using 1
{
    P11 = !P11;                     //���Կ�
    PCON &= ~LVDF;                  //��PCON.5д0��LVD�ж�
}

void main()
{
    PCON &= ~LVDF;                  //�ϵ����Ҫ��LVD�жϱ�־λ
    ELVD = 1;                       //ʹ��LVD�ж�
    EA = 1;                         //�����жϿ���
    P11= 1;
    while (1)
    {
        _nop_();
        _nop_();
    }
}
