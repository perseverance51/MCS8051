//���ܣ�����89C51��ȫ�Զ�ϴ�»�
//��ע��û����ˮʱ�ķ�����
#include <STC89C5xRC.H>
/************************�������ͺ궨��******************/
#define uchar unsigned char
#define uint unsigned int
/***********************ʹ�ܡ���ֹ�궨��*****************/
#define Enable(); EA=1;
#define Disable(); EA=0;
/********************����ѡ���ܱ�־�궨��**************/
#define BIAOZHUN 0
#define JINGJI 1
#define DANDU 2
#define PAISHUI 3
/********************ǿ��ѡ���ܱ�־�궨��**************/
#define QIANGXI 0
#define RUOXI 1
/*******************��ˮ����ˮʱ�����ƺ궨��*************/
#define T_JinShui 240 //4min*60s
#define T_PaiShui 60 //1min*60s
#define T_XiDi 360 //6min*60s
#define T_PiaoXi_1 240 //4min*60s
#define T_PiaoXi_2 120 //2min*60s
#define T_TuoShui 120 //2min*60s
/**********************������ƺ궨��********************/
#define ZHENGZHUAN 0
#define STOP 1
#define FANZHUAN 2
#define ZhengZhuan(); PIN_Motor_A = 0; PIN_Motor_B = 1;
#define Stop(); PIN_Motor_A = 1; PIN_Motor_B = 1;
#define FanZhuan(); PIN_Motor_A = 1; PIN_Motor_B = 0;
/*******************��ͣ���������ƺ궨��*****************/
#define ZanTIng(); ET0 = 0; ET1 = 0; Stop();
#define JiXu(); ET0 = 1; ET1 = 1;
#define flag_Ok (flag_JinShui||flag_PaiShui||flag_XiDi||flag_PiaoXi||flag_TuoShui)
/*************************�ܽŶ���***********************/
sbit LED_BiaoZhun = P1^0; //��׼������ʾ��
sbit LED_JingJi = P1^1; //���ó�����ʾ��
sbit LED_DanDu = P1^2; //����������ʾ��
sbit LED_PaiShui = P1^3; //��ˮ������ʾ��
sbit LED_QiangXi = P1^4; //ǿϴ״̬��
sbit LED_RuoXi = P1^5; //��ϴ״̬��
sbit LED_XiDi = P1^6; //ϴ��״̬��
sbit LED_PiaoXi = P1^7; //Ưϴ״̬��
sbit LED_TuoShui = P2^0; //��ˮ״̬��
sbit PIN_BaoJing = P2^1; //��������
sbit PIN_JinShui = P2^2; //��ˮ����
sbit PIN_PaiShui = P2^3; //��ˮ����
sbit PIN_Motor_A = P2^4; //�����A
sbit PIN_Motor_B = P2^5; //�����B
sbit K_SEL_ChengXu = P3^0; //ѡ������
sbit K_SEL_QiangRuo = P3^1; //ѡ��ǿ����
sbit p32 = P3^2; //��������ͣ����
sbit p33 = P3^3; //ˮλ/���Ǽ��
sbit SW_ShuiWei = P3^6; //ˮλ����
sbit SW_AnQuan = P3^7; //��ȫ����
void Programme_PaiShui(void);
uchar flag_SEL_ChengXu; //Ĭ��Ϊ��׼����,
bit flag_SEl_QiangRuo; //Ĭ��Ϊǿϴ,1Ϊ��ϴ
bit flag_Run; //���б�־,1Ϊ����
bit flag_JinShui; //��ˮ��־,1Ϊ��ˮ
bit flag_PaiShui; //��ˮ��־,1Ϊ��ˮ
bit flag_XiDi; //��ϴ�ӱ�־
bit flag_PiaoXi; //��Ưϴ��־
bit flag_TuoShui; //��ˮ��־
bit flag_BaoJing; //������־
uchar _50ms; //ÿ50ms��һ�εı���
uint s; //��
uint T_S; //��ʱ��ʱ��
uchar T_Motor; //�������ʱ��

void Delay_10ms(uint T1)
{
    uint t1,t2;
    for(t1=0; t1<T1; t1++)
        for(t2=0; t2<1250; t2++); //10ms
}

