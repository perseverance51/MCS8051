/*********************************************************************************
接线方式请看main.h说明
        二片74HC595 + 一个8*8点阵(1088AS共阴)
        实现文字静态显示+移动显示(左移/右移/上移/下移)
        代码精简,方便理解
**********************************************************************************/
//#include "main.h"
#include <STC89C5xRC.H>
#include <intrins.h>
typedef unsigned char uchar;
typedef unsigned char uint;
//定义变量
uchar temp = 0xfe;
sbit LEDARRAY_DI = P1^0;	//第二片74HC595的引脚14
sbit LEDARRAY_LAT = P1^1;	//第二片74HC595的引脚12
sbit LEDARRAY_CLK = P1^2;	//第二片74HC595的引脚11 

/*取模说明:
菜单选项:
"点阵格式" : 根据你的点阵是共阴还是共阳, 我的是共阴, 所以选择的是阴码
"取模走向" : 根据你的需求, 选择逆向或顺向, 我这里选择的是逆向
"取模方式" : 请看下面示例说明, 根据移动方向选择
"每行显示数据" : 点阵=8, 索引=8
"液晶面板仿真" : 像素大小=8
"输出数制" : 选择"十六进制数"
"输出选项" : 选择"输出精简格式"和"输出紧凑格式"
"自定义格式" : 选择"C51格式", 其它默认

主界面选项:
字体: 幼圆
字宽: 8                字高: 8
点阵宽: 8        点阵高: 8
*/

//动态显示数组
uchar Display_word1[48] = {                        //数组个数(这里的40代表有40个十六进制,因为是8*8点阵,所以每个字有8个十六进制)
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,        //空白(前面放一个空白屏,实现移动出字效果)
//0x10,0x38,0x54,0x10,0x10,0x10,0x10,0x10,        //箭头

//逆向+逐列式或列行式 (左移/右移)
0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,        //"一",0
0x80,0x80,0x80,0xFE,0x90,0x90,0x80,0x80,        //"上",1
0x00,0x40,0x44,0x44,0x44,0x44,0x40,0x00,        //"二",2
0x00,0x02,0x02,0xFE,0x0A,0x12,0x02,0x00,        //"下",3


//逆向+行列式或逐行式 (上移/下移)
/*0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,        //"一",0
0x00,0x08,0x08,0x08,0x38,0x08,0x08,0xFF,        //"上",1
0x00,0x00,0x3C,0x00,0x00,0x00,0x7E,0x00,        //"二",2
0x00,0x7E,0x08,0x18,0x28,0x08,0x08,0x08,        //"下",3
*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,        //空白(最后放一个空白屏,以免出现乱码)
};

//静态显示数组(第1个参数为几个字,第2个参数为每字多少字节)
uchar Display_word2[3][8] = {
0x0C,0x1E,0x3E,0x7C,0x7C,0x3E,0x1E,0x0C,        //心
0x10,0x38,0x54,0x10,0x10,0x10,0x10,0x10,        //箭头
0x0C,0x1E,0x3E,0x7C,0x7C,0x3E,0x1E,0x0C,        //心
};
void Delay_ms(uint n);
void Send_Data(uchar dat);
void Send_Byte(uchar dat);
//****************************************************
//主函数
//****************************************************
void main()
{
        uint i;
        uchar j,k;
//	P14 = 0x00;
        while(1)
        {               
                ///////////////////////////////////////////////////////////////////////////
                //静态显示
                for(j = 0 ; j < 3 ; j++) {                //共需显示3个字
                        for(i = 0 ; i < 1000 ; i++)        {       //静态显示时间
                                temp = 0x7f;
                                for(k = 0 ; k < 8 ; k++) {   //显示1-8列(8为显示8列)如果改成4则有4列显示,另4列黑屏             //显示1-8列(8为显示8列)如果改成4则有4列显示,另4列黑屏
                                        Send_Data(Display_word2[j][k]);
                                }
                        }
                }

                ///////////////////////////////////////////////////////////////////////////               
                //移动方式(想要不同的移动方式,就按以下说明修改)

                for(j = 0 ; j < 40 ; j++) //左移(逐列式)/上移(逐行式)
                //for(j = 39 ; j-1 > 0 ; j--)        //右移(逐列式)/下移(逐行式)

                for(j = 0 ; j < 40 ; j++) {     //共需显示3个字                           //要显示的字数(每个字8位,这里是显示一屏空白+4个字,所以是8+4*8=40)
                        for(i = 0 ; i < 50 ; i++) {  //静态显示时间                      //控制移动的速度（数字越小移动速度越快，越大越慢）
                                temp = 0x7f;

                                //移动方式(想要不同的移动方式,就按以下说明修改)
                                for(k = 0 ; k < 8 ; k++) //左移(逐列式)/右移(逐列式)
                                //for(k = 8 ; k-1 > 0 ; k--) //上移(逐行式)/下移(逐行式)

                                for(k = 0 ; k < 8 ; k++) {   //显示1-8列(8为显示8列)如果改成4则有4列显示,另4列黑屏             //(每个字取8次数组, 不要修改)
                                        Send_Data(Display_word1[j+k]);
                                }
                        }
                }
        }                                       
}
//****************************************************
//发送数据(不需要修改)
//****************************************************
void Send_Data(uchar dat)
{
	Send_Byte(temp);	//列选择
	Send_Byte(dat);		//数据
	LEDARRAY_LAT = 1;	//锁存数据
	_nop_();
	LEDARRAY_LAT = 0;
	_nop_();
	temp = (temp>>1)|0x80;
}
//****************************************************
//发送一个字节数据(不需要修改)
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
		LEDARRAY_CLK = 1;				//上升沿发送数据
		LEDARRAY_CLK = 0;
		dat >>= 1;			
	}		
}