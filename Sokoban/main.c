#include<reg52.h>
#include<stdlib.h>
#include<stdio.h>
#include<intrins.h>

#define uint unsigned int
#define uchar unsigned char
 
sbit LCD_RS = P2^2;
sbit LCD_RW = P2^1;
sbit LCD_EN = P2^0;
sbit LCD_cs1 = P2^4;
sbit LCD_cs2 = P2^3;
 
#define LCD_data P1

char cur_x,cur_y,X,dir[4][2]={-1,0,1,0,0,-1,0,1},where[3][2]={4,6,5,6,6,6};
 
sbit key_up = P3^0;
sbit key_down = P3^1;
sbit key_left = P3^2;
sbit key_right = P3^3;
 
uchar code tmp[]={//空格
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};
/* 0表示空格，1表示墙，2表示人，3表示箱子，4表示目的地 */
uchar code mat[8][8]={
1,1,1,1,1,1,1,1,
1,1,1,1,0,2,1,1,
1,1,0,0,3,0,1,1,
1,1,0,1,0,1,1,1,
1,0,0,1,0,1,4,1,
1,0,1,0,0,3,4,1,
1,0,3,0,0,0,4,1,
1,1,1,1,1,1,1,1
};
 
uchar map[8][8];
 
uchar code Qiang[]={//方格，有间隙
0xff,0x81,0x81,0x81,0x81,0x81,0x81,0xff
};
 
uchar code Ren[]={//人
0xff,0xff,0xe3,0xc5,0xb1,0xc5,0xe3,0xff
};
 
uchar code Xiang[]={//箱子  
0xff,0x42,0x24,0x18,0x18,0x24,0x42,0xff  
};
 
uchar code Mudi[]={//目的地  
0xff,0xff,0xe7,0x81,0x81,0xe7,0xff,0xff  
}; 
 
uchar code Zifu[][8]={//字符表 
0xFF,0xFD,0xF9,0x87,0xB3,0xFD,0xFD,0xFF,	//Y
0xFF,0xFF,0xC3,0xBD,0xBD,0xC3,0xFF,0xFF,	//O
0xFF,0xFF,0xC3,0xBF,0xBF,0xC3,0xFF,0xFF,	//U
 
0xFF,0xF1,0xCF,0xF1,0xF5,0xC3,0xFD,0xFF,	//W
0xFF,0xFF,0xFF,0x81,0xBD,0xFF,0xFF,0xFF,	//I
0xFF,0x81,0xBB,0xF7,0xEF,0xC1,0xFD,0xFF,	//N
0xFF,0xFF,0xA1,0xFF,0xFF,0xFF,0xFF,0xFF,		//!
 
0xF7,0xF7,0xF7,0x00,0x77,0xD7,0xEF,0x00,	// 7
0xB5,0xB7,0xB6,0x01,0xB7,0xB7,0xB7,0xFF,
0xFD,0xBD,0x7E,0x80,0xFF,0xFF,0xFF,0x00,
0xBB,0xBB,0xBB,0x80,0xBB,0xBB,0xBB,0xFF,	/*"推"*/
 
0xEF,0x77,0x78,0x7D,0x09,0x75,0x7D,0xED,	// 11
0x17,0xD8,0xDD,0xD9,0xC5,0x1D,0xFD,0xFF,
0xDF,0xEF,0xF3,0xFC,0x00,0xFD,0xF3,0xFF,
0x00,0xB6,0xB6,0xB6,0xB6,0x00,0xFF,0xFF,	/*"箱"*/
 
0xFF,0xFF,0xFD,0xFD,0xFD,0xFD,0xFD,0x1D,	// 15
0xED,0xF5,0xF9,0xFD,0xFF,0x7F,0xFF,0xFF,
0xFE,0xFE,0xFE,0xFE,0xFE,0xBE,0x7E,0x80,
0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFF,	/*"子"*/
 
0xEF,0xF7,0xFB,0x2C,0xA9,0xA5,0xAD,0xA5,	// 19
0x0B,0xAC,0xA9,0xA5,0x8D,0xFD,0xFD,0xFF,
0xFF,0xDF,0xDF,0xDC,0xED,0xED,0xF5,0xF9,
0x00,0xFD,0xFD,0xED,0xDD,0xE1,0xFF,0xFF,	/*"第"*/
 
0xFF,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,	// 23
0x7F,0x7F,0x7F,0x7F,0x7F,0x3F,0x7F,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,	/*"一"*/
 
0xFF,0xEF,0xEF,0xEF,0xEE,0xE1,0xEB,0x0F,	// 27
0xEF,0xE7,0xE8,0xED,0xE7,0xEF,0xFF,0xFF,
0xFE,0x7E,0xBE,0xDE,0xEE,0xF6,0xFA,0xFC,
0xFA,0xF6,0xCE,0x9E,0x3E,0xBE,0xFE,0xFF		/*"关"*/                                                                                   
};
 
