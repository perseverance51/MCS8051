/**********************************************************************************
 * ������:    TM1638��������
 * ����:      DaveoCKII
 * ����:      2020.2.29
 * �汾:      STC12C5A60S2
 **********************************************************************************/

#ifndef _TM1638_H_
#define _TM1638_H_
#include <REGX52.H>
#include	<intrins.h>

//------------------------------------�����б�------------------------------------//
sbit STB = P1^2;        // Ƭѡ��
sbit CLK = P1^1;        // ʱ����
sbit DIO = P1^0;        // ������
unsigned char code led_buffer[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 	// ��ʾ���ݻ�����
unsigned char code LED_CC[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};  // ����LED��ʾ���� '0~F'
unsigned char code LED_CA[16]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};  // ����LED��ʾ���� '0~F'
unsigned char code x[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};   	// ����ת����������ȡ��λ������
//------------------------------------�����б�------------------------------------//

void TM1638_LEDCC(unsigned char LEDnums);   // TM1638 LED��ʾ(����)

void TM1638_LEDCA(unsigned char LEDnums);   // TM1638 LED��ʾ(����)

void TM1638_Write(unsigned char	DATA);      // TM1638 д���ݺ���

void TM1638_WriteCOM(unsigned char cmd);    // TM1638 д�����

unsigned char TM1638_Read(void);            // TM1638 �����ݺ���

unsigned char TM1638_ReadKey(void);         // TM1638 ��������

//------------------------------------��������------------------------------------//

void TM1638_Write(unsigned char	DATA)	
{
	unsigned char i;
	for(i=0;i<8;i++)     // 1Byte 8λ����
	{
		CLK=0;           // ����ʱ����
		DIO=DATA&0X01;   // ��������
		DATA>>=1;        // ���ݸ�ʽ:��λ��ǰ
		CLK=1;           // ����ʱ����,д������
	}
}

void TM1638_WriteCOM(unsigned char cmd)
{
	STB=0;               // ����Ƭѡ��
	TM1638_Write(cmd);   // д����
	STB=1;               // ����Ƭѡ��
}

unsigned char TM1638_Read(void)
{
	unsigned char i,Data=0;
    
	DIO=1;               // ��ʼ��������
	for(i=0;i<8;i++)    
	{
		Data>>=1;        // ���ݸ�ʽ:��λ��ǰ
		CLK=0;
		if(DIO)
			Data|=0x80;
		CLK=1;
	}
	return Data;
}

unsigned char TM1638_ReadKey(void)
{
    unsigned char c[4];         // 4Byte���ݴ���
    unsigned char i=5;		// ��������ʱ
    unsigned char key_value=0;  
    
    STB=0;                      // ����Ƭѡ��,��ʼ����ֵ
    TM1638_Write(0x42);         // д'������'����
	while(i--)                  // �ȴ�׼�����
	for(i=0;i<4;i++)            // ��ȡ4��Byte
		c[i]=TM1638_Read();   
	STB=1;                      // ����Ƭѡ��,����ֵ����
    // 8��  col:K3  row:KS1~KS8
    if(c[0]==0x01)  key_value=1;
	if(c[0]==0x10)  key_value=5;
	if(c[1]==0x01)  key_value=2;
	if(c[1]==0x10)  key_value=6;
	if(c[2]==0x01)  key_value=3;
	if(c[2]==0x10)  key_value=7;
	if(c[3]==0x01)  key_value=4;
	if(c[3]==0x10)  key_value=8;
    
    return (key_value);         // ���ؼ�ֵ
}
//����led��
void TM1638_LEDCC(unsigned char LEDnums)
{
	unsigned char i;    
    TM1638_WriteCOM(0x40);      	// ���õ�ַ���� ģʽ  
    STB=0;				// ����Ƭѡ��,��ʼд����
    TM1638_Write(0xc0);             // д�׵�ַ
    for(i=0; i<LEDnums; i++) {      // д��ʾ����
        TM1638_Write(LED_CC[i]); 
			_nop_();
        TM1638_Write(LED_CC[i+8]);
			_nop_();
    }
    for(i=LEDnums; i<8; i++) {      
        TM1638_Write(0); 
			_nop_();
        TM1638_Write(0); 
			_nop_();
    }    
    STB=1;				// ����Ƭѡ��,����д����
}
void Write_oneLED(unsigned char num,unsigned char flag)	//��������һ��LED������numΪ��Ҫ���Ƶ�led��ţ�flagΪ0ʱϨ�𣬲�Ϊ0ʱ����
{
	unsigned char TM1638_Addr_LED[8]= {0xC1,0xC3,0xC5,0xC7,0xC9,0xCB,0xCD,0xCF}; //ģ����ҵ���İ˸����������
	unsigned char i;    
    TM1638_WriteCOM(0x44);      	// ���ù̶���ַд���� �����14BYTE���ݴ������ 
    STB=0;				// ����Ƭѡ��,��ʼд����
    TM1638_Write(TM1638_Addr_LED[num]);  // д���ݵ�ַ
    // д��ʾ����
        TM1638_Write(LED_CC[i]); // д����
			_nop_();
	
        TM1638_Write(LED_CC[i+8]);
			//		TM1638_Write(0x80);
			_nop_();   
				STB=1;				// ����Ƭѡ��,����д����
} 
//����led��
void TM1638_LEDCA(unsigned char LEDnums)
{
	unsigned char i;
    unsigned char datas[16];
    
    // ����ת��
    for(i=0; i<8; i++){
        datas[i]=(LED_CA[0]&x[i])+(LED_CA[1]&x[i])+(LED_CA[2]&x[i])+(LED_CA[3]&x[i])+
                 (LED_CA[4]&x[i])+(LED_CA[5]&x[i])+(LED_CA[6]&x[i])+(LED_CA[7]&x[i]);
    }
    if(LEDnums>8){
        for(i=0; i<8; i++)
            datas[i+8]=(LED_CA[8]&x[i])+(LED_CA[9]&x[i]);
    }
    else {
        for(i=0; i<8; i++)
            datas[i]=0x00;
    }
        
    TM1638_WriteCOM(0x40);			// ���õ�ַ����  
    STB=0;				// ����Ƭѡ��,��ʼд����
    TM1638_Write(0xc0);             // д�׵�ַ
    for(i=0; i<LEDnums; i++) {      // д��ʾ����
        TM1638_Write(datas[i]); 
        TM1638_Write(datas[i+8]);
    }
    for(i=LEDnums; i<8; i++) {
        TM1638_Write(0); 
        TM1638_Write(0); 
    }
    STB=1;				// ����Ƭѡ��,����д����
}

//TM1638��ʼ������
void init_TM1638(void)
{
	unsigned char i;
	 TM1638_WriteCOM(0x8b);//���� (0x88-0x8f)8�����ȿɵ�
	TM1638_WriteCOM(0x40);       //���õ�ַ�Զ���1
	STB=0;		           //
	TM1638_Write(0xc0);    //������ʼ��ַ
		_nop_();
	for(i=0;i<16;i++)	   //����16���ֽڵ�����0x00-0x0f
		TM1638_Write(0x00);
	STB=1;
}

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
//--------------------------------------------------------------------------------//


#endif 