void Pin_Init(void)
{   
		LED_BiaoZhun = 0; //Ĭ��Ϊ��׼����,����
    LED_JingJi = 1; //���ó�����ʾ��,��
    LED_DanDu = 1; //����������ʾ��,��
    LED_PaiShui = 1; //��ˮ������ʾ��,��
    LED_QiangXi = 0; //Ĭ��Ϊǿϴ״̬,����
    LED_RuoXi = 1; //��ϴ״̬��,��
    LED_XiDi = 1; //ϴ��״̬��,���к����
    LED_PiaoXi = 1; //Ưϴ״̬��,���к����
    LED_TuoShui = 1; //��ˮ״̬��,���к����
    PIN_BaoJing = 1; //��������,����
    PIN_JinShui = 1; //��ˮ����,��
    PIN_PaiShui = 1; //��ˮ����,��
    PIN_Motor_A = 1; //�����A,����
    PIN_Motor_B = 1; //�����B,����

}
/********************�ⲿ�ж�0��P32����ʼ������********************/
void ExInt_Init(void)
{   
		IT0 = 1;
    EX0 = 1;
    IT1 = 1;
    EX1 = 1;
}
/*******************��ʱ��0��ʼ������******************/
void Timer0_Init(void) //����ûET0��Ϊ�Ժ������ͣ�õ�
{   
	TMOD = TMOD | 0X01; //��ʱ��ʽ1
    TH0 = 0X3C; //50ms
    TL0 = 0XB0;
    TR0 = 1;
    _50ms = 0;
    s = 0;
}
/*******************��ʱ��1��ʼ������******************/
void Timer1_Init(void) //����ûET1��Ϊ�Ժ������ͣ�õ�
{   
		TMOD = TMOD | 0X10; //��ʱ��ʽ1
    TH1 = 0X3C; //50ms
    TL1 = 0XB0;
}
/********************�����ʼ������********************/
void T_Motor_Ctl(uchar t1,uchar t2,uchar t3)
{
    if(!flag_SEl_QiangRuo)//ǿϴ
        T_Motor = t1;
    if(flag_SEl_QiangRuo)//��ϴ
        T_Motor = t2;
    if(flag_TuoShui) //��ˮ
        T_Motor = t3;
}
/********************�����ʼ������********************/
void Motor_Init(void)
{   
		ZhengZhuan();
    T_Motor_Ctl(4,3,5);
}
/**********************�������************************/
void No_BaoJing(void)
{   
	flag_BaoJing= 0; //�ñ�����־
    PIN_BaoJing = 1; //��������
    TR1 = 0;//����ˮ����ˮ��ûʱ��������
}
/*******�ⲿ�ж�0����Ϊ���С��������������ͣ***********/
void int0(void) interrupt 0
{
	static  volatile bit flag_ZanTing = 0;
    if(!p32)
    {   
			Delay_10ms(1); //��ʱ10ms����,ȥ����
        if(!p32)
        {   
					if(!flag_Run)
               flag_Run = 1; //�����б�־
            else if(flag_BaoJing)
                No_BaoJing(); //�������
            else if(flag_Ok)
            {   							
                flag_ZanTing = ~flag_ZanTing;
                if(flag_ZanTing)
                {
                  //  ZanTing();
									Stop();
                }//�����{}
                else
                {
                    JiXu();
                } //�����{}
            }//end else
        }//end if(!p32) 2
    }//end if(!p32) 1
}
/**********************��������************************/
void BaoJing(void)
{   
	flag_BaoJing= 1; //�ñ�����־
    TR0 = 0;
    TR1 = 1;
}
/*******************ʱ�䵽�������*********************/
void Ok_Timer(void)
{   
		s = 0;
    if(flag_JinShui || flag_PaiShui) //��ˮ��~����ˮ��~
    {  
			BaoJing(); //��������
    }//end if(flag_JinShui || flag_PaiShui)
    else
    {  
			flag_XiDi = 0; //��ϴ�ӱ�־
        flag_PiaoXi = 0; //��Ưϴ��־
        flag_TuoShui= 0; //����ˮ��־
        Stop();
    }//end else
}
/********************������Ƴ���**********************/
void Motor(void)
{   
	static uchar s_Motor;
    s_Motor++;
    if(s_Motor == T_Motor)
    {   
			static uchar Motors;
        s_Motor = 0;
        Motors++;
        if(Motors==3)
            Motors = 0;
        switch(Motors)
        {
        case ZHENGZHUAN :
            ZhengZhuan();
            T_Motor_Ctl(4,3,5);
            break;

        case STOP :
            Stop();
            T_Motor_Ctl(1,2,2);
            break;

        case FANZHUAN :
            FanZhuan();
            T_Motor_Ctl(4,3,5);
            break;

        default :
            break;
        }//end switch
    }//end if
}
/*******************��ʱ��0�жϳ���********************/
void Timer0(void) interrupt 1
{
    TR0 = 0; //ֹͣ����
    TH0 = 0X3C; //��װ��ʱ��ֵ
    TL0 = 0XB0;
    TR0 = 1;
    _50ms++;
    if(_50ms == 20) //1s��
    {   _50ms = 0;
        s++;
        if(s == T_S) //��ʱ��
            Ok_Timer();
        if(flag_XiDi || flag_PiaoXi || flag_TuoShui)
           Motor(); //������Ƴ���
    }//end if(_50ms == 20)
}//end Timer0()
/**********�ⲿ�ж�1����Ϊˮλ�����߸ǿ�����***********/
void int1(void) interrupt 2
{   
	if(!p33)
    {  
			Delay_10ms(1); //��ʱ10ms����,ȥ����
        if(!p33)
        {
        }//end if(!p33) 2
    }//end if(!p33) 1
}
/************��ʱ��1�жϳ�����Ϊ��˸������**************/
void Timer1(void) interrupt 3
{   
	static uchar _50ms; //ÿ50ms��һ�εı���
    TR1 = 0; //ֹͣ����
    TH1 = 0X3C; //��װ��ʱ��ֵ
    TL1 = 0XB0;
    TR1 = 1;
    _50ms++;
    if(_50ms == 14) //0.7s��
    {   _50ms = 0;
        if(flag_XiDi) //ϴ����~
            LED_XiDi = ~LED_XiDi; //ϴ��״̬����˸
        if(flag_PiaoXi) //Ưϴ��~
            LED_PiaoXi = ~LED_PiaoXi; //Ưϴ״̬����˸
        if(flag_TuoShui) //��ˮ��~
            LED_TuoShui = ~LED_TuoShui; //��ˮ״̬����˸
        if(flag_BaoJing)
            PIN_BaoJing = ~PIN_BaoJing; //����
    }//end if(_50ms == 14)
}
/**********************����ѡ�����********************/
void SEL_ChengXU(void)
{  
	if(!K_SEL_ChengXu) //����ѡ���
    {   
			Delay_10ms(1); //��ʱ10ms����,ȥ����
        if(!K_SEL_ChengXu)
        {   while(!K_SEL_ChengXu);
            flag_SEL_ChengXu++;
            if(
							flag_SEL_ChengXu==4)
                flag_SEL_ChengXu = 0;
            switch(flag_SEL_ChengXu)
            {
            case BIAOZHUN :
                LED_BiaoZhun= 0; //��
                LED_JingJi = 1; //��
                LED_DanDu = 1; //��
                LED_PaiShui = 1; //��
                break;

            case JINGJI :
                LED_BiaoZhun= 1;
                LED_JingJi = 0;
                LED_DanDu = 1;
                LED_PaiShui = 1;
                break;

            case DANDU :
                LED_BiaoZhun= 1;
                LED_JingJi = 1;
                LED_DanDu = 0;
                LED_PaiShui = 1;
                break;

            case PAISHUI :
                LED_BiaoZhun= 1;
                LED_JingJi = 1;
                LED_DanDu = 1;
                LED_PaiShui = 0;
               break;

            default :
                break;
            }//end switch
        }//end if(!K_Sel_ChengXu) 2
    }//end if(!K_Sel_ChengXu) 1
}//END
/********************ǿ��ѡ�����**********************/
void SEL_QiangRuo(void)
{   
	if(!K_SEL_QiangRuo) //ǿ��ѡ���
    {   Delay_10ms(1); //��ʱ10ms����,ȥ����
        if(!K_SEL_QiangRuo)
        {   
					while(!K_SEL_QiangRuo);
            flag_SEl_QiangRuo = ~flag_SEl_QiangRuo;
            if(!flag_SEl_QiangRuo)
            {   
							LED_QiangXi = 0; //ǿϴ״̬,����
                LED_RuoXi = 1; //��ϴ״̬��,��
            }//end if(!flag_SEl_QiangRuo)
            else
            {   
							LED_QiangXi = 1; //ǿϴ״̬,����
                LED_RuoXi = 0; //��ϴ״̬��,��
            }//end else
        }//end if(!K_SEL_QiangRuo) 2
    }//end if(!K_SEL_QiangRuo) 1
}
/**********************��ˮ����***********************/
void JinShui(void)
{   
		PIN_JinShui = 0;
    flag_JinShui = 1;
    TR1 = 0;
    Timer0_Init();
    T_S = T_JinShui;
    while(SW_ShuiWei);
    PIN_JinShui = 1;
    flag_JinShui = 0;
    TR0 = 0;
    TF0 = 0;
}
/**********************��ˮ����***********************/
void PaiShui(void)
{   
		PIN_PaiShui = 0;
    flag_PaiShui = 1;
    TR1 = 0;
    Timer0_Init();
    T_S = T_PaiShui;
    while(!SW_ShuiWei);
    TR0 = 0;
    TF0 = 0;
    Delay_10ms(500);//��ʱ50s,����ֵ5000
    PIN_PaiShui = 1;
    flag_PaiShui = 0;
}
/**********************ϴ�ӳ���***********************/
void XiDi(void)
{   
		flag_XiDi = 1;
    TR1 = 1;
    Timer0_Init();
    T_S = T_XiDi;
    Motor_Init();
    while(flag_XiDi);
    TR1 = 0;
    LED_XiDi = 1;
}
/**********************Ưϴ����***********************/
void PiaoXi(uchar t)
{   
	flag_PiaoXi = 1;
    TR1 = 1;
    Timer0_Init();
    T_S = t;
    Motor_Init();
    while(flag_PiaoXi);
}
/*********************���Ǳ�������********************/
void BaoHu_GaiKai(void)
{   
	if(SW_AnQuan) //�ǿ�
    {  
		//	ZanTing();
				Stop();
        LED_TuoShui = 0;
    }//end if
    else
    {   
			JiXu(); //������ù�,���5s��ת
    }//end else
}
/***********************��ˮ����**********************/
void TuoShui(void)
{   
	flag_TuoShui = 1;
    PIN_PaiShui = 0;
    TR1 = 1;
    Timer0_Init();
    T_S = T_TuoShui;
    Motor_Init();
    while(flag_TuoShui)
    {  
			BaoHu_GaiKai();
    }//end while
    TR1 = 0;
    LED_TuoShui = 1; //����ˮ��һֱ����
}
/********************��׼ϴ�³���*********************/
void Programme_BiaoZhun(void)
{   
	LED_XiDi = 0; //ϴ��״̬��
    LED_PiaoXi = 0; //Ưϴ״̬��
    LED_TuoShui = 0; //��ˮ״̬��
    JinShui();
    XiDi();
    PaiShui();
    JinShui();
    PiaoXi(T_PiaoXi_1);//4min*60s//4*60
    PaiShui();
    JinShui();
    PiaoXi(T_PiaoXi_2);//2min*60s//2*60
    Programme_PaiShui();
}
/********************����ϴ�³���*********************/
void Programme_JingJi(void)
{  
		TR1 = 0;
    LED_XiDi = 0; //ϴ��״̬��
    LED_PiaoXi = 0; //Ưϴ״̬��
    LED_TuoShui = 0; //��ˮ״̬��
    JinShui();
    XiDi();
    PaiShui();
    JinShui();
    PiaoXi(T_PiaoXi_1);//4min*60s
    Programme_PaiShui();
}
/********************����ϴ�³���*********************/
void Programme_DanDu(void)
{   
		LED_XiDi = 0; //ϴ��״̬��
    LED_PiaoXi = 1; //Ưϴ״̬��
    LED_TuoShui = 1; //��ˮ״̬��
    JinShui();
    XiDi();
}
/********************��ˮϴ�³���*********************/
void Programme_PaiShui(void)
{   
	LED_XiDi = 1; //ϴ��״̬��
    LED_PiaoXi = 1; //Ưϴ״̬��
    LED_TuoShui = 0; //��ˮ״̬��
    PaiShui();
    TuoShui();
}
/************************������************************/
void main()
{
    Pin_Init(); //�ܽų�ʼ��
    ExInt_Init(); //���жϳ�ʼ��
    Timer1_Init();
    ET0 = 1;
    ET1 = 1;
    Enable(); //�����ж�
    while(1)
    {   
			if(!flag_Run) //����ǰ��������Ч
        {   
					SEL_ChengXU(); //����ѡ�����
            SEL_QiangRuo(); //ǿ��ѡ�����
        }//end if(!flag_Run)
        if(flag_Run)//��else��,����������,��ʱû��ӳ��������//
        {   
						PIN_PaiShui = 1; //����ˮ��
            switch(flag_SEL_ChengXu)
            {
            case BIAOZHUN :
                Programme_BiaoZhun(); //��׼ϴ�³���
                break;

            case JINGJI :
                Programme_JingJi(); //����ϴ�³���
                break;

            case DANDU :
                Programme_DanDu(); //����ϴ�³���
                break;

            case PAISHUI :
                Programme_PaiShui(); //��ˮϴ�³���
                break;

            default :
               break;
            }//end switch
        }//end esle
        flag_Run = 0;
    }//end while
}