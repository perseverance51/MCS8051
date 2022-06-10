#include "Software_SPI.h"

void SPI_Delay()	//每步的间隔 用于等待电平稳定和控制通讯速率
{

}

//MOSI拉高 移植时需修改
void MOSI_H()
{
	SPI_MOSI = 1;
}

//MOSI拉低 移植时需修改
void MOSI_L()
{
	SPI_MOSI = 0;
}

//MISO拉高 移植时需修改
void MISO_H()
{
	SPI_MISO = 1;
}

//MISO拉低 移植时需修改
void MISO_L()
{
	SPI_MISO = 0;
}

//SCK拉高 移植时需修改
void SPI_SCK_H()
{
	SPI_SCK = 1;
}

//SCK拉低 移植时需修改
void SPI_SCK_L()
{
	SPI_SCK = 0;
}

//读取MISO电平 移植时需修改
uint8_t MISO_Read()
{
	SPI_MISO = 1;
	SPI_Delay();
	return SPI_MISO;
}

//空闲时时钟极性(CPOL)		0为低电平 1为高电平
//数据有效时钟缘相位(CPHA)	0为奇数缘 1为偶数缘

/* CPOL = 0, CPHA = 0, MSB first */
uint8_t SOFT_SPI_RW_MODE0(uint8_t write_dat)
{
    uint8_t i, read_dat = 0;

	SPI_SCK_L();
	SPI_Delay();
    for( i = 0; i < 8; i++ )
    {
        if(write_dat & 0x80)
            MOSI_H();  
        else                    
            MOSI_L();  
        write_dat <<= 1;
        SPI_Delay();
        SPI_SCK_H(); 
        read_dat <<= 1;  
		read_dat |= MISO_Read();
        SPI_SCK_L(); 
    }
    return read_dat;
}
 
/* CPOL=0，CPHA=1, MSB first */
uint8_t SOFT_SPI_RW_MODE1(uint8_t write_dat) 
{
    uint8_t i, read_dat = 0;

	SPI_SCK_L();
	SPI_Delay();
	for(i = 0; i < 8; i++)	//循环8次
	{
		SPI_SCK_H();		//拉高时钟
		if(write_dat & 0x80)
			MOSI_H();		//若最到位为高，则输出高
		else      
			MOSI_L();		//若最到位为低，则输出低
		write_dat <<= 1;	//低一位移位到最高位
		SPI_Delay();
		SPI_SCK_L();		//拉低时钟
		read_dat <<= 1;		//数据左移
		read_dat |= MISO_Read();
	}
	return read_dat;		//返回数据
}
 
/* CPOL=1，CPHA=0, MSB first */
uint8_t SOFT_SPI_RW_MODE2(uint8_t write_dat) 
{
    uint8_t i, read_dat = 0;

	SPI_SCK_H();
	SPI_Delay();
	for(i = 0; i < 8; i++)	// 循环8次
	{
		if(write_dat & 0x80)
			MOSI_H();		//若最到位为高，则输出高
		else      
			MOSI_L();		//若最到位为低，则输出低
		write_dat <<= 1;	//低一位移位到最高位
		SPI_Delay();
		SPI_SCK_L();		//拉低时钟
		read_dat <<= 1;     //数据左移
		read_dat |= MISO_Read();    
		SPI_SCK_H();		//拉高时钟
	}
	return read_dat;		//返回数据
}
 
/* CPOL = 1, CPHA = 1, MSB first */
uint8_t SOFT_SPI_RW_MODE3(uint8_t write_dat)
{
    uint8_t i, read_dat = 0;

	SPI_SCK_H();
	SPI_Delay();
    for( i = 0; i < 8; i++ )
    {
		SPI_SCK_L(); 
        if(write_dat & 0x80)
            MOSI_H();  
        else                    
            MOSI_L();  
        write_dat <<= 1;
        SPI_Delay();	
        SPI_SCK_H(); 
        read_dat <<= 1;  
        read_dat |= MISO_Read(); 
    }
    return read_dat;
}
