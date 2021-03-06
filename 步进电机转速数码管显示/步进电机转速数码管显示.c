/*-----------------------------------------------
  名称：步进电机
  内容：本程序用于测试4相步进电机常规驱动 
        使用1-2相励磁
        1-2相激励功率增倍，步进角度减半，抖动减少
        顺序如下 a-ab-b-bc-c-cd-d-da   又称4相8拍
		数码管显示 03-19速度等级，数字越大，速度越慢
 ------------------------------------------------*/

#include <reg52.h>
typedef unsigned char   uchar;
typedef unsigned int    uint;

#define DataPort P0 //定义数据端口 程序中遇到DataPort 则用P0 替换
#define out_port  (P1)


sbit LATCH1=P2^2;//定义锁存使能端口 段锁存
sbit LATCH2=P2^3;//                 位锁存

uchar const sequence[8] = {0x02,0x06,0x04,0x0c,0x08,0x09,0x01,0x03};
unsigned char code  DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 显示段码值0~9
unsigned char code  WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分别对应相应的数码管点亮,即位码
unsigned char TempData[8]; //存储显示值的全局变量

sbit A1=P1^0; //定义步进电机连接端口
sbit B1=P1^1;
sbit C1=P1^2;
sbit D1=P1^3;
//0010-0110-0100-1100-1000-1001-0001-0011四相八拍

#define Coil_OFF {A1=0;B1=0;C1=0;D1=0;}//全部断电

unsigned char Speed;

void Display(unsigned char FirstBit,unsigned char Num);
void Init_Timer0(void);

/*------------------------------------------------
 mS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编
------------------------------------------------*/
void DelayMs(uint j)
{
uchar i;
   for(; j>0; j--)
    { i = 120;
      while (i--);  
    }
}
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main()
{
	uchar i;
 Init_Timer0();
 EA=1;          //全局中断开
 EX0=1;         //外部中断0开
 IT0=1;         //1表示边沿触发

 Speed=3;
 TempData[0]= DuanMa[Speed/10];//分解显示信息，如要显示68， 
 TempData[1]= DuanMa[Speed%10];//则68/10=6  68%10=8 
 Coil_OFF
  out_port = 0x03;
   while(1)
    { 
	i = i<8 ? i+1 : 0;
     out_port = sequence[i];    
     DelayMs(Speed);         //改变这个参数可以调整电机转速 ,
                             //数字越小，转速越大,力矩越小
  }
}

/*------------------------------------------------
                 外部中断程序
------------------------------------------------*/
void ISR_INT0(void) interrupt 0 
{
 
 if(!INT0)
   {
   DelayMs(10);//在此处可以添加去抖动程序，防止按键抖动造成错误
   if(!INT0)
   //while(!INT1);//等待按键释放
     {
     Speed++;   
	 if(Speed==20)
	   Speed=3;
   	 TempData[0]= DuanMa[Speed/10];//分解显示信息，如要显示68，  
	 TempData[1]= DuanMa[Speed%10];//则68/10=6  68%10=8
     }
   }
}
/*------------------------------------------------
 显示函数，用于动态扫描数码管
 输入参数 FirstBit 表示需要显示的第一位，如赋值2表示从第三个数码管开始显示
 如输入0表示从第一个显示。
 Num表示需要显示的位数，如需要显示99两位数值则该值输入2
------------------------------------------------*/
void Display(unsigned char FirstBit,unsigned char Num)
{
      static unsigned char i=0;
	  

	   DataPort=0;   //清空数据，防止有交替重影
       LATCH1=1;     //段锁存
       LATCH1=0;

       DataPort= WeiMa[i+FirstBit]; //取位码 
       LATCH2=1;     //位锁存
       LATCH2=0;

       DataPort=TempData[i]; //取显示数据，段码
       LATCH1=1;     //段锁存
       LATCH1=0;
       
	   i++;
       if(i==Num)
	      i=0;


}
/*------------------------------------------------
                    定时器初始化子程序
------------------------------------------------*/
void Init_Timer0(void)
{
 TMOD |= 0x01;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响		     
 //TH0=0x00;	      //给定初值
 //TL0=0x00;
 EA=1;            //总中断打开
 ET0=1;           //定时器中断打开
 TR0=1;           //定时器开关打开
 PT0=1;           //优先级打开
}
/*------------------------------------------------
                 定时器中断子程序
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
 TH0=(65536-2000)/256;		  //重新赋值 2ms
 TL0=(65536-2000)%256;
 
 Display(0,8);

}

