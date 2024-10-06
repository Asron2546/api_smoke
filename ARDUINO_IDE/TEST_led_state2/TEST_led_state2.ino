#define led 19
#define sw1 25
int sw, last_sw, state = 0;

void setup() {
    Serial.begin(115200);
    pinMode(sw1,INPUT);
    pinMode(led,OUTPUT);
    digitalWrite(led,HIGH);  
    delay(2000);    
}

void loop() {
    sw = digitalRead(sw1);
    
    if(sw == 0 && last_sw == 1){
      state++;
      if(state >= 3){
        state = 0;
        }
    }
    last_sw = sw;
    
    Serial.print(sw);
    Serial.print("  ");
    Serial.println(state);
    
    if(state == 0){        // กำลังเชื่อมต่ออินเตอร์เน็ต
    digitalWrite(led,HIGH);
    delay(250);
    digitalWrite(led,LOW);
    delay(250);
    }
    
    if(state == 1){          // เชื่อมต่ออินเตอร์เน็ตสำเร็จ
    digitalWrite(led,LOW);
    delay(1000);
    digitalWrite(led,HIGH);
    delay(50);
    digitalWrite(led,LOW);
    delay(50);
    digitalWrite(led,HIGH);
    delay(50);
    digitalWrite(led,LOW);
    delay(50);
    }

    if(state == 2){            //ทำงานได้ปกติ
    digitalWrite(led,HIGH);
    delay(50);
    digitalWrite(led,LOW);
    delay(5000);
    }

}
