/**********************************************************************************
 * 程序名:    TM1638驱动程序
 * 作者:      DaveoCKII
 * 日期:      2020.2.29
 * 版本:      STC12C5A60S2
 **********************************************************************************/

#ifndef _TM1638_H_
#define _TM1638_H_
#include <REGX52.H>
#include	<intrins.h>

//------------------------------------定义列表------------------------------------//
sbit STB = P1^2;        // 片选线
sbit CLK = P1^1;        // 时钟线
sbit DIO = P1^0;        // 数据线
unsigned char code led_buffer[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 	// 显示数据缓冲区
unsigned char code LED_CC[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};  // 共阴LED显示数据 '0~F'
unsigned char code LED_CA[16]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};  // 共阳LED显示数据 '0~F'
unsigned char code x[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};   	// 数据转换中用来提取各位的数据
//------------------------------------函数列表------------------------------------//

void TM1638_LEDCC(unsigned char LEDnums);   // TM1638 LED显示(共阴)

void TM1638_LEDCA(unsigned char LEDnums);   // TM1638 LED显示(共阳)

void TM1638_Write(unsigned char	DATA);      // TM1638 写数据函数

void TM1638_WriteCOM(unsigned char cmd);    // TM1638 写命令函数

unsigned char TM1638_Read(void);            // TM1638 读数据函数

unsigned char TM1638_ReadKey(void);         // TM1638 读键函数

//------------------------------------函数内容------------------------------------//

void TM1638_Write(unsigned char	DATA)	
{
	unsigned char i;
	for(i=0;i<8;i++)     // 1Byte 8位数据
	{
		CLK=0;           // 拉低时钟线
		DIO=DATA&0X01;   // 发送数据
		DATA>>=1;        // 数据格式:低位在前
		CLK=1;           // 拉高时钟线,写入数据
	}
}

void TM1638_WriteCOM(unsigned char cmd)
{
	STB=0;               // 拉低片选线
	TM1638_Write(cmd);   // 写命令
	STB=1;               // 拉高片选线
}

unsigned char TM1638_Read(void)
{
	unsigned char i,Data=0;
    
	DIO=1;               // 初始化数据线
	for(i=0;i<8;i++)    
	{
		Data>>=1;        // 数据格式:低位在前
		CLK=0;
		if(DIO)
			Data|=0x80;
		CLK=1;
	}
	return Data;
}

unsigned char TM1638_ReadKey(void)
{
    unsigned char c[4];         // 4Byte数据储存
    unsigned char i=5;		// 先用作延时
    unsigned char key_value=0;  
    
    STB=0;                      // 拉低片选线,开始读键值
    TM1638_Write(0x42);         // 写'读数据'命令
	while(i--)                  // 等待准备完毕
	for(i=0;i<4;i++)            // 读取4个Byte
		c[i]=TM1638_Read();   
	STB=1;                      // 拉高片选线,读键值结束
    // 8键  col:K3  row:KS1~KS8
    if(c[0]==0x01)  key_value=1;
	if(c[0]==0x10)  key_value=5;
	if(c[1]==0x01)  key_value=2;
	if(c[1]==0x10)  key_value=6;
	if(c[2]==0x01)  key_value=3;
	if(c[2]==0x10)  key_value=7;
	if(c[3]==0x01)  key_value=4;
	if(c[3]==0x10)  key_value=8;
    
    return (key_value);         // 返回键值
}
//共阴led灯
void TM1638_LEDCC(unsigned char LEDnums)
{
	unsigned char i;    
    TM1638_WriteCOM(0x40);      	// 设置地址自增 模式  
    STB=0;				// 拉低片选线,开始写数据
    TM1638_Write(0xc0);             // 写首地址
    for(i=0; i<LEDnums; i++) {      // 写显示数据
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
    STB=1;				// 拉高片选线,结束写数据
}
void Write_oneLED(unsigned char num,unsigned char flag)	//单独控制一个LED函数，num为需要控制的led序号，flag为0时熄灭，不为0时点亮
{
	unsigned char TM1638_Addr_LED[8]= {0xC1,0xC3,0xC5,0xC7,0xC9,0xCB,0xCD,0xCF}; //模块从右到左的八个发光二极管
	unsigned char i;    
    TM1638_WriteCOM(0x44);      	// 设置固定地址写数据 ，最多14BYTE数据传送完毕 
    STB=0;				// 拉低片选线,开始写数据
    TM1638_Write(TM1638_Addr_LED[num]);  // 写数据地址
    // 写显示数据
        TM1638_Write(LED_CC[i]); // 写数据
			_nop_();
	
        TM1638_Write(LED_CC[i+8]);
			//		TM1638_Write(0x80);
			_nop_();   
				STB=1;				// 拉高片选线,结束写数据
} 
//共阳led灯
void TM1638_LEDCA(unsigned char LEDnums)
{
	unsigned char i;
    unsigned char datas[16];
    
    // 数据转换
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
        
    TM1638_WriteCOM(0x40);			// 设置地址自增  
    STB=0;				// 拉低片选线,开始写数据
    TM1638_Write(0xc0);             // 写首地址
    for(i=0; i<LEDnums; i++) {      // 写显示数据
        TM1638_Write(datas[i]); 
        TM1638_Write(datas[i+8]);
    }
    for(i=LEDnums; i<8; i++) {
        TM1638_Write(0); 
        TM1638_Write(0); 
    }
    STB=1;				// 拉低片选线,结束写数据
}

//TM1638初始化函数
void init_TM1638(void)
{
	unsigned char i;
	 TM1638_WriteCOM(0x8b);//亮度 (0x88-0x8f)8级亮度可调
	TM1638_WriteCOM(0x40);       //采用地址自动加1
	STB=0;		           //
	TM1638_Write(0xc0);    //设置起始地址
		_nop_();
	for(i=0;i<16;i++)	   //传送16个字节的数据0x00-0x0f
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
