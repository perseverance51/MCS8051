#include <reg52.h>

void main()
{
	unsigned char i = 0;
	unsigned int temp;
	P1=0xff;  //P1�˿���һ�ر�LED
	P2=0xff;  //����P2�˿ڳ�ʼֵ
	while(1)  //ѭ��
	{
		if(P1 != 0xff) //����ް�����Ӧ��������
		{
			temp=(P2^P1)^0xff;	//����һ״���뱾��״���ϲ�
			P2 = temp;		    //����P2��
			while(P1 != 0xff);//�ȴ������ͷ�
		}
		
	}
}

