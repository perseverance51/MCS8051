
#include	<REGX51.H>
#include	<tm1638.h>
#include	<intrins.h>

unsigned char num[8];		//�����������ʾ��ֵ
//8��led�ĵ�ַ������Ϊled1-led8
const unsigned char TM1638_LedAddrTab[] =
{
	0xC1,0xC3,0xC5,0xC7,0xC9,0xCB,0xCD,0xCF
};
int main(void)
{
	unsigned char i;
	init_TM1638();	                           //��ʼ��TM1638
	for(i=0;i<8;i++)
	Write_DATA(2*i,tab[16]);//��ʼ���Ĵ���,��ʾ"--- ----"	
	Delay1000ms();
	
	while(1)
	{
			for(i=0;i<8;i++){
		Write_oneLED(i,1);//0-7λ��.1��ӦSEG9��LED��
	 Write_oneLED(i,2);//0-7λ��.2��ӦSEG10��LED��
			}	
/*
			for(i=0;i<8;i++){
	Write_DATA(2*i,tab[i]);Delay1000ms();}	

	for(i=0;i<8;i++){
	TM1638_display_Fix(i,18-i);Delay1000ms();}	

				for(i=0;i<8;i++)
	Write_DATA(2*i,tab[17]);//��ʼ���Ĵ���,Ϩ��
*/
}
}
