
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* --- QQ:  800003751 ----------------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/


/*************	����˵��	**************

���ļ�ΪSTC15xxxϵ�е���ʱ����,�û��������Բ��޸��������.


******************************************/

#include	"delay.h"



//========================================================================
// ����: void  delay_ms(unsigned char ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��, ����ֻ֧��1~255ms. �Զ���Ӧ��ʱ��.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע:
//========================================================================



void Delay25ms()		//@11.0592MHz
{
    unsigned char i, j;

    i = 45;
    j = 208;
    do
    {
        while (--j);
    } while (--i);
}


void Delay35us()		//@11.0592MHz
{
    unsigned char i;

    _nop_();
    i = 13;
    while (--i);
}
void Delay50us()		//@11.0592MHz
{
    unsigned char i;

    _nop_();
    i = 20;
    while (--i);
}
void Delay25us()		//@11.0592MHz
{
    unsigned char i;

    i = 9;
    while (--i);
}

void Delay10us()		//@11.0592MHz
{
    unsigned char i;

    i = 2;
    while (--i);
}
void Delay1000ms()		//@11.0592MHz
{
    unsigned char i, j, k;

    _nop_();
    i = 8;
    j = 1;
    k = 243;
    do
    {
        do
        {
            while (--k);
        } while (--j);
    } while (--i);
}
