//��Ƭ��+ADC0832ʵ������0~25V���ֵ�ѹ��ķ������
#include<reg51.h>
#include<intrins.h>

sbit CS=P3^5;
sbit CLK=P3^3;
sbit DI=P3^4;
sbit DO=P3^4;
sbit dot=P0^5;      //С����

unsigned char dat=0x00;
unsigned char nums[]={0x00,0x00,0x00,0x00};
unsigned char channel;      //ת��ͨ��
unsigned char LEDS[]={0xA0,0xBB,0x62,0x2A,0x39,0x2C,0x24,0xB8,0x20,0x28};       //����4λ����ܶ��룬�ַ���ʾ��'0'��'9'

unsigned char ADC0832(unsigned char channel)        //ģ��ת��
{
    unsigned char i;
    unsigned char x=0x00;
    unsigned char y=0x00;

    CLK=0;      //��ʼ��
    DI=1;
    _nop_();
    CS=0;
    _nop_();
    CLK=1;
    _nop_();

    if(channel==0x00)       //ͨ��ѡ��
    {       //ͨ��0
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
    else        //ͨ��1
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
    for(i=0;i<8;i++)        //��ȡADC0832�����ǰ8λ����
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
    for(i=0;i<8;i++)        //��ȡADC0832����ĺ�8λ����
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
    if(x==y)        //У�飬�Ƚ�ǰ8λ���8λ��ֵ
    {
        dat=x;
    }
    _nop_();
    CS=1;       //�ͷ�ADC0832
    DO=1;
    CLK=1;
    return dat;
}

void Convert(unsigned char dat0)        //���ݴ���
{
    unsigned int x;
    x=dat0*0.0196*100*5;
    nums[0]=x/1000;
    nums[1]=x%1000/100;
    nums[2]=x%1000%100/10;
    nums[3]=x%1000%100%10;
}

void Delay()        //�������ʾ
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
    P2 &=0xF0;//����

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