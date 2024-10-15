#include <Wire.h>
#include <U8g2lib.h>
#include <BH1750.h>
#include <DFRobot_DHT11.h>

#define DHT11_PIN 12
#define SOIL_MOISTURE_PIN A0
#define WATER_PUMP_PIN 9
#define WATER_PIN A1


const int MOTOR_A = 4;
const int MOTOR_B = 5;
const int PWM_PIN = 3;
 
DFRobot_DHT11 DHT;
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);
BH1750 lightMeter;

unsigned long lastUpdateTime = 0;
unsigned long updateInterval = 500; 
unsigned long lastWateringTime = 0;

void setup() {
  Serial.begin(9600);
  lightMeter.begin();
  u8g2.begin();
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(WATER_PUMP_PIN, OUTPUT);
  pinMode(WATER_PIN, INPUT);
  pinMode(MOTOR_A, OUTPUT);
  pinMode(MOTOR_B, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);
  digitalWrite(WATER_PUMP_PIN, HIGH);
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentMillis;
    
    DHT.read(DHT11_PIN);
    int water = analogRead(WATER_PIN);
    float soilMoisture = 1 - analogRead(SOIL_MOISTURE_PIN) / 1023.0;
    float lux = lightMeter.readLightLevel();
    float temperature = DHT.temperature;

    displayData(soilMoisture, lux, temperature, water);
    waterPlant(soilMoisture, lux, water);
    datadeliver(temperature, water, lux, water);
    // Serial.println(water);
    // Serial.println(soilMoisture);
    // Serial.println(lux);
    // Serial.println(temperature);
    
    
  }
}

#define DISPLAY_X_POS 10
#define DISPLAY_Y_POS_START 15
#define DISPLAY_LINE_HEIGHT 15

void displayData(float soilMoisture, float lightIntensity, float temperature, int water) {
  u8g2.clearBuffer(); 

  if (water >= 500) {
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.setCursor(DISPLAY_X_POS, DISPLAY_Y_POS_START);
    u8g2.print("Light:");
    u8g2.print(lightIntensity);

    u8g2.setCursor(DISPLAY_X_POS, DISPLAY_Y_POS_START + DISPLAY_LINE_HEIGHT);
    u8g2.print("Soil:");
    u8g2.print(soilMoisture * 100); 
    u8g2.print("%");

    u8g2.setCursor(DISPLAY_X_POS, DISPLAY_Y_POS_START + 2 * DISPLAY_LINE_HEIGHT);
    u8g2.print("Tem:");
    u8g2.print(temperature);
    u8g2.print("°C");

    u8g2.setCursor(DISPLAY_X_POS, DISPLAY_Y_POS_START + 3 * DISPLAY_LINE_HEIGHT);
    u8g2.print("Water:");
    u8g2.print(water);

    u8g2.sendBuffer(); 
  } else {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(25, 40);
    u8g2.print("No water");
    u8g2.sendBuffer(); 
  }
}

void waterPlant(float soilMoisture, float lightIntensity, int water) {
  unsigned long currentTime = millis();
  // 检查上次浇水时间与当前时间的间隔是否超过一天（86400000 毫秒）
  if (currentTime - lastWateringTime >= 3000) {
    // 检查浇水条件：土壤湿度低于设定值、光照强度高于设定值，并且水位足够
    if (soilMoisture < 1 && lightIntensity > 450 && water > 500) {
      // 执行浇水操作
      digitalWrite(WATER_PUMP_PIN, LOW);
      digitalWrite(MOTOR_A, LOW);
      digitalWrite(MOTOR_B, HIGH);
      analogWrite(PWM_PIN, 100);
      delay(2500);
      digitalWrite(WATER_PUMP_PIN, HIGH);
      digitalWrite(MOTOR_A, LOW);
      digitalWrite(MOTOR_B, LOW);
      analogWrite(PWM_PIN, 0);      
      delay(1000);
      
      
      // 记录浇水时间
      lastWateringTime = currentTime;
     
    }
  }
  
  else {
    // 如果距离上次浇水时间不超过一天，则不进行浇水操作
    digitalWrite(WATER_PUMP_PIN, HIGH);
    digitalWrite(MOTOR_A, LOW);
    digitalWrite(MOTOR_B, LOW);
    analogWrite(PWM_PIN, 0); 
    }
        
}
void datadeliver(float temperature,int water,float lux,float soilMoisture ){
  String data = String(temperature ) + "," + String(water) + ","+String(lux) + "," + String(soilMoisture);
  Serial.println(data); // 发送数据到ESP32

  delay(500); // 等待2秒再次发送
}