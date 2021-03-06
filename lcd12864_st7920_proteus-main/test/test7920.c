//ST7920的测试主程序，注意此文件要保存为GBK格式，否则汉字无法正常显示
#include <string.h>
#include <avr/pgmspace.h>
#include "st7920.h"

//liberty.bmp
//48x64
const unsigned char bmpData[] PROGMEM = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x01,0xE0,0x00,
0x00,0x00,0x00,0x01,0x20,0x00,0x00,0x00,0x00,0x03,0xE0,0x00,0x00,0x00,0x00,0x03,
0xF0,0x00,0x00,0x00,0x00,0x01,0xE0,0x00,0x00,0x00,0x00,0x01,0xC0,0x00,0x00,0x00,
0x00,0x01,0xE0,0x00,0x00,0x00,0x00,0x01,0xE1,0x80,0x00,0x00,0x00,0x01,0xF1,0x84,
0x00,0x00,0x00,0x01,0xF9,0x9C,0x00,0x00,0x00,0x00,0xFF,0xB8,0x00,0x00,0x00,0x00,
0xBF,0xFF,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0xDF,0xFC,0x00,0x00,
0x00,0x00,0xDF,0xFF,0x80,0x00,0x00,0x00,0xDC,0x7F,0x00,0x00,0x00,0x00,0xDE,0xFC,
0x00,0x00,0x00,0x00,0xDC,0xF0,0x00,0x00,0x00,0x01,0xFF,0x70,0x00,0x00,0x00,0x01,
0xE7,0xD8,0x00,0x00,0x00,0x01,0xB3,0xD8,0x00,0x00,0x00,0x00,0xE1,0xF0,0x00,0x00,
0x00,0x00,0xC1,0xF8,0x00,0x00,0x00,0x00,0x47,0xB8,0x00,0x00,0x00,0x00,0x4F,0xBC,
0x00,0x00,0x00,0x00,0x4B,0x1D,0xC0,0x00,0x00,0x00,0x7A,0x7F,0xE0,0x00,0x00,0x00,
0x36,0x6E,0x70,0x00,0x00,0x00,0x6C,0x6C,0xE0,0x00,0x00,0x00,0x6C,0x4C,0xE0,0x00,
0x00,0x00,0xD8,0xCC,0xC0,0x00,0x00,0x00,0xD8,0xC5,0xC0,0x00,0x00,0x00,0x70,0xC7,
0xC0,0x00,0x00,0x00,0x3C,0xC7,0x80,0x00,0x00,0x00,0x38,0xC7,0x80,0x00,0x00,0x00,
0x78,0xC7,0x00,0x00,0x00,0x00,0x70,0xC6,0x00,0x00,0x00,0x00,0x70,0xC6,0x00,0x00,
0x00,0x00,0x70,0xC6,0x00,0x00,0x00,0x00,0x30,0xC2,0x00,0x00,0x00,0x00,0x20,0xC2,
0x00,0x00,0x00,0x00,0x60,0xC2,0x00,0x00,0x00,0x00,0x60,0xC6,0x00,0x00,0x00,0x00,
0x60,0xFE,0x00,0x00,0x00,0x00,0x60,0xFE,0x00,0x00,0x00,0x00,0x60,0x7E,0x00,0x00,
0x00,0x00,0x60,0x7E,0x00,0x00,0x00,0x00,0x60,0xFE,0x00,0x00,0x00,0x00,0x63,0xFE,
0x00,0x00,0x00,0x00,0x67,0x7E,0x00,0x00,0x00,0x00,0x7F,0xFE,0x00,0x00,0x00,0x00,
0x7F,0xFE,0x00,0x00,0x00,0x00,0x7F,0xDF,0x00,0x00,0x00,0x00,0x5F,0xDF,0x00,0x00,
0x00,0x01,0xFF,0xFF,0x00,0x00,0x00,0x01,0x80,0x01,0x00,0x00,0x00,0x07,0xFF,0xFF,
0xC0,0x00,0x00,0x06,0xFF,0xFE,0xE0,0x00,0x00,0x04,0x00,0x00,0x60,0x00,0x00,0x07,
0xFF,0xFF,0xE0,0x00,0x00,0x07,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};


void drawBmp(void)
{
    unsigned char i, y;
    const unsigned char *p = bmpData;

    LCD_startGraphic();
    
    for (y = 0; y < 64; y++)
    {
        LCD_set_graphic_address(0, y);
        for (i = 0; i < 6; i++)
        {
            LCD_write_data(pgm_read_byte(p));
            p++;
        }
    }
    
    LCD_endGraphic();
}

int main(void)
{
    unsigned char row, prevRow;

#if LCD_INTERFACE != MODE_SERIAL
    unsigned char x, y;
#endif

    DDRD = 0xff;
    PORTD = 0x00;
    DDRB = 0xff;
    PORTB = 0x00;
    DDRC = 0xff;
    PORTC = 0x00;

    LCD_init();
    LCD_clear();
    LCD_write_string(0x0102, "CDHIGH设计");
    //画框，演示读ST7920，只有并行模式能读ST7920
    #if LCD_INTERFACE != MODE_SERIAL
        for (x = 31 - 3; x <= 112 + 3; x++)
        {
            LCD_write_dot(x, 16 - 3);
            LCD_write_dot(x, 31 + 3);
            //_delay_us(100);
        }
        for (y = 16 - 3; y <= 31 + 3; y++)
        {
            LCD_write_dot(31 - 3, y);
            LCD_write_dot(112 + 3, y);
            //_delay_us(100);
        }
    #endif
    _delay_ms(200);
    LCD_clear();

    prevRow = 0xff;
    while (1)
    {
        LCD_write_string(0x0003, "生命诚可贵");
        _delay_ms(200);
        LCD_write_string(0x0103, "爱情价更高");
        _delay_ms(200);
        LCD_write_string(0x0203, "若为自由故");
        _delay_ms(200);
        LCD_write_string(0x0303, "两者皆可抛");
        _delay_ms(200);
        drawBmp();
        _delay_ms(200);
        for (row = 0; row < 4; row++)
        {
            if (prevRow != 0xff)
            {
                LCD_Inverse_16X16(ROW_COL(prevRow, 3), 5, 0);
            }
            LCD_Inverse_16X16(ROW_COL(row, 3), 5, 1);
            prevRow = row;
            _delay_ms(200);
        }
        prevRow = 0xff;
        LCD_clear();
        _delay_ms(200);
    }

    return 1;
}
