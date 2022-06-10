
#include	<REGX51.H>
#include	<tm1638.h>
#include	<intrins.h>

unsigned char num[8];		//各个数码管显示的值
//8个led的地址，依次为led1-led8
const unsigned char TM1638_LedAddrTab[] =
{
	0xC1,0xC3,0xC5,0xC7,0xC9,0xCB,0xCD,0xCF
};
int main(void)
{
	unsigned char i;
	init_TM1638();	                           //初始化TM1638
	for(i=0;i<8;i++)
	Write_DATA(2*i,tab[16]);//初始化寄存器,显示"--- ----"	
	Delay1000ms();
	
	while(1)
	{
			for(i=0;i<8;i++){
		Write_oneLED(i,1);//0-7位置.1对应SEG9的LED灯
	 Write_oneLED(i,2);//0-7位置.2对应SEG10的LED灯
			}	
/*
			for(i=0;i<8;i++){
	Write_DATA(2*i,tab[i]);Delay1000ms();}	

	for(i=0;i<8;i++){
	TM1638_display_Fix(i,18-i);Delay1000ms();}	

				for(i=0;i<8;i++)
	Write_DATA(2*i,tab[17]);//初始化寄存器,熄灭
*/
}
}
