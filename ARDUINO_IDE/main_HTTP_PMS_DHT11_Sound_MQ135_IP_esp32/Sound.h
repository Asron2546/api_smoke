#ifndef SOUND_H
#define SOUND_H

#define sound_sensor 35 
extern const int buzzer;
void setupSOUND() {
  pinMode(sound_sensor, INPUT);
}
float soundSensor(){
  
  int value_sound = analogRead(sound_sensor);  // อ่านค่าจากเซ็นเซอร์เสียง
  //Serial.print("ADC Value: ");
  //Serial.println(value_sound);  // แสดงค่า ADC ที่อ่านได้

  // แปลงค่า ADC เป็นแรงดันไฟฟ้า
  float voltage = (value_sound / 4095.0) * 3.3;  // แปลงค่า ADC เป็นแรงดันไฟฟ้า (ESP32 ใช้ 12-bit ADC, 0-4095)
  
  // ปรับสูตรคำนวณให้ค่า 0 ที่เซ็นเซอร์รับมาเป็น 0 dB
  float decibel = 20 * log10(voltage / 0.001);  // ปรับค่าฐานให้ 0V -> 0dB

  // เพื่อป้องกันการเกิดข้อผิดพลาดจากการ log10(0) เมื่อไม่มีแรงดันไฟฟ้า
  if (voltage == 0) {
    decibel = 0;  // กำหนดให้ค่า dB เป็น 0 เมื่อไม่มีแรงดันไฟฟ้า
  }

  //Serial.print("\nDecibel: ");
  //Serial.print(decibel);
  //Serial.print(" dB");

  // ตรวจสอบระดับเสียง
  if (decibel > 68.50) {  // สามารถปรับเกณฑ์เสียงดังได้ตามต้องการ
    Serial.println("  เสียงดังเกิ๊นนนนนนนนนน");
    digitalWrite(buzzer, LOW);
    delay(500);
  }
  else{
    digitalWrite(buzzer, HIGH);
    }
  return decibel;  
}

#endif 
