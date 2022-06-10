
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

/*************	����˵��	**************

��������ʾ����ơ�
����ʹ��P2������ʾ��������������û������޸�Ϊ��Ŀڡ�

******************************************/

/*************	���س�������	**************/
#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long
#define s8 signed char
#define s16 signed int
#define s32 signed long
#define KEY_TIME 25                        //��������ʱ�� (����)
#define reset()        IAP_CONTR |= 0x20                //оƬ��λ
#define feed_dog()  WDT_CONTR |= 0x10   //���Ź�ι��
u8 code nbr[16] = {                //��������ܶ���16����
    0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71
};
u8 show[4];                //4λ����ܵ�ǰ��ʾ�Ķ��� �����������ʾ�Ķ��� ֻ����Ĵ�����
u8 wink;                //��˸������ܶ���        0x01: ��һλ��˸ 0x02:�ڶ�λ��˸ 0x04: ����λ��˸ 0x0F:ȫ��˸ 0x00:ȫ����˸

sbit P3_0 = P3 ^ 0;                //����S1 (I/O�ڶ���İ���δ������,�������ж���ֱ��ʹ��)
sbit P3_1 = P3 ^ 1;                //����S2
sbit P3_2 = P3 ^ 2;     //��Դ������ �͵�ƽ����
sbit P3_3 = P3 ^ 3;     //�̵�����MOS�� �͵�ƽ����(��ͨ)
u16 S1_ms, S2_ms;                //����0������1�������µ�ʱ�� ��ʱ���жϼ����˱��� �65535���� �ɿ�����

u16 delayms;  //��ʱ���ж���ʱ (����)
u16 time;      //��ʱʱ�� (��)
u16 beep_ms;   //����������ʱ�� (����)

u16 bdata Bit16 = 0;    //����16λ��λѰַ����
sbit delay = Bit16 ^ 0;      //��ʱ���ж���ʱ���뿪�� �ж�����1 ��ʱ���ж�����0
sbit test = Bit16 ^ 1;       //��ֵΪ1�����ȫ�� �������� ���а�����Ч ��������
sbit S1 = Bit16 ^ 2;         //����0 ��������N������0, �ɿ���1(��ʱ��0�ж�����)
sbit S2 = Bit16 ^ 3;         //����1 (����ʽ�밴��0��ͬ)
sbit hz10 = Bit16 ^ 4;      //10Hz�ź�Դ (��ʱ��0�жϲ���,���ڿ����������˸)
sbit bak = Bit16 ^ 5;       //��������ʶ������
sbit Beep = Bit16 ^ 6;      //��Դ������ �͵�ƽ��
sbit start = Bit16 ^ 7;     //����ʱ���� ��1����ʱ��ʼ ��0�����
sbit pause = Bit16 ^ 8;     //1��ͣ����ʱ 0����
sbit relay = Bit16 ^ 9;     //�̵�����MOS�� �͵�ƽ����(��ͨ) 1��ֻ�����ϻ��ͷ�1��

/*************	���ر�������	**************/


/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/
void InitTimer0() {        //��ʼ����ʱ��0�ж�
    TMOD = 0x01;
    TH0 = 0xFC;
    TL0 = 0x18;
    EA = 1;
    ET0 = 1;
    TR0 = 1;
}

void beep(u16 ms) {     //���������� ����:���к���
    beep_ms = ms;
    Beep = 0;
}

