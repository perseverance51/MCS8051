 #include <reg52.h>
#define uint unsigned int
#define uchar unsigned char	

sbit a = P2^1;							
sbit b = P2^0;
uchar code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90}; //����������0-9 

uint num=5;
uchar shi=0;	
void delayms(uint xms)	//��ʱ����
{		
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}

void display(uint num)				
{
		a=1;	//����������ܸߵ�ƽ��Ч
		P0=table[num%10];	//ѭ����ʾ0-9
		delayms(5);	//ˢ��
		P0=0XFF;			//����
		a=0;

		b=1;	//����������ܸߵ�ƽ��Ч
		P0=table[(num/10)%10];	//ѭ����ʾ0-9
		delayms(5);			//ˢ��
		P0=0XFF;		//����
		b=0;					
}
//�ⲿ�жϷ������
void exint0() interrupt 0 
{
		EX0=0;	//�ر��ⲿ�����ж�����λ
	TL0 = 0xb0;		//���ö�ʱ��ʼֵ
	TH0 = 0x3c;		//���ö�ʱ��ʼֵ
		TR0=1;//������ʱ��0�ж�

}

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1
{
//		EX0 = 0; //�ر�INT0�ж�
	if(shi==20)
	{		//20*50MS��ʱʱ��Ϊ1��
		shi=0;	
		if(!num--){
		TR0=0;
		EX0=1;	//�����ⲿ�����ж�����λ
		num =5;
	}	
	}
	TL0 = 0xb0;		//���ö�ʱ��ʼֵ(65536-50000)%256
	TH0 = 0x3c;		//���ö�ʱ��ʼֵ(65536-50000)/256

 shi++;	
	
}

void main()
{
	TMOD = 0x01; /*��ʱ��������ʽ1*/
	EA=1;	//�ж��ܿ���
	EX0 = 1; //ʹ��INT0�ж�
	ET0=1;	//ʹ�ܶ�ʱ��0�ж�
	IT0=1;//�ⲿ�жϲ����½��ش���
	INT0 = 1;//�ⲿ�ж�������1
	TR0=0;	//��ʱ��0�����ж�λ�ر�
	TF0 = 0;		//���TF0��־
	while(1)
	{											
		display(num);//���ú���	
	}
}


