const int buttonPin = 10;
const int ledPin = 12;

// 定义变量来存储按键状态和 LED 状态
int buttonState = 0;
bool ledState = false;
unsigned long pressStartTime = 0;

void setup() {
  // 将按键引脚设置为输入
  pinMode(buttonPin, INPUT);
  // 将 LED 引脚设置为输出
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // 读取按键状态
  buttonState = digitalRead(buttonPin);

  // 如果按键被按下
  if (buttonState == HIGH) {
    // 记录按键按下的时间
    pressStartTime = millis();
    // 等待按键释放
    while (digitalRead(buttonPin) == HIGH);
    
    // 如果按键按下时间超过 3 秒
    if (millis() - pressStartTime >= 3000) {
      // 切换 LED 状态
      ledState = !ledState;
      // 设置 LED 的状态
      digitalWrite(ledPin, ledState);
      // 等待一段时间以防止按键连续触发
      delay(250);
    }
  }
}
