//LCD12864(ST7920控制器)的PROTEUS仿真模型
//Author: cdhigh
//Repository: <https://github.com/cdhigh/lcd12864_st7920_proteus>
//Initial release: 2021-09-29
#ifdef LCD12864B_EXPORTS
#define LCD12864B_API __declspec(dllexport)
#else
#define LCD12864B_API __declspec(dllimport)
#endif

#include "vsm.hpp"

//LCD常量
#define LCD_WIDTH        128
#define LCD_HEIGHT       64
#define LCD_ROW_LEN      16

#define BLANK_WIDTH        50        //the width of blank
#define SYM_LINEWIDTH      28        //the width of symbol line

#define PIN_IS_HIGH(pin) (ishigh((pin)->istate()))
#define PIN_IS_LOW(pin)  (islow((pin)->istate()))
#define BYTE_BIT(bitno) (1 << (bitno))
#define TEST_BIT(value, bitno) (((1 << (bitno)) & (value)) != 0)
#define SET_BIT(value, bitno)  ((value) |= (1 << (bitno)))
#define CLR_BIT(value, bitno)  ((value) &= ~(1 << (bitno)))

#define BSIZE       512
void debugPrint(const char * text);
void debugPrint(const char * text, int val);
void debugPrint(const char * text, int val1, int val2);
void debugPrint(const char * text, int val1, int val2, int val3);
void debugPrint(const char * text, int val1, int val2, int val3, int val4);

#define ADDR_TYPE_INVALID 0
#define ADDR_TYPE_DDRAM   1
#define ADDR_TYPE_CGRAM   2
#define ADDR_TYPE_IRAM    3
#define ADDR_TYPE_GDRAM   4

#define HZCODE_GB2312     0
#define HZCODE_BIG5       1

#define SRL_STARTED_INVALID  0
#define SRL_STARTED_RW       1 //表示接下来接收RW
#define SRL_STARTED_RS       2 //表示接下来接收RS
#define SRL_STARTED_ZERO     3 //表示接下来的位应该为零
#define SRL_STARTED_CMD_DATA 4 //表示接下来可以正常接收命令和数据

extern "C" const unsigned char* getHzDot(BYTE codeHigh, BYTE codeLow);
extern "C" const unsigned char* binarySearchHzCode(BYTE codeHigh, BYTE codeLow);

//LCD元件既有数字电气特性，也有绘图特性，所以要继承IACTIVEMODEL和IDSIMMODEL
class LCD12864B_API LCD12864B : public IACTIVEMODEL, public IDSIMMODEL
{
public:
    LCD12864B(void);
    
    // 电气模型成员函数

    //数字电路总是返回TRUE
    INT isdigital (CHAR *pinname);

    //当创建模型实例时被调用，做初始化工作
    VOID setup (IINSTANCE *inst, IDSIMCKT *dsim);

    //仿真运行模式控制，交互仿真中每帧开始时被调用
    VOID runctrl (RUNMODES mode);
    
    //交互仿真时用户改变按键等的状态时被调用
    VOID actuate (REALTIME time, ACTIVESTATE newstate);

    //交互仿真时每帧结束时被调用，通过传递ACTIVEDATA数据与绘图模型通信，从而调用animate()进行绘图
    BOOL indicate (REALTIME time, ACTIVEDATA *data);

    //当引脚状态变化时被调用，主要用来处理数据输入和输出
    VOID simulate (ABSTIME time, DSIMMODES mode);

    //可通过setcallback()设置在给定时间调用的回调函数
    VOID callback (ABSTIME time, EVENTID eventid);


    // 绘图模型成员函数 

    //当创建模型实例时被调用，做初始化工作
    VOID initialize (ICOMPONENT *cpt);

    //被PROSPICE调用，返回模拟电气模型
    ISPICEMODEL *getspicemodel (CHAR *device);

    //被PROSPICE调用，返回数字电气模型
    IDSIMMODEL *getdsimmodel (CHAR *device);

    //当原理图需要重绘时被调用
    VOID plot (ACTIVESTATE state);

    //当相应的电气模型产生活动事件时被调用，常用来更新图形
    VOID animate (INT element, ACTIVEDATA *newstate);

    //用来处理键盘和鼠标事件
    BOOL actuate (WORD key, INT x, INT y, DWORD flags);

    void initVariables(void);
    void processBasicCmd(BYTE cmd);
    void processExtCmd(BYTE cmd);
    void parallelSimulate(ABSTIME time, DSIMMODES mode);
    void serialSimulate(ABSTIME time, DSIMMODES mode);
    void storeData(BYTE data);
    void outputData(ABSTIME time, DSIMMODES mode);
    inline void drawPixel(BYTE x, BYTE y);
    void drawCacheToScreen(void);

