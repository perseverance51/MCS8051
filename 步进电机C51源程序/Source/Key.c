
//https:	//shop366226707.taobao.com
//QQ:	3065674908
//-----------------------------------------------------------------
//  名称:按键处理程序(独立键盘)
//-----------------------------------------------------------------
#include "Key.h"

uchar KeyCode = 0;						//按键编码值

//共5个独立机械按键,通过短按实现5种按键编码值
uint KeyTimeCnt[5] = {0,0,0,0,0};		//按键去抖动延时计数器,分别对应KEY1/2/3/4/5
uchar KeyLock[5] = {0,0,0,0,0};			//按键触发后自锁的变量标志,分别对应KEY1/2/3/4/5

//-----------------------------------------------------------------
//函数名称：	void KeyScan()
//函数功能:		按键扫描程序,放在定时中断里，可较快实现按键响应
//调用子函数:	无
//输入参数: 	无
//返回值:			无，说明:返回的按键编码值保存在KeyCode变量里
//-----------------------------------------------------------------
void KeyScan()
{
//------KEY1实现-------------------------------------------------
	if(KEY1 == 1)							//IO是高电平，说明按键没有被按下，这时要及时清零一些标志位
	{	KeyLock[0] = 0;					//按键自锁变量标志清零
		KeyTimeCnt[0] = 0;  		//按键去抖动延时计数器清零 
	}
	else if(KeyLock[0] == 0)
	{
		 KeyTimeCnt[0]++; 
		 if(KeyTimeCnt[0] > KEY_TIME_SHORT)  
		 {
				KeyTimeCnt[0] = 0; 
				KeyLock[0] = 1;  		//自锁按键置位,避免一直触发
				KeyCode = 1;
		 }
	}
	
//------KEY2实现-----------------------------------
	if(KEY2 == 1)
	{	KeyLock[1] = 0;
		KeyTimeCnt[1] = 0; 
	}
	else if(KeyLock[1] == 0)
	{
		 KeyTimeCnt[1]++; 
		 if(KeyTimeCnt[1] > KEY_TIME_SHORT) 	//消抖
		 {	
				KeyTimeCnt[1] = 0; 								//按键去抖动延时计数器清零
				KeyLock[1] = 1;  									//自锁按键置位,避免一直触发
				KeyCode = 2;   					 			
		 }
	}

//------KEY3实现-----------------------------------
	if(KEY3 == 1)
	{	KeyLock[2] = 0;
		KeyTimeCnt[2] = 0;		
	}
	else if(KeyLock[2] == 0)
	{
		 KeyTimeCnt[2]++; 
		 if(KeyTimeCnt[2] > KEY_TIME_SHORT) 
		 {	
				KeyTimeCnt[2] = 0; 
				KeyLock[2] = 1;  
				KeyCode = 3;   					 			
		 }
	}

//------KEY4实现-----------------------------------
	if(KEY4 == 1)
	{	KeyLock[3] = 0;
		KeyTimeCnt[3] = 0;		
	}
	else if(KeyLock[3] == 0)
	{
		 KeyTimeCnt[3]++; 
		 if(KeyTimeCnt[3] > KEY_TIME_SHORT) 
		 {	
				KeyTimeCnt[3] = 0; 
				KeyLock[3] = 1;  
				KeyCode = 4;   					 			
		 }
	}
	
//------KEY5实现-----------------------------------
	if(KEY5 == 1)
	{	KeyLock[4] = 0;
		KeyTimeCnt[4] = 0;		
	}
	else if(KeyLock[4] == 0)
	{
		 KeyTimeCnt[4]++; 
		 if(KeyTimeCnt[4] > KEY_TIME_SHORT) 
		 {	
				KeyTimeCnt[4] = 0; 
				KeyLock[4] = 1;  
				KeyCode = 5;   					 			
		 }
	}
}