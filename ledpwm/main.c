

sbit led=P1^0;
unsigned char w=50;

void Timer0Init(void)		//100΢��@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x9C;		//���ö�ʱ��ֵ
	TH0 = 0xFF;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
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
	//ÿ��120ms����ռ�ձȣ���120000us
	if(++k==1200)
	{
	  k=0;
	  if(w==95)flag=0;//ռ�ձȵ���95�Ϳ�ʼ��С
	  if(w==5)flag=1;//ռ�ձȵ���5�Ϳ�ʼ����
	  if(flag)
	  {
	    w=w+5;
	  }
	  else w=w-5;
	}
    
}
