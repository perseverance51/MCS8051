
//https:	//shop366226707.taobao.com
//QQ:	3065674908
//-----------------------------------------------------------------
//  ����:�����������(��������)
//-----------------------------------------------------------------
#include "Key.h"

uchar KeyCode = 0;						//��������ֵ

//��5��������е����,ͨ���̰�ʵ��5�ְ�������ֵ
uint KeyTimeCnt[5] = {0,0,0,0,0};		//����ȥ������ʱ������,�ֱ��ӦKEY1/2/3/4/5
uchar KeyLock[5] = {0,0,0,0,0};			//���������������ı�����־,�ֱ��ӦKEY1/2/3/4/5

//-----------------------------------------------------------------
//�������ƣ�	void KeyScan()
//��������:		����ɨ�����,���ڶ�ʱ�ж���ɽϿ�ʵ�ְ�����Ӧ
//�����Ӻ���:	��
//�������: 	��
//����ֵ:			�ޣ�˵��:���صİ�������ֵ������KeyCode������
//-----------------------------------------------------------------
void KeyScan()
{
//------KEY1ʵ��-------------------------------------------------
	if(KEY1 == 1)							//IO�Ǹߵ�ƽ��˵������û�б����£���ʱҪ��ʱ����һЩ��־λ
	{	KeyLock[0] = 0;					//��������������־����
		KeyTimeCnt[0] = 0;  		//����ȥ������ʱ���������� 
	}
	else if(KeyLock[0] == 0)
	{
		 KeyTimeCnt[0]++; 
		 if(KeyTimeCnt[0] > KEY_TIME_SHORT)  
		 {
				KeyTimeCnt[0] = 0; 
				KeyLock[0] = 1;  		//����������λ,����һֱ����
				KeyCode = 1;
		 }
	}
	
//------KEY2ʵ��-----------------------------------
	if(KEY2 == 1)
	{	KeyLock[1] = 0;
		KeyTimeCnt[1] = 0; 
	}
	else if(KeyLock[1] == 0)
	{
		 KeyTimeCnt[1]++; 
		 if(KeyTimeCnt[1] > KEY_TIME_SHORT) 	//����
		 {	
				KeyTimeCnt[1] = 0; 								//����ȥ������ʱ����������
				KeyLock[1] = 1;  									//����������λ,����һֱ����
				KeyCode = 2;   					 			
		 }
	}

//------KEY3ʵ��-----------------------------------
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

//------KEY4ʵ��-----------------------------------
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
	
//------KEY5ʵ��-----------------------------------
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