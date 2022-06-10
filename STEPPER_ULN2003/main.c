#include<reg52.h>
#include<stdio.h>
#include<intrins.h>

#define uchar unsigned char
#define uint unsigned int

sbit RS = P2^0;//1602数据/命令选择端（H：数据寄存器L：指令寄存器）
sbit RW = P2^1;//1602读/写选择端
sbit E  = P2^2;//1602使能信号端

sbit key1=P3^0;//用户按键
sbit key2=P3^1;
sbit key3=P3^2;
sbit key4=P3^3;
sbit key5=P3^4;

//uchar code B_Rotation[8]={0x7f,0x3f,0xbf,0x9f,0xdf,0xcf,0xef,0x6f}; //反转表格
//uchar code F_Rotation[8]={0xef,0xcf,0xdf,0x9f,0xbf,0x3f,0x7f,0x6f}; //正转表格
uchar code B_Rotation[8]={0x09,0x08,0x0c,0x04,0x06,0x02,0x03,0x01}; //反转表格
uchar code F_Rotation[8]={0x01,0x03,0x02,0x06,0x04,0x0C,0x08,0x09}; //正转表格

//0x01,0x03,0x02,0x06,0x04,0x0C,0x08,0x09
uchar table1[]="dir: +  spe:005";
uchar table2[]="cir:000 ang:000";

uchar num;
uint key,flag,speed=5,zflag,znum,fflag,fnum,select,pp,qq;
//1键值2键值标志3速度4正转标志5正转圈数6反转标志7反转圈数8正转角度9反转角度10选择
float zang,fang;
/******************************************************************/
/*                    延时函数 Xms                                   */
/******************************************************************/
void delay(uint z)
{
        uint x,y;
        for(x=z;x>0;x--)
                for(y=110;y>0;y--);
}
/******************************************************************/
/*                   LCD1602写命令操作                            */
/******************************************************************/
void WriteCommand(uchar com)
{
        delay(5);//操作前短暂延时，保证信号稳定
        E=0;
        RS=0;
        RW=0;
        P0=com;
        E=1;
        delay(5);
        E=0;
}
/******************************************************************/
/*                   LCD1602写数据操作                            */
/******************************************************************/
void WriteData(uchar dat)
{
        delay(5);  //操作前短暂延时，保证信号稳定
        E=0;
        RS=1;
        RW=0;
        P0=dat;
        E=1;
        delay(5);
        E=0;
}
/******************************************************************/
/*                           1602初始化程序                       */
/******************************************************************/
void InitLcd()//1602初始化程序
{
        delay(15);
        WriteCommand(0x38); //display mode
        WriteCommand(0x38); //display mode
        WriteCommand(0x38); //display mode
        WriteCommand(0x06); //当读或写一个字符后地址指针加一，且光标加一
        WriteCommand(0x0c); //显示开及光标不显示
        WriteCommand(0x01); //清屏

        WriteCommand(0x80);
        for(num=0;num<15;num++)//第一行的显示
        {
                WriteData(table1[num]);
                delay(15);
        }
        WriteCommand(0x80+0x40);//第二行的显示
        for(num=0;num<15;num++)
        {
                WriteData(table2[num]);
                delay(15);
        }

}
/******************************************************************/
/*              独立键盘扫描函数                                  */
/******************************************************************/
void keyscan()  
{        
        
        P3=0xff;//拉高P3口，以读取P3口的值
        if(key1==0)//键1被按下
        {
                delay(5);//延时消抖
                if(key1==0)
                {
                        flag=1;
                }
                while(!key1);
                delay(5);//延时消抖
                while(!key1);//松手检测
        }
        if(key2==0)
        {
                delay(5);
                if(key2==0)
                {
                        flag=2;
                }
                while(!key2);
                delay(5);
                while(!key2);
        }
        if(key3==0)//键1被按下
        {
                delay(5);//延时消抖
                if(key3==0)
                {
                        flag=3;
                }
                while(!key3);
                delay(5);//延时消抖
                while(!key3);//松手检测
        }
        if(key4==0)
        {
                delay(5);
                if(key4==0)
                {
                        flag=4;
                }
                while(!key4);
                delay(5);
                while(!key4);
        }
        if(key5==0)
        {
                delay(5);
                if(key5==0)
                {
                        flag=5;
                }
                while(!key5);
                delay(5);
                while(!key5);
        }
}
/******************************************************************/
/*                           1602显示3位数                       */
/******************************************************************/
void write_sfm (uchar add,uint date)  reentrant //1602显示
{
        uchar bai,shi,ge;
        bai=date/100;
        shi=date%100/10;
        ge=date%10;
        WriteCommand(0x80+add);//设置数据地址指针
        WriteData(0x30+bai);
        WriteData(0x30+shi);
        WriteData(0x30+ge);
        WriteData(0x20);
}
/******************************************************************/
/*                           键值处理                             */
/******************************************************************/
void handle_flag()
{        
        uchar select1;
        if(TR0==0&&TR1==0)
        {        
                if(flag==1) //正转
                {
                        WriteCommand(0x80+0x05);
                        WriteData('+');
                        delay(20);
                        select=0;
                }
                if(flag==2) //反转
                {
                        WriteCommand(0x80+0x05);
                        WriteData('-');
                        delay(20);
                        select=1;
                }
                if(flag==3)//转速加
                {
                    speed++;
                        if(speed>=12)
                                speed=12;
                        write_sfm(0x0c,speed);//显示速度
                        delay(20);
            }
                if(flag==4)        //转速减
                {
                        speed--;
                        if(speed<=1)
                                speed=1;
                        write_sfm(0x0c,speed);//显示速度
                        delay(20);
                }
}
        if(flag==5)////关闭中断，停止电机
        {
                select1++;
                select1%=2;
                if( select1==0)
                {
                        TR0=0;
                        TR1=0;
                        if(select==0)//显示转动的角度
                                write_sfm(0x40+0x0c,zang);
                        else
                                write_sfm(0x40+0x0c,fang);
             }
                else
                {if(select==0)
                {
                        TR0=1;//开定时器0，产生正转相序
                        TR1=0;//关定时器1
                }
                else
                {        
                        TR1=1;//开定时器1，产生反转相序
                        TR0=0;//关定时器0        
                }
                }                        
        }
        flag=0;
}


