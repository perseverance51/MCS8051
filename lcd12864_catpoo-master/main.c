#include <reg52.h>
#include "LCD12864.h"
#include "font.h"
#include "delay.h"

void main()
{ char i=0,j;
	init_lcd();//��ʼ��LCD
	Clr_Scr();//����
	while(1)
    {
			/****��һ����ʾ****/
	LeftDisplay();//��������ʾ
  for(i=0;i<4;i++){
	 hz_LCDDisp16(16*i,0,Hz[i]);		
	}

	RightDisplay();//��������ʾ
	  for(i=0,j=4;i<4;i++,j++){
	 hz_LCDDisp16(16*i,0,Hz[j]);		
	}		
			/****�ڶ�����ʾ****/
	LeftDisplay();//��������ʾ
	  for(i=0;i<4;i++){
	 hz_LCDDisp16(16*i,2,Hz[8+i]);		
	}
		RightDisplay();//��������ʾ
	  for(i=0;i<4;i++){
	 hz_LCDDisp16(16*i,2,Hz[12+i]);		
	}	
			delay_ms(1000);
			Clr_Scr();//����
	}
		
}

