#include <reg52.h>
sbit led = P1^0; 

//ÑÓÊ±º¯Êý
void Delay100ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 195;
	j = 138;
	do
	{
		while (--j);
	} while (--i);
}

void main(){
 while(1){
	led = !led;
	Delay100ms();
	led = !led;
	Delay100ms();
	}
}