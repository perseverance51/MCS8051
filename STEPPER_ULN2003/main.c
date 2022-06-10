#include<reg52.h>
#include<stdio.h>
#include<intrins.h>

#define uchar unsigned char
#define uint unsigned int

sbit RS = P2^0;//1602����/����ѡ��ˣ�H�����ݼĴ���L��ָ��Ĵ�����
sbit RW = P2^1;//1602��/дѡ���
sbit E  = P2^2;//1602ʹ���źŶ�

sbit key1=P3^0;//�û�����
sbit key2=P3^1;
sbit key3=P3^2;
sbit key4=P3^3;
sbit key5=P3^4;

//uchar code B_Rotation[8]={0x7f,0x3f,0xbf,0x9f,0xdf,0xcf,0xef,0x6f}; //��ת���
//uchar code F_Rotation[8]={0xef,0xcf,0xdf,0x9f,0xbf,0x3f,0x7f,0x6f}; //��ת���
uchar code B_Rotation[8]={0x09,0x08,0x0c,0x04,0x06,0x02,0x03,0x01}; //��ת���
uchar code F_Rotation[8]={0x01,0x03,0x02,0x06,0x04,0x0C,0x08,0x09}; //��ת���

//0x01,0x03,0x02,0x06,0x04,0x0C,0x08,0x09
uchar table1[]="dir: +  spe:005";
uchar table2[]="cir:000 ang:000";

uchar num;
uint key,flag,speed=5,zflag,znum,fflag,fnum,select,pp,qq;
//1��ֵ2��ֵ��־3�ٶ�4��ת��־5��תȦ��6��ת��־7��תȦ��8��ת�Ƕ�9��ת�Ƕ�10ѡ��
float zang,fang;
/******************************************************************/
/*                    ��ʱ���� Xms                                   */
/******************************************************************/
void delay(uint z)
{
        uint x,y;
        for(x=z;x>0;x--)
                for(y=110;y>0;y--);
}
/******************************************************************/
/*                   LCD1602д�������                            */
/******************************************************************/
void WriteCommand(uchar com)
{
        delay(5);//����ǰ������ʱ����֤�ź��ȶ�
        E=0;
        RS=0;
        RW=0;
        P0=com;
        E=1;
        delay(5);
        E=0;
}
/******************************************************************/
/*                   LCD1602д���ݲ���                            */
/******************************************************************/
void WriteData(uchar dat)
{
        delay(5);  //����ǰ������ʱ����֤�ź��ȶ�
        E=0;
        RS=1;
        RW=0;
        P0=dat;
        E=1;
        delay(5);
        E=0;
}
/******************************************************************/
/*                           1602��ʼ������                       */
/******************************************************************/
void InitLcd()//1602��ʼ������
{
        delay(15);
        WriteCommand(0x38); //display mode
        WriteCommand(0x38); //display mode
        WriteCommand(0x38); //display mode
        WriteCommand(0x06); //������дһ���ַ����ַָ���һ���ҹ���һ
        WriteCommand(0x0c); //��ʾ������겻��ʾ
        WriteCommand(0x01); //����

        WriteCommand(0x80);
        for(num=0;num<15;num++)//��һ�е���ʾ
        {
                WriteData(table1[num]);
                delay(15);
        }
        WriteCommand(0x80+0x40);//�ڶ��е���ʾ
        for(num=0;num<15;num++)
        {
                WriteData(table2[num]);
                delay(15);
        }

}
/******************************************************************/
/*              ��������ɨ�躯��                                  */
/******************************************************************/
void keyscan()  
{        
        
        P3=0xff;//����P3�ڣ��Զ�ȡP3�ڵ�ֵ
        if(key1==0)//��1������
        {
                delay(5);//��ʱ����
                if(key1==0)
                {
                        flag=1;
                }
                while(!key1);
                delay(5);//��ʱ����
                while(!key1);//���ּ��
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
        if(key3==0)//��1������
        {
                delay(5);//��ʱ����
                if(key3==0)
                {
                        flag=3;
                }
                while(!key3);
                delay(5);//��ʱ����
                while(!key3);//���ּ��
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
/*                           1602��ʾ3λ��                       */
/******************************************************************/
void write_sfm (uchar add,uint date)  reentrant //1602��ʾ
{
        uchar bai,shi,ge;
        bai=date/100;
        shi=date%100/10;
        ge=date%10;
        WriteCommand(0x80+add);//�������ݵ�ַָ��
        WriteData(0x30+bai);
        WriteData(0x30+shi);
        WriteData(0x30+ge);
        WriteData(0x20);
}
/******************************************************************/
/*                           ��ֵ����                             */
/******************************************************************/
void handle_flag()
{        
        uchar select1;
        if(TR0==0&&TR1==0)
        {        
                if(flag==1) //��ת
                {
                        WriteCommand(0x80+0x05);
                        WriteData('+');
                        delay(20);
                        select=0;
                }
                if(flag==2) //��ת
                {
                        WriteCommand(0x80+0x05);
                        WriteData('-');
                        delay(20);
                        select=1;
                }
                if(flag==3)//ת�ټ�
                {
                    speed++;
                        if(speed>=12)
                                speed=12;
                        write_sfm(0x0c,speed);//��ʾ�ٶ�
                        delay(20);
            }
                if(flag==4)        //ת�ټ�
                {
                        speed--;
                        if(speed<=1)
                                speed=1;
                        write_sfm(0x0c,speed);//��ʾ�ٶ�
                        delay(20);
                }
}
        if(flag==5)////�ر��жϣ�ֹͣ���
        {
                select1++;
                select1%=2;
                if( select1==0)
                {
                        TR0=0;
                        TR1=0;
                        if(select==0)//��ʾת���ĽǶ�
                                write_sfm(0x40+0x0c,zang);
                        else
                                write_sfm(0x40+0x0c,fang);
             }
                else
                {if(select==0)
                {
                        TR0=1;//����ʱ��0��������ת����
                        TR1=0;//�ض�ʱ��1
                }
                else
                {        
                        TR1=1;//����ʱ��1��������ת����
                        TR0=0;//�ض�ʱ��0        
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

void time0() interrupt 1	//��ʱ��0
{        
        uchar zz;
        TH0=(65536-(14648/speed))/256; // 360/(5.625/64)=4096   
                                   //����60��תһȦ����60���ж�4096�Σ�
                                   //��ôһ���ж���Ҫ60000000/4096=14648��ʱ��
                                   //60000000/(4096*speed)   zflag=4096ʱһȦ
        TL0=(65536-(14648/speed))%256;
        zz%=8;
        P1=F_Rotation[zz];  //�����Ӧ����
        zz++;
        zflag++;
        if(zflag==4096) //��ת��һȦ
        {        
                zflag=0;
                znum++;
                write_sfm(0x40+0x04,znum);
        }
        if(zflag%64==0)//������ת���ĽǶ�
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
void time1() interrupt 3		//��ʱ��1
{               
        uchar ff;
        TH1=(65536-(14648/speed))/256;
        TL1=(65536-(14648/speed))%256;
    ff%=8;
        P1=B_Rotation[ff]; //�����Ӧ����
        ff++;
        fflag++;
        if(fflag==4096) //��ת��һȦ
        {        
                fflag=0;
                fnum++;
                write_sfm(0x40+0x04,fnum);
        }
        if(fflag%64==0)//���㷴ת���ĽǶ�
        {
                qq++;
                fang=(uint)(qq*5.625);
                if(fang==360)
                qq=0;
                write_sfm(0x40+0x0c,fang);
        }
}
