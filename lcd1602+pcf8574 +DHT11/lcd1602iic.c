#include "stdio.h"
#include <reg52.h>
#include "intrins.h"
#include	"delay.h"
#define uchar unsigned char
#define uint unsigned int
	
sbit SCL = P1^0;
sbit SDA = P1^1;

sbit Temp_data = P1^2 ;

sbit led = P1^5;//DHT响应指示灯
sbit led2 = P1^6;//效验数据指示灯

unsigned int rec_dat[4];
//#define   L1                0x80            // 第一行写入地址
//#define   L2                0xc0            // 第二行写入地址
char ADDR = 0x4E;    // PCF8574  T  模块的地址码
//char ADDR = 0x7e;    // PCF8574   AT  模块的地址码

void DHT11_start();//DHT开始信号
unsigned char DHT11_rec_byte();
void DHT11_receive();

//***************************** 延时毫秒函数***********************************************


void delay1(int y)   //
{
    while(y--)
    {
        unsigned char a,b,c;
        for(c=1; c>0; c--)
            for(b=142; b>0; b--)
                for(a=2; a>0; a--);
    }
}

//******************************** IIC 串口开始 ********************************************


void IIC_start(void)
{
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
//********************************** IIC 串口写1个字节 ******************************************


void IIC_writeByte(char temp)
{
    char i;
    for(i=0; i<8; i++)
    {
        SDA=(bit)(temp & 0x80) ;   // 根据规定1602的数据最高位必须为  1
        temp <<=1;
        _nop_();
        _nop_();
        SCL=1;
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        SCL=0;
    }
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    SDA=1;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    SCL=1;
    _nop_();
    _nop_();
    _nop_();
    while(SDA);
    _nop_();
    SCL=0;
}




//******************************** 1602写命令 ********************************************


void LCD_write_command(char comm)
{
    char tmp;
    IIC_start();          // 串口开始
    IIC_writeByte(ADDR);  // 先选PCF 8574T 的地址  （应该是相当于选中的意思吧）

    tmp = comm & 0xF0;    // 与0xf0 应该是取第四位的意思吧
    tmp |= 0x0C;         //保留高4位为指令的高四位，低四位为   RS = 0, RW = 0, EN = 1
    IIC_writeByte(tmp);  //从串口送出
    delay1(20);
    tmp &= 0xFB;        //Make EN = 0
    IIC_writeByte(tmp);

    tmp = (comm & 0x0F) << 4 ;  //将指令的低四位 送到高位置保存
    tmp |= 0x0C;        //RS = 0, RW = 0, EN = 1
    IIC_writeByte(tmp);
    delay1(20);
    tmp &= 0xFB; // Make EN = 0
    IIC_writeByte(tmp);

}
//******************************** 1602写数据 ********************************************


void LCD_write_data(char data1)
{
    char tmp;
    IIC_start();
    IIC_writeByte(ADDR);   // 先选PCF 8574T 的地址  （应该是相当于选中的意思吧）

    tmp = data1 & 0xF0;
    tmp |= 0x0D; //RS = 0, RW = 0, EN = 1
    IIC_writeByte(tmp);
    delay1(20);
    tmp &= 0xFB; //Make EN = 0
    IIC_writeByte(tmp);

    tmp = (data1 & 0x0F) << 4 ;
    tmp |= 0x0D; //RS = 0, RW = 0, EN = 1
    IIC_writeByte(tmp);
    delay1(20);
    tmp &= 0xFB ; // Make EN = 0
    IIC_writeByte(tmp);
}


//******************************** 1602初始化 ********************************************


void Init_Lcd(void)
{
    LCD_write_command(0x33); //将8位总线转为4位总线
    delay1(50) ;
    LCD_write_command(0x32); //
    delay1(50) ;
    LCD_write_command(0x28); // 4位数据线，显示2行，5*7点阵字符  ！如果是0x38  则为8位数据线，显示2行，5*7点阵字符
    delay1(50) ;
    LCD_write_command(0x0C); // 开显示，关闭光标，不闪烁
    delay1(50) ;
    LCD_write_command(0x06); // 设定输入方式，增量不位移
    delay1(50) ;
    LCD_write_command(0x01); // 清屏
    delay1(50) ;
}








//*************************************** 在指定位置显示字符串 *************************************


void Write_LCD(int x, int y, char *str)
{
    char addr;
    if( x < 0)
    {
        x = 0;
    }
    if(x > 15)
    {
        x = 15;
    }
    if(y<0)
    {
        y = 0;
    }
    if(y > 1)
    {
        y = 1;
    }

    addr = 0x80 + 0x40 * y + x;   // Move cursor  移动光标
    LCD_write_command(addr);
    while (*str)
    {
        LCD_write_data(*str++);
    }
}


//-------------------------------------------- 显示字符串的函数 ----------------------------------------------------

/*
void LCD_write_word(unsigned char *s) //显示字符串的函数
{
    while(*s>0)
    {
        LCD_write_data(*s);
        s++;
    }
}
*/
//********************************* 指定位置显示一个字符*******************************************
/*
void Print_Char (unsigned char line,unsigned char num,unsigned char date)
{
                LCD_write_command(line+num);
                LCD_write_data(date);
}


*/
/*
//按指定位置显示一个字符(针对1602液晶)-用在温度显示
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
    Y &= 0x1;
    X &= 0xF;                 //限制X不能大于15，Y不能大于1
    if (Y) X |= 0x40;        //当要显示第二行时地址码+0x40;
    X |= 0x80;               // 算出指令码
    LCD_write_command(X);    //这里不检测忙信号，发送地址码
    LCD_write_data(DData);
}
*/

//DHT11起始信号
void DHT11_start()
{
    Temp_data=1;
    Delay10us();
    Temp_data=0;
    Delay25ms();//这个延时不能过短,18ms以上,实际在仿真当中要想读到数据延时要在延时参数要在40以上才能出数据
    Temp_data=1;
    Delay50us();//MCU释放总线20~40us,需要在40微秒到100微秒之间
}

//接收一个字节
unsigned char DHT11_rec_byte()
{
    unsigned char i,dat=0;
    for(i=0; i<8; i++)
    {
        while(!Temp_data);//跳过数据前段的50us
        Delay35us();//过了数据0和数据1相同区域，再做检测
        dat <<=1;
        if(Temp_data==1)
        {   //如果读到的数据是1
            dat +=1;
        }
        while(Temp_data);
    }
    return dat;
}
//接收温湿度数据
void DHT11_receive()
{
    unsigned int R_H,R_L,T_H,T_L;
    unsigned char RH,RL,TH,TL,revise;
    DHT11_start();
    if(Temp_data==0)
    {
        led=1;//检测从设备DHT拉低总线信号，作为响应信号
        while(Temp_data==0); //等待DHT响应信号，80us内来拉高总线

        while(Temp_data==1); //等待DHT响应信号，80us内来拉低总线
        //	led=1;
        R_H=DHT11_rec_byte();    //接收湿度高八位
        R_L=DHT11_rec_byte();    //接收湿度低八位
        T_H=DHT11_rec_byte();    //接收温度高八位
        T_L=DHT11_rec_byte();    //接收温度低八位
        revise=DHT11_rec_byte(); //接收校正位
        Delay25us();    //结束

        if((R_H+R_L+T_H+T_L)==revise)      //校正
        {
            led2 = 0;

            RH=R_H;
            RL=R_L;
            TH=T_H;
            TL=T_L;
        }
        /*数据处理，方便显示*/
        rec_dat[0]=RH;
        rec_dat[1]=RL;
        rec_dat[2]=TH;
        rec_dat[3]=TL;

    }
//else{	led=1;}

}

void main() {
	
    float tem = 0.0,hum = 0.0;
    unsigned char Tempbuf[8],Humbuf[5];
    Init_Lcd();

    led = 0;
    led2 = 0;
    while(1) {
        DHT11_receive();//接收DHT11数据
        Write_LCD(3, 1, "Temp:");//在第一行第一个位置显示字符串
        Write_LCD(3, 0, "Hum:");//在第一行第一个位置显示字符串
        hum = rec_dat[0] + rec_dat[1]/10.0;
        tem = rec_dat[2] + rec_dat[3]/10.0;
        sprintf(Tempbuf, "%3.1f*C",tem);
        sprintf(Humbuf, "%2.0f%%", hum);
        Write_LCD(7, 0, Humbuf);
        Write_LCD(8, 1, Tempbuf);
        Delay1000ms();
        Delay1000ms();
        led=0;
        LCD_write_command(0x01); // 清屏
    }
}

