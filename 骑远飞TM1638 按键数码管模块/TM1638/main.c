#include	<intrins.h>
#include <REGX52.H>
#include "TM1638.h"

int main(void){
	unsigned char i;
	init_TM1638();
	while(1){
		for(i=0;i<8;i++){
		Write_oneLED(i,1);
		//	Delay1000ms();
		}
	}
}