u16 set_time() {        //�趨��ʱʱ��
    u8 n = 0;       //0:�༭ǧλ 1:��λ 2:ʮλ 3:��λ
    u8 o[4];
    o[0] = 0; o[1] = 0; o[2] = 0; o[3] = 0;
    while (1) {
        feed_dog();
        wink = 8 >> n;
        show[0] = nbr[o[0]];
        show[1] = nbr[o[1]];
        show[2] = nbr[o[2]];
        show[3] = nbr[o[3]];
        if (S2 == 0) {                        //S2���� �̰���1
            beep(50);
            if (o[n]++ >= 9) o[n] = 0;
            show[n] = nbr[o[n]];
            delayms = 500; delay = 1;
            while (delay && S2 == 0);        //������ʱ�ȴ�
            while (S2_ms >= 500) {                //��������,��ʼ����
                feed_dog();
                delayms = 125; delay = 1;                //0.125������һ��
                while (delay && S2 == 0);
                beep(50);
                if (o[n]++ >= 9) o[n] = 0;
                show[n] = nbr[o[n]];
            }
            delay = 0;
            delayms = 0;
        }

        if (S1 == 0) {                //S1����
            beep(50);
            n++;                //������һλ
            while (S1 == 0) {
                feed_dog();
                if (S1_ms >= 1000 && !bak) {                //�����˻ر༭
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

            if (n >= 4) {                //��λ�������
                wink = 0;
                return o[0] * 1000 + o[1] * 100 + o[2] * 10 + o[3];

            }
        }
    }
//    return 0;
}


/******************** IO���ú��� **************************/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
	GPIO_InitStructure.Pin  = GPIO_Pin_All;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);	//��ʼ��
}



/******************** ������ **************************/
void main(void)
{
	 u8 i = 0;
    Beep = 1;
    WDT_CONTR = 0x27;           //��ʼ�����Ź�
    P1M0 = 0x0F; P1M1 = 0x00;   //P1.0~P1.3ǿ����
    P2M0 = 0xFF; P2M1 = 0x00;   //P2.0~P2.7ǿ����
    //�ϵ�1�������ȫ�� ��������
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
        do {        //ʱ���趨
            time = set_time();
        } while (!time);
        time -= 1;
        start = 1;
        relay = 0;
        while (start) {
            feed_dog();
            if (S1 == 0) {      //S1���·�������1��
                beep(50);
                while (S1 == 0) {
                    feed_dog();
                }
            }

            if (S2 == 0) {      //S2������ͣ����ʱ
                beep(50);
                pause = !pause;
                relay = pause;
                while (S2 == 0) {
                    feed_dog();
                    if (pause && S2_ms >= 3000) {       //����3�븴λ
                        _nop_(); _nop_(); _nop_(); _nop_();
                        reset();
                        _nop_(); _nop_(); _nop_(); _nop_();
                    }
                }
            }

        }
        //����ʱ����
        show[0] = 0x79;     //E
        show[1] = 0x54;     //n
        show[2] = 0x5E;     //d
        show[3] = 0x00;
        for (i = 0; i < 6; i++) {       //��������6��
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

void Timer0Interrupt() interrupt 1{         //12Mhz 1mS��ʱ���ж�
    static u8 seg = 0;  //��ǰ����ɨ�������ܶ��� 4λ����� ��Χ:0~3
    static u8 ms1 = 0;
    static u16 ms = 0, ms2 = 0;
    TH0 = 0xFC;
    TL0 = 0x18;
    if (delay && !--delayms) {    //��ʱ�������
        delay = 0;
    }
    //����ܶ�̬ɨ�账��
    P1 |= 0xF;          //��Ӱ
    P2 = test ? 0xFF : (wink & (1 << (3 ^ seg)) && !hz10 ? 0 : show[seg]);      //����
    P1 &= ~(1 << seg);   //λ��
    if (++seg >= 4) seg = 0;

    if (test) {     //����ģʽ
        S1 = 1; S2 = 1;
        P3_2 = 0;
        return;
    }

    //����������
    P3_2 = Beep;
    if (!Beep && !--beep_ms) {
        Beep = 1;
    }
    if (++ms1 >= 50) {
        ms1 = 0;
        hz10 = !hz10;
    }

    //��������
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
    //�̵�������
    if (++ms2 >= 1000) {    //�̵���1��ֻ�ܽ���1�����ϻ��ͷţ���ֹƵ�����ص����豸��
        ms2 = 0;
        P3_3 = relay;
    }
    //ʱ�䵹��ʱ����
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

                if (!time--) {      //����ʱ����
                    start = 0;
                    relay = 1;
                }
            }
        }

    }


}


