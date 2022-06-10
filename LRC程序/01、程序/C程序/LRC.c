#include <reg52.h>
#define uchar unsigned char
#define uint  unsigned	int
#define C1  0.045

uchar code WenDu1[]="��\xfd�����ñ�   ";
uchar code WenDu2[]="��ӭʹ��        ";
uchar code code_L[]="��У�          ";	
uchar code code_R[]="���裺          ";
uchar code code_C[]="���ݣ�          ";
uchar code code_e[]="����          ";
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


	//�жϳ�ʼ��
  void init()
  {	  
 //	TMOD=0x11;		//��ʱ��0�Ͷ�ʱ��1���Ƿ�ʽ1,16λ��ʱ��

	//��ʱ������
    TMOD=0x51; 		//���ö�ʱ��0����ʽ1:16λ��ʱ��

	TH1=0x00;	 //��ʱ��1��λװ��ֵ
	TL1=0x00;	 //��ʱ��1��λװ��ֵ
	ET1=1;	//��ʱ��1	  ʹ��λ
	
	TH0=(65535-50000)/255;	  //��ʱ��1��λװ��ֵ
	TL0=(65535-50000)%255;;	  //��ʱ��1��λװ��ֵ
	ET0=1;   ////��ʱ��0	  ʹ��λ

	TR0=1;
	TR1=1;

	EA=1;//	���ж�
 }

void Measurement_L();//��в���
void Measurement_R();//�������
void Measurement_C();//���ݲ���
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

			//��ʮms���������ô�������
			// ��ôһ����������ھ���  50ms/tt/1000  (s)   ����õ�  f  20tt
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
	


			TH0=(65535-50000)/255;	  //��ʱ��1��λװ��ֵ
			TL0=(65535-50000)%255;;	  //��ʱ��1��λװ��ֵ 						
			TH1=0;	                  //���㿪ʼ���¼���
			TL1=0;
			pluse=0;
			TR0=1;//����ʱ����ʱ10ms;//ʹ��λ�ⲿ�ж�0 ��10���ж��ٸ�����
			TR1=1;//������һ�ֵļ���
			m_second=0;	
			
			     
//		    num++;
//			Lcd_Display(4,4,num);
		}
					 				
	}
}


 void time0()interrupt 1		//��ʱ��0 ����
{	
//	TR1=0;//�ص���ʱ��1

	
//	TH0=(65535-50000)/255;	  //��ʱ��1��λװ��ֵ
//	TL0=(65535-50000)%255;;	  //��ʱ��1��λװ��ֵ */
	TH0=60;	  //��ʱ��1��λװ��ֵ
	TL0=235;;	  //��ʱ��1��λװ��ֵ
	m_second++;
	if(m_second==20){
			TR1=0;//�ص���ʱ��1
			TR0=0; // �ص���ʱ��0
		m_second=0;	
		flag=1;
	}
			
}

 void time1()interrupt 3		//��ʱ��1 ����
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
				Lcd_ShowSring(4,0,"��У�");
			  Lcd_Display_u(4,4,LZ);
			} 
			
			if(LZ>99999999)	{
			// 	Lcd_ShowSring(4,0,code_e);
			}
																	  			
			
	
}
void Measurement_R()//�������
{
			
			if(f==0){
							
					RX=0;	
						
			}

			else if(  0< f&& f<30) {								   //1M   7

				   	RX=	14400000.0/f;
					RX= RX-3000;
					RX  =RX/2/0.95;  //����
					
			}
			else if(f <700) {								   //10k   650

				   	RX=	14400000.0/f;
					RX= RX-3000;
					RX  =RX/2/0.95;  //����
					 //
			}
			else if(f <1500) {								   //4.7k  1209

				   	RX=	14400000.0/f;
					RX= RX-3000;
					RX  =RX/2/0.94;  //����
					
			}
 			else if(f <3000) {								 //1k	2983

				   	RX=	14400000.0/f;
					RX= RX-3000;
				 	RX  =RX/2/0.91;  //����
					//0.91286863270777479892761394101877
					
			}
			else if(f <4500) {

				   	RX=	14400000.0/f;
					RX= RX-3000;
				                                                //����  200   4372
					RX  =RX/2/0.74;  //����
					
			}
			else if(f <5000) {

				   	RX=	14400000.0/f;
					RX= RX-3000;
				                                                //����  100  
					RX  =RX/2/0.50;  //����
					
			}

			if(RX==0){
					Lcd_ShowSring(4,0,code_R);
			} else{
					Lcd_ShowSring(4,0,"���裺");
					Value_to_ASCII1(4,3, RX);

			}

			if(RX>99999999)	{
			//	Lcd_ShowSring(4,0,code_e);	
			}
							
		
}
void Measurement_C()//C����
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
				CX=48100.00/f/1.0276595744680851063829787234043;//����	
				Lcd_ShowSring(4,0,"���ݣ�");
			    Value_to_ASCIIC(4,3,CX);					
			}
//			Lcd_ShowSring(4,0,"���ݣ�");
//			Value_to_ASCIIC(4,3,CX);				
			if(CX>99999999)	{
//				Lcd_ShowSring(4,0,code_e);	
			}
			
}