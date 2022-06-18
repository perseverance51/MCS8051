//名称12864LCD 显示驱动程序(不带字库)
#include "LCD_12864.H"

//液晶引脚定义
sbit DI=P2^0;
sbit RW=P2^1;
sbit E=P2^2;
sbit CS1=P2^3;
sbit CS2=P2^4;
sbit RST=P2^5;
//检查 LCD 是否忙
bit LCD_Check_Busy()
{
LCD_DB_PORT=0xFF;
RW=1;_nop_();DI=0;
E=1;_nop_();E=0;
return (bit)(P0 & 0X80);
}
//向 LCD 发送命令
void LCD_Write_Command(uchar c)
{
while (LCD_Check_Busy());
LCD_DB_PORT=0xFF;RW=0;_nop_();DI=0;
LCD_DB_PORT=c;E=1;_nop_();E=0;
}
//向 LCD 发送数据
void LCD_Write_Data(uchar d )
{
while (LCD_Check_Busy());
LCD_DB_PORT=0xFF;RW=0;_nop_();DI=1;
LCD_DB_PORT=d; E=1;_nop_();E=0;
}
//初始化 LCD
void LCD_Initialize()
{
CS1=1; CS2=1;
LCD_Write_Command(0x38); //8 位形式2 行字符
LCD_Write_Command(0x0F); //开显示
LCD_Write_Command(0x01); //清屏

LCD_Write_Command(0x06); //画面不动光标右移
LCD_Write_Command(LCD_START_ROW); //设置起始行
}
//通用显示函数
//从第 P 页第 L 列开始显示 W 字节数据数据在 r 所指向的缓冲
//每字节 8 位是垂直显示的高位在下低位在上
//每个 8*128 的矩形区域为一页每页分左半页与右半页
//整个 LCD 又由 64*64 的左半习工和 64*64 的右半屏构成
void Common_Show(uchar P,uchar L,uchar W,uchar *r)
{
	uchar i;
if(L<64) //显示在左半屏或右半屏
{
CS1=1;CS2=0;
LCD_Write_Command(LCD_PAGE+P);
LCD_Write_Command(LCD_COL+L);
if(L+W<64) //全部显示在左半屏
{
for(i=0;i<W;i++) LCD_Write_Data(r[i]);
}
else //如果越界则跨越左右半屏显示
{
for(i=0;i<64-L;i++) LCD_Write_Data(r[i]); //左半屏显示
CS1=0; CS2=1; //右半屏显示
LCD_Write_Command(LCD_PAGE+P);
LCD_Write_Command(LCD_COL);
for(i=64-L;i<W;i++) LCD_Write_Data(r[i]);
}
}
else //全部显示在右半屏
{
CS1=0;CS2=1;
LCD_Write_Command(LCD_PAGE+P);
LCD_Write_Command(LCD_COL+L-64);
for(i=0;i<W;i++) LCD_Write_Data(r[i]);
}
}
//显示一个 8*16 点阵字符(字符上半部分与下半部分分别处在相邻两页中)
void Display_A_char(uchar P,uchar L,uchar *M)
{
Common_Show(P,L,8,M);
Common_Show(P+1,L,8,M+8);
}
//显示一个 16*16 点阵汉字汉字上半部分与下半部分分别处在相邻两页中

void Display_A_WORD(uchar P,uchar L,uchar *M)
{
Common_Show(P,L,16,M); //显示汉字上半部分
Common_Show(P+1,L,16,M+16); //显示汉字下半部分
}
//显示一串 16*16 点阵汉字
void Display_A_WORD_String(uchar P,uchar L,uchar C,uchar *M)
{
uchar i;
for (i=0;i<C;i++)
Display_A_WORD(P,L+i*16,M+i*32);
}