    inline int gdramXyToIndex(BYTE x, BYTE y) {return (y * 32 + x * 2) % sizeof(GDRAM);}
    inline int ddramAddrToIndex(BYTE addr) {return (addr * 2) % sizeof(DDRAM);}
    inline int cgramAddrToIndex(BYTE addr) {return (addr * 2) % sizeof(CGRAM);}

private:
    IINSTANCE *instance;    //PROSPICE仿真原始模型
    IDSIMCKT *ckt;            //DSIM的数字元件
    ICOMPONENT *component;    //PROTEUS内部一个活动组件对象
    BYTE hzCodePage;     //0-GB2312,1-BIG5

    //引脚定义
    IDSIMPIN *rs;    //RS
    IDSIMPIN *rw;    //R/W
    IDSIMPIN *en;    //E
    IDSIMPIN *psb;    //PSB
    IDSIMPIN *rst;    //RST
    IDSIMPIN *d[8];    //D0~D7
    IBUSPIN *databus;    //D[0..7]
    //串口模式引脚
    IDSIMPIN *cs;    //RS
    IDSIMPIN *sid;    //R/W
    IDSIMPIN *sclk;    //E

    BYTE   srlStarted;   //串行传输启动 =1表示接下来接收 RW，=2表示接下来接收 RS，=3表示接下来应该是零，=4表示接下来可以正常接收命令和数据
    BYTE   srlHighCnt; //连续5个1启动串行传输
    BYTE   srlByteIdx; //串行模式下提示接下来接收到比特位是一个字节的第几位
    BYTE   srlRw;
    BYTE   srlRs;
    BYTE   srlCmd;  //串口命令的暂存，连续两个字节各取高4位凑成一个字节赋值给currCmd
    BYTE   srlLow4Bit;
    BYTE   srlNextIsData; //指示接下来的是数据还是命令, 1-数据，2-命令

    BYTE   currCmd; //用于4位模式下暂存未接收完的命令
    BYTE   cmdComplete; //接收完成8位命令
    BYTE   nextCmdIsLow4Bit; //下次接收到命令是高4位还是低4位
    BYTE   outputLow4Bit;  //指示MCU读RAM内容输出高4位还是低4位

    //LCD参数
    BYTE currAddrType; //上次选择的地址类型，读写都用到
    BYTE ddramAddr;    //DDRAM地址
    BYTE cgramAddr;    //CGRAM地址
    BYTE gdramX; //GDRAM地址X
    BYTE gdramY; //GDRAM地址Y
    signed char incAddr; //设定地址自动增加还是自动减小
    BYTE status;    //状态（见手册）
    BYTE graphicOn; //绘图功能是否开启
    BYTE displayOn; //屏幕是否开启
    BYTE cursorOn;  //游标是否开启
    BYTE cursorInverse; //游标位置是否反白
    BYTE addr4BitMode; //是否是4比特模式
    BYTE extCmdMode;   //是否是扩充指令模式

    BYTE dataLowReceiving; //表示接下来接收到字节为高字节还是低字节，0-高字节，1-低字节
    BYTE gdramXReceiving; //表示接下来的字节为GDRAM地址X
    
    //GDRAM水平每行16个X地址（0-15），每个地址管水平两个字节，左为高位D15，右为低位D0，最多可控制256点
    //垂直有64行，Y地址范围0-63
    //如果要控制12864，仅使用上边一半的绘图RAM，分左右半屏，写地址先写Y地址，再写X地址，写地址命令最高位为1
    // 0   1  ...  7  8  9 ... 15
    // 1
    // .. 左半屏        右半屏 
    // 31
    BYTE displayMemCache[LCD_WIDTH * LCD_HEIGHT];  //为了实现GDRAM/DDRAM/CGRAM相互异或的关系(实际硬件的逻辑)，使用内存缓冲
    BYTE GDRAM[LCD_WIDTH * LCD_HEIGHT * 2 / 8];    //LCD绘图显示RAM
    BYTE DDRAM[64 * 2]; //字符显示RAM
    BYTE CGRAM[16 * 8]; //4个16x16自定义字符，每个字符占16*2=32个字节，第二个字符地址0x10，第三个0x20，第四个0x30
    BOOL newFlag;    //新数据到达标志

    //显示参数
    BOX lcdArea;    //LCD显示区域
    LONG lcdCenterX, lcdCenterY;
    float pixWidth, pixHeight;    //每象素对应矩形的宽和高
    BYTE dummyRead; //设定地址后的第一次读数据返回随机数
};

