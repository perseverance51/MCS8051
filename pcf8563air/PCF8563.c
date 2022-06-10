/************************************************
�ļ���PCF8563.h
��;��PCF8563����ͷ�ļ�
ע�⣺ϵͳʱ��8M
************************************************/
#include "config.h"
unsigned char syserr;
unsigned char *week_list[7]={
	 				 		    "  Sunday ",
			 					"  Monday ",
			 					" Tuesday ",
								"Wednesday",
			 					"Thursday ",
			 					" Friday  ",
			 					"Saturday "
};
unsigned char write_buff[8],read_buff[8],time_tmp[]="00:00:00",data_tmp[]="2008.01.01",*week_tmp;
unsigned char time[7]={
		 	  		   0x40/*��*/,
					   0x59/*��*/,
					   0x23/*ʱ*/,
					   0x29/*��*/,
					   0x05/*����*/,
					   0x02/*��/����*/,
					   0x08/*��*/
};
/*************************************************************************
** ��������: clear(unsigned char *p,unsigned char num)
** ��������: ���ָ������ָ�����ȵ�����
** �䡡��: unsigned char *p :��ʼ��ַ   unsigned char num :�����ݳ���
** ���	 : 
** ȫ�ֱ���: 
** ����ģ��: 
** ˵����
** ע�⣺
**************************************************************************/
void clear(unsigned char *p,unsigned char num)
{
 for(;num>0;num--)
 	{
	 *p=0;
	 p++;
	}
}
/*************************************************************************
** ��������: PCF8536_wt(unsigned int add,unsigned char data)
** ��������: ��PCF8563ָ����ַд��һ������
** �䡡��: unsigned int add    ���߰�λΪ������ַ���Ͱ�λΪ�ڲ��Ĵ�����ַ
   		   unsigned char data  ����Ҫд�������
** ���	 : 
** ȫ�ֱ���: 
** ����ģ��: 
** ˵����
** ע�⣺
**************************************************************************/
void PCF8536_wt(unsigned int add,unsigned char data)
{
 unsigned char t;
 t=add>>8;
 t<<=1;
 i2cstart();
 if(i2cwt(W_ADD_COM+t)==SLAW)
 	{
	 i2cwt(add);
	 i2cwt(data);
	}
 else syserr=ERR_SLAW;
 i2cstop();
}
/*************************************************************************
** ��������: PCF8536_wt_p(unsigned int add,unsigned char *p,unsigned char num)
** ��������: ��PCF8563��ַ�����ļĴ���д��ϵ������
** �䡡��: unsigned int add    ���߰�λΪ������ַ���Ͱ�λΪ�ڲ��Ĵ�����ַ
   		   unsigned char *p    ����Ҫд������������ʼ��ַ
		   unsigned char num   ��д�����ݵĸ���
** ���	 : 
** ȫ�ֱ���: 
** ����ģ��: 
** ˵����д����������Ϊ��ַ�����ļĴ���
** ע�⣺
**************************************************************************/
void PCF8536_wt_p(unsigned int add,unsigned char *p,unsigned char num)
{
 unsigned char t;
 t=add>>8;
 t<<=1;
 i2cstart();
 if(i2cwt(W_ADD_COM+t)==SLAW)
 	{
	 i2cwt(add);
	 for(;num>0;num--)
	 	{
		 i2cwt(*p);
		 p++;
		 _NOP();
		}
	}
 else syserr=ERR_SLAW;
 i2cstop();
}
/*************************************************************************
** ��������: PCF8536_rd(unsigned int add,unsigned char *p,unsigned char num)
** ��������: ��PCF8563
** �䡡��: unsigned int add    ���߰�λΪ������ַ���Ͱ�λΪ�ڲ��Ĵ�����ַ
   		   unsigned char *p    �����������ݴ�ŵ�ַ����ʼ��ַ
		   unsigned char num   ���������ݵĸ���
** ���	 : 
** ȫ�ֱ���: 
** ����ģ��: 
** ˵����
** ע�⣺
**************************************************************************/
void PCF8536_rd(unsigned int add,unsigned char *p,unsigned char num)
{
 unsigned char t;
 t=add>>8;
 t<<=1;
 i2cstart();
 if(i2cwt(W_ADD_COM+t)==SLAW)
 	{
	 i2cwt(add);
	}
 else syserr=ERR_SLAW;
 
 i2cstart();
 if(i2cwt(R_ADD_COM+t)==SLAW)
 	{
	 i2cwt(add);
	}
 else syserr=ERR_SLAW;
 
 for(;num>0;num--)
 	{
	 *p=i2crd();
	 p++;
	} 
}
/*************************************************************************
** ��������: PCF8563_init(void)
** ��������: PCF8563��ʼ��
** �䡡��: 
** ���	 : 
** ȫ�ֱ���: 
** ����ģ��: 
** ˵����
** ע�⣺
**************************************************************************/
void PCF8563_init(void)
{
 clear(write_buff,8);
 clear(read_buff,8);
 
 PCF8536_wt(0x00,0x20);//д�Ĵ���1��ֹͣ��ʱ
 //PCF8536_wt_p(0x02,time,7);//�趨ʱ��
 PCF8536_wt(0x00,0x00);//д�Ĵ���1����ʼ��ʱ
}
/*************************************************************************
** ��������: Updata_time(void)
** ��������: ˢ��ʱ��Ĵ����������������
** �䡡��: 
** ���	 : 
** ȫ�ֱ���: 
** ����ģ��: 
** ˵����
** ע�⣺
**************************************************************************/
/*void Updata_time(void)
{
 PCF8536_rd(0x02,time,7);//��ȡʱ��
 
 time[0]=time[0]&0x7F;//��0x02
 time[1]=time[1]&0x7F;//��0x03
 time[2]=time[2]&0x3F;//ʱ0x04
 time[3]=time[3]&0x3F;//��0x05
 time[4]=time[4]&0x07;//week 0x06
 time[5]=time[5]&0x1F;//��0x07
 //����ʱ��BCDת10����
 time_tmp[7]=(time[0]&0x0F)+0x30;
 time_tmp[6]=(time[0]>>4)+0x30;//��
 time_tmp[4]=(time[1]&0x0F)+0x30;
 time_tmp[3]=(time[1]>>4)+0x30;//��
 time_tmp[1]=(time[2]&0x0F)+0x30;
 time_tmp[0]=(time[2]>>4)+0x30;//ʱ
 //��������BCDת10����
 data_tmp[9]=(time[3]&0x0F)+0x30;
 data_tmp[8]=(time[3]>>4)+0x30;//��
 data_tmp[6]=(time[5]&0x0F)+0x30;
 data_tmp[5]=(time[5]>>4)+0x30;//��
 data_tmp[3]=(time[6]&0x0F)+0x30;
 data_tmp[2]=(time[6]>>4)+0x30;//��
 //��������
 week_tmp=week_list[time[4]&0x0F];
}*/