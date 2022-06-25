//-----------------------------------------------------------------
//  名称:按键处理程序头文件(独立键盘)
//-----------------------------------------------------------------
//按键编码 1 ==》按键1  
//按键编码 2 ==》按键2  
//按键编码 3 ==》按键3 	
//按键编码 4 ==》按键4 
//按键编码 5 ==》按键5
#ifndef _KEY_H_
#define _KEY_H_

//---包含头文件---//
#include "Config.h"

#define KEY_TIME_SHORT 	20		//短按的按键去抖动延时的时间

//---加入全局变量--//
extern uchar KeyCode;
extern uint  KeyTimeCnt[5];			//按键去抖动延时计数器,分别对应KEY1/2/3/4/5
extern uchar KeyLock[5];				//按键触发后自锁的变量标志,分别对应KEY1/2/3/4/5

//-----------------------------------------------------------------
// 相关函数声明
//-----------------------------------------------------------------
void KeyScan();				//按键扫描函数 放在定时中断里			
void Delay10ms();			//延时函数
							
#endif