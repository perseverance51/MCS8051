   ;*********************************************;
   ;	        DHT21ʹ�÷��� 
   ;��Ƭ�� �� AT89S52�� STC89C52RC
   ;����   �� ���ڷ�����ʪ������ ������ 9600
   ;Ӳ�����ӣ� P2.0��ΪͨѶ������DHT11,DHT11�ĵ�Դ�͵����ӵ�Ƭ���ĵ�Դ�͵أ���Ƭ�����ڼ�MAX232���ӵ��� 
   ;��˾   �� ���ݰ��ɵ��� 
   ;*********************************************;

  ;--------------------
  ;----����������------
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
        MOV TMOD,#20H     ;T1������ʽ2             
        MOV TH1,#0FDH     ;���ڲ�����Ϊ9600               
        MOV SCON,#50H     ;���ڹ����ڷ�ʽ1����������                
        SETB TR1          ;����T1                
        SETB ES           ;���������ж�                
        SETB EA           ;����ȫ�������ж�  
MAIN:   

;---------------------------
;-----����ʪ�ȶ�ȡ�ӳ���----
;---------------------------
        LCALL  RH
;---------------------------
;------���ڷ��ͳ��� --------
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

        //��ʱ���� 
        LCALL  Delay_2S

        LJMP MAIN


;--------------------------------
;-----ʪ�ȶ�ȡ�ӳ��� ------------
;--------------------------------
;----���±�����Ϊȫ�ֱ���--------
;----�¶ȸ�8λ== U8T_data_H------
;----�¶ȵ�8λ== U8T_data_L------
;----ʪ�ȸ�8λ== U8RH_data_H-----
;----ʪ�ȵ�8λ== U8RH_data_L-----
;----У�� 8λ == U8checkdata-----
;----��������ӳ�������----------
;---- Read,Delay_10us , Delay 
;--------------------------------
RH:
       ;������������,��ʱ500us 
        CLR   P2.0  
        MOV   A,#36       	     
	    LCALL Delay
	    SETB  P2.0
	   ;��ʱ40us 
        LCALL Delay_10us   
        LCALL Delay_10us   
        LCALL Delay_10us   
        LCALL Delay_10us 
       ;���ӻ��Ƿ���Ӧ������Ӧ������	  
        JB    P2.0,RH_END
        MOV   FLAG,#0
LOOP1:
        INC   FLAG
        MOV   A,FLAG
        JB    P2.0,LOOP1_END  ;Ϊ�ߵ�ƽ������   
        CJNE  A,#0,LOOP1
        LJMP  RH_END
LOOP1_END:
        MOV  FLAG,#0
LOOP2:
        INC  FLAG
        MOV  A,FLAG
        JNB  P2.0,LOOP2_END  ;Ϊ�͵�ƽ������	 
        CJNE A,#0,LOOP2
        LJMP RH_END
LOOP2_END:
       ;�������ݽ��ս׶�		  
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
	   ;----ע�⣺������û�м�У����� ����û���ҪУ���밲����˵����дУ�����-----	  
	   ;----------------------------------------------------------------------------
	   ;   ���ݴ�����ȷʱУ���U8checkdata��ֵ����  
	   ;  "8bitʪ����������+8bitʪ��С������+8bi�¶���������+8bit�¶�С������"���ý����ĩ8λ
	   ;-----------------------------------------------------------------------------
	   ;-----------------------------------------------------------------------------
	   ;----------------------------------------------------------------------------- 

RH_END:
        RET
;--------------------------------
;---------�����ӳ���-------------
;--------------------------------
Read:
        MOV temp,#8
	    MOV U8comdata,#0
LOOP_COM:
      ;�жϵ͵�ƽ�Ƿ����		 
        MOV  FLAG,#0
LOOP3:
        INC  FLAG
        MOV  A,FLAG
        JB  P2.0,LOOP3_END  ;Ϊ�ߵ�ƽ������		 
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
        JNB P2.0,LOOP4_END  ;Ϊ�͵�ƽ������	 
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
       ;�жϸߵ�ƽ�Ƿ����	  
        DJNZ temp,LOOP_COM
COM_END:
        RET
;--------------------------------
;------��ʱ���� -----------------
;--------------------------------
Delay:  MOV R4,A	       ;2.5ms
D1:     MOV R5,#0FFH
	    DJNZ R5,$
	    DJNZ R4,D1
	    RET
;--------------------------------
;------��ʱ10us���� -----------------
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
;------��ʱ2S���� -----------------
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
;------�������ݷ��ͺ��� ---------
;--------------------------------
TX_DATA:
       CLR ES		  ;��ֹ�����ж�		    
       MOV SBUF,A
	   JNB TI,$
	   CLR TI            ;��ͬһ���ݷ��͵�����	 
       SETB ES           ;���������ж�   
	   RET
;--------------------------------
;------�����жϺ��� -------------
;--------------------------------
SET_INT:
	   CLR ES		  ;��ֹ�����ж�	 
	   JNB RI,$
       CLR RI
   	   MOV A,SBUF        ;���ܴ�������	    
	   MOV P0,A          ;��������������P0
	   MOV SBUF,A
	   JNB TI,$
	   CLR TI            ;��ͬһ���ݷ��͵�����	  
	   SETB ES           ;���������ж�     
	   RETI
	   END
;--------------------------------
;----------����------------------
;--------------------------------	                            