//---�⺯����������ض���---//
#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int

sbit SDA=P3^1; //����
sbit SCL=P3^0; //ʱ��

#define     LCDIO      P0  //1602���ݿ�
sbit rs=P2^0;  //1602��������ѡ������
sbit rd=P2^1;        //��дѡ��
sbit lcden=P2^2;        //1602ѡͨ����

//uchar code table[]={" 2021-1-2 SUN"};
//uchar code table1[]={" 15:45:00       "};
uchar code table2[]= "SUNMONTUEWEDTHUFRISAT";

static uchar g8563_Store[8]="0"; /*ʱ�佻����,ȫ�ֱ�������*/
uchar code c8563_Store[8]={0x50,0x59,0x23,0x31,0x05,0x07,0x88,0x20}; /*д��ʱ���ֵ�����ͣ�08�� 7�� ����һ 13�� 07:59:00*/

sbit menu=P0^0;//�˵�
sbit add=P0^1;//��һ
sbit dec=P0^2;//��һ

uchar second,minute,hour,day,month,year,week,count=0;  

uchar flags = 1; //1�붨ʱ��־
static uchar cnt = 0;  //������¼�жϴ��������õ��ж϶�ʱΪ1ms���ж�200�μ�һ����
void delay1602(uint n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}
void write_com(uchar com)//1602д�����ӳ���
{
        rs=0;//RS����������ѡ��̣��ߵ�ƽд���ݣ��͵�ƽд����
		delay1602(1);
        lcden=1;//1602ѡͨ�ˣ��ߵ�ƽѡͨ���͵�ƽ��ֹ
        LCDIO=com;
        delay1602(1);
        lcden=0;        
}

void write_date(uchar date)//1602д�����ӳ���
{
        rs=1;        //RS����������ѡ��̣��ߵ�ƽд���ݣ��͵�ƽд����
			delay1602(1);
        rd=0;        //RD�Ƕ�дѡ��̣��ߵ�ƽ�����͵�ƽд
       lcden=0;        //1602ѡͨ�ˣ��ߵ�ƽѡͨ���͵�ƽ��ֹ	
       LCDIO=date;
   
        lcden=1;
        delay1602(1);
        lcden=0;        
}

void init()
{
//        uchar num;
        lcden=0;
				rd = 0;
        write_com(0x38);        //0011 1000B������ģʽ���ã�����Ϊ8Ϊ���ݿڣ�������ʾ��5*7����
        write_com(0x0c);        //0000 1011B����ʾ����������ã�����ʾ����ʾ��꣬�����˸
        write_com(0x06);        //0000 0110B����ʾ����ƶ����ã�����дһ���ַ�����ַָ���һ�ҹ���һ��дһ���ַ���Ļ��ʾ���ƶ�
        write_com(0x01);        //0000 0001B����ʾ����������ָ���������ʾ����
        write_com(0x80);        //1000 000B���ر���ʾ
        delay1602(5);
        write_com(0x80);        //1000 000B������Ϊ2����ʾ��д���һ���ַ��ĵ�ַ����һ�е�ַ��00-2F
//        for(num=0;num<15;num++)
//        {
//                write_date(table[num]);        //д���һ������
//                delay1602(5);
//        }
//        write_com(0x80+0x40);        //1100 0000B,����Ϊ2����ʾ��д��ڶ����ַ��ĵ�ַ���ڶ��е�ַ��40-67
//        for(num=0;num<15;num++)        //д��ڶ�������
//        {
//                write_date(table1[num]);//д��ڶ�������
//                delay1602(5);
//        }
}

/********************************************
			�ڲ���������ʱ1
********************************************/
/*���ݾ���Ƶ���ƶ���ʱʱ��*/
void Delay()
{
    _nop_();
   _nop_();     
	_nop_();
   _nop_(); 
	    _nop_();
   _nop_(); 
}
/********************************************
�ڲ�������I2C��ʼ
********************************************/
void Start()
{  //EA=0;

    SDA=1;
    _nop_();
    SCL=1;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    SDA=0;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    SCL=0;
}


