/*********************************************************************************
���߷�ʽ�뿴main.h˵��
        ��Ƭ74HC595 + һ��8*8����(1088AS����)
        ʵ�����־�̬��ʾ+�ƶ���ʾ(����/����/����/����)
        ���뾫��,��������
**********************************************************************************/
//#include "main.h"
#include <STC89C5xRC.H>
#include <intrins.h>
typedef unsigned char uchar;
typedef unsigned char uint;
//�������
uchar temp = 0xfe;
sbit LEDARRAY_DI = P1^0;	//�ڶ�Ƭ74HC595������14
sbit LEDARRAY_LAT = P1^1;	//�ڶ�Ƭ74HC595������12
sbit LEDARRAY_CLK = P1^2;	//�ڶ�Ƭ74HC595������11 

/*ȡģ˵��:
�˵�ѡ��:
"�����ʽ" : ������ĵ����ǹ������ǹ���, �ҵ��ǹ���, ����ѡ���������
"ȡģ����" : �����������, ѡ�������˳��, ������ѡ���������
"ȡģ��ʽ" : �뿴����ʾ��˵��, �����ƶ�����ѡ��
"ÿ����ʾ����" : ����=8, ����=8
"Һ��������" : ���ش�С=8
"�������" : ѡ��"ʮ��������"
"���ѡ��" : ѡ��"��������ʽ"��"������ո�ʽ"
"�Զ����ʽ" : ѡ��"C51��ʽ", ����Ĭ��

������ѡ��:
����: ��Բ
�ֿ�: 8                �ָ�: 8
�����: 8        �����: 8
*/

//��̬��ʾ����
uchar Display_word1[48] = {                        //�������(�����40������40��ʮ������,��Ϊ��8*8����,����ÿ������8��ʮ������)
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,        //�հ�(ǰ���һ���հ���,ʵ���ƶ�����Ч��)
//0x10,0x38,0x54,0x10,0x10,0x10,0x10,0x10,        //��ͷ

//����+����ʽ������ʽ (����/����)
0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,        //"һ",0
0x80,0x80,0x80,0xFE,0x90,0x90,0x80,0x80,        //"��",1
0x00,0x40,0x44,0x44,0x44,0x44,0x40,0x00,        //"��",2
0x00,0x02,0x02,0xFE,0x0A,0x12,0x02,0x00,        //"��",3


//����+����ʽ������ʽ (����/����)
/*0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,        //"һ",0
0x00,0x08,0x08,0x08,0x38,0x08,0x08,0xFF,        //"��",1
0x00,0x00,0x3C,0x00,0x00,0x00,0x7E,0x00,        //"��",2
0x00,0x7E,0x08,0x18,0x28,0x08,0x08,0x08,        //"��",3
*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,        //�հ�(����һ���հ���,�����������)
};

//��̬��ʾ����(��1������Ϊ������,��2������Ϊÿ�ֶ����ֽ�)
uchar Display_word2[3][8] = {
0x0C,0x1E,0x3E,0x7C,0x7C,0x3E,0x1E,0x0C,        //��
0x10,0x38,0x54,0x10,0x10,0x10,0x10,0x10,        //��ͷ
0x0C,0x1E,0x3E,0x7C,0x7C,0x3E,0x1E,0x0C,        //��
};
void Delay_ms(uint n);
void Send_Data(uchar dat);
void Send_Byte(uchar dat);
//****************************************************
//������
//****************************************************
void main()
{
        uint i;
        uchar j,k;
//	P14 = 0x00;
        while(1)
        {               
                ///////////////////////////////////////////////////////////////////////////
                //��̬��ʾ
                for(j = 0 ; j < 3 ; j++) {                //������ʾ3����
                        for(i = 0 ; i < 1000 ; i++)        {       //��̬��ʾʱ��
                                temp = 0x7f;
                                for(k = 0 ; k < 8 ; k++) {   //��ʾ1-8��(8Ϊ��ʾ8��)����ĳ�4����4����ʾ,��4�к���             //��ʾ1-8��(8Ϊ��ʾ8��)����ĳ�4����4����ʾ,��4�к���
                                        Send_Data(Display_word2[j][k]);
                                }
                        }
                }

                ///////////////////////////////////////////////////////////////////////////               
                //�ƶ���ʽ(��Ҫ��ͬ���ƶ���ʽ,�Ͱ�����˵���޸�)

                for(j = 0 ; j < 40 ; j++) //����(����ʽ)/����(����ʽ)
                //for(j = 39 ; j-1 > 0 ; j--)        //����(����ʽ)/����(����ʽ)

                for(j = 0 ; j < 40 ; j++) {     //������ʾ3����                           //Ҫ��ʾ������(ÿ����8λ,��������ʾһ���հ�+4����,������8+4*8=40)
                        for(i = 0 ; i < 50 ; i++) {  //��̬��ʾʱ��                      //�����ƶ����ٶȣ�����ԽС�ƶ��ٶ�Խ�죬Խ��Խ����
                                temp = 0x7f;

                                //�ƶ���ʽ(��Ҫ��ͬ���ƶ���ʽ,�Ͱ�����˵���޸�)
                                for(k = 0 ; k < 8 ; k++) //����(����ʽ)/����(����ʽ)
                                //for(k = 8 ; k-1 > 0 ; k--) //����(����ʽ)/����(����ʽ)

                                for(k = 0 ; k < 8 ; k++) {   //��ʾ1-8��(8Ϊ��ʾ8��)����ĳ�4����4����ʾ,��4�к���             //(ÿ����ȡ8������, ��Ҫ�޸�)
                                        Send_Data(Display_word1[j+k]);
                                }
                        }
                }
        }                                       
}
//****************************************************
//��������(����Ҫ�޸�)
//****************************************************
void Send_Data(uchar dat)
{
	Send_Byte(temp);	//��ѡ��
	Send_Byte(dat);		//����
	LEDARRAY_LAT = 1;	//��������
	_nop_();
	LEDARRAY_LAT = 0;
	_nop_();
	temp = (temp>>1)|0x80;
}
//****************************************************
//����һ���ֽ�����(����Ҫ�޸�)
//****************************************************
void Send_Byte(uchar dat)
{
	uchar i;
	LEDARRAY_CLK = 0;
	_nop_();	
	LEDARRAY_LAT = 0;
	_nop_();

	for( i = 0 ; i < 8 ; i++ )
	{
		if( dat&0x01 ) {
			LEDARRAY_DI = 1;	
		} else {
			LEDARRAY_DI = 0;
		}		
		LEDARRAY_CLK = 1;				//�����ط�������
		LEDARRAY_CLK = 0;
		dat >>= 1;			
	}		
}