//#include <reg51.h>
#include <intrins.h>
#include <stdio.h> 	       // Ϊʹ��KEIL�Դ��Ŀ⺯��printf������
#include <STC15F2K60S2.H>
#include <string.h> 
#define uchar unsigned char
#define uint  unsigned int
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};

void delay(uchar x);  //x*0.14MS
void delay1(int ms);


sbit IRIN = P3^2;         //���������������


uchar IRCOM[7];

sbit LCD_RS = P2^6;             
sbit LCD_RW = P2^5;
sbit LCD_EN = P2^7;
//sfr AUXR  = 0x8E; 
sfr BRT = 0x9C; //S2 Baud-Rate Timer 

uchar code  cdis1[ ] = {" Red Control "};
uchar code  cdis2[ ] = {"  IR-CODE: --H  "};

void UART_init(void)
{		  
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR &= 0xFB;		//��ʱ��ʱ��12Tģʽ
	T2L = 0xE8;		//���ö�ʱ��ʼֵ
	T2H = 0xFF;		//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	TI=1;			// �ܹؼ���ʹ��printf����ʱ�����д�����
} 


/*******************************************************************/
main()
{
    IE = 0x81;                 //�������ж��ж�,ʹ�� INT0 �ⲿ�ж�
    TCON = 0x01;               //������ʽΪ���帺���ش���    
    IRIN=1;                    //I/O�ڳ�ʼ��   
   
	UART_init();
	 delay1(10);                 //��ʱ
	 printf ("stc408as\n"); 
   while(1)	;
} //end main
/**********************************************************/
void IR_IN() interrupt 0 using 0
{
  unsigned char j,k,N=0;
     EX0 = 0;   
	 delay(15);
	 if (IRIN==1) 
     { EX0 =1;
	   return;
	  } 
                           //ȷ��IR�źų���
  while (!IRIN)            //��IR��Ϊ�ߵ�ƽ������9ms��ǰ���͵�ƽ�źš�
    {delay(1);}

 for (j=0;j<4;j++)         //�ռ���������
 { 
  for (k=0;k<8;k++)        //ÿ��������8λ
  {
   while (IRIN)            //�� IR ��Ϊ�͵�ƽ������4.5ms��ǰ���ߵ�ƽ�źš�
     {delay(1);}
    while (!IRIN)          //�� IR ��Ϊ�ߵ�ƽ
     {delay(1);}
     while (IRIN)           //����IR�ߵ�ƽʱ��
      {
    delay(1);
    N++;           
    if (N>=30)
	 { EX0=1;
	 return;}                  //0.14ms���������Զ��뿪��
      }                        //�ߵ�ƽ�������                
     IRCOM[j]=IRCOM[j] >> 1;                  //�������λ����0��
     if (N>=8) {IRCOM[j] = IRCOM[j] | 0x80;}  //�������λ����1��
     N=0;
  }//end for k
 }//end for j
   
   if (IRCOM[2]!=~IRCOM[3])
   { EX0=1;
     return; }

   IRCOM[5]=IRCOM[2] & 0x0F;     //ȡ����ĵ���λ
   IRCOM[6]=IRCOM[2] >> 4;       //����4�Σ�����λ��Ϊ����λ

   if(IRCOM[5]>9)
    { IRCOM[5]=IRCOM[5];}
   else
	  IRCOM[5]=IRCOM[5];

   if(IRCOM[6]>9)
    { IRCOM[6]=IRCOM[6];}
   else
	  IRCOM[6]=IRCOM[6];
	printf("RECE_IR DATA:%02bx-%02bx-%02bx-%02bx \n", IRCOM[0], IRCOM[1],IRCOM[2], IRCOM[3]); 
 //  printf ("rece_ir:%s\n",IRCOM); 
			memset(IRCOM, 0, sizeof(IRCOM));          //�������   
     EX0 = 1; 
} 


/**********************************************************/
void delay(unsigned char x)    //x*0.14MS
{
 unsigned char i;
  while(x--)
 {
  for (i = 0; i<13; i++) {}
 }
}

/**********************************************************/
void delay1(int ms)
{
 unsigned char y;
  while(ms--)
 {
  for(y = 0; y<250; y++)
  {
   _nop_();
   _nop_();
   _nop_();
   _nop_();
  }
 }
}

