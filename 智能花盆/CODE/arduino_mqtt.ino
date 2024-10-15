#define WATER_PIN A1
#include <DFRobot_DHT11.h>
#define DHT11_PIN 12
DFRobot_DHT11 DHT;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(WATER_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  DHT.read(DHT11_PIN);
  float temperature = DHT.temperature;
  int water = analogRead(WATER_PIN);
  datadeliver(water,temperature);
  
}
void datadeliver(int water,float temperature){
  
  String data =String(temperature)+","+String(water);
  Serial.println(data); // 发送数据到ESP32
  delay(2000); // 等待2秒再次发送
}