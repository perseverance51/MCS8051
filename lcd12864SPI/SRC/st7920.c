//ST7920 LCD ����
#include "st7920.h"
#include "Software_SPI.h"

void delay_ms(int i);

void st7920_delay()
{

}

void LCD12864_CS_H()	//��SPI������ֻ����Ļ����ɽ�VCC
{
	LCD12864_CS = 1;
}

void LCD12864_CS_L()
{
	LCD12864_CS = 0;
}

void LCD12864_PSB_L()	//��GND����͵�ƽ��ʵ��SPIͨѶ
{
	LCD12864_PSB = 0;
}

void LCD12864_RS_H()
{
	RS_Pin = 1;
}

void LCD12864_RS_L()
{
	RS_Pin = 0;
}

void LCD12864_WR_H()
{
	WR_Pin = 1;
}

void LCD12864_WR_L()
{
	WR_Pin = 0;
}

void LCD12864_EN_H()
{
	EN_Pin = 1;
}

void LCD12864_EN_L()
{
	EN_Pin = 0;
}

uint8_t LCD12864_Port_Read()
{
	return LCD12864_Port;
}

//����ģʽ��д��λ
void LCD12864_Port_Write_8B(uint8_t Data)
{
	LCD12864_Port = Data;
}

//4λģʽ��д����ֽڣ���4λ��
void LCD12864_Port_Write_4B(uint8_t Data)
{
	LCD12864_Port &= 0x0F;
	LCD12864_Port |= Data;
}

//��ʼ��
void LCD_init(void)
{
#if LCD_INTERFACE == MODE_SPI
	LCD12864_PSB_L();
#endif
    delay_ms(50);
    LCD_write_command(0x33);
    delay_ms(1);
    LCD_write_command(0x32);
    delay_ms(1);
#if LCD_INTERFACE == MODE_4BIT
    LCD_write_command(0x28);
    delay_ms(1);
    LCD_write_command(0x28);
    delay_ms(1);
#endif
    LCD_write_command(0x08);  //��ʾ��
    delay_ms(1);             //����100uS����ʱ����
    LCD_write_command(0x10);  //�������
    delay_ms(1);             //����100uS����ʱ����
    LCD_write_command(0x0C);  //��ʾ��
    delay_ms(1);             //����100uS����ʱ����
    LCD_write_command(0x01);  //Display Clear
    delay_ms(10);            //����10mS����ʱ����
    LCD_write_command(0x06);  //�����������1λ�ƶ�
    delay_ms(10);            //����10mS����ʱ����
}

//д����
void LCD_write_command(unsigned char command)
{
#if LCD_INTERFACE == MODE_SPI
    LCD2_spi_write_byte(0xf8);
	st7920_delay();
    LCD2_spi_write_byte_standard(command);
#else
    LCD12864_RS_L();   //RS=0
    LCD_write_byte(command);
    delay_ms(1);
#endif
}

//д����
void LCD_write_data(unsigned char Data)
{
#if LCD_INTERFACE == MODE_SPI
    LCD2_spi_write_byte(0xfa);
	st7920_delay();
    LCD2_spi_write_byte_standard(Data);
#else
    LCD12864_RS_H();    //RS=1
    LCD_write_byte(Data);
#endif
}

//����ģʽдһ���ֽڵ�������
void LCD_write_byte(unsigned char Data)
{
#if LCD_INTERFACE == MODE_4BIT
    LCD_write_half_byte(Data);
    Data <<= 4;
    LCD_write_half_byte(Data);
#else
	LCD12864_Port_Write_8B(Data);
    LCD12864_WR_L();
    LCD12864_EN_H();     //EN�˲���һ���ɵ͵�ƽ��ߵ�ƽ��дLCD
    st7920_delay();
    LCD12864_EN_L();      //EN�˲���һ���ɸߵ�ƽ��͵�ƽ��дLCD
    st7920_delay();
    LCD12864_WR_H();
#endif
}

//4λģʽ��д����ֽڣ���4λ��
void LCD_write_half_byte(unsigned char half_byte)
{
	LCD12864_Port_Write_4B(half_byte);
    LCD12864_WR_L();
    LCD12864_EN_H();         //EN�˲���һ���ɵ͵�ƽ��ߵ�ƽ��дLCD
    st7920_delay();
    LCD12864_EN_L();          //EN�˲���һ���ɸߵ�ƽ��͵�ƽ��дLCD
    st7920_delay(); 
    LCD12864_WR_H();
}

//����ģʽ��дһ���ֽ�
void LCD2_spi_write_byte(unsigned char Data)
{
	LCD12864_CS_H();
	SOFT_SPI_RW_MODE3(Data);
	LCD12864_CS_L();
}

