#include <reg52.h>
extern unsigned char TX_buf[10];
void uart_init(void)
{
	TMOD=0x20;//���ö�ʱ��1Ϊ������ʽ2
	TH1=0xfd;//װ��ֵ
	TL1=0xfd;
	TR1=1; //������ʱ��
	TI=1;
	//REN=1; //�������
	SM0=0; //���ù�����ʽ 1
	SM1=1;
	EA=1;   //�������ж�
	ES=1;  
    //IP = 0x10;	 //���ô���Ϊ�����ȼ�
}
void uart_test(unsigned char length)
{

		unsigned char i;
	    for(i=0;i<length;i++)
		{
				ES=0;
		        SBUF = TX_buf[i];//�����ܵ����ݷ���ȥ
				while(!TI);	
				TI=0;				
		}	
		ES=1;	  
}			  
