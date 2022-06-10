/*获取脉宽宽度原函数pulseIn
 * pulseIn(pin, value) 
pulseIn(pin, value, timeout)
pin:你要进行脉冲计时的引脚号（int）。 
value:要读取的脉冲类型，HIGH或LOW（int）。 
timeout (可选）：指定脉冲计数的等待时间，单位为微秒，默认值是1秒（unsigned long）
 * unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout) {
  pinMode(pin, INPUT);
  uint32_t start = micros();
  while(digitalRead(pin) == state && (micros() - start) < timeout);
  while(digitalRead(pin) != state && (micros() - start) < timeout);
  start = micros();
  while(digitalRead(pin) == state && (micros() - start) < timeout);
  return micros() - start;
}
 */
byte PWM_PIN = 3; //将PWM的信号线输入到3号引脚
 
unsigned long pwm_value;
 
void setup() {
  pinMode(PWM_PIN, INPUT);//将该引脚设置为输入模式
  Serial.begin(115200);
}
 
void loop() {
  pwm_value = pulseIn(PWM_PIN, HIGH);//检测高电平
  Serial.println(pwm_value);//串口输出值
}
