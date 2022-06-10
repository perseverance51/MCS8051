//单片机+ADC0832实现量程0~25V数字电压表的仿真程序
#include<reg51.h>
#include<intrins.h>

sbit CS=P3^5;
sbit CLK=P3^3;
sbit DI=P3^4;
sbit DO=P3^4;
sbit dot=P0^5;      //小数点

unsigned char dat=0x00;
unsigned char nums[]={0x00,0x00,0x00,0x00};
unsigned char channel;      //转换通道
unsigned char LEDS[]={0xA0,0xBB,0x62,0x2A,0x39,0x2C,0x24,0xB8,0x20,0x28};       //共阳4位数码管段码，字符显示从'0'到'9'

unsigned char ADC0832(unsigned char channel)        //模数转换
{
    unsigned char i;
    unsigned char x=0x00;
    unsigned char y=0x00;

    CLK=0;      //初始化
    DI=1;
    _nop_();
    CS=0;
    _nop_();
    CLK=1;
    _nop_();

    if(channel==0x00)       //通道选择
    {       //通道0
        CLK=0;
        DI=1;
        _nop_();
        CLK=1;
        _nop_();
        CLK=0;
        DI=0;
        _nop_();
        CLK=1;
        _nop_();
    }
    else        //通道1
    {
        CLK=0;
        DI=1;
        _nop_();
        CLK=1;
        _nop_();
        CLK=0;
        DI=1;
        _nop_();
        CLK=1;
        _nop_();
    }

    CLK=0;
    DI=1;
    for(i=0;i<8;i++)        //读取ADC0832输出的前8位数据
    {
        x<<=1;
        _nop_();
        CLK=1;
        _nop_();
        CLK=0;
        if(DO==1)
        {
            x|=0x01;
        }
        else
        {
            x|=0x00;
        }
    }
    for(i=0;i<8;i++)        //读取ADC0832输出的后8位数据
    {
        y>>=1;
        if(DO==1)
        {
            y|=0x80;
        }
        else
        {
            y|=0x00;
        }
        _nop_();
        CLK=1;
        _nop_();
        CLK=0;
    }
    if(x==y)        //校验，比较前8位与后8位的值
    {
        dat=x;
    }
    _nop_();
    CS=1;       //释放ADC0832
    DO=1;
    CLK=1;
    return dat;
}

void Convert(unsigned char dat0)        //数据处理
{
    unsigned int x;
    x=dat0*0.0196*100*5;
    nums[0]=x/1000;
    nums[1]=x%1000/100;
    nums[2]=x%1000%100/10;
    nums[3]=x%1000%100%10;
}

void Delay()        //数码管显示
{
    unsigned char i,j;
    for(i=0;i<1;i++)
    {
        for(j=0;j<20;j++);
    }
}

void Display()
{
    P0=LEDS[nums[0]];
    P2 =~0xFE;
    Delay();
    P2 &=0xF0;//消隐

    P0=LEDS[nums[1]];
    dot=0;
    P2 =~0xFD;
    Delay();
    P2 &=0xF0;

    P0=LEDS[nums[2]];
    P2 =~0xFB;
    Delay();
    P2 &=0xF0;

    P0=LEDS[nums[3]];
    P2 =~0xF7;
    Delay();
    P2 &=0xF0;
}

void main()
{
    while(1)
    {
        dat=ADC0832(channel);
        Convert(dat);
        Display();
    }
}