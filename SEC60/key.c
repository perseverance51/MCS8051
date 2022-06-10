#include "key.h"

uchar Trg;
uchar Cont;
uchar cnt_plus;
//uchar count=1;

void KeyRead()
{
    uchar ReadData = KeyValue^0x01;   // 
    Trg = ReadData & (ReadData ^ Cont);   // 按键触发为1
    Cont = ReadData;                     // 长按触发为1
	
}


void KeyProc()
{
       if (Trg & KEY_MODE) // 如果按下的是KEY_MODE，而且你常按这按键也没有用，
    {                    //它是不会执行第二次的哦 ， 必须先松开再按下
         count++;         // 模式寄存器加1，当然，这里只是演示，你可以执行你想
                         // 执行的任何代码
    }
    if (Cont & KEY_PLUS) // 如果“加”按键被按着不放
    {
         cnt_plus++;       // 计时
         if (cnt_plus > 100) // 20ms*100 = 2S 如果时间到
         {
              count +=10;      // 你需要的执行的程序
         }          
    }
}
