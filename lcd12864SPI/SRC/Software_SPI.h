#ifndef SOFTWARE_SPI_H_
#define SOFTWARE_SPI_H_

#include <STC89C5xRC.H>


#define uint8_t unsigned char
	
sbit SPI_SCK	= P2^7;
sbit SPI_MOSI	= P2^5;
sbit SPI_MISO	= P2^5;

uint8_t SOFT_SPI_RW_MODE0(uint8_t write_dat);
uint8_t SOFT_SPI_RW_MODE1(uint8_t write_dat);
uint8_t SOFT_SPI_RW_MODE2(uint8_t write_dat);
uint8_t SOFT_SPI_RW_MODE3(uint8_t write_dat);

#endif
