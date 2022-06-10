#include <intrins.h>			//引用程序目录的相对路径中的头文件
#include <reg51.h>			//引用编译器的类库路径里面的头文件

#define LCD P0			//LCD2004数据接口
#define Key P1			//Px.0~Px.3为第1行~第4行,Px.4~Px.7为第1列~第4列

sbit RS=P2^7;			//设置RS引脚接口,RS=0,指令寄存器；RS=1,数据寄存器
sbit RW=P2^6;			//设置R/W引脚接口,R/W=0,写；R/W=1,读
sbit E=P2^5;			//设置E引脚接口,E允许信号
sbit Relay1=P2^3;			//继电器1接口
sbit Relay2=P2^4;			//继电器2接口

unsigned long int time_min=20;			//中断时间最小值，可修改。(范围: 0~4294967295)
unsigned long int time=20;			//中断时间。(范围: 0~4294967295)
unsigned long int time_temp=0;			//暂存中断时间。(范围: 0~4294967295)
unsigned char num=50;  			//存储键盘按键值(范围: 0~255)
unsigned char T[]="00000";			//中断持续时间,H整型转换为字符型。(范围: 0~255)
bit set_up_able=0;			//参数使能,防止参数误修改(范围: 0~1)

/******************************
		延时1毫秒,12T单片机,12MHz晶振
******************************/
void Delay(unsigned long int n)
	{
		unsigned int i=0,j=0;
    for(i=0;i<n;i++)
		for(j=0;j<123;j++);
	}

/******************************
		LCD忙检测函数子程序
******************************/
void CheckBusy(void)
	{
		unsigned int nTimeOut=0;
		RS=0;			//为寄存器选择脚,高电平时选择数据寄存器、低电平时选择指令寄存器
		RW=1;			//为读、写信号线,低电平时进行写操作
		E=0;			//E端为使能端,当E端由高电平跳变为低电平时,液晶模块执行命令
		E=1;			//E端为使能端,当E端由高电平跳变为低电平时,液晶模块执行命令
		while((LCD&0x80)&&(++nTimeOut !=0));
		E=0;			//E端为使能端,当E端由高电平跳变为低电平时,液晶模块执行命令
		RS=0;			//为寄存器选择脚,高电平时选择数据寄存器、低电平时选择指令寄存器
		RW=1;			//为读、写信号线,低电平时进行写操作
	}

/******************************
		LCD发送命令或数据子程序
******************************/
void SendCmdorData(unsigned char byCmdorData,bit DI)
	{
		CheckBusy();
		RS=DI;			//为寄存器选择脚,高电平时选择数据寄存器、低电平时选择指令寄存器
		RW=0;			//为读、写信号线,低电平时进行写操作
		E=0;			//E端为使能端,当E端由高电平跳变为低电平时,液晶模块执行命令
		LCD=byCmdorData;			//送数据
		_nop_();			//空操作，给硬件反应时间
		_nop_();			//空操作，给硬件反应时间
		E=1;			//E端为使能端,当E端由高电平跳变为低电平时,液晶模块执行命令
		_nop_();			//空操作，给硬件反应时间
		_nop_();			//空操作，给硬件反应时间
		E=0;			//E引脚要有一个下降沿,才能把数据线上数据,命令送入LCD
		RW=1;			//为读、写信号线,高电平时进行读操作
		RS=0;			//为寄存器选择脚,低电平时选择指令寄存器
	}

/******************************
		LCD初始化子程序
******************************/
void Init(void)
	{
		SendCmdorData(0x38,0);			//写命令,设置16*2显示,5*7点阵,8位数据接口
		Delay(50);			//50ms延时，给硬件反应时间
		SendCmdorData(0x01,0);			//写命令,显示清屏：1、数据指针清零 2、所有显示清零
		Delay(50);			//50ms延时，给硬件反应时间
		SendCmdorData(0x06,0);			//写命令,当读或写一个字符后地址指针加一,且光标加一
		Delay(50);			//50ms延时，给硬件反应时间
		SendCmdorData(0x0c,0);			//写命令,开显示、不显示光标、光标不显示
		Delay(50);			//50ms延时，给硬件反应时间
	}

