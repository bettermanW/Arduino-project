
const int buttonPin = 10;
const int ledPin = 12;
int buttonState = 0;
bool ledState = false;
void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  // 将 LED 引脚设置为输出
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    // 等待一段时间以防止按键连续触发
    delay(250);
  }
}
