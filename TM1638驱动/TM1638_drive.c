/*********************************************************************

 ********************************************************************/

#include <TM1638_drive.h>
#include "intrins.h"
 //共阴数码管段码
unsigned char code Num_tab[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
															0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
															0x40,0x00};

//TM1638低8段位码  【SEG1-SEG8】
unsigned char code WeiL_tab[]={0xce,0xcc,0xca,0xc8,0xc6,0xc4,0xc2,0xc0};

//TM1638高2段位码  【SEG9-SEG10】
unsigned char code WeiH_tab[]={0xcf,0xcd,0xcb,0xc9,0xc7,0xc5,0xc3,0xc1};

void TM1638_W_DATA(unsigned char	DATA)			//写数据函数
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

void TM1638_W_CMD(unsigned char cmd)		//写命令函数
{
	STB=0;
	TM1638_W_DATA(cmd);
	STB=1;
}

void Write_DATA(unsigned char add,unsigned char DATA)		//指定地址写入数据
{
	TM1638_W_CMD(0x44);
	STB=0;
	TM1638_W_DATA(add);
	TM1638_W_DATA(DATA);
	STB=1;
}

unsigned char TM1638_Read(void)					//读数据函数
{
	unsigned char i;
	unsigned char temp=0;
	DIO=1;	//设置为输入
	for(i=0;i<8;i++)
	{
		temp>>=1;
		
		CLK=0;
		CLK=1;							   //CLK上升沿读数据才有效

		if(DIO)
			temp|=0x80;
	}
	return temp;
}

unsigned char Read_Onekey(void)	           //读一下按键值，不支持组合键
{
	unsigned char key_val[4],i,key=0;	
	STB=0;	
	TM1638_W_DATA(0x42);		           //读键扫数据命令  注意：这里要先读完4字节键值才能将STB置1，否则读键值无效
				_nop_();				//至少延时1微秒以上
				_nop_();
	for(i=0;i<4;i++)		
		key_val[i]=TM1638_Read();	
			STB=1;					           //4个字节数据合成一个字节
	 if(key_val[0]!=0) //按键在KS1或KS2
  {
    switch(key_val[0])
    {
      case 1:
        key=K3S1;
      break;      
      case 2:
        key=K2S1;
      break;
      case 4:
        key=K1S1;
      break;
      case 16:
        key=K3S5;
      break;      
      case 32:
        key=K2S5;
      break;     
      case 64:
        key=K1S5;
      break;
    }
  }
  else if(key_val[1]!=0)        //按键在KS3或KS4
  {
    switch(key_val[1])
    {
      case 1:
        key=K3S2;
      break;      
      case 2:
        key=K2S2;
      break;      
      case 4:
        key=K1S2;
      break;
      case 16:
        key=K3S6;
      break;     
      case 32:
        key=K2S6;
      break;    
      case 64:
        key=K1S6;
      break;
      case 255:
      init_TM1638();
       break;
    }
  }
  else if(key_val[2]!=0)        //按键在KS5或KS6
  {
    switch(key_val[2])
    {
      case 1:
        key=K3S3;
      break;     
      case 2:
        key=K2S3;
      break;     
      case 4:
        key=K1S3;
      break;
      case 16:
        key=K3S7;
      break;     
      case 32:
        key=K2S7;
      break;
      case 64:
        key=K1S7;
      break;
      case 255:
      init_TM1638();
       break;
    }
  }
  else if(key_val[3]!=0)        //按键在KS7或KS8
  {
    switch(key_val[3])
    {
      case 1:
        key=K3S4;
      break;      
      case 2:
        key=K2S4;
      break;      
      case 4:
        key=K1S4;
      break;
      case 16:
        key=K3S8;
      break;     
      case 32:
        key=K2S8;
      break;     
      case 64:
        key=K1S8;
      break;
        case 255:
      init_TM1638();
       break;
    }
  }
  
  return key;

}

void WriteSEG9_LED(unsigned char sta)					//控制全部LED函数，LED_flag表示各个LED状态
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		if(sta&(1<<i))
			Write_DATA(WeiH_tab[i],1);
		else
			Write_DATA(WeiH_tab[i],0);
	}
}

void WriteSEG10_LED(unsigned char sta)					//控制全部LED函数，LED_flag表示各个LED状态
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		if(sta&(1<<i))
			Write_DATA(WeiH_tab[i],2);
		else
			Write_DATA(WeiH_tab[i],0);
	}
}

void Write_LED(unsigned char sta9,unsigned char sta10)					//控制全部LED函数，LED_flag表示各个LED状态
{
	// 0b 1 0 0 0 0 0 0 0
	// 0b 0 1 1 0 0 0 0 0 

	Write_DATA(WeiH_tab[7],((sta10&0x80)>>6)|((sta9&0x80)>>7));
	Write_DATA(WeiH_tab[6],((sta10&0x40)>>5)|((sta9&0x40)>>6));
	Write_DATA(WeiH_tab[5],((sta10&0x20)>>4)|((sta9&0x20)>>5));
	Write_DATA(WeiH_tab[4],((sta10&0x10)>>3)|((sta9&0x10)>>4));
	Write_DATA(WeiH_tab[3],((sta10&0x08)>>2)|((sta9&0x08)>>3));
	Write_DATA(WeiH_tab[2],((sta10&0x04)>>1)|((sta9&0x04)>>2));
	Write_DATA(WeiH_tab[1],(sta10&0x02)|((sta9&0x02)>>1));
	Write_DATA(WeiH_tab[0],((sta10&0x01)<<1)|(sta9&0x01));

}

//TM1638初始化函数
void init_TM1638(void)
{
	unsigned char i;
	TM1638_W_CMD(0x89);       //亮度 (0x88-0x8f)8级亮度可调
	TM1638_W_CMD(0x40);       //采用地址自动加1
	STB=0;		           //
	TM1638_W_DATA(0xc0);    //设置起始地址

	for(i=0;i<16;i++)	   //传送16个字节的数据
		TM1638_W_DATA(0x00);
	STB=1;
}