/******************************
		地址转换子程序
		x为行号 y为列号
******************************/
void SetAddress(unsigned char x,y)
	{
		unsigned char byAddress;
		switch(x)
			{
			case 1:			//第一行首地址
				byAddress=0x80+y;
			break;
			case 2:			//第二行首地址
				byAddress=0xC0+y;
			break;
			case 3:			//第三行首地址
				byAddress=0x94+y;
			break;
			case 4:			//第四行首地址
				byAddress=0xD4+y;
			break;
			default:break;			//default就是如果没有符合的case就执行它,default并不是必须的.
			} 
			SendCmdorData(byAddress,0);			//执行发送指令子程序
	}

/******************************
		发送字符串到LCD子程序
******************************/
void PutString(unsigned char x,y,unsigned char *pData)
	{
		SetAddress(x,y);			//执行地址转换子程序
		while(*pData !='\0')			//表示显示内容不为空时,执行循环体的代码段,
			{
				SendCmdorData(*pData++,1);			//发送命令子程序,*就是声明他的类型,是一个指向整型数据的指针
			}
	}

/******************************
	4位数的H整型转换为字符型,	加48为转ASCII。
	*就是声明他的类型,是一个指向整型数据的指针
******************************/
void Inttochar5(unsigned long int a,unsigned char *chx)
	{
		chx[0]=a/10000+48;			//延时万位
		chx[1]=(a/1000)%10+48;			//延时千位
		chx[2]=(a/100)%10+48;			//延时百位
		chx[3]=(a/10)%10+48;			//延时十位
		chx[4]=a%10+48;			//延时个位
		}

/******************************
		LCD数据显示子程序
******************************/
void Display(void)
	{
		Inttochar5(time,(unsigned char *)T);			//中断时间,H整型转换为字符型。*就是声明他的类型,是一个指向整型数据的指针
		PutString(2,4,"Please enter");			//12字节
		PutString(3,3,"time:");			//字体,5字节
		PutString(3,9,T);			//T为中断时间的ASCII数组,5字节
		PutString(3,15,"ms");			///字体,2字节
		}

/******************************
		显示光标、准备修改
******************************/
void Set()
	{
 		if(set_up_able==1)			//参数使能
			{
				time_temp=0;			//暂存中断时间
				SendCmdorData(0xA1,0);			//向LCD发送首地址指令
				SendCmdorData(0x0f,0);			//LCD发送开显示、显示光标、光标闪烁指令
				}
		else
 			{
				if(time<time_min)
					{
						time=time_min;
						Display();			//数据显示子程序
					}
					SendCmdorData(0x0c,0);			// 写命令,开显示、不显示光标、光标不显示
					}
	}


/******************************
		修改中断时间子程序
******************************/
void Setnum(unsigned char num)
	{
		if(set_up_able==1)			//当参数使能为1时允许修改
			{
				if(time_temp > 9999)			//暂存中断时间大于9999
					{
						time_temp=0;			//暂存中断时间重新赋值
						}
							time_temp=time_temp*10+num;			//中断时间进位，存入临时中断时间
							time=time_temp;
							Display();
							SendCmdorData(0xA1,0);			//向LCD发送首地址指令
							SendCmdorData(0x0f,0);			//LCD发送开显示、显示光标、光标闪烁指令
						}
	}

