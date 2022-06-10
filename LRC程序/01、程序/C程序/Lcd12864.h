/********************************************
Description:  LCD12864 ���� ,���ô��з�ʽ,
********************************************/ 

//******12864**************
//#define CS =    ;   //RS(cs)  �Ӹߵ�ƽ//RW//EN


sbit lcd_RS =  P0^0; 
sbit Lcd_Rw	=	 P0^1;
sbit Lcd_En	=	 P0^2;

#define BIT(n)	(1<<(n))
#define SID_0   Lcd_Rw=0;
#define SID_1   Lcd_Rw=1; 
#define SCLK_0  Lcd_En=0;
#define SCLK_1  Lcd_En=1; 
//15�� PSB �ӵ͵�ƽ
void SendByte(uchar bye)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		if((bye<<i)&0x80){SID_1;}
		else {SID_0;}
		SCLK_1;
	//	delay1us(1);//û����ʱ����
	//     Delay(2);

		SCLK_0;

	}
}

void Lcd_WriteCom(uchar CMD)
{
	SendByte(0xf8);
	SendByte(CMD&0xf0);
	SendByte((CMD<<4)&0xf0);
//	delay1us(90);//8155��ģ�⴮��ͨ��,����,����8155����ʱ,//8M
//	delay1us(250);//16Mʱ����ʱ90us�Ѿ�����������
//     Delay(250);
//	  Delay(250);
//	  Delay(250);
}

void Lcd_WriteDate(uchar Data)
{	SendByte(0xf8+2);
	SendByte(Data&0xf0);
	SendByte((Data<<4)&0xf0);
}

void LCDInit()
{/*	RST=0;
	delay1MS(50);
	RST=1;          */
	lcd_RS=1;
//	DDRB|=(1 << 1)|(1<< 2);
    Lcd_WriteCom(0x34);//�������ã�һ����8λ���ݣ�����ָ�
    Lcd_WriteCom(0x30);//0000,1100  ������ʾ���α�off���α�λ��off
    Lcd_WriteCom(0x01);//0000,0001 ��DDRAM
    Lcd_WriteCom(0x06);//0000,0010 DDRAM��ַ��λ
    Lcd_WriteCom(0x0c);//1000,0000 �趨DDRAM 7λ��ַ000��0000����ַ������AC//     
}

/*****************************
�����ַ���
*****************************/
//void show_str(uchar const *s)//��AVRȥ��const
void  Lcd_String(const uchar  *str)
{ while(*str !='\0')
   { 
   		Lcd_WriteDate(*str++);
     // 	s++;
    //	delay1ms(1);
	//	_delay_ms(2);
	//    Delay(2);
   }
}



 ////////////////////////////////////////////////////////////////////
//����	  Lcd_init(uchar com)
//����    ��ȷ��Ļ��ʾ����
//����    line  Һ�����ĵڼ���  list  �ڼ���λ�ÿ�ʼд �����8���У�  DateҪд������

void Lcd_Display(uchar line,uchar list,uint Date)
{
		uchar i;
		 uchar ta[]={0,0,0,0,0,'\0'};	  //��һ��������װ��
	   list=list &0x07;			       //������ 0-7��
	   switch(line){
				case 1:   Lcd_WriteCom(0x80+list);break;	//��һ��
				case 2:   Lcd_WriteCom(0x90+list);break;	//�ڶ���
				case 3:   Lcd_WriteCom(0x88+list);break;	//������
				case 4:   Lcd_WriteCom(0x98+list);break;	//������
				default : break;
	
	   }
	 for(i=5;i>0;i--)
	 {							  //ѭ���Ĵΰ��ĸ����Ӹߵ���װ��ta
	 	ta[i-1]=Date%10+48;
		Date=Date/10;
	 }
	  Lcd_String(ta);
}

 ////////////////////////////////////////////////////////////////////
//����	  Lcd_init(uchar com)
//����    ��ȷ��Ļ��ʾ����
//����    line  Һ�����ĵڼ���  list  �ڼ���λ�ÿ�ʼд �����8���У�  DateҪд������

void Lcd_Display_u(uchar line,uchar list,unsigned long Date)
{
		uchar i;
		 uchar ta[]={0,0,0,0,0,'\0'};	  //��һ��������װ��
	   list=list &0x07;			       //������ 0-7��
	   switch(line){
				case 1:  // Lcd_WriteCom(0x80+list);Lcd_String("        ");
						  Lcd_WriteCom(0x80+list);break;	//��һ��
				case 2:  // Lcd_WriteCom(0x90+list);Lcd_String("        ");
						  Lcd_WriteCom(0x90+list);break;	//�ڶ���
				case 3:  // Lcd_WriteCom(0x88+list);Lcd_String("        ");
						  Lcd_WriteCom(0x88+list);break;	//������
				case 4:   //Lcd_WriteCom(0x98+list);Lcd_String("        ");
						  Lcd_WriteCom(0x98+list);break;	//������
				default : break;
	
	   }
	 for(i=5;i>0;i--)
	 {							  //ѭ���Ĵΰ��ĸ����Ӹߵ���װ��ta
	 	ta[i-1]=Date%10+48;
		Date=Date/10;
	 }

								  //ѭ���Ĵΰ��ĸ����Ӹߵ���װ��ta
	 if(ta[0]!=48){

			Lcd_WriteDate(ta[0]); 
			Lcd_WriteDate(ta[1]);
			Lcd_WriteDate(ta[2]);
			Lcd_WriteDate(ta[3]);
			Lcd_WriteDate(ta[4]);
			Lcd_WriteDate('u');
	        Lcd_WriteDate('H');
				
	}
	else{
		   if(ta[1]!=48){
			   Lcd_WriteDate(ta[1]); 
			   Lcd_WriteDate(ta[2]);
			   Lcd_WriteDate(ta[3]);
			   Lcd_WriteDate(ta[4]);
			   Lcd_WriteDate('u');
	           Lcd_WriteDate('H');
			   Lcd_WriteDate(' ');


		   }
		   else{
				   if(ta[2]!=48){
					   Lcd_WriteDate(ta[2]);
					   Lcd_WriteDate(ta[3]);
					   Lcd_WriteDate(ta[4]);
					   Lcd_WriteDate('u');
	 				   Lcd_WriteDate('H');
					   Lcd_WriteDate(' ');
					   Lcd_WriteDate(' ');
				   }
				   else{
						   	   if(ta[3]!=48){
							   		Lcd_WriteDate(ta[3]);
							   		Lcd_WriteDate(ta[4]);
									Lcd_WriteDate('u');
									Lcd_WriteDate('H');
									Lcd_WriteDate(' ');
									Lcd_WriteDate(' ');
									Lcd_WriteDate(' ');
						  		}
								else{
								
									 Lcd_WriteDate(ta[4]);
									 Lcd_WriteDate('u');
	 								 Lcd_WriteDate('H');
									 Lcd_WriteDate(' ');
									 Lcd_WriteDate(' ');
									 Lcd_WriteDate(' ');
									 Lcd_WriteDate(' ');
								}
				   
				   }		   
		   
		   
		   }
	
	}
	 	  
}


 ////////////////////////////////////////////////////////////////////
//����	  Lcd_init(uchar com)
//����    ��ȷ��Ļ��ʾ�ַ���
//����    line  Һ�����ĵڼ���  list  �ڼ���λ�ÿ�ʼд �����8���У�  *pҪд���ַ���
void  Lcd_ShowSring(uchar line,uchar list,uchar*p)
{
	 switch(line){
				case 1:   Lcd_WriteCom(0x80+list);break;	//��һ��
				case 2:   Lcd_WriteCom(0x90+list);break;	//�ڶ���
				case 3:   Lcd_WriteCom(0x88+list);break;	//������
				case 4:   Lcd_WriteCom(0x98+list);break;	//������
				default : break;
	
	   }

	    Lcd_String(p);

}

void  Value_to_ASCII1(uchar line,uchar list,unsigned long value)  //ŷķ��
{
    unsigned char temp[] = "00000000ŷ";

	temp[0] = value/10000000%10 + 0x30; 
    temp[1] = value/1000000%10 + 0x30;    
    temp[2] = value/100000%10 + 0x30;
    temp[3] = value/10000%10 + 0x30;
    temp[4] = value/1000%10 + 0x30;
    temp[5] = value/100%10 + 0x30;
    temp[6] = value/10%10 + 0x30;
    temp[7] = value%10 + 0x30;
	if(temp[0]==0x30)  {
		temp[0]=' ';
			if(temp[1]==0x30)  {
					temp[1]=' ';
					if(temp[2]==0x30)  {
							temp[2]=' ';
							if(temp[3]==0x30)  {
								temp[3]=' ';
								if(temp[4]==0x30)  {
									temp[4]=' ';
									if(temp[5]==0x30)  {
										temp[5]=' ';
										if(temp[6]==0x30)  {
											temp[6]=' ';	
										}											
									}										
								}									
							}															
	
					}	
			}	
	}

	Lcd_ShowSring( line,list,temp) ;
    
}

void  Value_to_ASCIIC(uchar line,uchar list,unsigned long value)
{
    unsigned char temp[] = "00000000nF";

	temp[0] = value/10000000%10 + 0x30; 
    temp[1] = value/1000000%10 + 0x30;    
    temp[2] = value/100000%10 + 0x30;
    temp[3] = value/10000%10 + 0x30;
    temp[4] = value/1000%10 + 0x30;
    temp[5] = value/100%10 + 0x30;
    temp[6] = value/10%10 + 0x30;
    temp[7] = value%10 + 0x30;
	if(temp[0]==0x30)  {
		temp[0]=' ';
			if(temp[1]==0x30)  {
					temp[1]=' ';
					if(temp[2]==0x30)  {
							temp[2]=' ';
							if(temp[3]==0x30)  {
								temp[3]=' ';
								if(temp[4]==0x30)  {
									temp[4]=' ';
									if(temp[5]==0x30)  {
										temp[5]=' ';
										if(temp[6]==0x30)  {
											temp[6]=' ';	
										}											
									}										
								}									
							}															
	
					}	
			}	
	}  
   Lcd_ShowSring( line, list,temp) ;
}



