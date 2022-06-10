//#include<at89x51.h>
#include <STC89C5xRC.H>

sbit DAT=P1^1;
sbit CLK=P1^0;

unsigned char code tab[]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40,0xff}; // 显示0~9,-,全灭
//unsigned  char code ff[]= {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x77}; //位选;
unsigned char hour,minite,second;



//开启计时
void loading()
{
  TMOD=0x01;		//工作方式1,65536
	TL0 = 0xb0;		//设置定时50ms初始值
	TH0 = 0x3c;		//设置定时50ms初始值
		TR0=1;//开启定时器0中断
    TR0=1;
    ET0=1;
    EA=1;
}

//以下是显示程序,共八个,因为有八个数码管;
void sendbyte(unsigned char byte)
{
    unsigned char num,c;
    num=tab[byte];
    for(c=0; c<8; c++)
    {
        CLK=0;
        DAT=num&0x80;
        CLK=1;
        num<<=1;
    }
    num=0xfe;
    for(c=0; c<8; c++)
    {

        CLK=0;
        DAT=num;
        CLK=1;
        num<<=1;
    }
}


void sendbyte_1(unsigned char byte)
{
    unsigned char num,c;
    num=tab[byte];
    for(c=0; c<8; c++)
    {
        CLK=0;
        DAT=num&0x80;
        CLK=1;
        num<<=1;
    }
    num=0xfd;
    for(c=0; c<8; c++)
    {

        CLK=0;
        DAT=num&0x80;
        CLK=1;
        num<<=1;
    }
}

void sendbyte_2(unsigned char byte)
{
    unsigned char num,c;
    num=tab[byte];
    for(c=0; c<8; c++)
    {
        CLK=0;
        DAT=num&0x80;
        CLK=1;
        num<<=1;
    }
    num=0xfb;
    for(c=0; c<8; c++)
    {

        CLK=0;
        DAT=num&0x80;
        CLK=1;
        num<<=1;
    }
}


void sendbyte_3(unsigned char byte)
{
    unsigned char num,c;
    num=tab[byte];
    for(c=0; c<8; c++)
    {
        CLK=0;
        DAT=num&0x80;
        CLK=1;
        num<<=1;
    }
    num=0xf7;
    for(c=0; c<8; c++)
    {

        CLK=0;
        DAT=num&0x80;
        CLK=1;
        num<<=1;
    }
}


void sendbyte_4(unsigned char byte)
{
    unsigned char num,c;
    num=tab[byte];
    for(c=0; c<8; c++)
    {
        CLK=0;
        DAT=num&0x80;
        CLK=1;
        num<<=1;
    }
    num=0xef;
    for(c=0; c<8; c++)
    {

        CLK=0;
        DAT=num&0x80;
        CLK=1;
        num<<=1;
    }
}


void sendbyte_5(unsigned char byte)
{
    unsigned char num,c;
    num=tab[byte];
    for(c=0; c<8; c++)
    {
        CLK=0;
        DAT=num&0x80;
        CLK=1;
        num<<=1;
    }
    num=0xdf;
    for(c=0; c<8; c++)
    {

        CLK=0;
        DAT=num&0x80;
        CLK=1;
        num<<=1;
    }
}


void sendbyte_6(unsigned char byte)
{
    unsigned char num,c;
    num=tab[byte];
    for(c=0; c<8; c++)    {
        CLK=0;
        DAT=num&0x80;
        CLK=1;
        num<<=1;
    }
    num=0xbf;
    for(c=0; c<8; c++)
    {

        CLK=0;
        DAT=num&0x80;
        CLK=1;
        num<<=1;
    }
}


void sendbyte_7(unsigned char byte)
{
    unsigned char num,c;
    num=tab[byte];
    for(c=0; c<8; c++)
    {
        CLK=0;
        DAT=num&0x80;
        CLK=1;
        num<<=1;
    }
    num=0x7f;
    for(c=0; c<8; c++)
    {
        CLK=0;
        DAT=num&0x80;
        CLK=1;
        num<<=1;
    }
}


//延时
void delay(unsigned int xms)
{
	
	unsigned int i,j;
	for(i=xms;i>0;i--)
		for(j=96;j>0;j--);
}

void main()
{

    loading();
    while(1)
    {
		   sendbyte(second%10);	//秒，个位
				delay(5);
				sendbyte(11);//消隐
        sendbyte_1(second/10);	//秒，十位
        delay(5);

			
        sendbyte_2(10);	//‘-’
				delay(5);
        sendbyte_3(minite%10);	//分，个位			
       delay(5);

        sendbyte_4(minite/10);	//分，十位			
       delay(5);

        sendbyte_5(10);	//‘-’
       delay(5);
        sendbyte_6(hour%10);	//小时，个位
       delay(5);

        sendbyte_7(hour/10);	//小时，十位							
				delay(5);


    }
}


void time(void) interrupt 1
{
   static volatile unsigned char tt=0;
    tt++;		
		TL0 = 0xb0;		//设置定时初始值(65536-50000)%256
		TH0 = 0x3c;		//设置定时初始值(65536-50000)/256
    if(tt==20)
    {
        tt=0;
        second++;
        if(second==60)
        {
            second=0;
            minite++;
            if(minite==60)
            {
                minite=0;
                hour++;
                if(hour==24)  hour=0;
            }
        }
    }

}