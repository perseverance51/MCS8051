   ;*********************************************;
   ;	        DHT21使用范例 
   ;单片机 ： AT89S52或 STC89C52RC
   ;功能   ； 串口发送温湿度数据 波特率 9600
   ;硬件连接： P2.0口为通讯口连接DHT11,DHT11的电源和地连接单片机的电源和地，单片机串口加MAX232连接电脑 
   ;公司   ； 广州奥松电子 
   ;*********************************************;

  ;--------------------
  ;----变量定义区------
  ;--------------------
  FLAG        EQU 30H
  U8comdata   EQU 31H
  U8T_data_H  EQU 32H
  U8T_data_L  EQU 33H
  U8RH_data_H EQU 34H
  U8RH_data_L EQU 35H
  U8checkdata EQU 36H
  U8temp      EQU 37H
  temp        EQU 38H
  ;--------------------
  ;--------------------
  ;--------------------
        org 0000h
	    AJMP START
	    org 0023h
	    AJMP SET_INT
START:             
	    MOV P0,#0FFH                    
        MOV TMOD,#20H     ;T1工作方式2             
        MOV TH1,#0FDH     ;串口波特率为9600               
        MOV SCON,#50H     ;串口工作于方式1并允许接受                
        SETB TR1          ;启动T1                
        SETB ES           ;允许串口中断                
        SETB EA           ;允许全部外设中断  
MAIN:   

;---------------------------
;-----调用湿度读取子程序----
;---------------------------
        LCALL  RH
;---------------------------
;------串口发送程序 --------
;---------------------------  
TT:	   
        MOV    A,U8RH_data_H
	    LCALL  TX_DATA
	    MOV    A,U8RH_data_L
	    LCALL  TX_DATA
	    MOV    A,U8T_data_H
	    LCALL  TX_DATA
	    MOV    A,U8T_data_L
	    LCALL  TX_DATA
	    MOV    A,U8checkdata
	    LCALL  TX_DATA

        //延时两秒 
        LCALL  Delay_2S

        LJMP MAIN


;--------------------------------
;-----湿度读取子程序 ------------
;--------------------------------
;----以下变量均为全局变量--------
;----温度高8位== U8T_data_H------
;----温度低8位== U8T_data_L------
;----湿度高8位== U8RH_data_H-----
;----湿度低8位== U8RH_data_L-----
;----校验 8位 == U8checkdata-----
;----调用相关子程序如下----------
;---- Read,Delay_10us , Delay 
;--------------------------------
RH:
       ;主机拉低总线,延时500us 
        CLR   P2.0  
        MOV   A,#36       	     
	    LCALL Delay
	    SETB  P2.0
	   ;延时40us 
        LCALL Delay_10us   
        LCALL Delay_10us   
        LCALL Delay_10us   
        LCALL Delay_10us 
       ;检测从机是否响应，无响应则跳出	  
        JB    P2.0,RH_END
        MOV   FLAG,#0
LOOP1:
        INC   FLAG
        MOV   A,FLAG
        JB    P2.0,LOOP1_END  ;为高电平则跳出   
        CJNE  A,#0,LOOP1
        LJMP  RH_END
LOOP1_END:
        MOV  FLAG,#0
LOOP2:
        INC  FLAG
        MOV  A,FLAG
        JNB  P2.0,LOOP2_END  ;为低电平则跳出	 
        CJNE A,#0,LOOP2
        LJMP RH_END
LOOP2_END:
       ;进入数据接收阶段		  
        LCALL  Read
	    MOV U8RH_data_H,U8comdata;
	    LCALL  Read
	    MOV U8RH_data_L,U8comdata;
	    LCALL  Read
	    MOV U8T_data_H,U8comdata;
	    LCALL  Read
	    MOV U8T_data_L,U8comdata;
	    LCALL  Read
	    MOV U8checkdata,U8comdata;
	    SETB   P2.0
	   ;----------------------------------------------------------------------------
	   ;----注意：本程序没有加校验程序 如果用户需要校验请安以下说明编写校验程序-----	  
	   ;----------------------------------------------------------------------------
	   ;   数据传送正确时校验和U8checkdata的值等于  
	   ;  "8bit湿度整数数据+8bit湿度小数数据+8bi温度整数数据+8bit温度小数数据"所得结果的末8位
	   ;-----------------------------------------------------------------------------
	   ;-----------------------------------------------------------------------------
	   ;----------------------------------------------------------------------------- 

RH_END:
        RET
;--------------------------------
;---------读数子程序-------------
;--------------------------------
Read:
        MOV temp,#8
	    MOV U8comdata,#0
LOOP_COM:
      ;判断低电平是否结束		 
        MOV  FLAG,#0
LOOP3:
        INC  FLAG
        MOV  A,FLAG
        JB  P2.0,LOOP3_END  ;为高电平则跳出		 
        CJNE A,#0,LOOP3
        LJMP COM_END
LOOP3_END:
       ;----------------------
	    ;CLR    P2.1
		;SETB   P2.1
	   ;----------------------
        LCALL Delay_10us    
        LCALL Delay_10us   
        LCALL Delay_10us   	 
	   ;----------------------
	    ;CLR    P2.1
		;SETB   P2.1
	   ;----------------------
        MOV   U8temp,#0
        JNB    P2.0,HIGH_END
        MOV   U8temp,#1
HIGH_END:
      
        MOV  FLAG,#0
LOOP4:
        INC  FLAG
        MOV  A,FLAG
        JNB P2.0,LOOP4_END  ;为低电平则跳出	 
        CJNE A,#0,LOOP4
        LJMP COM_END
LOOP4_END:
        MOV  A,FLAG
        CJNE  A,#0,NO_FLAG
        LJMP  COM_END
NO_FLAG:
        MOV A,U8comdata
        RL  A
        ORL A,U8temp
        MOV U8comdata,A
       ;判断高电平是否结束	  
        DJNZ temp,LOOP_COM
COM_END:
        RET
;--------------------------------
;------延时函数 -----------------
;--------------------------------
Delay:  MOV R4,A	       ;2.5ms
D1:     MOV R5,#0FFH
	    DJNZ R5,$
	    DJNZ R4,D1
	    RET
;--------------------------------
;------延时10us函数 -----------------
;--------------------------------
Delay_10us:				   ;10us
        NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    RET
;--------------------------------
;------延时2S函数 -----------------
;--------------------------------
Delay_2S:
       MOV A,#250
	   LCALL Delay
       MOV A,#250
	   LCALL Delay
	   MOV A,#250
	   LCALL Delay
	   MOV A,#250
	   LCALL Delay
       MOV A,#250
	   LCALL Delay
       MOV A,#250
	   LCALL Delay
	   MOV A,#250
	   LCALL Delay
	   MOV A,#250
	   LCALL Delay
	   RET
;--------------------------------
;------串口数据发送函数 ---------
;--------------------------------
TX_DATA:
       CLR ES		  ;禁止串口中断		    
       MOV SBUF,A
	   JNB TI,$
	   CLR TI            ;将同一数据发送到串口	 
       SETB ES           ;允许串口中断   
	   RET
;--------------------------------
;------串口中断函数 -------------
;--------------------------------
SET_INT:
	   CLR ES		  ;禁止串口中断	 
	   JNB RI,$
       CLR RI
   	   MOV A,SBUF        ;接受串口数据	    
	   MOV P0,A          ;将串口数据送往P0
	   MOV SBUF,A
	   JNB TI,$
	   CLR TI            ;将同一数据发送到串口	  
	   SETB ES           ;允许串口中断     
	   RETI
	   END
;--------------------------------
;----------结束------------------
;--------------------------------	                            