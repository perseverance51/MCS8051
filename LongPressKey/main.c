#include <STC89C5xRC.H>
#include <intrins.h>
typedef unsigned int u16;	  // typedef（类型定义） 用来给无符号整型类型数据 unsigned int 定义新名字 uint
typedef unsigned char u8;  //用来给无符号字符型类型数据 unsigned char 定义新名字 uchar

#define KEY_MODE 0x01    // 模式按键
#define KEY_PLUS 0x02     // 加
#define FOSC 11059200L
#define T1MS (65536-FOSC/12/1000)   //1ms timer calculation method in 12T mode

sbit KeyValue=P3^7;
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
unsigned char code smgduan[]= {
//0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90
	0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f
};
//定义共阳数码管显示0~9
 u8 cnt_plus=0;
u8 Trg,Cont,CNT;
u16  Count=0;

u8 DisplayData[8];
u8 datnum1=0,datnum2=0,datnum3=0,datnum4=0;

void delay(u16 i)
{
	while(i--);	
}
void Timer0Init(void)		//20毫秒@11.0592MHz
{
	 TMOD = 0x01;                    //set timer0 as mode1 (16-bit)
    TL0 = T1MS;                     //initial timer0 low byte
    TH0 = T1MS >> 8;                //initial timer0 high byte
    TR0 = 0;                        //timer0 start running
    ET0 = 1;                        //enable timer0 interrupt
    EA = 1;                         //open global interrupt switch
    CNT = 0;                      //initial counter
}

void KeyRead()
{
    unsigned char ReadData = KeyValue^0x01;   //异或运算，按下时为1，没有按时为0
    Trg = ReadData & (ReadData ^ Cont);   // 按键触发为1
    Cont = ReadData;                     // 长按触发为1
}
void KeyProc(void)
{
	
    if (Trg & KEY_MODE) // 如果按下的是KEY_MODE，而且你常按这按键也没有用，
    {   //它是不会执行第二次的 ， 必须先松开再按下
        Count++;         // 执行+1
    
    if (Cont^KeyValue) // 如果“加”按键被按着不放
    {
			TR0=1;//开启定时器
         if (cnt_plus>50) // 20ms*50 = 1S 如果时间到
            Count +=9;      // 执行+10
				 cnt_plus =0;
  }else {
		TR0=0;//如果不是长按就关闭定时器
		cnt_plus=0;
		}
		if(Count>=9999) {  //超过0~9，数码管显示回到0
                	Count=9999;
            }
}
		}
/*******************************************************************************
* 函 数 名         : DigDisplay
* 函数功能		   : 数码管动态扫描函数，循环扫描8个数码管显示
*******************************************************************************/
void DigDisplay()
{
	u8 i;

	for(i=0;i<8;i++)
	{
		switch(i)	 //位选，选择点亮的数码管，
		{
			case(0):
				LSA=1;LSB=1;LSC=1; break;//显示第7位
			case(1):
				LSA=0;LSB=1;LSC=1; break;//显示第6位
			case(2):
				LSA=1;LSB=0;LSC=1; break;//显示第5位
			case(3):
				LSA=0;LSB=0;LSC=1; break;//显示第4位
			case(4):
				LSA=1;LSB=1;LSC=0; break;//显示第3位 
			case(5):
				LSA=0;LSB=1;LSC=0; break;//显示第2位 
			case(6):
				LSA=1;LSB=0;LSC=0; break;//显示第1位 
			case(7):
				LSA=0;LSB=0;LSC=0; break;//显示第0位	
		}
		P0=DisplayData[i];//发送段码
		delay(100); //间隔一段时间扫描	
		P0=0x00;//消隐
	}
}

void main() {
	
	Timer0Init();
    while(1) {
      KeyRead();
			KeyProc();
 			DigDisplay();	
			DisplayData[0]=smgduan[Count%1000%100%10];
			DisplayData[1]=smgduan[Count%1000%100/10];
			DisplayData[2]=smgduan[Count%1000/100];
			DisplayData[3]=smgduan[Count/1000];		
    }
}
void tm0_isr() interrupt 1
{  
    TL0 = T1MS;                     //reload timer0 low byte
    TH0 = T1MS >> 8;                //reload timer0 high byte
    if (CNT++ > 20)               //1ms * 1000 -> 1s
    {
        CNT = 0;               //reset counter
			cnt_plus++;       // 计时
         
       
    }
}