#include <reg52.h>
#include "LCD12864.h"
#include "font.h"
#include "delay.h"

void main()
{ char i=0,j;
	init_lcd();//初始化LCD
	Clr_Scr();//清屏
	while(1)
    {
			/****第一行显示****/
	LeftDisplay();//左屏开显示
  for(i=0;i<4;i++){
	 hz_LCDDisp16(16*i,0,Hz[i]);		
	}

	RightDisplay();//右屏开显示
	  for(i=0,j=4;i<4;i++,j++){
	 hz_LCDDisp16(16*i,0,Hz[j]);		
	}		
			/****第二行显示****/
	LeftDisplay();//左屏开显示
	  for(i=0;i<4;i++){
	 hz_LCDDisp16(16*i,2,Hz[8+i]);		
	}
		RightDisplay();//右屏开显示
	  for(i=0;i<4;i++){
	 hz_LCDDisp16(16*i,2,Hz[12+i]);		
	}	
			delay_ms(1000);
			Clr_Scr();//清屏
	}
		
}