/********************************************
�ڲ�������I2C����
********************************************/
void Stop()
{
   SDA=0;
   SCL=0;
   Delay();
   SCL=1;
   Delay();
   SDA=1;
   Delay();
  // EA=1;
}
/********************************************
�ڲ����������ACK ,ÿ���ֽڴ�����ɣ����ack=0,��������ݣ�ack=1;
********************************************/
void WriteACK(uchar ack)
{
   SDA=ack;
   Delay();
   SCL=1;
   Delay();
   SCL=0;
}
/********************************************
�ڲ��������ȴ�ACK
********************************************/
void WaitACK()
{  uchar errtime=20;
   SDA=1;
   Delay(); /*��ACK*/
   SCL=1;
   Delay();
   while(SDA)
   {  errtime--;
      if(!errtime) Stop();
   }
   SCL=0;
   Delay();
}
/********************************************
�ڲ�����.��������ֽ�
���:B=����
********************************************/
void writebyte(uchar wdata)
{
   uchar i;
   for(i=0;i<8;i++)
   {
       if(wdata&0x80) SDA=1;
       else SDA=0;
       wdata<<=1;
       SCL=1;
  Delay();
       SCL=0;
   }
 //  WaitACK();     //I2C������ͨѶ���������˳�I2CͨѶ
}
/********************************************
�ڲ�����.��������
����:B
********************************************/
uchar Readbyte()
{
   uchar i,bytedata;
   SDA=1;
   for(i=0;i<8;i++)
   {
      SCL=1;
      bytedata<<=1;
      bytedata|=SDA;
      SCL=0;
      Delay();
   }
   return(bytedata);
}
/********************************************
�������->pcf8563
********************************************/
void writeData(uchar address,uchar mdata)
{
   Start();
   writebyte(0xa2); /*д����*/
   writebyte(address); /*д��ַ*/
   writebyte(mdata); /*д����*/
   Stop();
}
/********************************************
��������<-pcf8563
********************************************/
uchar ReadData(uchar address) /*���ֽ�*/
{  uchar rdata;
   Start();
   writebyte(0xa2); /*д����*/
   writebyte(address); /*д��ַ*/
   Start();
   writebyte(0xa3); /*������*/
   rdata=Readbyte();
   WriteACK(1);
   Stop();
   return(rdata);
}
void ReadData1(uchar address,uchar count,uchar * buff) /*���ֽ�*/
{  uchar i;
   Start();
   writebyte(0xa2); /*д����*/
	WriteACK(0);
   writebyte(address); /*д��ַ*/
	WriteACK(0);
   Start();
   writebyte(0xa3); /*������*/
	WriteACK(0);
   for(i=0;i<count;i++)
   {
        buff[i]=Readbyte();
        if(i<count-1) WriteACK(0);
   }
  WriteACK(1);
   Stop();
}
/********************************************
�ڲ�����,����ʱ�䵽�ڲ�������
********************************************/
void P8563_Read()
{   uchar time[7],century;
        ReadData1(0x02,0x07,time);
        century=time[5];
        g8563_Store[0]=time[0]&0x7f; /*��*/
        g8563_Store[1]=time[1]&0x7f; /*��*/
        g8563_Store[2]=time[2]&0x3f; /*Сʱ*/
	
        g8563_Store[3]=time[3]&0x3f; /*��*/
        g8563_Store[4]=time[4]&0x07; /*����*/
        g8563_Store[5]=time[5]&0x1f; /*��*/
        g8563_Store[6]=time[6]&0xff; /*��*/
       g8563_Store[7]=century&0x80; /*ȥ����,00Ϊ2000��80Ϊ1900*/
        if(g8563_Store[7]==0x00)g8563_Store[7]=20;
        if(g8563_Store[7]==0x80)g8563_Store[7]=19;
}
/********************************************
����ʱ�䵽�ڲ�������----�ⲿ����
********************************************/
void P8563_gettime()
{
    P8563_Read();
    if(g8563_Store[0]==0)
      P8563_Read(); /*���Ϊ��=0��Ϊ��ֹʱ��仯���ٶ�һ��*/
}
/********************************************
дʱ���޸�ֵ
********************************************/
void P8563_settime()
{      
        writeData(2,g8563_Store[0]);//��
        writeData(3,g8563_Store[1]);//��
        writeData(4,g8563_Store[2]);//ʱ
        writeData(5,g8563_Store[3]);//��
        writeData(6,g8563_Store[4]);//����
        if(g8563_Store[7]==20)g8563_Store[5]=(g8563_Store[5]&0x3F);//�ж�����
        if(g8563_Store[7]==0x19)g8563_Store[5]=(g8563_Store[5]|0x80);//�ж�����
        writeData(7,g8563_Store[1]);//��
        writeData(8,g8563_Store[0]);//��
}

