#include<reg51.h>
#include<intrins.h>
#include<LCD1602.h>
#define uchar unsigned char
#define uint unsigned int
sbit Data=P3^6;   //����������
uchar rec_dat[9];   //������ʾ�Ľ�����������

bit fg=1;

//��ʼ������
void DHT11_delay_us(uchar n)
{
    while(--n);
}

void DHT11_delay_ms(uint z)
{
   uint i,j;
   for(i=z;i>0;i--)
      for(j=110;j>0;j--);
}

void DHT11_start()
{
   Data=1;
   DHT11_delay_us(2);
   Data=0;
   DHT11_delay_ms(30);   //��ʱ18ms����
   Data=1;
   DHT11_delay_us(30);
}

uchar DHT11_rec_byte()      //����һ���ֽ�
{
   uchar i,dat=0;
  for(i=0;i<8;i++)    //�Ӹߵ������ν���8λ����
   {         
      while(!Data);   ////�ȴ�50us�͵�ƽ��ȥ
      DHT11_delay_us(8);     //��ʱ60us�������Ϊ��������Ϊ1������Ϊ0
      dat<<=1;           //��λʹ��ȷ����8λ���ݣ�����Ϊ0ʱֱ����λ
      if(Data==1)    //����Ϊ1ʱ��ʹdat��1����������1
         dat+=1;
      while(Data);  //�ȴ�����������   
    }  
    return dat;
}

void DHT11_receive()      //����40λ������
{
    uchar R_H,R_L,T_H,T_L,RH,RL,TH,TL,revise;
    DHT11_start();
    if(Data==0)
    {
        while(Data==0);   //�ȴ�����     
        DHT11_delay_us(40);  //���ߺ���ʱ80us
        R_H=DHT11_rec_byte();    //����ʪ�ȸ߰�λ  
        R_L=DHT11_rec_byte();    //����ʪ�ȵͰ�λ  
        T_H=DHT11_rec_byte();    //�����¶ȸ߰�λ  
        T_L=DHT11_rec_byte();    //�����¶ȵͰ�λ
        revise=DHT11_rec_byte(); //����У��λ

        DHT11_delay_us(25);    //����

        if((R_H+R_L+T_H+T_L)==revise)      //У��
        {
            RH=R_H;
            RL=R_L;
            TH=T_H;
            TL=T_L;
        }
        /*���ݴ���������ʾ*/
        rec_dat[0]='0'+(RH/10);
        rec_dat[1]='0'+(RH%10);
        rec_dat[2]='R';
        rec_dat[3]='H';
        rec_dat[4]=' ';
        rec_dat[5]=' ';
        rec_dat[6]='0'+(TH/10);
        rec_dat[7]='0'+(TH%10);
        rec_dat[8]='C';
    }
}
 


void main()   
{   
	LCD_init();
	LCD_send(1,0,"Temperature:",12);

  DHT11_delay_ms(100);    //DHT11�ϵ��Ҫ�ȴ�1S��Խ�����ȶ�״̬�ڴ��ڼ䲻�ܷ����κ�ָ��
       
	while(1)   
	{   
		DHT11_receive();
		LCD_send(2,0,rec_dat,9);
	}   
	
}   
   
                                       