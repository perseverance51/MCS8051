#include "hardware.h"
#include "software.h"
//共阴数码管段码
uchar code block[] = {  //定义无符号字符型类型数组来储存段码
		0x3F,  //"0"
    0x06,  //"1"
    0x5B,  //"2"
    0x4F,  //"3"
    0x66,  //"4"
    0x6D,  //"5"
    0x7D,  //"6"
    0x07,  //"7"
    0x7F,  //"8"
    0x6F  //"9"
};
void SEGDisplay()    //数码管扫描函数
{
  uchar i;  //定义i变量
	for(i=0;i<2;i++)
	{
		switch(i)	
		{														 
		   case(0):			 //位选，第一个数码管	
				LSA=0;
				LSB=1;
				LSC=1;
		        P0 = block[num2/10];  //十位数拆分
				 break;
			case(1):          //位选，第一个数码管
				LSA=1;
				LSB=1;
				LSC=1;
			    P0 = block[num2%10];  //个位数拆分
				 break;
		}
		delayms(5);   //延时一会（视觉残留）
		P0=0x00;      //消影
	}
}
