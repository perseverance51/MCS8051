//74hc164�������������Դ����
//���������74hc164��������
#include <STC89C5xRC.H>
#define uint unsigned int
#define uchar unsigned char
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
sbit DAT = P3^7; //ģ�⴮�����ݷ��Ͷ�
sbit CLK = P3^4;//ģ��ʱ�ӿ��ƶ�
//���������
uchar code tab[]= {0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xff,0xf6}; //0-9,-,ȫ��
uchar code

void sendbyte(uchar a)
 {
    uchar num ,c;
    num = tab[a];
    for(c=0; c<8; c++)
        {
        CLK =0;
		//			DAT = num&0x01;
        DAT = bitRead(num,c);//��λ�����
        CLK = 1;
    //   num >>= 1;       
    }   
}

void delay_50ms(uint x) //��ʱ50ms    12����
{
 TMOD=0x01; //����ʱ��0��������ʽΪ1
 TR0=1;     //������ʱ��0��
 while(x--)
 {  
    TH0=(65536-50000)/256; //��ʱ50ms��ֵ�ĸ߰�λװ��TH0
    TL0=(65536-50000)%256; //��ʱ50ms��ֵ�ĵͰ�λװ��TL0 
    while(!TF0); //�ȴ���ֱ��TF0Ϊ1
    TF0=0;    //���������־λ
  }
  TR0=0;  //ֹͣ��ʱ��0��
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