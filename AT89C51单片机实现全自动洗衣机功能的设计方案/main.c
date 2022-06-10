//功能：基于89C51的全自动洗衣机
//备注：没有脱水时的防振功能
#include <STC89C5xRC.H>
/************************数据类型宏定义******************/
#define uchar unsigned char
#define uint unsigned int
/***********************使能、禁止宏定义*****************/
#define Enable(); EA=1;
#define Disable(); EA=0;
/********************程序选择功能标志宏定义**************/
#define BIAOZHUN 0
#define JINGJI 1
#define DANDU 2
#define PAISHUI 3
/********************强弱选择功能标志宏定义**************/
#define QIANGXI 0
#define RUOXI 1
/*******************进水、排水时间限制宏定义*************/
#define T_JinShui 240 //4min*60s
#define T_PaiShui 60 //1min*60s
#define T_XiDi 360 //6min*60s
#define T_PiaoXi_1 240 //4min*60s
#define T_PiaoXi_2 120 //2min*60s
#define T_TuoShui 120 //2min*60s
/**********************电机控制宏定义********************/
#define ZHENGZHUAN 0
#define STOP 1
#define FANZHUAN 2
#define ZhengZhuan(); PIN_Motor_A = 0; PIN_Motor_B = 1;
#define Stop(); PIN_Motor_A = 1; PIN_Motor_B = 1;
#define FanZhuan(); PIN_Motor_A = 1; PIN_Motor_B = 0;
/*******************暂停、继续控制宏定义*****************/
#define ZanTIng(); ET0 = 0; ET1 = 0; Stop();
#define JiXu(); ET0 = 1; ET1 = 1;
#define flag_Ok (flag_JinShui||flag_PaiShui||flag_XiDi||flag_PiaoXi||flag_TuoShui)
/*************************管脚定义***********************/
sbit LED_BiaoZhun = P1^0; //标准程序显示灯
sbit LED_JingJi = P1^1; //经济程序显示灯
sbit LED_DanDu = P1^2; //单独程序显示灯
sbit LED_PaiShui = P1^3; //排水程序显示灯
sbit LED_QiangXi = P1^4; //强洗状态灯
sbit LED_RuoXi = P1^5; //弱洗状态灯
sbit LED_XiDi = P1^6; //洗涤状态灯
sbit LED_PiaoXi = P1^7; //漂洗状态灯
sbit LED_TuoShui = P2^0; //脱水状态灯
sbit PIN_BaoJing = P2^1; //报警喇叭
sbit PIN_JinShui = P2^2; //进水控制
sbit PIN_PaiShui = P2^3; //排水控制
sbit PIN_Motor_A = P2^4; //电机脚A
sbit PIN_Motor_B = P2^5; //电机脚B
sbit K_SEL_ChengXu = P3^0; //选择程序键
sbit K_SEL_QiangRuo = P3^1; //选择强弱键
sbit p32 = P3^2; //启动和暂停按键
sbit p33 = P3^3; //水位/开盖检测
sbit SW_ShuiWei = P3^6; //水位开关
sbit SW_AnQuan = P3^7; //安全开关
void Programme_PaiShui(void);
uchar flag_SEL_ChengXu; //默认为标准程序,
bit flag_SEl_QiangRuo; //默认为强洗,1为弱洗
bit flag_Run; //运行标志,1为运行
bit flag_JinShui; //进水标志,1为进水
bit flag_PaiShui; //排水标志,1为排水
bit flag_XiDi; //置洗涤标志
bit flag_PiaoXi; //置漂洗标志
bit flag_TuoShui; //脱水标志
bit flag_BaoJing; //报警标志
uchar _50ms; //每50ms加一次的变量
uint s; //秒
uint T_S; //定时总时间
uchar T_Motor; //电机控制时间

void Delay_10ms(uint T1)
{
    uint t1,t2;
    for(t1=0; t1<T1; t1++)
        for(t2=0; t2<1250; t2++); //10ms
}

