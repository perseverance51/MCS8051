#include "wave.h"  
#include "PCF8591.h"
#include "delay.h"


uchar time1;
uchar a,range=100;  
void  triangular(void)	//���ǲ�
   {
        write_add(0x40,a);  
        delay_1ms(time1);  
        a++;  
        if(a>range)
		while(a>0)  
        {	
			a--;
			write_add(0x40,a);  
			delay_1ms(time1);
		}
	
			 
  }

  void  triangular1(void)//��ݲ�
   {
        write_add(0x40,a);  
        delay_1ms(time1);  
        a++;  
        if(a>range)  
            a=0; 
  }
  void square(void)	   //����
 {
        a=range;
  		write_add(0x40,a);
        delay_1ms(time1);
		a=0;
  		write_add(0x40,a);
        delay_1ms(time1); 
 }

