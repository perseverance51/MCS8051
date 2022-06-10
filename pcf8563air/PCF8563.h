/************************************************
�ļ���PCF8563.h
��;��PCF8563����ͷ�ļ�
ע�⣺ϵͳʱ��8M
************************************************/
#ifndef __PCF8563_H__
#define __PCF8563_H__

//������ַ��ʽ�� 1 0 1 0 0 0 A0 R/W
#define W_ADD_COM 0xa2
#define R_ADD_COM 0xa3

#define SLAW 0x18

#define ERR_SLAW 1

extern void clear(unsigned char *p,unsigned char num);

extern void PCF8563_init(void);
extern void PCF8536_wt(unsigned int add,unsigned char data);
extern void PCF8536_wt_p(unsigned int add,unsigned char *p,unsigned char num);
extern void PCF8536_rd(unsigned int add,unsigned char *p,unsigned char num);
extern void Updata_time(void);

extern unsigned char write_buff[8],read_buff[8],time_tmp[],data_tmp[],*week_tmp,time[7];

#endif