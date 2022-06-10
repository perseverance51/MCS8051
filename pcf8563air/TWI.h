/************************************************
文件：TWI.h
用途：TWI头文件
************************************************/
#ifndef __TWI_H__
#define __TWI_H__



extern void twi_init(void);
extern void i2cstart(void);
extern unsigned char i2cwt(unsigned char data);
extern unsigned char i2crd(void);
extern void i2cstop(void);




#endif