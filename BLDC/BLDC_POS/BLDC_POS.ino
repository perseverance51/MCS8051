
#define UH  9
#define VH  10
#define WH  11

#define UL  3
#define VL  5
#define WL  7

#define POS_U A1
#define POS_V A2
#define POS_W A3

#define HON   1
#define HOFF  0

#define LON   0
#define LOFF  1

#define UH_ON  digitalWrite(UH, HON)
#define UH_OFF digitalWrite(UH, HOFF)

#define VH_ON  digitalWrite(VH, HON)
#define VH_OFF digitalWrite(VH, HOFF)

#define WH_ON  digitalWrite(WH, HON)
#define WH_OFF digitalWrite(WH, HOFF)


#define UL_ON  digitalWrite(UL, LON)
#define UL_OFF digitalWrite(UL, LOFF)

#define VL_ON  digitalWrite(VL, LON)
#define VL_OFF digitalWrite(VL, LOFF)

#define WL_ON  digitalWrite(WL, LON)
#define WL_OFF digitalWrite(WL, LOFF)

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

  setPwmFrequency(UH);
  setPwmFrequency(VH);
  setPwmFrequency(WH);

  pinMode(POS_U, INPUT_PULLUP); //BEMFPinA
  pinMode(POS_V, INPUT_PULLUP); // BEMFPinB
  pinMode(POS_W, INPUT_PULLUP); // BEMFPinC

  Serial.begin(9600);
}

void loop() {

      while(1)
      {
          /*UH_ON;UL_OFF;VH_OFF;VL_ON;WH_OFF;WL_OFF;    // UV
          delay(5); 
          HallValue1= digitalRead(POS_U)*4+(int)digitalRead(POS_V)*2+digitalRead(POS_W);

          UH_ON;UL_OFF;VH_OFF;VL_OFF;WH_OFF;WL_ON;    // UW
          delay(5);  
          HallValue2= digitalRead(POS_U)*4+(int)digitalRead(POS_V)*2+digitalRead(POS_W);

          UH_OFF;UL_OFF;VH_ON;VL_OFF;WH_OFF;WL_ON;    // VW
          delay(5);
          HallValue3= digitalRead(POS_U)*4+(int)digitalRead(POS_V)*2+digitalRead(POS_W);

          UH_OFF;UL_ON;VH_ON;VL_OFF;WH_OFF;WL_OFF;    // VU
          delay(5);
          HallValue4= digitalRead(POS_U)*4+(int)digitalRead(POS_V)*2+digitalRead(POS_W);
          

          UH_OFF;UL_ON;VH_OFF;VL_OFF;WH_ON;WL_OFF;    // WU
          delay(5);
          HallValue5= digitalRead(POS_U)*4+(int)digitalRead(POS_V)*2+digitalRead(POS_W);
          

          UH_OFF;UL_OFF;VH_OFF;VL_ON;WH_ON;WL_OFF;    // WV
          delay(5);
          HallValue6= digitalRead(POS_U)*4+(int)digitalRead(POS_V)*2+digitalRead(POS_W);*/

                  //UH_ON;UL_OFF;VH_OFF;VL_ON;WH_OFF;WL_OFF;    // 
          UL_OFF;VL_ON;WL_OFF;    // UV
          analogWrite(UH, 120);
          analogWrite(VH, 0);
          analogWrite(WH, 0);
          delay(5);
          HallValue1= digitalRead(POS_U)*4+(int)digitalRead(POS_V)*2+digitalRead(POS_W);

          UL_OFF;VL_OFF;WL_ON;    // UW
          analogWrite(UH, 120);
          analogWrite(VH, 0);
          analogWrite(WH, 0);
          delay(5);
          HallValue2= digitalRead(POS_U)*4+(int)digitalRead(POS_V)*2+digitalRead(POS_W);

          UL_OFF;VL_OFF;WL_ON;    //
          analogWrite(UH, 0);
          analogWrite(VH, 120);
          analogWrite(WH, 0);
          delay(5);
          HallValue3= digitalRead(POS_U)*4+(int)digitalRead(POS_V)*2+digitalRead(POS_W);

          //UH_OFF;UL_ON;VH_ON;VL_OFF;WH_OFF;WL_OFF;    // VU
          UL_ON;VL_OFF;WL_OFF;    // 
          analogWrite(UH, 0);
          analogWrite(VH, 120);
          analogWrite(WH, 0);
          delay(5);
          HallValue4= digitalRead(POS_U)*4+(int)digitalRead(POS_V)*2+digitalRead(POS_W);

          UL_ON;VL_OFF;WL_OFF;    // WU
          analogWrite(UH, 0);
          analogWrite(VH, 0);
          analogWrite(WH, 120);
          delay(5);
          HallValue5= digitalRead(POS_U)*4+(int)digitalRead(POS_V)*2+digitalRead(POS_W);

          UL_OFF;VL_ON;WL_OFF;    // 
          analogWrite(UH, 0);
          analogWrite(VH, 0);
          analogWrite(WH, 120);   
          delay(5);
          HallValue6= digitalRead(POS_U)*4+(int)digitalRead(POS_V)*2+digitalRead(POS_W);
            

          Serial.println(HallValue1);
          Serial.println(HallValue2);
          Serial.println(HallValue3);
          Serial.println(HallValue4);
          Serial.println(HallValue5);
          Serial.println(HallValue6);
          Serial.println("Hello");
      }

}

void setPwmFrequency(int pin) 
{
    if (pin == 5 || pin == 6  ) 
    {
      TCCR0A = ((TCCR0A & 0b11111100) | 0x01); // configure the pwm phase-corrected mode
      TCCR0B = ((TCCR0B & 0b11110000) | 0x01); // set prescaler to 1
    }
    if (pin == 9 || pin == 10 )
      TCCR1B = ((TCCR1B & 0b11111000) | 0x01);     // set prescaler to 1
    if (pin == 3 || pin == 11) 
      TCCR2B = ((TCCR2B & 0b11111000) | 0x01);// set prescaler to 1
}
