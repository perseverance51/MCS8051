#include <reg52.h>
#define uchar unsigned char
#define uint  unsigned	int
#define C1  0.045

uchar code WenDu1[]="数\xfd字万用表   ";
uchar code WenDu2[]="欢迎使用        ";
uchar code code_L[]="电感：          ";	
uchar code code_R[]="电阻：          ";
uchar code code_C[]="电容：          ";
uchar code code_e[]="错误：          ";
uchar  num;


sbit key1  =P1^0;
sbit key2  =P1^1;
sbit key3  =P1^2;
//unsigned long LZ=0;
//float f=0;

float LZ=0;
float RX=0;
float CX=0;

float f=0;

float temp_f[4];

unsigned int tt,th1,tl1;
unsigned int pluse=0;	
bit flag=0;	
unsigned char key_num=0;

#define  L 1
#define  R 2
#define  C 3

sbit aa1 =P2^0;
sbit bb1 =P2^1;

#include<Lcd12864.h>

void cd4051(unsigned char num)
{
		switch(num){
				case 1: bb1=0;aa1=0;break;
				case 2: bb1=0;aa1=1; break;
				case 3: bb1=1;aa1=0; break;
				default : aa1=1;aa1=1;break;
		}
}

void scan_key()
{
		if(!key1){
				if(!key1){
						while(!key1);
						key_num=1;
						cd4051(L);
					//	aa1=0;bb1=0;
					    Lcd_ShowSring(4,0,code_L);
						Lcd_ShowSring(1,15,"L");
				}
		}
		if(!key2){
				if(!key2){
						while(!key2);
						key_num=2;
						cd4051(R);
					//aa1=0;bb1=1;
						Lcd_ShowSring(4,0,code_R);
						Lcd_ShowSring(1,15,"R");
				}
		}
		if(!key3){
				if(!key3){
						while(!key3);
						key_num=3;
						cd4051(C);
					   //aa1=1;bb1=0;
						Lcd_ShowSring(4,0,code_C);
						Lcd_ShowSring(1,15,"C");
				}
		}
}


	//中断初始化
  void init()
  {	  
 //	TMOD=0x11;		//定时器0和定时器1都是方式1,16位定时器

	//定时器设置
    TMOD=0x51; 		//设置定时器0，方式1:16位定时器

	TH1=0x00;	 //定时器1高位装初值
	TL1=0x00;	 //定时器1低位装初值
	ET1=1;	//定时器1	  使能位
	
	TH0=(65535-50000)/255;	  //定时器1高位装初值
	TL0=(65535-50000)%255;;	  //定时器1高位装初值
	ET0=1;   ////定时器0	  使能位

	TR0=1;
	TR1=1;

	EA=1;//	总中断
 }

void Measurement_L();//电感测量
void Measurement_R();//电阻测量
void Measurement_C();//电容测量
unsigned char gather_num=0;
 unsigned char m_second=0;
