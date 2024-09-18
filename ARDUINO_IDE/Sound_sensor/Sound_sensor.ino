#define sensor 4

void setup() {
  Serial.begin(115200);
  pinMode(sensor, INPUT);
}

void loop() {
  int value = analogRead(sensor);
  
  if(value > 4000){
    Serial.print(value);
    Serial.println("   เสียงดังเกิ๊นนนนนนนนนน");
    }
  else{Serial.println(value);}
}
