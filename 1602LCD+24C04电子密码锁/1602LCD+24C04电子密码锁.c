#include<reg51.h>
#include<string.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define     LCDIO      P0
#define delay4us() _nop_();_nop_();_nop_();_nop_();
uchar buffer[6]={0}; 
sbit sda=P3^3;
sbit scl=P3^2;
sbit beep=P3^7;
bit  flag=0,aa;        //用户蹲渊义定时溢出标志位
uchar  DSY_BUFFER[16]="                ";
uchar  DSY_BUFFER1[16]="                ";
uchar  Userpassword[6]={0};
sbit  rs=P2^0;  
sbit  rd=P2^1;
sbit  lcden=P2^2;
sbit  led=P2^7;
uchar code table2[]="123456";
uchar code table[]="Your Password...";
void delayms(uint z)
{
  uint x,y;
  for(x=z;x>0;x--)
    for(y=110;y>0;y--);
}
void delay()      //短延时，两个机器周期,做总线的延时用
{;;}
void write_com(uchar com)
{
  rs =0;
  rd =0;
  lcden = 0;
  P0 = com;
  delayms(3);
  lcden=1;
  delayms(3);
  lcden=0;  
}

void write_date(uchar date)
{
  rs=1;
  rd=0;
  lcden=0;
  P0=date;
  delayms(3);
  lcden=1;
  delayms(3);
  lcden=0;  
}
void Display_String(uchar *p,uchar com)
{  uchar i;
   write_com(com);
   for(i=0;i<16;i++)
   {
   write_date(p[i]);
  }
}
void init_lcd()
{
  lcden=0;
  write_com(0x38);
  write_com(0x0c);
  write_com(0x06);
  write_com(0x01);
  write_com(0x80);
    Display_String(table,0x80);
  Display_String("Lock OK!         ",0xc0);
}

void start()
{
  sda=1;
  scl=1;
  delay4us();
  sda=0;
  delay4us();
  scl=0;
}
void stop()
{
  sda=0;
  scl=1;
  delay4us();
  sda=1;
  delay4us();
  scl=0;
}
void init()              //初始化
{
sda=1;
delay();
scl=1;
delay();
}


void ack()
{
  sda=0;
  scl=1;
  delay4us();
  scl=0;
  sda=1;
}
void noack()
{
  sda=1;
  scl=1;
  delay4us();
  scl=0;
  sda=0;
}
uchar recbyte()
{
  uchar i,rd;
  rd=0x00;
  sda=1;
  for(i=0;i<8;i++)
  {
    scl=1;
    rd<<=1;
    rd|=sda;
    delay4us();
    scl=0;
    delay4us();
  }
  scl=0;
  delay4us();
  return rd;
}
uchar sendbyte(uchar wd)
{
  uchar i;
  bit ack0;
  for(i=0;i<8;i++)
  {
    sda=(bit)(wd&0x80);
    _nop_();
    _nop_();
    scl=1;
    delay4us();
    scl=0;
    wd<<=1;
  }
  delay4us();
  sda=1;
  scl=1;
  delay4us();
  ack0=!sda;
  scl=0;
  delay4us();
  return ack0;
}
uchar Recstring(uchar slave,uchar subaddr,uchar *buffer,uchar n)
{
 uchar i; 
 start();
 if(!sendbyte(slave)) return 0;
 if(!sendbyte(subaddr)) return 0;
 start();
 if(!sendbyte(slave+1)) return 0;
 for(i=0;i<n-1;i++)
  {
    buffer[i]=recbyte();
  ack();
  }
 buffer[n-1]=recbyte();
 noack();
 stop();
 return 1;
}

