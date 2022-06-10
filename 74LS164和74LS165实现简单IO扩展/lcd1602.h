/**
  ***************************************************************************************
  * ��  ����LCD1602ģ��ʵ��ͷ�ļ�
  ***************************************************************************************
  */
 
sbit LCD1602_RS = P2^0;  //LCD1602��
sbit LCD1602_RW = P2^1;  //LCD1602��
sbit LCD1602_E  = P2^2;  //LCD1602��
 
#define LCD1602_DATA P3  //LCD1602ģ��8λ������
	
void LCD1602_isBusy    (void);  //LCD1602�ж�æ����
void LCD1602_WriteCmd  (unsigned char);  //LCD1602дָ�д��ʾ��ַ����
void LCD1602_WriteData (unsigned char);  //LCD1602д���ݺ���
void LCD1602_DispChar  (unsigned char,unsigned char,unsigned char);  //LCD1602ָ������λ����ʾ�ַ�����
void LCD1602_Init      (void);  //LCD1602��ʼ������