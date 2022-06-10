/**
  ***************************************************************************************
  * 文  件：LCD1602模块实现头文件
  ***************************************************************************************
  */
 
sbit LCD1602_RS = P2^0;  //LCD1602线
sbit LCD1602_RW = P2^1;  //LCD1602线
sbit LCD1602_E  = P2^2;  //LCD1602线
 
#define LCD1602_DATA P3  //LCD1602模块8位数据线
	
void LCD1602_isBusy    (void);  //LCD1602判断忙函数
void LCD1602_WriteCmd  (unsigned char);  //LCD1602写指令、写显示地址函数
void LCD1602_WriteData (unsigned char);  //LCD1602写数据函数
void LCD1602_DispChar  (unsigned char,unsigned char,unsigned char);  //LCD1602指定行列位置显示字符函数
void LCD1602_Init      (void);  //LCD1602初始化函数