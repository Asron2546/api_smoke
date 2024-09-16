
#define sensor 39

void setup() {
  Serial.begin(9600);
  pinMode(sensor, INPUT);
}

void loop() {
  //int value = analogRead(sensor);
  int value = digitalRead(sensor);
  
  if(value > 4000){
    Serial.print(value);
    Serial.println("   เสียงดังเกิ๊นนนนนนนนนน");
    }
  else{Serial.println(value);}
}

/*
#define analogMin 521
#define analogMax 525
int analogAverage = (analogMin+analogMax)/2; //ไม่เอาทศนิยม

void setup() {
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  int sensorValue = analogRead(39);
  
  if(sensorValue >= analogMin && sensorValue <= analogMax){
    sensorValue = analogAverage;
  }
  
  Serial.println(sensorValue);
}
*/
