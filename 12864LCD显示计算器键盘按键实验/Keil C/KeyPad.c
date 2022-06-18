//����ɨ�����
#include "KeyPad.H"

//ɨ�����ʱ���͵� 0~3 ���ϵ�ɨ��
unsigned char KeyboardScanCode[4]= {0xEF,0xDF,0xBF,0x7F};
//ɨ����̲����ذ����ڼ����ϵ�λ�����
unsigned char KeyScan ()
{
    unsigned char Row=0,Col=0,Temp;
    EX0=0; //�����ر�
    for(Col=0; Col<4; Col++) //ɨ�����
    {
        P1=KeyboardScanCode[Col]; //���ɨ��λ
        Temp=P1 & 0x0F; //��ȡɨ��λ
        if(Temp!=0x0F)
        {
            switch(Temp) //�жϵ� i ���Ƿ��а�����
            {
            case 0x0E:
                Row=0;
                break;//�õ���Ӧ���к�
            case 0x0D:
                Row=1;
                break;
            case 0x0B:
                Row=2;
                break;
            case 0x07:
                Row=3;
                break;
            }
            break;
        }
    }
    P1=0x0F;
    EX0=1; //������ɰ����ж�
    return Row*4+Col; //���ذ���λ����� 0~15
}