/********************************************
					P8563�ĳ�ʼ��
********************************************/
void P8563_init()
{
    uchar i;
    if((ReadData(0xa)&0x3f)!=0x8) /*����Ƿ��һ�������������ʼ��ʱ��*/
    {
        for(i=0;i<8;i++) g8563_Store[i]=c8563_Store[i]; /*��ʼ��ʱ��*/
                  P8563_settime();
        writeData(0x0,0x00);
        writeData(0xa,0x8); /*8:00����*/
        writeData(0x1,0x12); /*������Ч*/
        writeData(0xd,0xf0);
    }
}


void DisplaySecond(uchar x)        //д��1602��
{
        uchar i,j;   
        i=x/10;
        j=x%10;
        write_com(0xca);   //д1602�������֣�����������ݵ�ַΪ47H
        write_date(0x30+i); //д����ĵ�λ  
        write_date(0x30+j); //д����ĸ�λ      
}

void DisplayMinute(uchar x)         //д��1602��
{
        uchar i,j;   
        i=x/10;
        j=x%10;
        write_com(0xc7);   
        write_date(0x30+i);  
        write_date(0x30+j);
				write_date(':'); //���棬��Ӽ������":"		
}

void DisplayHour(uchar x)         //д��1602Сʱ
{
        uchar i,j;   
        i=x/10;
        j=x%10;
        write_com(0xc4);   
        write_date(0x30+i);  
        write_date(0x30+j); 
				write_date(':'); //���棬��Ӽ������":"	
}

void DisplayDay(uchar x)         //д��1602,��
{
        uchar i,j;   
        i=x/10;
        j=x%10;   
        write_com(0x89);   
        write_date(0x30+i);  
        write_date(0x30+j); 
		
}

void DisplayMonth(uchar x)         //д��1602,��
{
        uchar i,j;     
        i=x/10;
        j=x%10;  
        write_com(0x86);   
        write_date(0x30+i);  
        write_date(0x30+j); 
				write_date('-'); //���棬��Ӽ������"-"	
}

void DisplayYear(uchar x)         //д��1602��
{
        uchar i,j;  
        i=x/10;
        j=x%10;
        write_com(0x83);   
        write_date(0x30+i);
        write_date(0x30+j); 
					write_date('-'); //���棬����Ӽ������"-"	 	
}   

void DisplayWeek(uchar x)        //д������
{
        uchar i;
        x=(x-1)*3;//��Ҫ��һ����
        write_com(0x8d);   
        for(i=0;i<3;i++)
        {
                write_date(table2[x]);
                x++;  
        }
}  


void DisplayCentury(uchar x)         //д������
{
        uchar i,j;  
        i=x/10;
        j=x%10;
        write_com(0x81);   
        write_date(0x30+i);
        write_date(0x30+j);  
}   

//---BCDת��16����---//
void BCD_16( )
{
uchar i;//ucData1,ucData2;
        for(i=0;i<7;i++)
        {
//                ucData1=g8563_Store[i]/16; //BCD��תʮ������
//                ucData2=g8563_Store[i]%16;
//                g8563_Store[i]=ucData1*10+ucData2;

//					ucData1=g8563_Store[i]&0x0f;
//					ucData2=(g8563_Store[i]&0xf0)>>4;
//					g8563_Store[i] =	ucData2*10+ucData1;
					g8563_Store[i] = (g8563_Store[i] /16*10) + (g8563_Store[i]%16);
                }
}

void display(void)
{		DisplayCentury(g8563_Store[7]);//0x81
	 DisplayYear(g8563_Store[6]); 
	 DisplayMonth(g8563_Store[5]); 
	DisplayDay(g8563_Store[3]);  
	DisplayWeek(g8563_Store[4]);
       
        DisplayHour(g8563_Store[2]);                                       
        DisplayMinute(g8563_Store[1]);           
           DisplaySecond(g8563_Store[0]);         
              
        
        
}

