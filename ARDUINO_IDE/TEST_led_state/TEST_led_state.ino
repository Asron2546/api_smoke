#define led 19

void setup() {
    Serial.begin(115200);
    pinMode(led,OUTPUT);
    digitalWrite(led,HIGH);  
    delay(2000);    
}

void loop() {

        // กำลังเชื่อมต่ออินเตอร์เน็ต
    digitalWrite(led,HIGH);
    delay(250);
    digitalWrite(led,LOW);
    delay(250);
    
         // เชื่อมต่ออินเตอร์เน็ตสำเร็จ
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
    
         //ทำงานได้ปกติ
    digitalWrite(led,HIGH);
    delay(50);
    digitalWrite(led,LOW);
    delay(5000);
    

}
