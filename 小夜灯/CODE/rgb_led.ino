const int R_pin = 9;
const int G_PIN = 10;
const int B_PIN = 11;
const int B = 8;
int button = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(R_pin, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  pinMode(B, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(B) == HIGH) {
    button = (button + 1) % 3;

    switch (button) {
      case 0:
        analogWrite(R_pin, 255);
        analogWrite(G_PIN, 0);
        analogWrite(B_PIN, 255);
        break;
      case 1:
        analogWrite(R_pin, 255);
        analogWrite(G_PIN, 255);
        analogWrite(B_PIN, 0);
        break;
      case 2:
        analogWrite(R_pin, 0);
        analogWrite(G_PIN, 255);
        analogWrite(B_PIN, 255);
        break;
    }
    delay(250);
  }
  Serial.println(button);
}