void turn_val(uchar newval,uchar flag,uchar  newaddr,uchar s1num)        
//newval��ʱ������ֵ��flag��ʱ������������ʶ��newaddr�Ƕ�ȡ���ݵĵ�ַ��s1num��������ʱ�������ڵ����ֱ�ʶ
{
uchar ucData1,ucData2;
        newval=ReadData(newaddr);
//        century=newval��
        switch(newaddr)
                {
                case 0x02:newval=(newval&0x7f); /*��*/
                break;
                case 0x03:newval=(newval&0x7f); /*��*/
                break;
                case 0x04:newval=(newval&0x3f);/*Сʱ*/
                break;
                case 0x05:newval=(newval&0x3f); /*��*/
                break;
                case 0x06:newval=(newval&0x07); /*����*/
                break;
                case 0x07:newval=(newval&0x1f);/*��*/
                break;
                case 0x08:newval=(newval&0xff);/*��*/
                }

                ucData1=newval/16; //BCD��תʮ������
                ucData2=newval%16;
                newval=ucData1*10+ucData2;

        if(flag)    //�ж��Ǽ�һ���Ǽ�һ
        {
                newval++;
                switch(s1num)
                {
                        case 1: if(newval>99) newval=0;
                        DisplayYear(newval);      
                        break;
                        case 2: if(newval>12) newval=1;
                        DisplayMonth(newval);
                        break;
                        case 3: if(newval>31) newval=1;
                        DisplayDay(newval);  
                        break;
                        case 4: if(newval>6) newval=0;
                        DisplayWeek(newval);
                        break;
                        case 5: if(newval>23) newval=0;
                        DisplayHour(newval);
                        break;
                        case 6: if(newval>59) newval=0;
                        DisplayMinute(newval);
                        break;
                        case 7: if(newval>59) newval=0;
                        DisplaySecond(newval);
                        break;
                        default:break;
                }
        }   
        else
        {
                newval--;
                switch(s1num)
                {
                        case 1: if(newval==0XFF) newval=99;
                        DisplayYear(newval);
                        break;
                        case 2: if(newval==0) newval=12;
                        DisplayMonth(newval);
                        break;
                        case 3: if(newval==0) newval=31;
                        DisplayDay(newval);
                        break;
                        case 4: if(newval==0xff) newval=6;
                        DisplayWeek(newval);
                        break;
                        case 5: if(newval==0xff) newval=23;
                        DisplayHour(newval);
                        break;
                        case 6: if(newval==0xff) newval=59;
                        DisplayMinute(newval);
                        break;
                        case 7: if(newval==0xff) newval=59;
                        DisplaySecond(newval);
                        break;
                        default:break;
                }
        }

                ucData1=newval/10; //BCD��תʮ������
                ucData2=newval%10;
                newval=ucData1*16+ucData2;
								writeData(newaddr,newval);
}   
void delay(uint z)
{
        uint x,y;
        for(x=z;x>0;x--)
        for(y=110;y>0;y--);
}
//����ɨ�����
void key_scan(void)
{
        uchar s1num=0;                        //s1num��¼����λ�õı��
        if(menu==0)
        {
					EA=0;
                delay(5);
                if(menu==0)
                {
                        while(!menu);          //menum���Ե���0��1�ı仯��Ҳ���ǰ����ɿ��ı仯��s1num��1
                        s1num++;
                        while(1)
                        {
                                if(menu==0)
                                {
                                delay(5);
                                if(menu==0)
                                {
                                while(!menu);
                                s1num++;        //menum���Ե���0��1�ı仯��Ҳ���ǰ����ɿ��ı仯��s1num�ټ�1
                                }
                                }
        writeData(0x0,0x20);//ֹͣ��ʱ����ʼ������á�
         write_com(0x0f);//������䣬0X0F��1602�������֣���ʾ������꿪�������˸                       
                                if(s1num==1)
                                {
                                        year=ReadData(0x08);        //��ȡ������ݡ�
																	//year=g8563_Store[6]++;															
                                        write_com(0x80+4);        //���꣬0X80+��ַ�룬�������ݵ�ַָ��
                                        if(add==0)
                                        {
                                                delay(3);
                                                if(add==0)
                                                {
                                                while(!add);
                                                turn_val(year,1,0x08,1);               
                                                }
                                        }
                                        if(dec==0)
                                        {
                                                delay(3);
                                                if(dec==0)
                                                {
                                                while(!dec);
                                                turn_val(year,0,0x08,1);//��ȡ��ǰ��ֵ�����ݼ�С��ֵ����д��1302                              
                                                }
                                        }
                                }
                        
                                if(s1num==2)
                                {
                                        month=ReadData(0x07);
                                        write_com(0x80+7);        //�¹��
                                        if(add==0)
                                        {
                                                delay(3);
                                                if(add==0)
                                                {
                                                        while(!add);
                                                        turn_val(month,1,0x07,2);
                                                }
                                        }
                                        if(dec==0)
                                        {
                                                delay(3);
                                                if(dec==0)
                                                {
                                                        while(!dec);
                                                        turn_val(month,0,0x07,2);
                                                }
                                        }
                                }
                        
                                if(s1num==3)
                                {
                                        day=ReadData(0x05);
                                        write_com(0x80+10);//�չ��
                                        if(add==0)
                                        {
                                                delay(3);
                                                if(add==0)
                                                {
                                                        while(!add);
                                                        turn_val(day,1,0x05,3);
                                                }
                                        }
                                        if(dec==0)
                                        {
                                                delay(3);
                                                if(dec==0)
                                                {
                                                        while(!dec);
                                                        turn_val(day,0,0x05,3);        //д���ռĴ���
                                                }
                                        }
                                }
                                
                                if(s1num==4)
                                {
                                        week=ReadData(0x06);
                                        write_com(0x80+14);        //���ڹ��
                                        if(add==0)
                                        {
                                                delay(3);
                                                if(add==0)
                                                {
                                                        while(!add);
                                                        turn_val(week,1,0x06,4);
                                                }
                                        }
                                        if(dec==0)
                                        {
                                                delay(3);
                                                if(dec==0)
                                                {
                                                        while(!dec);
                                                        turn_val(week,0,0x06,4);
                                                }
                                        }
                                }
                                
                                if(s1num==5)
                                {
                                        hour=ReadData(0x04);
                                        write_com(0xc0+5);        //ʱ��꣬��Ϊ�ǵڶ��еĵ�ַ��0X40��ʼ�����Լ�0X40��
                                        if(add==0)
                                        {
                                                delay(3);
                                                if(add==0)
                                                {
                                                        while(!add);
                                                        turn_val(hour,1,0x04,5);
                                                }
                                        }
                                        if(dec==0)
                                        {
                                                delay(3);
                                                if(dec==0)
                                                {
                                                        while(!dec);
                                                        turn_val(hour,0,0x04,5);
                                                }
                                        }
                                }
                                
                                if(s1num==6)//��ʱ���
                                {
                                        minute=ReadData(0x03);
                                        write_com(0xc0+8);
                                        if(add==0)
                                        {
                                                delay(5);
                                                if(add==0)
                                                {
                                                        while(!add);
                                                        turn_val(minute,1,0x03,6);                                //д��ּĴ���   
                                                }
                                        }
                                        if(dec==0)
                                        {
                                                delay(3);
                                                if(dec==0)
                                                {
                                                        while(!dec);
                                                        turn_val(minute,0,0x03,6);                                //д��ּĴ���
                                                }
                                        }
                                }
                               
                                if(s1num==7)//��ʱ����
                                {
                                        second=ReadData(0x02);
                                        write_com(0xc0+11);//����
                                        if(add==0)
                                        {
                                                delay(3);
                                                if(add==0)
                                                {
                                                while(!add);
                                                if(second==0x60)
                                                second=0x00;
                                                turn_val(second,1,0x02,7);        
                                                }
                                        }
                                        if(dec==0)
                                        {
                                                delay(3);
                                                if(dec==0)
                                                {
                                                        while(!dec);
                                                        turn_val(second,0,0x02,7);                                
                                                }
                                        }
                                }
                                
                                if(s1num==8)
                                {
          writeData(0x0,0x00);//���ý�������ʼ��ʱ
                                s1num=0;//s1num����//
                                        write_com(0x0c);//��겻��˸//
                                        break;
                                }
                        }
                }
        }
				EA=1;
}