void main ()
{

	LCDInit();		
	Lcd_ShowSring(1,0,WenDu1);
	Lcd_ShowSring(2,0,WenDu2);
//	Lcd_ShowSring(4,0,code_R);
	init();
//	cd4051(R);
//	key_num=2;
	while(1){
		
		
	    scan_key();
		if(flag){
			flag=0;

			//在十ms里面记了这么多个脉冲
			// 那么一个脉冲的周期就是  50ms/tt/1000  (s)   化简得到  f  20tt
			if(gather_num<4) {
					temp_f[gather_num]=pluse*65536+256*TH1+TL1;	  //1000000000000 12  100000000 8
					gather_num++;

			}
			else{
					gather_num=0;
					f=(temp_f[0]+temp_f[1]+temp_f[2]+temp_f[3])/4.0;	
				//	Lcd_Display(3,0,f);
					switch(key_num)	 {
		
							case 1:Measurement_L();	break;
							case 2:Measurement_R(); break;
							case 3:Measurement_C(); break;
							default :break;
					
					} 
			}
	


			TH0=(65535-50000)/255;	  //定时器1高位装初值
			TL0=(65535-50000)%255;;	  //定时器1高位装初值 						
			TH1=0;	                  //清零开始重新计数
			TL1=0;
			pluse=0;
			TR0=1;//开定时器计时10ms;//使能位外部中断0 数10内有多少个脉冲
			TR1=1;//进行新一轮的计数
			m_second=0;	
			
			     
//		    num++;
//			Lcd_Display(4,4,num);
		}
					 				
	}
}


 void time0()interrupt 1		//定时器0 服务
{	
//	TR1=0;//关掉定时器1

	
//	TH0=(65535-50000)/255;	  //定时器1高位装初值
//	TL0=(65535-50000)%255;;	  //定时器1高位装初值 */
	TH0=60;	  //定时器1高位装初值
	TL0=235;;	  //定时器1高位装初值
	m_second++;
	if(m_second==20){
			TR1=0;//关掉定时器1
			TR0=0; // 关掉定时器0
		m_second=0;	
		flag=1;
	}
			
}

 void time1()interrupt 3		//定时器1 服务
 {
   		pluse++;
		TH1=0;
		TL1=0;
 }

void Measurement_L()
{

			if(f==0){
				LZ=0;
				Lcd_ShowSring(4,0,code_L);
			}
			else {
				LZ=5.63466627*10e10/f/f;
				LZ=LZ/1.0727272727272727272727272727273;
				Lcd_ShowSring(4,0,"电感：");
			  Lcd_Display_u(4,4,LZ);
			} 
			
			if(LZ>99999999)	{
			// 	Lcd_ShowSring(4,0,code_e);
			}
																	  			
			
	
}
void Measurement_R()//电阻测量
{
			
			if(f==0){
							
					RX=0;	
						
			}

			else if(  0< f&& f<30) {								   //1M   7

				   	RX=	14400000.0/f;
					RX= RX-3000;
					RX  =RX/2/0.95;  //修正
					
			}
			else if(f <700) {								   //10k   650

				   	RX=	14400000.0/f;
					RX= RX-3000;
					RX  =RX/2/0.95;  //修正
					 //
			}
			else if(f <1500) {								   //4.7k  1209

				   	RX=	14400000.0/f;
					RX= RX-3000;
					RX  =RX/2/0.94;  //修正
					
			}
 			else if(f <3000) {								 //1k	2983

				   	RX=	14400000.0/f;
					RX= RX-3000;
				 	RX  =RX/2/0.91;  //修正
					//0.91286863270777479892761394101877
					
			}
			else if(f <4500) {

				   	RX=	14400000.0/f;
					RX= RX-3000;
				                                                //修正  200   4372
					RX  =RX/2/0.74;  //修正
					
			}
			else if(f <5000) {

				   	RX=	14400000.0/f;
					RX= RX-3000;
				                                                //修正  100  
					RX  =RX/2/0.50;  //修正
					
			}

			if(RX==0){
					Lcd_ShowSring(4,0,code_R);
			} else{
					Lcd_ShowSring(4,0,"电阻：");
					Value_to_ASCII1(4,3, RX);

			}

			if(RX>99999999)	{
			//	Lcd_ShowSring(4,0,code_e);	
			}
							
		
}
void Measurement_C()//C测量
{
			if(f==0){
							
					CX=0;
					Lcd_ShowSring(4,0,code_C);			
			}
			else if(f>5000){

				   CX=0;
				   Lcd_ShowSring(4,0,code_C);
			}
			else {
				CX=48100.00/f/1.0276595744680851063829787234043;//修正	
				Lcd_ShowSring(4,0,"电容：");
			    Value_to_ASCIIC(4,3,CX);					
			}
//			Lcd_ShowSring(4,0,"电容：");
//			Value_to_ASCIIC(4,3,CX);				
			if(CX>99999999)	{
//				Lcd_ShowSring(4,0,code_e);	
			}
			
}