void main()
{        
        pp=0;
        qq=0;
   
        InitLcd();
        TMOD=0x11;
				EA=1;
        ET0=1;
        ET1=1;

        TH0=(65536-(14648/speed))/256;
        TL0=(65536-(14648/speed))%256;
        TH1=(65536-(14648/speed))/256;
        TL1=(65536-(14648/speed))%256;

        TR0=0;
        TR1=0;
        
        while(1)
        {        
                keyscan();
                handle_flag();        
        }

}

void time0() interrupt 1	//定时器0
{        
        uchar zz;
        TH0=(65536-(14648/speed))/256; // 360/(5.625/64)=4096   
                                   //假设60秒转一圈，即60秒中断4096次，
                                   //那么一次中断需要60000000/4096=14648个时钟
                                   //60000000/(4096*speed)   zflag=4096时一圈
        TL0=(65536-(14648/speed))%256;
        zz%=8;
        P1=F_Rotation[zz];  //输出对应的相
        zz++;
        zflag++;
        if(zflag==4096) //正转了一圈
        {        
                zflag=0;
                znum++;
                write_sfm(0x40+0x04,znum);
        }
        if(zflag%64==0)//计算正转动的角度
        {
                pp++;
                zang=(uint)(pp*5.625);
                if(zang==360)
                        pp=0;
                write_sfm(0x40+0x0c,zang);
        }
        //zang=0.088*zflag;
        //write_sfm(0x40+0x0c,zang);
}
void time1() interrupt 3		//定时器1
{               
        uchar ff;
        TH1=(65536-(14648/speed))/256;
        TL1=(65536-(14648/speed))%256;
    ff%=8;
        P1=B_Rotation[ff]; //输出对应的相
        ff++;
        fflag++;
        if(fflag==4096) //反转了一圈
        {        
                fflag=0;
                fnum++;
                write_sfm(0x40+0x04,fnum);
        }
        if(fflag%64==0)//计算反转动的角度
        {
                qq++;
                fang=(uint)(qq*5.625);
                if(fang==360)
                qq=0;
                write_sfm(0x40+0x0c,fang);
        }
}
