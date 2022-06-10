//---库函数声明及相关定义---//
#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int

sbit SDA=P3^1; //数据
sbit SCL=P3^0; //时钟

#define     LCDIO      P0  //1602数据口
sbit rs=P2^0;  //1602数据命令选择引脚
sbit rd=P2^1;        //读写选择
sbit lcden=P2^2;        //1602选通引脚

//uchar code table[]={" 2021-1-2 SUN"};
//uchar code table1[]={" 15:45:00       "};
uchar code table2[]= "SUNMONTUEWEDTHUFRISAT";

static uchar g8563_Store[8]="0"; /*时间交换区,全局变量声明*/
uchar code c8563_Store[8]={0x50,0x59,0x23,0x31,0x05,0x07,0x88,0x20}; /*写入时间初值：世纪，08年 7月 星期一 13日 07:59:00*/

sbit menu=P0^0;//菜单
sbit add=P0^1;//加一
sbit dec=P0^2;//减一

uchar second,minute,hour,day,month,year,week,count=0;  

uchar flags = 1; //1秒定时标志
static uchar cnt = 0;  //用来记录中断次数，设置的中断定时为1ms，中断200次即一秒钟
void delay1602(uint n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}
void write_com(uchar com)//1602写命令子程序
{
        rs=0;//RS是数据命令选择短，高电平写数据，低电平写命令
		delay1602(1);
        lcden=1;//1602选通端，高电平选通，低电平禁止
        LCDIO=com;
        delay1602(1);
        lcden=0;        
}

void write_date(uchar date)//1602写数据子程序
{
        rs=1;        //RS是数据命令选择短，高电平写数据，低电平写命令
			delay1602(1);
        rd=0;        //RD是读写选择短，高电平读，低电平写
       lcden=0;        //1602选通端，高电平选通，低电平禁止	
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
        write_com(0x38);        //0011 1000B，功能模式设置，设置为8为数据口，两行显示，5*7点阵
        write_com(0x0c);        //0000 1011B，显示开及光标设置，关显示，显示光标，光标闪烁
        write_com(0x06);        //0000 0110B，显示光标移动设置，读或写一个字符，地址指针减一且光标减一，写一个字符屏幕显示不移动
        write_com(0x01);        //0000 0001B，显示清屏，数据指针和所有显示清屏
        write_com(0x80);        //1000 000B，关闭显示
        delay1602(5);
        write_com(0x80);        //1000 000B，设置为2行显示，写入第一行字符的地址，第一行地址是00-2F
//        for(num=0;num<15;num++)
//        {
//                write_date(table[num]);        //写入第一行数据
//                delay1602(5);
//        }
//        write_com(0x80+0x40);        //1100 0000B,设置为2行显示，写入第二行字符的地址，第二行地址是40-67
//        for(num=0;num<15;num++)        //写入第二行数据
//        {
//                write_date(table1[num]);//写入第二行数据
//                delay1602(5);
//        }
}

