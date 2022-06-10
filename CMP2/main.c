/*比较器介绍以及比较器中断方式作为掉电检测示例程序*/
#include "STC15Wxx.H"
//#include "STC15Fxxxx.H"
sbit Led =P1^0;
void main()
{
	CMPCR1 = 0xb0;//1011,0000 开启比较器中断
	CMPCR2 = 0x00;//0000 0000 比较器只延时0.1us
	EA =1;
	while(1);
}
void cmp() interrupt 21 using 1
{
	CMPCR1 &= 0xbf;//清除中断标志，1011,1111
Led = (CMPCR1 & 0x01); //将比较器结果CMPRES输出到测试口显示

}