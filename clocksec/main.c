/* Main.c file generated by New Project wizard
 *0-999999秒表
 * Created:   周四 6月 14 2022
 * Processor: AT89C52
 * Compiler:  Keil for 8051
 */

#include <reg52.h>
#define FOSC 12000000uL
//#define T50MS 15536  //50ms timer calculation method in 12T mode
#define T50MS (65536-FOSC*50/12/1000)   //1ms*50 timer calculation method in 12T mode

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit led = P1^5; 
unsigned char code LedChar[] = {  //数码管显示字符转换表
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};
unsigned char LedBuff[6] = { //数码管显示缓冲区，初值 0xFF 确保启动时都不亮
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
    unsigned int cnt = 0; //记录 T0 中断次数
    unsigned long sec = 0; //记录经过的秒数
void tm0_isr() interrupt 1
{
    TL0 = T50MS;                     //reload timer0 low byte
    TH0 = T50MS >> 8;                //reload timer0 high byte
    if (cnt-- == 0)               //1ms * 1000 -> 1s
    {
        cnt = 20;               //reset counter
		sec++;  //秒计数自加 1
        led = ! led;      //work LED flash          
                //以下代码将 sec 按十进制位从低到高依次提取并转为数码管显示字符
                LedBuff[0] = LedChar[sec/100000];
                LedBuff[1] = LedChar[sec%100000/10000];
                LedBuff[2] = LedChar[sec%100000%10000/1000];
                LedBuff[3] = LedChar[sec%100000%10000%1000/100];
                LedBuff[4] = LedChar[sec%100000%10000%1000%100/10];
                LedBuff[5] = LedChar[sec%100000%10000%1000%100%10];
    }
}
//延时函数
void delay(int xms)
{
	unsigned int i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}
void main(){
    unsigned char i = 0;  //动态扫描的索引      
    ENLED = 0;  //使能 U3，选择控制数码管
    ADDR3 = 1; //因为需要动态改变 ADDR0-2 的值，所以不需要再初始化了
   
    TMOD = 0x01;  //设置 T0 为模式 1
    TL0 = T50MS;                     //initial timer0 low byte
    TH0 = T50MS >> 8; 
	TR0 = 1;                        //timer0 start running
    ET0 = 1;                        //enable timer0 interrupt
    EA = 1;                         //open global interrupt switch

    while (1){
            //以下代码完成数码管动态扫描刷新
            switch (i){
                case 0: ENLED=1;ADDR2=0; ADDR1=0; ADDR0=0; i++; P0=LedBuff[0]; delay(5);ENLED=0;break;
                case 1: ENLED=1;ADDR2=0; ADDR1=0; ADDR0=1; i++; P0=LedBuff[1]; delay(5);ENLED=0;break;
                case 2: ENLED=1;ADDR2=0; ADDR1=1; ADDR0=0; i++; P0=LedBuff[2]; delay(5);ENLED=0;break;
                case 3: ENLED=1;ADDR2=0; ADDR1=1; ADDR0=1; i++; P0=LedBuff[3]; delay(5);ENLED=0;break;
                case 4: ENLED=1;ADDR2=1; ADDR1=0; ADDR0=0; i++; P0=LedBuff[4]; delay(5);ENLED=0;break;
                case 5: ENLED=1;ADDR2=1; ADDR1=0; ADDR0=1; i=0; P0=LedBuff[5]; delay(5);ENLED=0;break;
                default: break;
            } 
			P0=0xFF;//消隐
} 
}