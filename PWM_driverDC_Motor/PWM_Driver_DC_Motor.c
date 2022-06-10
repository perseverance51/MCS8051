/********************************
直流电机H桥驱动PWM 单向控制
********************************/
#include <REGX52.H>
#include <intrins.H>

// Define P3 pins
sbit Inc =P3^4; 
sbit Dec  = P3^5;
sbit Dir =  P3^6 ;
sbit PWM  = P3^7;

// Define new types
typedef unsigned char   uchar;
typedef unsigned int    uint;

void delay(uint);

void main(void)
 { 
	 int speed;
   // Select initial direction and speed.
   Dir = 1;
   if (Dir)
      speed = 400;
   else
      speed = 100;
   while(1)
    { 
			if(!Inc)
      // Increase speed   
         speed = speed > 0 ? speed - 1 : 0;
      if(!Dec)
      // Decrease speed
         speed = speed < 500 ? speed + 1 : 500;
      
      // Drive a PWM signal out. 
      PWM=1;
      delay(speed);
      PWM=0;
      delay(500-speed);
    }
 }

void delay(uint j)
 { for(; j>0; j--)
    { 
			_nop_();
    }
 }