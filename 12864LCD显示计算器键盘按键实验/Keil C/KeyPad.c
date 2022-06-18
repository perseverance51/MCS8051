//键盘扫描程序
#include "KeyPad.H"

//扫描键盘时发送到 0~3 列上的扫描
unsigned char KeyboardScanCode[4]= {0xEF,0xDF,0xBF,0x7F};
//扫描键盘并返回按键在键盘上的位置序号
unsigned char KeyScan ()
{
    unsigned char Row=0,Col=0,Temp;
    EX0=0; //防抖关闭
    for(Col=0; Col<4; Col++) //扫描各列
    {
        P1=KeyboardScanCode[Col]; //输出扫描位
        Temp=P1 & 0x0F; //读取扫描位
        if(Temp!=0x0F)
        {
            switch(Temp) //判断第 i 列是否有按键下
            {
            case 0x0E:
                Row=0;
                break;//得到对应的行号
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
    EX0=1; //重新许可按键中断
    return Row*4+Col; //返回按键位置序号 0~15
}