/******************************
		矩阵键盘扫描子检测。
		PX.0~PX.3为第1行~第4行
		PX.4~PX.7为第1列~第4列
******************************/
void Keyscan()
	{
		unsigned char temp,z;			//temp存按键列值,z存按键行值
		Key=0xf0;			//Key低四位置0(行),Key高四位置0(列),准备查询列按键
		if(Key!=0xf0)			//如果有变化则表示有键按下
			{
				Delay(20);			//延时20ms,按钮去抖动
				if(Key!=0xf0)			//再次检测是否有键按下,按下的列被拉低
					{
						temp=Key;			//取得当前Key口的状态送temp暂存
						Key=0x0f;			//Key高四位置0(列),Key低四位置1(行),准备查询行按键,按下的行被拉低
						z=temp|Key;			//高低四位,位逻辑或运算,计算出按键值
						temp=Key&0x0f;			//去掉高四位,只保留低四位
						while(temp!=0x0f)			//松手检测，如果temp不等于0x0f成立，循环执行{}内的指令，如果temp不等于0x0f不成立，执行switch(z)
							{
								temp=Key&0x0f;			//读取新的Key值按键值
							}
							switch(z)			//按键值转换
								{
									case 0xee:num=1;break;			//第1行第1列
									case 0xde:num=2;break;			//第1行第2列
									case 0xbe:num=3;break;			//第1行第3列
									case 0x7e:num=11;break;			//第1行第4列
									case 0xed:num=4;break;			//第2行第1列
									case 0xdd:num=5;break;			//第2行第2列
									case 0xbd:num=6;break;			//第2行第3列
									case 0x7d:num=0;break;			//第2行第4列
									case 0xeb:num=7;break;			//第3行第1列
									case 0xdb:num=8;break;			//第3行第2列
									case 0xbb:num=9;break;			//第3行第3列
									case 0x7b:num=12;break;			//第3行第4列
									case 0xe7:num=50;break;			//第4行第1列
									case 0xd7:num=50;break;			//第4行第2列
									case 0xb7:num=50;break;			//第4行第3列
									case 0x77:num=50;break;			//第4行第4列
									}
					}
			}
	}

/******************************
		开始中断子程序
******************************/
void Start()
	{
		SendCmdorData(0x01,0);			// 写命令,显示清屏：1、数据指针清零 2、所有显示清零
		Delay(50);			//50ms延时，给硬件反应时间
		PutString(2,4,"Interrupting");
		Relay1=0;			//继电器1吸起
		Relay2=0;			//继电器2吸起
		Delay(time);			//中断时间
		Relay1=1;			//继电器1落下
		Relay2=1;			//继电器1落下
		SendCmdorData(0x01,0);			// 写命令,显示清屏：1、数据指针清零 2、所有显示清零
		Display();			//数据显示子程序
	}

/******************************
		主程序
******************************/
void Main()
	{
		Init();			//LCD初始化子程序
		PutString(2,2,"Time interrupter");			//欢迎界面
		PutString(3,3,"20ms---99999ms");			//欢迎界面
		Delay(2000);			//欢迎界面显示时长
		SendCmdorData(0x01,0);			// 写命令,显示清屏：1、数据指针清零 2、所有显示清零
		Delay(50);			//50ms延时，给硬件反应时间
		PutString(2,5,"");			//欢迎界面
		PutString(3,5,"2021-04-06");			//欢迎界面
		Delay(2000);			//欢迎界面显示时长
		SendCmdorData(0x01,0);			// 写命令,显示清屏：1、数据指针清零 2、所有显示清零
		Delay(50);			//50ms延时，给硬件反应时间
		time=time_min;			//初始值为最小值
		Display();			//数据显示子程序
		num=50;			//键盘按键值赋空值
		Key=0xf0;			//键盘口全部高电平
		while(1)			//循环指令
			{
				Keyscan();			//键盘检测
				switch(num)
					{
						case 0:Setnum(num);num=50;break;			//执行数字添加子程序,键盘按键值赋空值
						case 1:Setnum(num);num=50;break;			//执行数字添加子程序,键盘按键值赋空值
						case 2:Setnum(num);num=50;break;			//执行数字添加子程序,键盘按键值赋空值
						case 3:Setnum(num);num=50;break;			//执行数字添加子程序,键盘按键值赋空值
						case 4:Setnum(num);num=50;break;			//执行数字添加子程序,键盘按键值赋空值
						case 5:Setnum(num);num=50;break;			//执行数字添加子程序,键盘按键值赋空值
						case 6:Setnum(num);num=50;break;			//执行数字添加子程序,键盘按键值赋空值
						case 7:Setnum(num);num=50;break;			//执行数字添加子程序,键盘按键值赋空值
						case 8:Setnum(num);num=50;break;			//执行数字添加子程序,键盘按键值赋空值
						case 9:Setnum(num);num=50;break;			//执行数字添加子程序,键盘按键值赋空值
						case 11:set_up_able=~set_up_able;			//参数使能取反
										Set();num=50; break;			//执行设置参数子程序,键盘按键值赋空值
						case 12:set_up_able=0;Set();			//参数使能置0
										Start();num=50; break;			//开始测试程序子程序,键盘按键值赋空值
					}
			}
	}