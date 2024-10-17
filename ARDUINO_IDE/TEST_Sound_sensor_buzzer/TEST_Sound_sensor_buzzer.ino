#define sensor A1 
#define buzzer D10 
void setup() {
  Serial.begin(115200);
  pinMode(sensor, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer,LOW);
}

void loop() {
  int value = analogRead(sensor);
  
  if(value > 4000){
    Serial.print(value);
    Serial.println("   เสียงดังเกิ๊นนนนนนนนนน");
    digitalWrite(buzzer,HIGH);
    }
  else{Serial.println(value);   digitalWrite(buzzer,LOW);}
  delay(50);
}
