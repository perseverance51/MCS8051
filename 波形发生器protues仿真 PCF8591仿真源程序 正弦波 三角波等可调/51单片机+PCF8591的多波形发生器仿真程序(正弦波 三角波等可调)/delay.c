#include "delay.h"
#include <intrins.h>
void delay()    //��ʱ4-5��΢��  
{;;}  
  
void delay_1ms(uint z)  
{  
    uint x,y;  
    for(x=z;x>0;x--)  
        for(y=100;y>0;y--)  
            ;  
}  
  