/*�Ƚ��������Լ��Ƚ����жϷ�ʽ��Ϊ������ʾ������*/
#include "STC15Wxx.H"
//#include "STC15Fxxxx.H"
sbit Led =P1^0;
void main()
{
	CMPCR1 = 0xb0;//1011,0000 �����Ƚ����ж�
	CMPCR2 = 0x00;//0000 0000 �Ƚ���ֻ��ʱ0.1us
	EA =1;
	while(1);
}
void cmp() interrupt 21 using 1
{
	CMPCR1 &= 0xbf;//����жϱ�־��1011,1111
Led = (CMPCR1 & 0x01); //���Ƚ������CMPRES��������Կ���ʾ

}