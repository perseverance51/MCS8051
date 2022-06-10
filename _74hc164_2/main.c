//74hc164驱动共阴数码管源程序
//共阴数码管74hc164串行驱动
#include <STC89C5xRC.H>
#define uint unsigned int
#define uchar unsigned char
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
sbit DAT = P3^7; //模拟串口数据发送端
sbit CLK = P3^4;//模拟时钟控制端
//共阴数码管
uchar code tab[]= {0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xff,0xf6}; //0-9,-,全灭
uchar code

void sendbyte(uchar a)
 {
    uchar num ,c;
    num = tab[a];
    for(c=0; c<8; c++)
        {
        CLK =0;
		//			DAT = num&0x01;
        DAT = bitRead(num,c);//高位先输出
        CLK = 1;
    //   num >>= 1;       
    }   
}

void delay_50ms(uint x) //延时50ms    12晶振
{
 TMOD=0x01; //开定时器0，工作方式为1
 TR0=1;     //启动定时器0；
 while(x--)
 {  
    TH0=(65536-50000)/256; //定时50ms初值的高八位装入TH0
    TL0=(65536-50000)%256; //定时50ms初值的低八位装入TL0 
    while(!TF0); //等待，直到TF0为1
    TF0=0;    //重置溢出标志位
  }
  TR0=0;  //停止定时器0；
}

void main( )
 {
    unsigned char h;
    while(1) {
        for(h=0; h<10; h++) {
            sendbyte(h);
            delay_50ms(20);           
        }
        h=0;        
    } 		
}