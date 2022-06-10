#include <REGX52.H>
#include <intrins.h>
sbit KeyValue=P3^7;
unsigned char code segment[]= {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
//���干���������ʾ0~9
unsigned char Trg;
unsigned char Cont;
static char count=1;
void KeyRead( void )
{
    unsigned char ReadData = KeyValue^0x01;   // 
    Trg = ReadData & (ReadData ^ Cont);   // ��������Ϊ1
    Cont = ReadData;                     // ��������Ϊ1
	
}

void main() {
		P3=0XFF;
    P0=segment[0]; //��ʼ������ʾ0
    while(1) {
			KeyRead();
        if(Trg==1) {
            P0=segment[count];
            count++;
            if(count>=10) {  //����0~9���������ʾ�ص�0
                count=0;
            }
        }
    }
}