void delay_1ms(uchar x){
	uchar i,j;
	for(j=0;j<x;j++)
		for(i=0;i<110;i++);
}
 
void write_command(uchar command){
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EN = 0;
	LCD_data = command;
	delay_1ms(2);
	LCD_EN = 1;	 		//EN 由1 -- 0 完成 有效数据 的 锁存
	delay_1ms(2);
	LCD_EN = 0;
}
 
void write_data(uchar fuck){
	LCD_RS = 1;
	LCD_RW = 0;
	LCD_EN = 0;
	LCD_data = fuck;
	delay_1ms(2);
	LCD_EN = 1;
	delay_1ms(2);
	LCD_EN = 0;
}
 
void lcd_post(int X,int Y){
	write_command(0xb8+X);
	write_command(0x40+Y*8);
}
/* 0表示空格，1表示墙，2表示人，3表示箱子，4表示目的地 */
uchar judge(int x,int y,int id){							//id 表示方向数组的行标，0,1,2,3 分别表示上下左右
	int xx,yy,xxx,yyy;
	xx=x+dir[id][0]; yy=y+dir[id][1];
	if(map[xx][yy]==0 || map[xx][yy]==4)return 1;			   //1表示前面是 空格 或者 目的地，就是可以直接移动
	else if(map[xx][yy]==1)return 0;						   //0表示无法移动
	else if(map[xx][yy]==3){
		xxx=xx+dir[id][0]; yyy=yy+dir[id][1];
		if(map[xxx][yyy]==1 || map[xxx][yyy]==3)return 0;
		else if(map[xxx][yyy]==0 || map[xxx][yyy]==4)return 2; //2表示需要间接移动，先把前面的箱子移动一，再让人移动一
	}return 0;
}
 
void lcd_init(){
	uchar i,j;	

	LCD_cs1 = 1; //刚开始关闭两屏
	LCD_cs2 = 1;
 	delay_1ms(100);
	
	write_command(0x30);
	write_command(0x0c);
	write_command(0x01);
 
	cur_x=1; cur_y=5;
	X=3;
	
	for(i=0;i<8;i++){
		for(j=0;j<8;j++)map[i][j]=mat[i][j];
	}
}
 
void show_map(){
	uchar i,j,k;
	LCD_cs1=0; LCD_cs2=1;
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			lcd_post(i,j);
			if(map[i][j]==0)
				for(k=0;k<8;k++)write_data(tmp[k]);
			else if(map[i][j]==1)
				for(k=0;k<8;k++)write_data(Qiang[k]);
			else if(map[i][j]==2)
				for(k=0;k<8;k++)write_data(Ren[k]);
			else if(map[i][j]==3)
				for(k=0;k<8;k++)write_data(Xiang[k]);
			else if(map[i][j]==4)
				for(k=0;k<8;k++)write_data(Mudi[k]);
		}
	}delay_1ms(10);
	LCD_cs1=!LCD_cs1; LCD_cs2=!LCD_cs2;
 
	lcd_post(1,1); for(i=0;i<8;i++)write_data(Zifu[7][i]);
	lcd_post(1,2); for(i=0;i<8;i++)write_data(Zifu[8][i]);
	lcd_post(2,1); for(i=0;i<8;i++)write_data(Zifu[9][i]);
	lcd_post(2,2); for(i=0;i<8;i++)write_data(Zifu[10][i]);	//推
 
	lcd_post(1,3); for(i=0;i<8;i++)write_data(Zifu[11][i]);
	lcd_post(1,4); for(i=0;i<8;i++)write_data(Zifu[12][i]);
	lcd_post(2,3); for(i=0;i<8;i++)write_data(Zifu[13][i]);
	lcd_post(2,4); for(i=0;i<8;i++)write_data(Zifu[14][i]);	//箱
 
	lcd_post(1,5); for(i=0;i<8;i++)write_data(Zifu[15][i]);
	lcd_post(1,6); for(i=0;i<8;i++)write_data(Zifu[16][i]);
	lcd_post(2,5); for(i=0;i<8;i++)write_data(Zifu[17][i]);
	lcd_post(2,6); for(i=0;i<8;i++)write_data(Zifu[18][i]);	//子
 
	LCD_cs1=!LCD_cs1; LCD_cs2=!LCD_cs2;
	delay_1ms(100);
}
 
