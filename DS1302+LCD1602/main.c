#include <reg52.h>
#include "Lcd1602.h"
#include "DS1302.h"

#define 	MAIN_Fosc			11059200uL	//定义主时钟
bit flag200ms = 0; //200ms 定时标志
unsigned char T0RH = 0; //T0 重载值的高字节
unsigned char T0RL = 0; //T0 重载值的低字节

/*范围0 -50ms*/
void delayms(unsigned int ms){
    unsigned long tmp; //临时变量
    tmp = MAIN_Fosc / 12; //定时器计数频率
    tmp = (tmp * ms) / 1000; //计算所需的计数值
    tmp = 65536 - tmp; //计算定时器重载值
    tmp = tmp + 12; //补偿中断响应延时造成的误差
    T0RH = (unsigned char)(tmp>>8); //定时器重载值拆分为高低字节
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0; //清零 T0 的控制位
    TMOD |= 0x01; //配置 T0 为模式 1
    TH0 = T0RH; //加载 T0 重载值
    TL0 = T0RL;
    ET0 = 1; //使能 T0 中断
    TR0 = 1; //启动 T0
}
/* T0 中断服务函数，执行 200ms 定时 */
void InterruptTimer0() interrupt 1{
    static unsigned char tmr200ms = 0;
    TH0 = T0RH; //重新加载重载值
    TL0 = T0RL;
    tmr200ms++;
    if (tmr200ms >= 200){ //定时 200ms
        tmr200ms = 0;
        flag200ms = 1;
    }
}

void main(){
    unsigned char psec=0xAA; //秒备份，初值 AA 确保首次读取时间后会刷新显示
    unsigned char time[8]; //当前时间数组
    unsigned char str[10]; //字符串转换缓冲区
		unsigned char *weeklist[7]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
		unsigned char week;
    EA = 1; //开总中断
    delayms(1); //T0 定时 1ms
    InitDS1302(); //初始化实时时钟
    InitLcd1602(); //初始化液晶

    while (1){
        if (flag200ms){ //每 200ms 读取依次时间
            flag200ms = 0;
            DS1302BurstRead(time); //读取 DS1302 当前时间
            if (psec != time[0]){ //检测到时间有变化时刷新显示
                str[0] = '2'; //添加年份的高 2 位：20
                str[1] = '0';
                str[2] = (time[6] >> 4) + '0'; //“年”高位数字转换为 ASCII 码
                str[3] = (time[6]&0x0F) + '0'; //“年”低位数字转换为 ASCII 码
                str[4] = '-'; //添加日期分隔符
                str[5] = (time[4] >> 4) + '0'; //“月”
                str[6] = (time[4]&0x0F) + '0';
                str[7] = '-';
                str[8] = (time[3] >> 4) + '0'; //“日”
                str[9] = (time[3]&0x0F) + '0';
                str[10] = '\0';
                LcdShowStr(0, 0, str); //显示到液晶的第一行
                week = (time[5]&0x0F) ; //“星期”
                LcdShowStr(13, 0, weeklist[week - 1]); //显示到液晶的第一行
					/******  LCD1602第二行显示：时分秒显示   ******/
                str[0] = (time[2] >> 4) + '0'; //“时”
                str[1] = (time[2]&0x0F) + '0';
                str[2] = ':'; //添加时间分隔符
                str[3] = (time[1] >> 4) + '0'; //“分”
                str[4] = (time[1]&0x0F) + '0';
                str[5] = ':';
                str[6] = (time[0] >> 4) + '0'; //“秒”
                str[7] = (time[0]&0x0F) + '0';
                str[8] = '\0';
                LcdShowStr(4, 1, str); //显示到液晶的第二行
                psec = time[0]; //用当前值更新上次秒数
            }
        }
    }
}


