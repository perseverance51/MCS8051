#include "hardware.h"
#include "software.h"
//��������ܶ���
uchar code block[] = {  //�����޷����ַ��������������������
		0x3F,  //"0"
    0x06,  //"1"
    0x5B,  //"2"
    0x4F,  //"3"
    0x66,  //"4"
    0x6D,  //"5"
    0x7D,  //"6"
    0x07,  //"7"
    0x7F,  //"8"
    0x6F  //"9"
};
void SEGDisplay()    //�����ɨ�躯��
{
  uchar i;  //����i����
	for(i=0;i<2;i++)
	{
		switch(i)	
		{														 
		   case(0):			 //λѡ����һ�������	
				LSA=0;
				LSB=1;
				LSC=1;
		        P0 = block[num2/10];  //ʮλ�����
				 break;
			case(1):          //λѡ����һ�������
				LSA=1;
				LSB=1;
				LSC=1;
			    P0 = block[num2%10];  //��λ�����
				 break;
		}
		delayms(5);   //��ʱһ�ᣨ�Ӿ�������
		P0=0x00;      //��Ӱ
	}
}