//��׼����һ���ֽ�Ҫ��������ֽڷ���
void LCD2_spi_write_byte_standard(unsigned char Data)
{
    LCD2_spi_write_byte(Data & 0xf0);
    LCD2_spi_write_byte((Data << 4) & 0xf0);
}

//����ģʽ�¶�����
unsigned char LCD_read_data(void)
{
    unsigned char Data;

    LCD12864_RS_H();
    Data = LCD_read_byte();

#if LCD_INTERFACE == MODE_4BIT
    Data = (Data & 0xf0) | ((LCD_read_byte() >> 4) & 0x0f);
#endif
    return Data;
}

//����ģʽ�¶�״̬
unsigned char LCD_read_status(void)
{
    unsigned char Data;
    LCD12864_RS_L();
    Data = LCD_read_byte();
#if LCD_INTERFACE == MODE_4BIT
    Data = (Data & 0xf0) | ((LCD_read_byte() >> 4) & 0x0f);
#endif
    return Data;
}

//����ģʽ�¶�һ���ֽڣ������4bitģʽ����ֻ�и�4λ����Ч��
unsigned char LCD_read_byte(void)
{
    unsigned char Data;
    LCD12864_WR_H();
    
    LCD12864_EN_L();         //EN�˲��������أ�ST7920��ʼ�������
    st7920_delay();
    LCD12864_EN_H();
    st7920_delay();

    Data = LCD12864_Port_Read();
    LCD12864_EN_L();
    return Data;
}

//�����ַ�ģʽ�ĵ�ַ
//��һ���׵�ַ��0x80
//�ڶ����׵�ַ��0x80+0x10 (0x90)
//�������׵�ַ��0x80+0x08 (0x88)
//�������׵�ַ��0x80+0x10+0x08 (0x98)
void LCD_set_text_address(unsigned int rowCol)
{
    unsigned char start = 0x80;
    unsigned char row = (rowCol >> 8) & 0xff;
    unsigned char col = rowCol & 0xff;

    if (row == 1) {
        start = 0x90;
    }
    if (row == 2) {
        start = 0x88;
    }
    if (row == 3) {
        start = 0x98;
    }
    LCD_write_command(start + col);
}

//���ַ�ģʽ�����к�ת��Ϊ�ڲ���XY���꣬���صĸ��ֽ�ΪX�����ֽ�ΪY
//row (HIGH_BYTE(rowCol)): �кţ�0-3
//col (LOW_BYTE(rowCol)): �кţ�0-7
unsigned int LCD_rowCol_to_inter_Xy(unsigned int rowCol)
{
    unsigned char row = (rowCol >> 8) & 0x03;
    unsigned char col = rowCol & 0x07;
    unsigned char x = col + 8 * (unsigned char)(row / 2);
    unsigned char y = (row * 16) & 0x1f;
    return (x << 8) | y;
}

//���û�ͼģʽ�ĵ�ַ
//x: 0-127 (ע��x����Ϊ16��������)
//y:0-63
void LCD_set_graphic_address(unsigned char x, unsigned char y)
{
    unsigned char xWord, downPage, yInter;

    x &= 0x7f;
    y &= 0x3f;
    xWord = x / 16;           //�ڲ�X��ַ��һ����ַ��16λ
    downPage = y / 32;        //0:�ϰ��� 1:�°���
    yInter = y & 0x1f;        //�ڲ�Y����
    
    LCD_write_command(0x80 + yInter); //������Y��ַ
    LCD_write_command(0x80 + xWord + 8 * downPage);
}

//��ʾ��������
void LCD_clear(void)
{
    unsigned char x, y;
    LCD_write_command(0x08);  //����������������˸���ȹ���ʾ
    st7920_delay();
    LCD_write_command(0x01);  //��DDRAM

    //��GDRAM,12864������һ���GDRAM����һ�뼴��
    //���û��ʹ�õ���ͼGDRAM��Ҳ���Բ���GDRAM��ʡ��ʱ��ʹ���ռ�
    LCD_startGraphic();
    for (y = 0; y < 32; y++)
    {
        LCD_write_command(0x80 + y); //y
        LCD_write_command(0x80 + 0); //x
        for (x = 0; x < 16; x++)
        {
            LCD_write_data(0x00);
            LCD_write_data(0x00);
        }
    }
    LCD_endGraphic();

    LCD_write_command(0x0C);  //��ʾ��
    delay_ms(10);
}