void fun(uchar key,uchar dirction){
	uchar x,y,i;
/* 0表示空格，1表示墙，2表示人，3表示箱子，4表示目的地 */
	if(key==1){
		lcd_post(cur_x,cur_y);
		if(mat[cur_x][cur_y]==4) for(i=0;i<8;i++)write_data(Mudi[i]);
		else for(i=0;i<8;i++)write_data(tmp[i]);
		map[cur_x][cur_y]=0;
					
		cur_x+=dir[dirction][0]; cur_y+=dir[dirction][1];
		lcd_post(cur_x,cur_y);
		for(i=0;i<8;i++)write_data(Ren[i]);
		map[cur_x][cur_y]=2;
					
	}
	else if(key==2){
		lcd_post(cur_x,cur_y);
		if(mat[cur_x][cur_y]==4) for(i=0;i<8;i++)write_data(Mudi[i]);
		else for(i=0;i<8;i++)write_data(tmp[i]);
		map[cur_x][cur_y]=0;
					
		cur_x+=dir[dirction][0]; cur_y+=dir[dirction][1];
		lcd_post(cur_x,cur_y);
		for(i=0;i<8;i++)write_data(Ren[i]);
		map[cur_x][cur_y]=2;
 
		x=cur_x+dir[dirction][0]; y=cur_y+dir[dirction][1];
		lcd_post(x,y);
		for(i=0;i<8;i++)write_data(Xiang[i]);
		map[x][y]=3;
	}
}
 
void main()
{
	uchar i,num,index=6;
	lcd_init();
	
	show_map();
 
	while(1){
		num=0;
		for(i=0;i<3;i++){
			if(map[ where[i][0] ][ where[0][1] ]==3)num++;
			else break;
		}
		if(num==X)goto WIN;
		if(key_up==0){
			delay_1ms(5);
			if(key_up==0){
				index=judge(cur_x,cur_y,0);
				fun(index,0);
			}while(!key_up);
		}
		else if(key_down==0){
			delay_1ms(5);
			if(key_down==0){
				index=judge(cur_x,cur_y,1);
				fun(index,1);
			}while(!key_down);
		}
		else if(key_left==0){
			delay_1ms(5);
			if(key_left==0){
				index=judge(cur_x,cur_y,2);
				fun(index,2);
			}while(!key_left);
		}
		else if(key_right==0){
			delay_1ms(5);
			if(key_right==0){
				index=judge(cur_x,cur_y,3);
				fun(index,3);
			}while(!key_right);
		}
	}
WIN:
	LCD_cs1=!LCD_cs1; LCD_cs2=!LCD_cs2;
			
	lcd_post(5,0); for(i=0;i<8;i++)write_data(Zifu[0][i]);
	lcd_post(5,1); for(i=0;i<8;i++)write_data(Zifu[1][i]);
	lcd_post(5,2); for(i=0;i<8;i++)write_data(Zifu[2][i]);
 
	lcd_post(5,4); for(i=0;i<8;i++)write_data(Zifu[3][i]);
	lcd_post(5,5); for(i=0;i<8;i++)write_data(Zifu[4][i]);
	lcd_post(5,6); for(i=0;i<8;i++)write_data(Zifu[5][i]);
	lcd_post(5,7); for(i=0;i<8;i++)write_data(Zifu[6][i]);
	delay_1ms(100);
	while(1);
}