#ifndef	_TM1638_H
#define	_TM1638_H

#include	<REGX51.H>
#include	<intrins.h>
#define	DATA_COMMAND	0X40
#define	DISP_COMMAND	0x80
#define	ADDR_COMMAND	0XC0

//TM1638ģ�����Ŷ���
sbit	DIO=P1^0;
sbit	CLK=P1^1;
sbit	STB=P1^2;
void Delay1000ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 8;
	j = 154;
	k = 122;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

//�����������ʾ����  //---------------      0    1    2    3    4    5    6    7    8 9    0.   1.   2.   3.   4.   5.   6.   7.   8.   9.   �� 
unsigned char code tab[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
                           0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
														0x40,0x00
};


void TM1638_Write(unsigned char	DATA)			//д���ݺ���
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		CLK=0;
		if(DATA&0X01)
			DIO=1;
		else
			DIO=0;
		DATA>>=1;
		CLK=1;
	}
}
unsigned char TM1638_Read(void)					//�����ݺ���
{
	unsigned char i;
	unsigned char temp=0;
	DIO=1;	//����Ϊ����
	for(i=0;i<8;i++)
	{
		temp>>=1;
		CLK=0;
		if(DIO)
			temp|=0x80;
		CLK=1;
	}
	return temp;
}
void Write_COM(unsigned char cmd)		//����������
{
	STB=0;
	TM1638_Write(cmd);
	STB=1;
}
unsigned char Read_key(void)
{
	unsigned char c[4],i,key_value=0;
	STB=0;
	TM1638_Write(0x42);		           //����ɨ���� ����
	for(i=0;i<4;i++)		
		c[i]=TM1638_Read();
	STB=1;					           //4���ֽ����ݺϳ�һ���ֽ�
	for(i=0;i<4;i++)
		key_value|=c[i]<<i;
	for(i=0;i<8;i++)
		if((0x01<<i)==key_value)
			break;
	return i;
}
/*****ָ����ַд������*******/
void Write_DATA(unsigned char addr,unsigned char DATA)		//ָ����ַд������
{
	Write_COM(0x44);//����д�Դ����������,���ù̶���ַ
	STB=0;
	TM1638_Write(0xc0|addr);//C0��ʼ��ַ��
	_nop_();
	TM1638_Write(DATA);
	STB=1;
}

/*****�̶���ַ����*******/
void TM1638_display_Fix(unsigned char add,unsigned char DATA)
{	//�̶���ַ����
	//	unsigned char TM1638_Addr_SEG[8]= {0xC0,0xC2,0xC4,0xC6,0xC8,0xCA,0xCC,0xCE}; //ģ����ҵ���İ˸������
const unsigned char buff[22]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0xbf,0x86,0xdb,0xcf,
									0xe6,0xed,0xfd,0x87,0xff,0xef,0x40,0x00};//����0~9��0~9�ӵ���ʾ�����    
	Write_COM(0X44);
	STB=0;
	TM1638_Write(0xC0+2*add);
		_nop_();
	TM1638_Write(buff[DATA]);			
	STB=1;
}

void Write_oneLED(unsigned char num,unsigned char flag)	//��������һ��LED������numΪ��Ҫ���Ƶ�led��ţ�flagΪ0ʱϨ�𣬲�Ϊ0ʱ����
{

unsigned char TM1638_Addr_LED[8]= {0xc1,0xC3,0xC5,0xC7,0xC9,0xCB,0xCD,0xCF}; //�˸���������ܵ�ַ
	Write_COM(0x44);
	if(flag)
	{
//	 Write_DATA(2*num+1,1);
	Write_COM(0x44);//����д�Դ����������,���ù̶���ַ
	STB=0;
	TM1638_Write(TM1638_Addr_LED[num]);//C0��ʼ��ַ+ledλ�õ�ַ
	TM1638_Write(flag);
	STB=1;
	//Write_DATA(TM1638_Addr_LED[num],1);
	}
	else{
//		Write_DATA(2*num+1,0);
	Write_COM(0x44);//����д�Դ����������,���ù̶���ַ
	STB=0;
	TM1638_Write(TM1638_Addr_LED[num]);//C0��ʼ��ַ��
	TM1638_Write(flag);
	STB=1;
	}
}  	
void Write_allLED(unsigned char LED_flag)					//����ȫ��LED������LED_flag��ʾ����LED״̬
{
	unsigned char i;
	for(i=0;i<8;i++){
		
//			for(i=8;i<11;i++){

			if(LED_flag&(1<<i)){
			Write_DATA(2*i+1,1);

	}
			else
				Write_DATA(2*i+1,0);
		}
}

//TM1638��ʼ������
void init_TM1638(void)
{
	unsigned char i;
	Write_COM(0x8b);       //���� (0x88-0x8f)8�����ȿɵ�
	Write_COM(0x40);       //���õ�ַ�Զ���1
	STB=0;		           //
	TM1638_Write(0xc0);    //������ʼ��ַ
		_nop_();
	for(i=0;i<16;i++)	   //����16���ֽڵ�����0x00-0x0f
		TM1638_Write(0x00);
	STB=1;
}
#endif
