#define buzzer 23 
void setup() {
  pinMode(buzzer,OUTPUT);
  digitalWrite(buzzer,HIGH);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(buzzer,HIGH);
  delay(100);
  digitalWrite(buzzer,LOW);
  delay(100);
}
