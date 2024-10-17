#define sound_sensor A1

void setup() {
  Serial.begin(115200);
  pinMode(sound_sensor, INPUT);
}

void loop() {
  int value = analogRead(sound_sensor);  // อ่านค่าจากเซ็นเซอร์เสียง
  
  // แปลงค่า ADC เป็นแรงดันไฟฟ้า
  float voltage = (value / 4095.0) * 3.3;  // แปลงค่า ADC เป็นแรงดันไฟฟ้า (ESP32 ใช้ 12-bit ADC, 0-4095)
  
  // ปรับสูตรคำนวณให้ค่า 0 ที่เซ็นเซอร์รับมาเป็น 0 dB
  float decibel = 20 * log10(voltage / 0.001);  // ปรับค่าฐานให้ 0V -> 0dB

  // เพื่อป้องกันการเกิดข้อผิดพลาดจากการ log10(0) เมื่อไม่มีแรงดันไฟฟ้า
  if (voltage == 0) {
    decibel = 0;  // กำหนดให้ค่า dB เป็น 0 เมื่อไม่มีแรงดันไฟฟ้า
  }

  // แสดงผลค่าเป็นเดซิเบล
  Serial.print("ADC Value: ");
  Serial.print(value);
  Serial.print("\t");
  Serial.print("Decibel: ");
  Serial.print(decibel);
  Serial.println(" dB");

  // ตรวจสอบระดับเสียง
  if (decibel > 70) {  // สามารถปรับเกณฑ์เสียงดังได้ตามต้องการ
    Serial.println("เสียงดังเกิ๊นนนนนนนนนน");
  }

  delay(100);  // รอ 800 มิลลิวินาที
}
