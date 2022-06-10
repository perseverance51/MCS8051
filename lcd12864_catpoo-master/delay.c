

#include	"delay.h"


void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}
void  delay_ms(unsigned int ms)
{
	 do{
	 Delay1ms();
     }while(ms++);
}