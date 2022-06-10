
#include <STC89C5xRC.H>

sbit ds=P1^0;//数据线595的14引脚
sbit shcp=P1^1;//数据输入时钟战595的11脚
sbit stcp=P1^2;//输出存储器锁存时钟线5958的12脚
sbit mr=P1^3;//数据清零
unsigned char code shuju[8]={
//0x00,0x38,0x38,0x38,0xfe,0x7c,0x38,0x10
	0x0C,0x1E,0x3E,0x7C,0x7C,0x3E,0x1E,0x0C
};//LED点阵高电平端口数据
unsigned char code dzwei[8]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};//LED点阵低电平端口位
void QuDong595(unsigned char sj){
	unsigned char aa=8;
				mr=0;
				mr=1;
				while(aa--)
				{
			ds=sj>>7;
			sj<<=1;//十六进制数据发送
			shcp=1;//上升沿发全移位，上升沿时数据寄存器锁存。
			shcp=0;
			}
				stcp=0;
				stcp=1;//上升沿将数据锁存
				stcp=0;
			}
void main()
{
unsigned char Xd=0,ss=1,cs=0,d=0,y=0;
unsigned int ys=0;
	P14 = 0x00;
//ZhongDcionSheZhi();
	while(1)
	{
		P0=dzwei[7-cs];//控制图形方向数据出现((cs+8-d) < 8 ? shuju[cs+8-d]:0x00)
		QuDong595((((7-cs)-d)<8?shuju[(7-cs)-d]:0x00));//数据消失cs+d倒退 (7-cs)-d前进
		//QuDong595(((cs+8-d) < 8?shuju[cs+8-d]:0x00));//数据消失cs+d倒退(7-cs)-d前进
		//P0=~((cs+d) <8 ?shuju[(7-cs)-d]:0x00);
		//QuDong595(~dzwei[7-cs])//90°转向
		while(++Xd);
			mr=0;
			mr=1;
		QuDong595(0x00);
		if(++cs >7)	cs=0;
		if(++y==0){
		if(++d > 8*1) 
		d = 0;
		}
		
		}
	}
