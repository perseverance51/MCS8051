import logging
import sys
import time
import math
import smbus
import RPi.GPIO

I2C_Addr = 0x51


CTRL_BUF1 = 0x00
CTRL_BUF2 = 0x01

SECOND_DATA_BUF = 0x02
MINUTE_DATA_BUF = 0x03
HOUR_DATA_BUF = 0x04

DAY_DATA_BUF = 0x05
WEEK_DATA_BUF = 0x06
MONTH_DATA_BUF = 0x07
YEAR_DATA_BUF = 0x08

MINUTE_AE_BUF = 0x09
HOUR_AE_BUF = 0x0A
DAY_AE_BUF = 0x0B
WEEK_AE_BUF = 0x0C

CLK_FRQ_BUF = 0x0D
TIMER_CTRL_BUF = 0x0E
COUNT_VAL_BUF = 0x0F



TIMER_FREQUENCY_4096 = 0 # 4096HZ
TIMER_FREQUENCY_64 = 1 # 64HZ
TIMER_FREQUENCY_1 = 2 # 1HZ
TIMER_FREQUENCY_1_60 = 3 # 1/60Hz



class PCF8563:
    INT_PIN  = 4
    def __init__(self,address = I2C_Addr):
        self.bus = smbus.SMBus(1)
        self.address = address#0x20
        self.GPIO = RPi.GPIO
        self.GPIO.setmode(self.GPIO.BCM)
        self.GPIO.setwarnings(False)
        self.GPIO.setup(self.INT_PIN, self.GPIO.IN)
        
    def changeIntToHex(self,dec):
        return ((((int)((dec)/10)) <<4) + ((dec)%10) )
    def converseIntToHex(self,dec): 
        return (((dec)>>4) *10) + ((dec)%16) 
    
    def changeHexToInt(self,hex):
        return ((((hex)>>4) *10 ) + ((hex)%16) )
    def converseHexToInt(self,hex):
        return (((int)((hex)/10)) <<4 ) + ((hex)%10) 
        
    def write(self, reg, value):
        "Writes an 8-bit value to the specified register/address"
        self.bus.write_byte_data(self.address, reg, value)
      
    def read(self, reg):
        "Read an unsigned byte from the I2C device"
        result = self.bus.read_byte_data(self.address, reg)
        return result
    
    """***************************************************************************
    function: Init
    parameter:
    Info:
    ****************************************************************************"""
    def Init(self):
        self.write(0x00,0x00)  #basic setting
        self.write(0x01,0x00)  #Disable INT
        self.Timer_Disable()
        self.Alarm_Disable()

    """***************************************************************************
    function: Set  Time
    parameter:
        hour:0~23
        minute:0~60
        second:0~60
    Info:
    ****************************************************************************"""
    def Set_Time(self, hour, minute, second):
        if(hour>=0):
            hour = self.changeIntToHex(hour%60)
            self.write(HOUR_DATA_BUF, hour)
        
        if(minute>=0):
            minute = self.changeIntToHex(minute%60)
            self.write(MINUTE_DATA_BUF, minute)
        
        if(second>=0):
            second = self.changeIntToHex(second%60)
            self.write(SECOND_DATA_BUF, second)

    """***************************************************************************
    function: Set  date
    parameter:
        year:1900~2099
        months:1~12
        days:0~31
    Info:
    ****************************************************************************"""
    def Set_Days(self, year, months, days):

        if(days >= 0 and days <= 31):
            days = self.changeIntToHex(days)
            self.write(DAY_DATA_BUF, days)
        
        if(months>=0 and months <= 12 ):
            months = self.changeIntToHex(months)
            self.write(MONTH_DATA_BUF, (self.read(MONTH_DATA_BUF)&0x80)| months)
        
        
        if(year>=1900 and year<2000):
            self.write(MONTH_DATA_BUF, self.read(MONTH_DATA_BUF) | 0x80)
            """
                7  century this bit is toggled when the years register
                overflows from 99 to 00
                0 indicates the century is 20xx
                1 indicates the century is 19xx
            """
            self.write(YEAR_DATA_BUF, year%100)
        elif(year>= 2000 and year < 3000):
            self.write(MONTH_DATA_BUF, self.read(MONTH_DATA_BUF) & 0x7F)
            self.write(YEAR_DATA_BUF, year%100)


    """***************************************************************************
    function: Get date
    parameter:
        buf: Data buffer
    Info:
    ****************************************************************************"""
    def Get_Days(self):
        buf = [0x00] * 4
        buf[0] = self.read(DAY_DATA_BUF)&0x3f
        buf[1] = self.read(MONTH_DATA_BUF) & 0x1f
        buf[2] = self.read(YEAR_DATA_BUF)&0xff

        buf[0] = self.changeHexToInt(buf[0])
        buf[1] = self.changeHexToInt(buf[1])
        

        if(self.read(MONTH_DATA_BUF) & 0x80):
            buf[3] = 19
        else:
            buf[3] = 20
        return buf


    """***************************************************************************
    function: Set Week Data
    parameter:
        WeekData:0~7
    Info:
    ****************************************************************************"""
    def Set_WeekData(self, WeekData):
        if(WeekData <= 7):
            self.write(WEEK_DATA_BUF, WeekData)
        

    """***************************************************************************
    function: Get Time
    parameter:
        buf: Data buffer
    Info:
    ****************************************************************************"""
    def Get_Time(self):

        buf = [0x00] * 3
        buf[0] = self.read(SECOND_DATA_BUF) & 0x7f #get second data
        buf[1] = self.read(MINUTE_DATA_BUF) & 0x7f #get minute data
        buf[2] = self.read(HOUR_DATA_BUF) & 0x3f #get hour data

        buf[0] = self.changeHexToInt(buf[0])
        buf[1] = self.changeHexToInt(buf[1])
        buf[2] = self.changeHexToInt(buf[2])
        return buf


    """***************************************************************************
    function: Set alarm parameters
    parameter:
        hour: 0~23
        minute: 0~59
    Info:
    ****************************************************************************"""
    def Set_Alarm(self, hour, minute):
        if(minute >= 0):
            minute = self.changeIntToHex(minute)
            self.write(MINUTE_AE_BUF, minute)
        
        if(hour >= 0):
            hour = self.changeIntToHex(hour) 
            self.write(HOUR_AE_BUF, hour)
        
     

    """***************************************************************************
    function: Enable Alarm
    parameter:
    Info:
    ****************************************************************************"""
    def Alarm_Enable(self):
        self.write(0x01, (self.read(0x01)|0x02))
        self.write(MINUTE_AE_BUF, self.read(MINUTE_AE_BUF)&0x7f)
        self.write(HOUR_AE_BUF, self.read(HOUR_AE_BUF)&0x7f)
        self.write(DAY_AE_BUF,0x80)#
        self.write(WEEK_AE_BUF,0x80)#


    """***************************************************************************
    function: Disable Alarm
    parameter:
    Info:
    ****************************************************************************"""
    def Alarm_Disable(self):

        self.write(0x01, (self.read(0x01)&0xfd))
        self.write(MINUTE_AE_BUF, self.read(MINUTE_AE_BUF)|0x80)
        self.write(HOUR_AE_BUF, self.read(HOUR_AE_BUF)|0x80)
        self.write(DAY_AE_BUF,0x80)#
        self.write(WEEK_AE_BUF,0x80)#




    """***************************************************************************
    function: Set timer register
    parameter:
        Timer_Frequency : Choose the corresponding frequency
                        4096    :TIMER_FREQUENCY_4096
                        64      :TIMER_FREQUENCY_64
                        1       :TIMER_FREQUENCY_1
                        0       :TIMER_FREQUENCY_1_60
        Value           : Value
                        Total cycle = Value/TIMER_FREQUENCY
    Info:
        TIMER_CTRL_BUF#0x0E
        TIMER_FREQUENCY_4096    0 # 4096HZ      MAX  0.062 second
        TIMER_FREQUENCY_64      1 # 64HZ        MAX  3.98 second
        TIMER_FREQUENCY_1       2 # 1HZ         MAX  255 second
        TIMER_FREQUENCY_1_60    3 # 1/60Hz      MAX  255 minute
    ****************************************************************************"""
    def Set_Timer(self, Timer_Frequency , Value):
        self.write(COUNT_VAL_BUF, Value)
        if(Timer_Frequency == 4096):
            self.write(TIMER_CTRL_BUF, ((self.read(TIMER_CTRL_BUF))&0xfc)|TIMER_FREQUENCY_4096)
        elif(Timer_Frequency == 64):
            self.write(TIMER_CTRL_BUF, ((self.read(TIMER_CTRL_BUF))&0xfc)|TIMER_FREQUENCY_64)
        elif(Timer_Frequency == 1):
            self.write(TIMER_CTRL_BUF, ((self.read(TIMER_CTRL_BUF))&0xfc)|TIMER_FREQUENCY_1)
        elif(Timer_Frequency == 0):# 1/60
            self.write(TIMER_CTRL_BUF, ((self.read(TIMER_CTRL_BUF))&0xfc)|TIMER_FREQUENCY_1_60)
        else:
            print("Set Timer Error\r\n")
        

    """***************************************************************************
    function: Enable timer
    parameter:
    Info:
    ****************************************************************************"""
    def Timer_Enable(self):
        self.write(0x01, (self.read(0x01)|0x01))
        self.write(TIMER_CTRL_BUF, self.read(TIMER_CTRL_BUF)|0x80)


    """***************************************************************************
    function: Disable timer
    parameter:
    Info:
    ****************************************************************************"""
    def Timer_Disable(self):
        self.write(0x01, (self.read(0x01)&0xfe))
        self.write(TIMER_CTRL_BUF, self.read(TIMER_CTRL_BUF)&0x7f)

    """***************************************************************************
    function: Clear alarm interrupt flag
    parameter:
    Info:
    ****************************************************************************"""
    def Cleare_AF_Flag(self):
        self.write(CTRL_BUF2, self.read(CTRL_BUF2)&0xf7)

    """***************************************************************************
    function: Clear timer interrupt flag
    parameter:
    Info:
    ****************************************************************************"""
    def Cleare_TF_Flag(self):
        self.write(CTRL_BUF2, self.read(CTRL_BUF2)&0xfB)


    """***************************************************************************
    function: Get Flag
    parameter:
    Info:
        return: 1:AF alarm
                2:TF timer
                3:AF and TF
    ****************************************************************************"""
    def Get_Flag(self):
        temp=0
        if(self.read(CTRL_BUF2)&0x08):
            temp  =  temp|0x01
        
        if(self.read(CTRL_BUF2)&0x04):
            temp  =  temp|0x02
        
        return temp
        
    """***************************************************************************
    function: Set timer register
    parameter:
        Timer_Frequency : Choose the corresponding frequency
                        32768   :327.68KHz
                        1024    :1024Hz
                        32      :32Hz
                        1       :1Hz
        Value           : Value
                        Total cycle = Value/TIMER_FREQUENCY
    Info:

    ****************************************************************************"""
    def CLKOUT_FrequencyHZ(self, Frequency):
        if(Frequency == 32768):
            self.write(CLK_FRQ_BUF, (self.read(CLK_FRQ_BUF)&0xfC)|0x00)
        elif(Frequency == 1024):
            self.write(CLK_FRQ_BUF, (self.read(CLK_FRQ_BUF)&0xfC)|0x01)
        elif(Frequency == 32):
            self.write(CLK_FRQ_BUF, (self.read(CLK_FRQ_BUF)&0xfC)|0x02)
        elif(Frequency == 1):
            self.write(CLK_FRQ_BUF, (self.read(CLK_FRQ_BUF)&0xfC)|0x03)
        else:
            print("Set CLKOUT requency Selection Error\r\n")
            
    """***************************************************************************
    function: Enable CLKOUT
    parameter:
    Info:
    ****************************************************************************"""
    def CLKOUT_Enable(self):
        self.write(CLK_FRQ_BUF, self.read(CLK_FRQ_BUF)|0x80)
    
    """***************************************************************************
    function: Disable CLKOUT
    parameter:
    Info:
    ****************************************************************************"""
    def CLKOUT_Disable(self):
        self.write(CLK_FRQ_BUF, self.read(CLK_FRQ_BUF)&0x7f)

    def Digital_Read_INT(self):
        return self.GPIO.input(self.INT_PIN)









