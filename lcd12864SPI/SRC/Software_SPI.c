#include "Software_SPI.h"

void SPI_Delay()	//ÿ���ļ�� ���ڵȴ���ƽ�ȶ��Ϳ���ͨѶ����
{

}

//MOSI���� ��ֲʱ���޸�
void MOSI_H()
{
	SPI_MOSI = 1;
}

//MOSI���� ��ֲʱ���޸�
void MOSI_L()
{
	SPI_MOSI = 0;
}

//MISO���� ��ֲʱ���޸�
void MISO_H()
{
	SPI_MISO = 1;
}

//MISO���� ��ֲʱ���޸�
void MISO_L()
{
	SPI_MISO = 0;
}

//SCK���� ��ֲʱ���޸�
void SPI_SCK_H()
{
	SPI_SCK = 1;
}

//SCK���� ��ֲʱ���޸�
void SPI_SCK_L()
{
	SPI_SCK = 0;
}

//��ȡMISO��ƽ ��ֲʱ���޸�
uint8_t MISO_Read()
{
	SPI_MISO = 1;
	SPI_Delay();
	return SPI_MISO;
}

//����ʱʱ�Ӽ���(CPOL)		0Ϊ�͵�ƽ 1Ϊ�ߵ�ƽ
//������Чʱ��Ե��λ(CPHA)	0Ϊ����Ե 1Ϊż��Ե

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
 
/* CPOL=0��CPHA=1, MSB first */
uint8_t SOFT_SPI_RW_MODE1(uint8_t write_dat) 
{
    uint8_t i, read_dat = 0;

	SPI_SCK_L();
	SPI_Delay();
	for(i = 0; i < 8; i++)	//ѭ��8��
	{
		SPI_SCK_H();		//����ʱ��
		if(write_dat & 0x80)
			MOSI_H();		//���λΪ�ߣ��������
		else      
			MOSI_L();		//���λΪ�ͣ��������
		write_dat <<= 1;	//��һλ��λ�����λ
		SPI_Delay();
		SPI_SCK_L();		//����ʱ��
		read_dat <<= 1;		//��������
		read_dat |= MISO_Read();
	}
	return read_dat;		//��������
}
 
/* CPOL=1��CPHA=0, MSB first */
uint8_t SOFT_SPI_RW_MODE2(uint8_t write_dat) 
{
    uint8_t i, read_dat = 0;

	SPI_SCK_H();
	SPI_Delay();
	for(i = 0; i < 8; i++)	// ѭ��8��
	{
		if(write_dat & 0x80)
			MOSI_H();		//���λΪ�ߣ��������
		else      
			MOSI_L();		//���λΪ�ͣ��������
		write_dat <<= 1;	//��һλ��λ�����λ
		SPI_Delay();
		SPI_SCK_L();		//����ʱ��
		read_dat <<= 1;     //��������
		read_dat |= MISO_Read();    
		SPI_SCK_H();		//����ʱ��
	}
	return read_dat;		//��������
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