void LCD1602_write_word(unsigned char *s)
{
	while(*s>0)
	{
		write_date(*s);
		s++;
	}
}
void TimeInit()
{
    EA = 1;                       //���ж��ܿ���
    ET0 = 1;                      //��T0�жϿ���
    TMOD = 0x01;    //���ö�ʱ��������ģʽ1
		TH0=(65536-5000)/256;  //����λ�Ĵ�����ֵ��//5000us=5ms
		TL0=(65536-5000)%256;  //����λ�Ĵ�����ֵ��
    TR0 = 1;                      //����ʱ��T0����
}

//---������---//
void main()
{
	TimeInit();
		P8563_init();        
		init(); 
	delay1602(1000);
        while(1)
        {
					if(flags){
			P8563_gettime();
       BCD_16();		
			flags=0;	
					}
		display();
//					write_com(0x80);
//					LCD1602_write_word(table);
//										write_com(0x80+40);
//					LCD1602_write_word(table1);
					
			 key_scan();
				}
				}

void InterruptTimer0() interrupt 1
{

		TH0=(65536-5000)/256;  //����λ�Ĵ�����ֵ��//5000us=5ms
		TL0=(65536-5000)%256;  //����λ�Ĵ�����ֵ��
    cnt++;                                     //�жϴ�������ֵ��1
    if(cnt>=200)                                 //�жϴ����ﵽ200�μ�Ϊ1��
    {
        cnt = 0;                               //���㣬���¼�¼�жϴ���
        flags = 1;                             //����1�붨ʱ��־λ1
    }
	}