#include "reg51.h"
#include "intrins.h"
sfr CMPCR1 = 0xE6; //比较器控制寄存器1
#define CMPEN 0x80 //CMPCR1.7 : 比较器模块使能位
#define CMPIF 0x40 //CMPCR1.6 : 比较器中断标志位
#define PIE 0x20 //CMPCR1.5 : 比较器上升沿中断使能位
#define NIE 0x10 //CMPCR1.4 : 比较器下降沿中断使能位
#define PIS 0x08 //CMPCR1.3 : 比较器正极选择位
#define NIS 0x04 //CMPCR1.2 : 比较器负极选择位
#define CMPOE 0x02 //CMPCR1.1 : 比较结果输出控制位
#define CMPRES 0x01 //CMPCR1.0 : 比较器比较结果标志位
sfr CMPCR2 = 0xE7; //比较器控制寄存器2
#define INVCMPO 0x80 //CMPCR2.7 : 比较结果反?输出控制位
#define DISFLT 0x40 //CMPCR2.6 : 比较器输出端虑波使能控制位
#define LCDTY 0x3F //CMPCR2.[5:0] : 比较器输出的区抖时间控制
sbit LED = P1^0; //测试脚

void cmp_isr() interrupt 21 using 1 //比较器中断向量入口
{
    CMPCR1 &= ~CMPIF; //清除完成标志
    LED = (CMPCR1 & CMPRES); //将比较器结果CMPRES输出到测试口显示
}
void main()
{
    CMPCR1 = 0; //初始化比较器控制寄存器1
    CMPCR2 = 0; //初始化比较器控制寄存器2
    CMPCR1 &= ~PIS; //选择外部管脚P5.5（CMP+）为比较器的正极输入源
// CMPCR1 |= PIS; //选择ADCIS[2:0]所选的ADCIN为比较器的正极输入源
    CMPCR1 &= ~NIS; //选择内部BandGap电压BGV为比较器的负极输入源
// CMPCR1 |= NIS; //选择外部管脚P5.4（CMP-）为比较器的负极输入源
    CMPCR1 &= ~CMPOE; //禁用比较器的比较结果输出
// CMPCR1 |= CMPOE; //使能比较器的比较结果输出到P1.2
    CMPCR2 &= ~INVCMPO; //比较器的比较结果正常输出到P1.2
// CMPCR2 |= INVCMPO; //比较器的比较结果取反后输出到P1.2
    CMPCR2 &= ~DISFLT; //不禁用(使能)比较器输出端的0.1uS虑波电路
// CMPCR2 |= DISFLT; //禁用比较器输出端的0.1uS虑波电路
    CMPCR2 &= ~LCDTY; //比较器结果不去抖动,直接输出
// CMPCR2 |= (DISFLT & 0x10); //比较器结果在经过16个时钟后再输出
    CMPCR1 |= PIE;
   //使能比较器的上升沿中断
// CMPCR1 |= NIE; //使能比较器的下降沿中断

    CMPCR1 |= CMPEN; //使能比较器

    EA = 1;

    while (1);
}

