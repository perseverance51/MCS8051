
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* --- QQ:  800003751 ----------------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/


#include	"config.h"
#include	"GPIO.h"
#include	"delay.h"

/*************	功能说明	**************

本程序演示跑马灯。
程序使用P2口来演示，输出低驱动。用户可以修改为别的口。

******************************************/

/*************	本地常量声明	**************/
#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long
#define s8 signed char
#define s16 signed int
#define s32 signed long
#define KEY_TIME 25                        //按键消抖时间 (毫秒)
#define reset()        IAP_CONTR |= 0x20                //芯片复位
#define feed_dog()  WDT_CONTR |= 0x10   //看门狗喂狗
u8 code nbr[16] = {                //共阴数码管段码16进制
    0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71
};
u8 show[4];                //4位数码管当前显示的段码 更改数码管显示的段码 只需更改此数组
u8 wink;                //闪烁的数码管段码        0x01: 第一位闪烁 0x02:第二位闪烁 0x04: 第三位闪烁 0x0F:全闪烁 0x00:全不闪烁

sbit P3_0 = P3 ^ 0;                //按键S1 (I/O口定义的按键未经消抖,请勿在中断外直接使用)
sbit P3_1 = P3 ^ 1;                //按键S2
sbit P3_2 = P3 ^ 2;     //有源蜂鸣器 低电平鸣叫
sbit P3_3 = P3 ^ 3;     //继电器或MOS管 低电平吸合(导通)
u16 S1_ms, S2_ms;                //按键0、按键1连续按下的时间 定时器中断计数此变量 最长65535毫秒 松开清零

u16 delayms;  //定时器中断延时 (毫秒)
u16 time;      //定时时间 (秒)
u16 beep_ms;   //蜂鸣器鸣叫时间 (毫秒)

u16 bdata Bit16 = 0;    //定义16位可位寻址变量
sbit delay = Bit16 ^ 0;      //定时器中断延时毫秒开关 中断外置1 延时完中断内清0
sbit test = Bit16 ^ 1;       //此值为1数码管全亮 蜂鸣器响 所有按键无效 否则正常
sbit S1 = Bit16 ^ 2;         //按键0 连续按下N毫秒清0, 松开置1(定时器0中断消抖)
sbit S2 = Bit16 ^ 3;         //按键1 (处理方式与按键0相同)
sbit hz10 = Bit16 ^ 4;      //10Hz信号源 (定时器0中断产生,用于控制数码管闪烁)
sbit bak = Bit16 ^ 5;       //按键长按识别处理用
sbit Beep = Bit16 ^ 6;      //有源蜂鸣器 低电平响
sbit start = Bit16 ^ 7;     //倒计时开关 置1倒计时开始 清0则结束
sbit pause = Bit16 ^ 8;     //1暂停倒计时 0正常
sbit relay = Bit16 ^ 9;     //继电器或MOS管 低电平吸合(导通) 1秒只能吸合或释放1次

/*************	本地变量声明	**************/


/*************	本地函数声明	**************/



/*************  外部函数和变量声明 *****************/
void InitTimer0() {        //初始化定时器0中断
    TMOD = 0x01;
    TH0 = 0xFC;
    TL0 = 0x18;
    EA = 1;
    ET0 = 1;
    TR0 = 1;
}

void beep(u16 ms) {     //蜂鸣器鸣叫 参数:鸣叫毫秒
    beep_ms = ms;
    Beep = 0;
}

u16 set_time() {        //设定定时时间
    u8 n = 0;       //0:编辑千位 1:百位 2:十位 3:个位
    u8 o[4];
    o[0] = 0; o[1] = 0; o[2] = 0; o[3] = 0;
    while (1) {
        feed_dog();
        wink = 8 >> n;
        show[0] = nbr[o[0]];
        show[1] = nbr[o[1]];
        show[2] = nbr[o[2]];
        show[3] = nbr[o[3]];
        if (S2 == 0) {                        //S2按下 短按加1
            beep(50);
            if (o[n]++ >= 9) o[n] = 0;
            show[n] = nbr[o[n]];
            delayms = 500; delay = 1;
            while (delay && S2 == 0);        //长按延时等待
            while (S2_ms >= 500) {                //按键长按,开始连加
                feed_dog();
                delayms = 125; delay = 1;                //0.125秒连加一次
                while (delay && S2 == 0);
                beep(50);
                if (o[n]++ >= 9) o[n] = 0;
                show[n] = nbr[o[n]];
            }
            delay = 0;
            delayms = 0;
        }

        if (S1 == 0) {                //S1按下
            beep(50);
            n++;                //输入下一位
            while (S1 == 0) {
                feed_dog();
                if (S1_ms >= 1000 && !bak) {                //长按退回编辑
                    beep(50);
                    n = 0;
                    wink = 8 >> n;
                    show[0] = nbr[o[0]];
                    show[1] = nbr[o[1]];
                    show[2] = nbr[o[2]];
                    show[3] = nbr[o[3]];
                    bak = 1;
                }
            }

            if (bak) {
                bak = 0;
                continue;
            }

            if (n >= 4) {                //个位输入完成
                wink = 0;
                return o[0] * 1000 + o[1] * 100 + o[2] * 10 + o[3];

            }
        }
    }
//    return 0;
}