void Pin_Init(void)
{   
		LED_BiaoZhun = 0; //默认为标准程序,灯亮
    LED_JingJi = 1; //经济程序显示灯,灭
    LED_DanDu = 1; //单独程序显示灯,灭
    LED_PaiShui = 1; //排水程序显示灯,灭
    LED_QiangXi = 0; //默认为强洗状态,灯亮
    LED_RuoXi = 1; //弱洗状态灯,灭
    LED_XiDi = 1; //洗涤状态灯,运行后灯亮
    LED_PiaoXi = 1; //漂洗状态灯,运行后灯亮
    LED_TuoShui = 1; //脱水状态灯,运行后灯亮
    PIN_BaoJing = 1; //报警喇叭,不叫
    PIN_JinShui = 1; //进水控制,关
    PIN_PaiShui = 1; //排水控制,关
    PIN_Motor_A = 1; //电机脚A,不走
    PIN_Motor_B = 1; //电机脚B,不走

}
/********************外部中断0（P32）初始化程序********************/
void ExInt_Init(void)
{   
		IT0 = 1;
    EX0 = 1;
    IT1 = 1;
    EX1 = 1;
}
/*******************定时器0初始化程序******************/
void Timer0_Init(void) //其中没ET0是为以后控制暂停用的
{   
	TMOD = TMOD | 0X01; //定时方式1
    TH0 = 0X3C; //50ms
    TL0 = 0XB0;
    TR0 = 1;
    _50ms = 0;
    s = 0;
}
/*******************定时器1初始化程序******************/
void Timer1_Init(void) //其中没ET1是为以后控制暂停用的
{   
		TMOD = TMOD | 0X10; //定时方式1
    TH1 = 0X3C; //50ms
    TL1 = 0XB0;
}
/********************电机初始化程序********************/
void T_Motor_Ctl(uchar t1,uchar t2,uchar t3)
{
    if(!flag_SEl_QiangRuo)//强洗
        T_Motor = t1;
    if(flag_SEl_QiangRuo)//弱洗
        T_Motor = t2;
    if(flag_TuoShui) //脱水
        T_Motor = t3;
}
/********************电机初始化程序********************/
void Motor_Init(void)
{   
		ZhengZhuan();
    T_Motor_Ctl(4,3,5);
}
/**********************解除报警************************/
void No_BaoJing(void)
{   
	flag_BaoJing= 0; //置报警标志
    PIN_BaoJing = 1; //报警喇叭
    TR1 = 0;//而进水、排水已没时间限制了
}
/*******外部中断0程序为运行、解除报警或者暂停***********/
void int0(void) interrupt 0
{
	static  volatile bit flag_ZanTing = 0;
    if(!p32)
    {   
			Delay_10ms(1); //延时10ms左右,去抖动
        if(!p32)
        {   
					if(!flag_Run)
               flag_Run = 1; //置运行标志
            else if(flag_BaoJing)
                No_BaoJing(); //解除报警
            else if(flag_Ok)
            {   							
                flag_ZanTing = ~flag_ZanTing;
                if(flag_ZanTing)
                {
                  //  ZanTing();
									Stop();
                }//必须加{}
                else
                {
                    JiXu();
                } //必须加{}
            }//end else
        }//end if(!p32) 2
    }//end if(!p32) 1
}
/**********************报警程序************************/
void BaoJing(void)
{   
	flag_BaoJing= 1; //置报警标志
    TR0 = 0;
    TR1 = 1;
}
/*******************时间到处理程序*********************/
void Ok_Timer(void)
{   
		s = 0;
    if(flag_JinShui || flag_PaiShui) //进水中~、排水中~
    {  
			BaoJing(); //报警程序
    }//end if(flag_JinShui || flag_PaiShui)
    else
    {  
			flag_XiDi = 0; //清洗涤标志
        flag_PiaoXi = 0; //清漂洗标志
        flag_TuoShui= 0; //清脱水标志
        Stop();
    }//end else
}
/********************电机控制程序**********************/
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
/*******************定时器0中断程序********************/
void Timer0(void) interrupt 1
{
    TR0 = 0; //停止计数
    TH0 = 0X3C; //重装定时器值
    TL0 = 0XB0;
    TR0 = 1;
    _50ms++;
    if(_50ms == 20) //1s到
    {   _50ms = 0;
        s++;
        if(s == T_S) //定时到
            Ok_Timer();
        if(flag_XiDi || flag_PiaoXi || flag_TuoShui)
           Motor(); //电机控制程序
    }//end if(_50ms == 20)
}//end Timer0()
/**********外部中断1程序为水位到或者盖开处理***********/
void int1(void) interrupt 2
{   
	if(!p33)
    {  
			Delay_10ms(1); //延时10ms左右,去抖动
        if(!p33)
        {
        }//end if(!p33) 2
    }//end if(!p33) 1
}
/************定时器1中断程序作为闪烁及报警**************/
void Timer1(void) interrupt 3
{   
	static uchar _50ms; //每50ms加一次的变量
    TR1 = 0; //停止计数
    TH1 = 0X3C; //重装定时器值
    TL1 = 0XB0;
    TR1 = 1;
    _50ms++;
    if(_50ms == 14) //0.7s到
    {   _50ms = 0;
        if(flag_XiDi) //洗涤中~
            LED_XiDi = ~LED_XiDi; //洗涤状态灯闪烁
        if(flag_PiaoXi) //漂洗中~
            LED_PiaoXi = ~LED_PiaoXi; //漂洗状态灯闪烁
        if(flag_TuoShui) //脱水中~
            LED_TuoShui = ~LED_TuoShui; //脱水状态灯闪烁
        if(flag_BaoJing)
            PIN_BaoJing = ~PIN_BaoJing; //报警
    }//end if(_50ms == 14)
}
/**********************程序选择程序********************/
void SEL_ChengXU(void)
{  
	if(!K_SEL_ChengXu) //程序选择键
    {   
			Delay_10ms(1); //延时10ms左右,去抖动
        if(!K_SEL_ChengXu)
        {   while(!K_SEL_ChengXu);
            flag_SEL_ChengXu++;
            if(
							flag_SEL_ChengXu==4)
                flag_SEL_ChengXu = 0;
            switch(flag_SEL_ChengXu)
            {
            case BIAOZHUN :
                LED_BiaoZhun= 0; //亮
                LED_JingJi = 1; //灭
                LED_DanDu = 1; //灭
                LED_PaiShui = 1; //灭
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
/********************强弱选择程序**********************/
void SEL_QiangRuo(void)
{   
	if(!K_SEL_QiangRuo) //强弱选择键
    {   Delay_10ms(1); //延时10ms左右,去抖动
        if(!K_SEL_QiangRuo)
        {   
					while(!K_SEL_QiangRuo);
            flag_SEl_QiangRuo = ~flag_SEl_QiangRuo;
            if(!flag_SEl_QiangRuo)
            {   
							LED_QiangXi = 0; //强洗状态,灯亮
                LED_RuoXi = 1; //弱洗状态灯,灭
            }//end if(!flag_SEl_QiangRuo)
            else
            {   
							LED_QiangXi = 1; //强洗状态,灯亮
                LED_RuoXi = 0; //弱洗状态灯,灭
            }//end else
        }//end if(!K_SEL_QiangRuo) 2
    }//end if(!K_SEL_QiangRuo) 1
}
/**********************进水程序***********************/
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
/**********************排水程序***********************/
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
    Delay_10ms(500);//延时50s,经验值5000
    PIN_PaiShui = 1;
    flag_PaiShui = 0;
}
/**********************洗涤程序***********************/
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
/**********************漂洗程序***********************/
void PiaoXi(uchar t)
{   
	flag_PiaoXi = 1;
    TR1 = 1;
    Timer0_Init();
    T_S = t;
    Motor_Init();
    while(flag_PiaoXi);
}
/*********************开盖保护程序********************/
void BaoHu_GaiKai(void)
{   
	if(SW_AnQuan) //盖开
    {  
		//	ZanTing();
				Stop();
        LED_TuoShui = 0;
    }//end if
    else
    {   
			JiXu(); //电机不用管,最多5s不转
    }//end else
}
/***********************脱水程序**********************/
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
    LED_TuoShui = 1; //而出水阀一直开着
}
/********************标准洗衣程序*********************/
void Programme_BiaoZhun(void)
{   
	LED_XiDi = 0; //洗涤状态灯
    LED_PiaoXi = 0; //漂洗状态灯
    LED_TuoShui = 0; //脱水状态灯
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
/********************经济洗衣程序*********************/
void Programme_JingJi(void)
{  
		TR1 = 0;
    LED_XiDi = 0; //洗涤状态灯
    LED_PiaoXi = 0; //漂洗状态灯
    LED_TuoShui = 0; //脱水状态灯
    JinShui();
    XiDi();
    PaiShui();
    JinShui();
    PiaoXi(T_PiaoXi_1);//4min*60s
    Programme_PaiShui();
}
/********************单独洗衣程序*********************/
void Programme_DanDu(void)
{   
		LED_XiDi = 0; //洗涤状态灯
    LED_PiaoXi = 1; //漂洗状态灯
    LED_TuoShui = 1; //脱水状态灯
    JinShui();
    XiDi();
}
/********************排水洗衣程序*********************/
void Programme_PaiShui(void)
{   
	LED_XiDi = 1; //洗涤状态灯
    LED_PiaoXi = 1; //漂洗状态灯
    LED_TuoShui = 0; //脱水状态灯
    PaiShui();
    TuoShui();
}
/************************主程序************************/
void main()
{
    Pin_Init(); //管脚初始化
    ExInt_Init(); //外中断初始化
    Timer1_Init();
    ET0 = 1;
    ET1 = 1;
    Enable(); //开总中断
    while(1)
    {   
			if(!flag_Run) //运行前按键才有效
        {   
					SEL_ChengXU(); //程序选择程序
            SEL_QiangRuo(); //强弱选择程序
        }//end if(!flag_Run)
        if(flag_Run)//用else不,按启动键后,有时没反映？？？？//
        {   
						PIN_PaiShui = 1; //关排水阀
            switch(flag_SEL_ChengXu)
            {
            case BIAOZHUN :
                Programme_BiaoZhun(); //标准洗衣程序
                break;

            case JINGJI :
                Programme_JingJi(); //经济洗衣程序
                break;

            case DANDU :
                Programme_DanDu(); //单独洗衣程序
                break;

            case PAISHUI :
                Programme_PaiShui(); //排水洗衣程序
                break;

            default :
               break;
            }//end switch
        }//end esle
        flag_Run = 0;
    }//end while
}