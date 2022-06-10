#include <PCF8563.h>

void setup() {
  // put your setup code here, to run once:
    UBYTE buf[10];
    PCF8563_Init();
    PCF8563_Set_Time(23,59,58);//Time
    PCF8563_Set_Alarm(0,0);//Alarm
    PCF8563_Set_Days(2021,9,28);//Days
    PCF8563_Set_Timer(1,10);//Timer 10S
    
    PCF8563_CLKOUT_FrequencyHZ(1);
    
    PCF8563_Alarm_Enable();
    PCF8563_Timer_Enable();
    PCF8563_CLKOUT_Enable();
    
    while(1){
        PCF8563_Get_Time(buf);
        PCF8563_Get_Days(&buf[3]);
        
        Serial.print(buf[6]);
        Serial.print(buf[5]);
        Serial.print(" Year ");
        Serial.print(buf[4]);
        Serial.print(" Month ");
        Serial.print(buf[3]);
        Serial.print(" Day ");
        Serial.print(buf[2]);
        Serial.print(" : ");
        Serial.print(buf[1]);
        Serial.print(" : ");
        Serial.println(buf[0]);
        delay(994);
        if(digitalRead(INT_PIN) == 0){
            
            if(PCF8563_Get_Flag() == 1){
                Serial.println("Alarm !!!!!!!!!!!!!\r\n");
                PCF8563_Cleare_AF_Flag();
                
            }
            else if(PCF8563_Get_Flag() == 2){
                Serial.println("Timer !!!!!!!!!!!!!\r\n");
                PCF8563_Cleare_TF_Flag();
                
            }else if(PCF8563_Get_Flag() == 3){
                Serial.println("Timer and Alarm !!!!!!!!!!!!!\r\n");
                PCF8563_Cleare_TF_Flag();
                PCF8563_Cleare_AF_Flag();
            }
        }
        
    }
}

void loop() {
  // put your main code here, to run repeatedly:

}