/******************** IO配置函数 **************************/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_All;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);	//初始化
}



/******************** 主函数 **************************/
void main(void)
{
	 u8 i = 0;
    Beep = 1;
    WDT_CONTR = 0x27;           //初始化看门狗
    P1M0 = 0x0F; P1M1 = 0x00;   //P1.0~P1.3强推挽
    P2M0 = 0xFF; P2M1 = 0x00;   //P2.0~P2.7强推挽
    //上电1秒数码管全亮 蜂鸣器响
    test = 1;
		GPIO_config();
    InitTimer0();
    delayms = 1000; delay = 1;
    while (delay);
    test = 0;

    show[0] = nbr[0];
    show[1] = nbr[0];
    show[2] = nbr[0];
    show[3] = nbr[0];
	while(1)
	{
relay = 1;
        pause = 0;
        do {        //时间设定
            time = set_time();
        } while (!time);
        time -= 1;
        start = 1;
        relay = 0;
        while (start) {
            feed_dog();
            if (S1 == 0) {      //S1按下蜂鸣器响1下
                beep(50);
                while (S1 == 0) {
                    feed_dog();
                }
            }

            if (S2 == 0) {      //S2按下暂停倒计时
                beep(50);
                pause = !pause;
                relay = pause;
                while (S2 == 0) {
                    feed_dog();
                    if (pause && S2_ms >= 3000) {       //长按3秒复位
                        _nop_(); _nop_(); _nop_(); _nop_();
                        reset();
                        _nop_(); _nop_(); _nop_(); _nop_();
                    }
                }
            }

        }
        //倒计时结束
        show[0] = 0x79;     //E
        show[1] = 0x54;     //n
        show[2] = 0x5E;     //d
        show[3] = 0x00;
        for (i = 0; i < 6; i++) {       //蜂鸣器响6下
            delayms = 500; delay = 1; while (delay);
            beep(500);
            while (!Beep);
            feed_dog();
        }

        while (1) {
            feed_dog();
            if (S1 == 0 || S2 == 0) {
                beep(50);
                while (S1 == 0 || S2 == 0) {
                    feed_dog();
                }
                break;
            }
        }
	}
}

void Timer0Interrupt() interrupt 1{         //12Mhz 1mS定时器中断
    static u8 seg = 0;  //当前正在扫描的数码管段码 4位数码管 范围:0~3
    static u8 ms1 = 0;
    static u16 ms = 0, ms2 = 0;
    TH0 = 0xFC;
    TL0 = 0x18;
    if (delay && !--delayms) {    //延时任意毫秒
        delay = 0;
    }
    //数码管动态扫描处理
    P1 |= 0xF;          //消影
    P2 = test ? 0xFF : (wink & (1 << (3 ^ seg)) && !hz10 ? 0 : show[seg]);      //段码
    P1 &= ~(1 << seg);   //位码
    if (++seg >= 4) seg = 0;

    if (test) {     //测试模式
        S1 = 1; S2 = 1;
        P3_2 = 0;
        return;
    }

    //蜂鸣器处理
    P3_2 = Beep;
    if (!Beep && !--beep_ms) {
        Beep = 1;
    }
    if (++ms1 >= 50) {
        ms1 = 0;
        hz10 = !hz10;
    }

    //按键处理
    if (P3_0 == 0) {
        if (S1_ms != 0xFFFF) S1_ms++;
    }
    else {
        S1_ms = 0;
    }

    if (P3_1 == 0) {
        if (S2_ms != 0xFFFF) S2_ms++;
    }
    else {
        S2_ms = 0;
    }

    S1 = !(S1_ms >= KEY_TIME);
    S2 = !(S2_ms >= KEY_TIME);
    //继电器处理
    if (++ms2 >= 1000) {    //继电器1秒只能进行1次吸合或释放，防止频繁开关导致设备损坏
        ms2 = 0;
        P3_3 = relay;
    }
    //时间倒计时处理
    if (start) {
        if (pause) {
            wink = 0xF;
        }
        else {
            wink = 0;
            if (++ms >= 1000) {
                ms = 0;
                show[0] = nbr[time / 1000 % 10];
                show[1] = nbr[time / 100 % 10];
                show[2] = nbr[time / 10 % 10];
                show[3] = nbr[time % 10];

                if (!time--) {      //倒计时结束
                    start = 0;
                    relay = 1;
                }
            }
        }

    }


}


