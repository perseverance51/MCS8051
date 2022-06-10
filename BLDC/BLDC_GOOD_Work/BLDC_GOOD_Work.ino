#define UH  2
#define UL  3
#define VH  4
#define VL  5
#define WH  6
#define WL  7

#define HON   1
#define HOFF  0

#define LON   0
#define LOFF  1

#define UH_ON digitalWrite(UH, HON)
#define UH_OFF digitalWrite(UH, HOFF)

#define VH_ON digitalWrite(VH, HON)
#define VH_OFF digitalWrite(VH, HOFF)

#define WH_ON digitalWrite(WH, HON)
#define WH_OFF digitalWrite(WH, HOFF)


#define UL_ON digitalWrite(UL, LON)
#define UL_OFF digitalWrite(UL, LOFF)

#define VL_ON digitalWrite(VL, LON)
#define VL_OFF digitalWrite(VL, LOFF)

#define WL_ON digitalWrite(WL, LON)
#define WL_OFF digitalWrite(WL, LOFF)

int HallValue,BEMFPinA,BEMFPinB,BEMFPinC;
int HallValue1,HallValue2,HallValue3,HallValue4,HallValue5,HallValue6;
void setup() {
  pinMode(UL, OUTPUT);
  pinMode(VL, OUTPUT);
  pinMode(WL, OUTPUT);

  digitalWrite(UL, LOFF);
  digitalWrite(VL, LOFF);
  digitalWrite(WL, LOFF);

  pinMode(UH, OUTPUT);
  pinMode(VH, OUTPUT);
  pinMode(WH, OUTPUT);

  digitalWrite(UH, HOFF);
  digitalWrite(VH, HOFF);
  digitalWrite(WH, HOFF);

  pinMode(8, INPUT_PULLUP); //BEMFPinA
  pinMode(9, INPUT_PULLUP); // BEMFPinB
  pinMode(10, INPUT_PULLUP); // BEMFPinC

  
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  /************* 启动加速 ******************/
  for(int i=100; i>0; i-=2)
  {
      UH_ON;UL_OFF;VH_OFF;VL_ON;WH_OFF;WL_OFF;    // UV
      delay(5);
      HallValue= digitalRead(8)*4+(int)digitalRead(9)*2+digitalRead(10);
      
      UH_ON;UL_OFF;VH_OFF;VL_OFF;WH_OFF;WL_ON;    // UW
      delay(5);  
      HallValue= digitalRead(8)*4+(int)digitalRead(9)*2+digitalRead(10);
      
      UH_OFF;UL_OFF;VH_ON;VL_OFF;WH_OFF;WL_ON;    // VW
      delay(5);
      HallValue= digitalRead(8)*4+(int)digitalRead(9)*2+digitalRead(10);
      
      UH_OFF;UL_ON;VH_ON;VL_OFF;WH_OFF;WL_OFF;    // VU
      delay(5);
      HallValue= digitalRead(8)*4+(int)digitalRead(9)*2+digitalRead(10);
      
      UH_OFF;UL_ON;VH_OFF;VL_OFF;WH_ON;WL_OFF;    // WU
      delay(5);
      HallValue= digitalRead(8)*4+(int)digitalRead(9)*2+digitalRead(10);

      UH_OFF;UL_OFF;VH_OFF;VL_ON;WH_ON;WL_OFF;    // WV
      delay(5);
      HallValue= digitalRead(8)*4+(int)digitalRead(9)*2+digitalRead(10);
  }

  /******************* 运转 ***************************/
  while(1)
  {
       switch(HallValue)
      {
        case 4:
        UH_ON;UL_OFF;VH_OFF;VL_ON;WH_OFF;WL_OFF;    // UV
        delay(3);
        HallValue= digitalRead(8)*4+(int)digitalRead(9)*2+digitalRead(10);

        break;
        
        case 6:
        UH_ON;UL_OFF;VH_OFF;VL_OFF;WH_OFF;WL_ON;    // UW
        delay(3); 
        HallValue= digitalRead(8)*4+(int)digitalRead(9)*2+digitalRead(10);
        break;
        
        case 2:
        UH_OFF;UL_OFF;VH_ON;VL_OFF;WH_OFF;WL_ON;    // VW
        delay(3); 
        HallValue= digitalRead(8)*4+(int)digitalRead(9)*2+digitalRead(10);
        break;
        
        case 3:
        UH_OFF;UL_ON;VH_ON;VL_OFF;WH_OFF;WL_OFF;    // VU
        delay(3);
        HallValue= digitalRead(8)*4+(int)digitalRead(9)*2+digitalRead(10);
        break;
        
        case 1:
        UH_OFF;UL_ON;VH_OFF;VL_OFF;WH_ON;WL_OFF;    // WU
        delay(3);
        HallValue= digitalRead(8)*4+(int)digitalRead(9)*2+digitalRead(10);
        break;
        
        case 5:
        UH_OFF;UL_OFF;VH_OFF;VL_ON;WH_ON;WL_OFF;    // WV
        delay(3);       
        HallValue= digitalRead(8)*4+(int)digitalRead(9)*2+digitalRead(10); 
        break;  
      }
  }
}
