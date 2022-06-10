#include <reg52.h>

void main()
{
	unsigned char i = 0;
	unsigned int temp;
	P1=0xff;  //P1端口制一关闭LED
	P2=0xff;  //设置P2端口初始值
	while(1)  //循环
	{
		if(P1 != 0xff) //如果无按键反应，则跳出
		{
			temp=(P2^P1)^0xff;	//将上一状况与本次状况合并
			P2 = temp;		    //送至P2口
			while(P1 != 0xff);//等待按键释放
		}
		
	}
}

