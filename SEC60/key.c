#include "key.h"

uchar Trg;
uchar Cont;
uchar cnt_plus;
//uchar count=1;

void KeyRead()
{
    uchar ReadData = KeyValue^0x01;   // 
    Trg = ReadData & (ReadData ^ Cont);   // ��������Ϊ1
    Cont = ReadData;                     // ��������Ϊ1
	
}


void KeyProc()
{
       if (Trg & KEY_MODE) // ������µ���KEY_MODE�������㳣���ⰴ��Ҳû���ã�
    {                    //���ǲ���ִ�еڶ��ε�Ŷ �� �������ɿ��ٰ���
         count++;         // ģʽ�Ĵ�����1����Ȼ������ֻ����ʾ�������ִ������
                         // ִ�е��κδ���
    }
    if (Cont & KEY_PLUS) // ������ӡ����������Ų���
    {
         cnt_plus++;       // ��ʱ
         if (cnt_plus > 100) // 20ms*100 = 2S ���ʱ�䵽
         {
              count +=10;      // ����Ҫ��ִ�еĳ���
         }          
    }
}
