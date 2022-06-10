import time
import sys
import os

libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_PCF8563 import PCF8563

logging.basicConfig(level=logging.INFO)


try:
    RTC = PCF8563.PCF8563()
    RTC.Init()
    RTC.Set_Time(23,59,58);#Time
    RTC.Set_Alarm(1,1);#Alarm
    RTC.Set_Days(2021,9,28);#Days
    RTC.Set_Timer(1,10);#Timer 10S
    
    RTC.CLKOUT_FrequencyHZ(1);
    
    RTC.Alarm_Enable();
    RTC.Timer_Enable();
    RTC.CLKOUT_Enable();
    buf1 = [0x00] * 3
    buf2 = [0x00] * 4
    while True:
        buf1 = RTC.Get_Time();
        buf2 = RTC.Get_Days();
        ch = str(((buf2[3]*100)+buf2[2])) + " Year " + str(buf2[1]) + " Month " + str(buf2[0]) +" Day " 
        ch = ch + str(buf1[2]) +":"+ str(buf1[1]) + ":" + str(buf1[0]) 
        print(ch)
        time.sleep(0.5);
        if(RTC.Digital_Read_INT() == 0):
            
            if(RTC.Get_Flag() == 1):
                print("Alarm !!!!!!!!!!!!!\r\n");
                RTC.Cleare_AF_Flag();
                
            elif(RTC.Get_Flag() == 2):
                print("Timer !!!!!!!!!!!!!\r\n");
                RTC.Cleare_TF_Flag();
                
            elif(RTC.Get_Flag() == 3):
                print("Timer and Alarm !!!!!!!!!!!!!\r\n");
                RTC.Cleare_TF_Flag();
                RTC.Cleare_AF_Flag();
            
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    RTC.Alarm_Disable();
    RTC.Timer_Disable();
    RTC.CLKOUT_Disable();
    exit()

