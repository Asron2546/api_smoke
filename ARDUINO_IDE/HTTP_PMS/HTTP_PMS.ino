void setup() {
  // เริ่มต้น Serial Monitor
  Serial.begin(115200);

  // เริ่มต้น Serial1 สำหรับทดสอบ RX/TX
  Serial1.begin(9600, SERIAL_8N1, 0, 1); // RX = GPIO 0, TX = GPIO 1
  Serial.println("เริ่มการทดสอบ RX/TX...");
}

void loop() {
  // ส่งข้อความจาก TX ไปยัง RX
  Serial1.println("ทดสอบการสื่อสาร TX/RX");

  // ถ้ามีข้อมูลที่ได้รับทาง RX ให้แสดงผล
  if (Serial1.available()) {
    String receivedData = Serial1.readString();  // อ่านข้อมูลจาก RX
    Serial.print("ข้อมูลที่ได้รับทาง RX: ");
    Serial.println(receivedData);  // แสดงผลข้อมูลที่ได้รับใน Serial Monitor
  }

  delay(1000);  // ส่งข้อมูลใหม่ทุกๆ 1 วินาที
}