//������չ����Ļ�ͼָ��
void LCD_startGraphic(void)
{
#if LCD_INTERFACE == MODE_4BIT
    LCD_write_command(0x24);          //��չָ�
    LCD_write_command(0x26);          //��ͼ�����
#else
    LCD_write_command(0x34);          //��չָ�
    LCD_write_command(0x36);          //��ͼ�����
#endif
}

//������չ����Ļ�ͼָ��(��ѡ)
void LCD_endGraphic(void)
{
#if LCD_INTERFACE == MODE_4BIT
    LCD_write_command(0x26); //��ͼ����ر�
    LCD_write_command(0x20); //�ص�����ָ��
#else
    LCD_write_command(0x36); //��ͼ����ر�
    LCD_write_command(0x30); //�ص�����ָ��
#endif
}

//���׻�ȡ�����׶�Ӧ16X16����һ�����ڲ˵�ѡ��
//row (HIGH_BYTE(rowCol)): �кţ�0-3
//col (LOW_BYTE(rowCol)): �кţ�0-7
//charNum: Ҫ���׵��ַ�������ע�ⲻҪ������ĩ
//ST7920���õķ������ò�����ֻ�����з��ף����ҷ��׵�һ�У�������Ҳ��һ�𷴰�
//���Դ˺���ʹ������ķ���ʵ�֣���ͼ����Ӧ��λȫ��д0xff�����󼴿ɷ���
void LCD_Inverse_16X16(unsigned int rowCol, unsigned char charNum, unsigned char reverse)
{
    unsigned char i, ch;
    unsigned int xy = LCD_rowCol_to_inter_Xy(rowCol);
    unsigned char x = (xy >> 8) & 0xff;
    unsigned char y = xy & 0x3f;

    LCD_startGraphic();
    for (i = 0; i < 16; i++) //�Ѷ�Ӧ�ַ���GDRAMȫ��д��0xff��������Ƿ���
    {
        LCD_write_command(0x80 + y + i);   //��д��ֱ��ַy
        LCD_write_command(0x80 + x);       //x
        for (ch = 0; ch < charNum; ch++)
        {
            LCD_write_data(reverse ? 0xff : 0x00);
            LCD_write_data(reverse ? 0xff : 0x00);
        }
    }
    LCD_endGraphic();
}

//��ʾһ���ַ���12864������ʾ4�У�ÿ��8�����ֻ�ÿ��16����ĸ
//col: �кţ�0-15
//row: �кţ�0-3
//�ַ����룺С��0x80��ΪASCII������Ϊ���֣����ֱ�����ֽ������
void LCD_write_char(unsigned int rowCol, unsigned int Code)
{
    unsigned char high = (Code >> 8) & 0xff;
    unsigned char low = Code & 0xff;
    LCD_set_text_address(rowCol);
    if (Code > 0x80)
    {
        LCD_write_data(high);
    }
    LCD_write_data(low);
}

//��ʾһ���ַ�����ע�ⲻҪ����һ�еĳ��ȣ�8�����ֻ�16����ĸ��
//rowCol: ��8λΪ��(0-3)����8λΪ��(0-15)
void LCD_write_string(unsigned int rowCol, const char * p)
{
    LCD_set_text_address(rowCol);
    while (*p != 0)
    {
        LCD_write_data(*p);
        p++;
    }
}

void LCD_Draw_Bmp(unsigned char *pImage, unsigned char col, unsigned char row, unsigned char x0, unsigned char y0)
{
    unsigned char i, y;

    LCD_startGraphic();
    
    for (y = y0; y < row + y0; y++)
    {
        LCD_set_graphic_address(x0, y);
        for (i = 0; i < col/8; i++)
        {
            LCD_write_data(*pImage);
            pImage++;
        }
    }
    
    LCD_endGraphic();
}

//ֻ�в��нӿ��ܶ�ST7920��������нӿ���Ҫ��㣬����Ҫ����RAM������
#if LCD_INTERFACE != MODE_SPI
//�ڶ�ӦX/Yλ����ʾһ����
void LCD_write_dot(unsigned char x, unsigned char y)
{
    unsigned char xBit, high, low;

    xBit = x & 0x0f;
    LCD_startGraphic();
    LCD_set_graphic_address(x, y);
    LCD_read_data();    //�����ֲᣬ�������ַ��ĵ�һ�ζ��������ص�������Ч

    high = LCD_read_data();
    low = LCD_read_data();

    //��д
    LCD_set_graphic_address(x, y);
    if (xBit < 8)   //��8λ
    {
        LCD_write_data(high | (0x01 << (7 - xBit)));
        LCD_write_data(low);
    }
    else
    {
        LCD_write_data(high);
        LCD_write_data(low | (0x01 << (15 - xBit)));
    }
    LCD_endGraphic();
}
#endif

