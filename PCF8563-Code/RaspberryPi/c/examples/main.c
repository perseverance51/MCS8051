#include <stdio.h>		//printf()
#include <stdlib.h>		//exit()
#include <signal.h>     //signal()

#include "DEV_Config.h"
#include "PCF8563.h"
#include <time.h>
void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:exit\r\n");
    
    PCF8563_Alarm_Disable();
    PCF8563_Timer_Disable();
    PCF8563_CLKOUT_Disable();
    DEV_ModuleExit();
    exit(0);
}


int main(void)
{
    // Exception handling:ctrl + c
    signal(SIGINT, Handler);
    DEV_ModuleInit();
    
    UBYTE buf[10];
    DEV_ModuleInit();
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
        
        printf("%d%d Year %d Month %d Day %d:%d:%d\r\n",buf[6],buf[5],buf[4],buf[3],buf[2],buf[1],buf[0]);
        DEV_Delay_ms(500);
        if(DEV_Digital_Read(INT_PIN) == 0){
            
            if(PCF8563_Get_Flag() == 1){
                printf("Alarm !!!!!!!!!!!!!\r\n");
                PCF8563_Cleare_AF_Flag();
                
            }
            else if(PCF8563_Get_Flag() == 2){
                printf("Timer !!!!!!!!!!!!!\r\n");
                PCF8563_Cleare_TF_Flag();
                
            }else if(PCF8563_Get_Flag() == 3){
                printf("Timer and Alarm !!!!!!!!!!!!!\r\n");
                PCF8563_Cleare_TF_Flag();
                PCF8563_Cleare_AF_Flag();
            }
        }
        
    }
    
    //System Exit
    DEV_ModuleExit();
    return 0;
}