uchar Sendstring(uchar slave,uchar subaddr,uchar *buffer,uchar n)
{
  uchar i;
  start();
  if(!sendbyte(slave)) return 0;
  if(!sendbyte(subaddr)) return 0;
  for(i=0;i<n;i++)
  {
    if(!sendbyte(buffer[i])) return 0;
  }
  stop();
  return 1;
}
void clear_password( )
{  uchar i;
  for(i=0;i<6;i++)
  {
   Userpassword[i]=' ';
  }
  for(i=0;i<16;i++)
  {
     DSY_BUFFER[i]=' ';   
   }
}
uchar Keys_Scan()
{  
  uchar temp,keynum;
  P1=0x0F;
  delayms(5);
  temp=P1^0x0F;
  switch(temp)
  {
    case 1:keynum=0;break;
    case 2:keynum=1;break;
    case 4:keynum=2;break;
    case 8:keynum=3;break;
    break;
  }
  P1=0xF0;
  delayms(5);
  temp=P1>>4^0x0F;
  switch(temp)
  {
    case 1:keynum+=0;break;
    case 2:keynum+=4;break;
    case 4:keynum+=8;break;
    case 8:keynum+=12;break;
    break;
  }
  delayms(600);
   return keynum;
}
void main()
{   uchar temp,i=0,j=0,k=0,n;
  uchar IS_valid_user; 
  beep=1;
  init();      
    init_lcd();
  delayms(5);
  aa=Sendstring(0xa0,1,table2,6);
  delayms(5);
  aa=Recstring(0xa0,1,buffer,6);
  delayms(10);
   P1=0x0f;     
    while(1)
    { 
     
      if(P1!=0x0f)
      {  
          temp=Keys_Scan();
          switch(temp)
                 {
                       case 0:  case 1: case 2: case 3: case 4: 
                       case 5:  case 6: case 7: case 8: case 9:
                        
                       if (i<=5)             //密码限制在6位以内
                       {
                        Userpassword[i]=temp;
                        DSY_BUFFER[i]='*';
                        Display_String(DSY_BUFFER,0xc0);    
            i++;
                         }                
                        break;
                
                        case 10: //按A键开锁
            for(k=0;k<6;k++)
            {
              if(buffer[k]==(Userpassword[k]+48))
              flag=1;
              else
              flag=0;
            }
                        if (flag==1) 
                        { flag=0;
              i=0;
                          led=0;  //点亮LED
                           clear_password();
                           Display_String("OPEN    OK!      ",0xc0);
                           IS_valid_user = 1;
               j=0;
                        }
                       else 
                        { 
             j++;
                         led=1;   //关闭LED
                         clear_password();
                         Display_String("ERROR!Have try   ",0xc0);
             write_com(0xcf);
             write_date(0x30+j);
                         IS_valid_user=0;
                         }
                        i=0;
                        break;
                    
                      case 11: //按B键上锁
                      led=1;   
                      clear_password();
                      Display_String(table,0x80);
                      Display_String("Lock OK!         ",0xc0);
                      i=0;                         
                      IS_valid_user=0;               
                      break;
                                   
                      case 12: //按C键设置新密码
                       //如果是合法用户则提示输入新密码    
                      if ( !IS_valid_user) 
            {
             i=0;
             Display_String("No rights !      ",0xc0);
             delayms(1000);
             Display_String("Your Password...",0x80);
             Display_String("Lock OK!         ",0xc0);
             }
                       else 
                        {
                          i=0;
                          Display_String("New Password:   ",0x80);
                          Display_String("                ",0xc0);
                         }
           
                       break;                        
                    
                      case 13: //按D键保存新密码
                      if ( !IS_valid_user)
            {   i=0;
               Display_String("No rights !       ",0xc0);
                delayms(1000);
             Display_String("Your Password...",0x80);
             Display_String("Lock OK!         ",0xc0);
             }
                       else 
                      {i = 0;  
             init();
             delayms(5);
             for(k=0;k<6;k++)
             {
               Userpassword[k]=Userpassword[k]+48;
             }
                       aa=Sendstring(0xa0,1,Userpassword,6);
             delayms(5);
                       aa=Recstring(0xa0,1,buffer,6);  
             delayms(5); 
                       clear_password();                         
                       Display_String(table,0x00);
                       Display_String("Password Saved!   ",0xc0);
             delayms(1000);
             Display_String("Do lock agian ?   ",0xc0);
                      }               
                      break;                        
       
                      case 14: //按E键消除所有输入
                      i=0;
                      clear_password();
                      Display_String("                ",0xc0);            
            break;

            case 15:   //清除一位
             if(i!=0)i--;
             for(n=0;n<i;n++)
             {
               DSY_BUFFER1[n]='*';  
             }
             Display_String(DSY_BUFFER1,0xc0);    
             
            }
            P1=0x0f;
      }
      if(j==3)
      {  Display_String("THIEF!!!THIEF!!!",0xc0); 
        j=0;
        beep=0;
      }
    }
}
