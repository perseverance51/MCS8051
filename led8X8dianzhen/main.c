
#include <STC89C5xRC.H>

sbit ds=P1^0;//������595��14����
sbit shcp=P1^1;//��������ʱ��ս595��11��
sbit stcp=P1^2;//����洢������ʱ����5958��12��
sbit mr=P1^3;//��������
unsigned char code shuju[8]={
//0x00,0x38,0x38,0x38,0xfe,0x7c,0x38,0x10
	0x0C,0x1E,0x3E,0x7C,0x7C,0x3E,0x1E,0x0C
};//LED����ߵ�ƽ�˿�����
unsigned char code dzwei[8]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};//LED����͵�ƽ�˿�λ
void QuDong595(unsigned char sj){
	unsigned char aa=8;
				mr=0;
				mr=1;
				while(aa--)
				{
			ds=sj>>7;
			sj<<=1;//ʮ���������ݷ���
			shcp=1;//�����ط�ȫ��λ��������ʱ���ݼĴ������档
			shcp=0;
			}
				stcp=0;
				stcp=1;//�����ؽ���������
				stcp=0;
			}
void main()
{
unsigned char Xd=0,ss=1,cs=0,d=0,y=0;
unsigned int ys=0;
	P14 = 0x00;
//ZhongDcionSheZhi();
	while(1)
	{
		P0=dzwei[7-cs];//����ͼ�η������ݳ���((cs+8-d) < 8 ? shuju[cs+8-d]:0x00)
		QuDong595((((7-cs)-d)<8?shuju[(7-cs)-d]:0x00));//������ʧcs+d���� (7-cs)-dǰ��
		//QuDong595(((cs+8-d) < 8?shuju[cs+8-d]:0x00));//������ʧcs+d����(7-cs)-dǰ��
		//P0=~((cs+d) <8 ?shuju[(7-cs)-d]:0x00);
		//QuDong595(~dzwei[7-cs])//90��ת��
		while(++Xd);
			mr=0;
			mr=1;
		QuDong595(0x00);
		if(++cs >7)	cs=0;
		if(++y==0){
		if(++d > 8*1) 
		d = 0;
		}
		
		}
	}
