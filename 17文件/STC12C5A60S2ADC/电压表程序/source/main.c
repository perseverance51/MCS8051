/*------------------------------
*
使用STC12C5A60S2单片机内部ADC（10位：1024）制作电压表
方式：中断查询方式
2018年11月20日
开发者：画中仙
注意：
所有P1口都可以作为ADC的输入口，本实验使用P1.0作为ADC输入

1.5V
*/
#include <STC12C5A60S2.H>


unsigned int 	ADC_VALUE,Voltage;   



/*--------------------------------
*ADC初始化函数
*
具体通道只要修改  P1ASF=0XFF; 
*/
void ADC_init()
{
    P1ASF=0x01;        //设置输入通道
	ADC_RES=0;         //清除当前值
	ADC_CONTR=0x88;    //打开ADC电源并清除当前的显示
    EADC=1;    			//使能ADC
}

void main()
{
UART_init();
	ADC_init();
	EA=1;
	while(1)
	{


	}
}


 /**********************************
*
*函数名称：
*函数功能：
*函数参数：无
*函数返回：无
*备注： Voltage是测量转换的到的实际电压，在主循环中扫描即可
*
***********************************/
void ADC_interrupt() interrupt 5
{
	 ADC_CONTR &= !0x10;                 //ADC完成标志位
	 ADC_VALUE=(ADC_RES*4+ADC_RESL);    //获得ADC的采样值
	
	 Voltage=(unsigned int)((long)ADC_VALUE*5000/1024);  //转换为0~5V的电压
	
     ADC_CONTR=0X88;     //打开ADC电源并清除当前的显示
}