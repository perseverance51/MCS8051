

sbit led=P1^0;
unsigned char w=50;

void Timer0Init(void)		//100微秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x9C;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	EA = 1;
	ET0 = 1;
}
void main()
{

   Timer0Init();
   while(1)
   {
   }

}

void Timer0() interrupt 1
{
  	static unsigned int i,k;
	static bit flag=1;
	if(++i*5<w)led=0x01; 
	else led=0x00;
	if(i==10){i=0;}
	//每隔120ms更新占空比，即120000us
	if(++k==1200)
	{
	  k=0;
	  if(w==95)flag=0;//占空比等于95就开始减小
	  if(w==5)flag=1;//占空比等于5就开始增大
	  if(flag)
	  {
	    w=w+5;
	  }
	  else w=w-5;
	}
    
}