/********************************************
			内部函数，延时1
********************************************/
/*根据晶振频率制定延时时间*/
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
内部函数，I2C开始
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
内部函数，I2C结束
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
内部函数，输出ACK ,每个字节传输完成，输出ack=0,结束读书据，ack=1;
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
内部函数，等待ACK
********************************************/
void WaitACK()
{  uchar errtime=20;
   SDA=1;
   Delay(); /*读ACK*/
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
内部函数.输出数据字节
入口:B=数据
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
 //  WaitACK();     //I2C器件或通讯出错，将会退出I2C通讯
}
/********************************************
内部函数.输入数据
出口:B
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
输出数据->pcf8563
********************************************/
void writeData(uchar address,uchar mdata)
{
   Start();
   writebyte(0xa2); /*写命令*/
   writebyte(address); /*写地址*/
   writebyte(mdata); /*写数据*/
   Stop();
}
/********************************************
输入数据<-pcf8563
********************************************/
uchar ReadData(uchar address) /*单字节*/
{  uchar rdata;
   Start();
   writebyte(0xa2); /*写命令*/
   writebyte(address); /*写地址*/
   Start();
   writebyte(0xa3); /*读命令*/
   rdata=Readbyte();
   WriteACK(1);
   Stop();
   return(rdata);
}
void ReadData1(uchar address,uchar count,uchar * buff) /*多字节*/
{  uchar i;
   Start();
   writebyte(0xa2); /*写命令*/
	WriteACK(0);
   writebyte(address); /*写地址*/
	WriteACK(0);
   Start();
   writebyte(0xa3); /*读命令*/
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
内部函数,读入时间到内部缓冲区
********************************************/
void P8563_Read()
{   uchar time[7],century;
        ReadData1(0x02,0x07,time);
        century=time[5];
        g8563_Store[0]=time[0]&0x7f; /*秒*/
        g8563_Store[1]=time[1]&0x7f; /*分*/
        g8563_Store[2]=time[2]&0x3f; /*小时*/
	
        g8563_Store[3]=time[3]&0x3f; /*日*/
        g8563_Store[4]=time[4]&0x07; /*星期*/
        g8563_Store[5]=time[5]&0x1f; /*月*/
        g8563_Store[6]=time[6]&0xff; /*年*/
       g8563_Store[7]=century&0x80; /*去世纪,00为2000，80为1900*/
        if(g8563_Store[7]==0x00)g8563_Store[7]=20;
        if(g8563_Store[7]==0x80)g8563_Store[7]=19;
}
/********************************************
读入时间到内部缓冲区----外部调用
********************************************/
void P8563_gettime()
{
    P8563_Read();
    if(g8563_Store[0]==0)
      P8563_Read(); /*如果为秒=0，为防止时间变化，再读一次*/
}
/********************************************
写时间修改值
********************************************/
void P8563_settime()
{      
        writeData(2,g8563_Store[0]);//秒
        writeData(3,g8563_Store[1]);//分
        writeData(4,g8563_Store[2]);//时
        writeData(5,g8563_Store[3]);//日
        writeData(6,g8563_Store[4]);//星期
        if(g8563_Store[7]==20)g8563_Store[5]=(g8563_Store[5]&0x3F);//判断世纪
        if(g8563_Store[7]==0x19)g8563_Store[5]=(g8563_Store[5]|0x80);//判断世纪
        writeData(7,g8563_Store[1]);//月
        writeData(8,g8563_Store[0]);//年
}

/********************************************
					P8563的初始化
********************************************/
void P8563_init()
{
    uchar i;
    if((ReadData(0xa)&0x3f)!=0x8) /*检查是否第一次启动，是则初始化时间*/
    {
        for(i=0;i<8;i++) g8563_Store[i]=c8563_Store[i]; /*初始化时间*/
                  P8563_settime();
        writeData(0x0,0x00);
        writeData(0xa,0x8); /*8:00报警*/
        writeData(0x1,0x12); /*报警有效*/
        writeData(0xd,0xf0);
    }
}


void DisplaySecond(uchar x)        //写入1602秒
{
        uchar i,j;   
        i=x/10;
        j=x%10;
        write_com(0xca);   //写1602的命令字，设置秒的数据地址为47H
        write_date(0x30+i); //写入秒的低位  
        write_date(0x30+j); //写入秒的高位      
}

void DisplayMinute(uchar x)         //写入1602分
{
        uchar i,j;   
        i=x/10;
        j=x%10;
        write_com(0xc7);   
        write_date(0x30+i);  
        write_date(0x30+j);
				write_date(':'); //后面，添加间隔符号":"		
}

void DisplayHour(uchar x)         //写入1602小时
{
        uchar i,j;   
        i=x/10;
        j=x%10;
        write_com(0xc4);   
        write_date(0x30+i);  
        write_date(0x30+j); 
				write_date(':'); //后面，添加间隔符号":"	
}

void DisplayDay(uchar x)         //写入1602,天
{
        uchar i,j;   
        i=x/10;
        j=x%10;   
        write_com(0x89);   
        write_date(0x30+i);  
        write_date(0x30+j); 
		
}

void DisplayMonth(uchar x)         //写入1602,月
{
        uchar i,j;     
        i=x/10;
        j=x%10;  
        write_com(0x86);   
        write_date(0x30+i);  
        write_date(0x30+j); 
				write_date('-'); //后面，添加间隔符号"-"	
}

void DisplayYear(uchar x)         //写入1602年
{
        uchar i,j;  
        i=x/10;
        j=x%10;
        write_com(0x83);   
        write_date(0x30+i);
        write_date(0x30+j); 
					write_date('-'); //后面，免添加间隔符号"-"	 	
}   

void DisplayWeek(uchar x)        //写入星期
{
        uchar i;
        x=(x-1)*3;//需要减一处理
        write_com(0x8d);   
        for(i=0;i<3;i++)
        {
                write_date(table2[x]);
                x++;  
        }
}  


void DisplayCentury(uchar x)         //写入世纪
{
        uchar i,j;  
        i=x/10;
        j=x%10;
        write_com(0x81);   
        write_date(0x30+i);
        write_date(0x30+j);  
}   

//---BCD转换16函数---//
void BCD_16( )
{
uchar i;//ucData1,ucData2;
        for(i=0;i<7;i++)
        {
//                ucData1=g8563_Store[i]/16; //BCD码转十六进制
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
//newval是时间日期值，flag是时间日期增减标识，newaddr是读取数据的地址，s1num是年月日时分秒星期的数字标识
{
uchar ucData1,ucData2;
        newval=ReadData(newaddr);
//        century=newval；
        switch(newaddr)
                {
                case 0x02:newval=(newval&0x7f); /*秒*/
                break;
                case 0x03:newval=(newval&0x7f); /*秒*/
                break;
                case 0x04:newval=(newval&0x3f);/*小时*/
                break;
                case 0x05:newval=(newval&0x3f); /*日*/
                break;
                case 0x06:newval=(newval&0x07); /*星期*/
                break;
                case 0x07:newval=(newval&0x1f);/*月*/
                break;
                case 0x08:newval=(newval&0xff);/*年*/
                }

                ucData1=newval/16; //BCD码转十六进制
                ucData2=newval%16;
                newval=ucData1*10+ucData2;

        if(flag)    //判断是加一还是减一
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

                ucData1=newval/10; //BCD码转十六进制
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
//键盘扫描程序
void key_scan(void)
{
        uchar s1num=0;                        //s1num记录光标的位置的编号
        if(menu==0)
        {
					EA=0;
                delay(5);
                if(menu==0)
                {
                        while(!menu);          //menum测试到由0到1的变化，也就是按下松开的变化，s1num加1
                        s1num++;
                        while(1)
                        {
                                if(menu==0)
                                {
                                delay(5);
                                if(menu==0)
                                {
                                while(!menu);
                                s1num++;        //menum测试到由0到1的变化，也就是按下松开的变化，s1num再加1
                                }
                                }
        writeData(0x0,0x20);//停止计时，开始相关设置。
         write_com(0x0f);//光标闪射，0X0F是1602的命令字，显示开，光标开，光标闪烁                       
                                if(s1num==1)
                                {
                                        year=ReadData(0x08);        //读取年的数据。
																	//year=g8563_Store[6]++;															
                                        write_com(0x80+4);        //年光标，0X80+地址码，设置数据地址指针
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
                                                turn_val(year,0,0x08,1);//读取当前年值，根据减小年值，并写入1302                              
                                                }
                                        }
                                }
                        
                                if(s1num==2)
                                {
                                        month=ReadData(0x07);
                                        write_com(0x80+7);        //月光标
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
                                        write_com(0x80+10);//日光标
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
                                                        turn_val(day,0,0x05,3);        //写入日寄存器
                                                }
                                        }
                                }
                                
                                if(s1num==4)
                                {
                                        week=ReadData(0x06);
                                        write_com(0x80+14);        //星期光标
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
                                        write_com(0xc0+5);        //时光标，因为是第二行的地址从0X40开始，所以加0X40。
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
                                
                                if(s1num==6)//调时间分
                                {
                                        minute=ReadData(0x03);
                                        write_com(0xc0+8);
                                        if(add==0)
                                        {
                                                delay(5);
                                                if(add==0)
                                                {
                                                        while(!add);
                                                        turn_val(minute,1,0x03,6);                                //写入分寄存器   
                                                }
                                        }
                                        if(dec==0)
                                        {
                                                delay(3);
                                                if(dec==0)
                                                {
                                                        while(!dec);
                                                        turn_val(minute,0,0x03,6);                                //写入分寄存器
                                                }
                                        }
                                }
                               
                                if(s1num==7)//调时间秒
                                {
                                        second=ReadData(0x02);
                                        write_com(0xc0+11);//秒光标
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
          writeData(0x0,0x00);//设置结束，开始计时
                                s1num=0;//s1num清零//
                                        write_com(0x0c);//光标不闪烁//
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
    EA = 1;                       //打开中断总开关
    ET0 = 1;                      //打开T0中断开关
    TMOD = 0x01;    //设置定时器工作在模式1
		TH0=(65536-5000)/256;  //高四位寄存器的值；//5000us=5ms
		TL0=(65536-5000)%256;  //低四位寄存器的值；
    TR0 = 1;                      //允许定时器T0工作
}

//---主函数---//
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

		TH0=(65536-5000)/256;  //高四位寄存器的值；//5000us=5ms
		TL0=(65536-5000)%256;  //低四位寄存器的值；
    cnt++;                                     //中断次数计数值加1
    if(cnt>=200)                                 //中断次数达到200次即为1秒
    {
        cnt = 0;                               //清零，重新记录中断次数
        flags = 1;                             //设置1秒定时标志位1
    }
	}