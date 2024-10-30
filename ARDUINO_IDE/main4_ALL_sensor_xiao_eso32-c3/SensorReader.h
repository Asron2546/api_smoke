#ifndef SENSORREADER_H
#define SENSORREADER_H

#include <Arduino.h>
#include <SoftwareSerial.h>

extern const int buzzer;

// ฟังก์ชันสำหรับการอ่านค่าฝุ่นจากเซนเซอร์
bool readSensor(String &pm1, String &pm2_5, String &pm10) {
  int index = 0;
  char value;
  char previousValue;
  bool validData = true;

  while (Serial1.available()) {
    value = Serial1.read();
    if ((index == 0 && value != 0x42) || (index == 1 && value != 0x4d)) {
      Serial.println("Cannot find the data header.");
      validData = false;
      break;
    }

    if (index == 4 || index == 6 || index == 8 || index == 10 || index == 12 || index == 14) {
      previousValue = value;
    }
    else if (index == 5) {
      pm1 = 256 * previousValue + value;
      Serial.print("\npm1: ");
      Serial.print(pm1);
      Serial.print("  ");
    }
    else if (index == 7) {
      pm2_5 = 256 * previousValue + value;
      Serial.print("pm2_5: ");
      Serial.print(pm2_5);
      Serial.print("  ");
    }
    else if (index == 9) {
      pm10 = 256 * previousValue + value;
      Serial.print("pm10: ");
      Serial.print(pm10);
      Serial.print("\t");
    }
    else if (index > 15) {
      break;
    }
    index++;
  }

  while (Serial1.available())
    Serial1.read();  // เคลียร์ buffer

  if (pm1.toInt() > 50){
    digitalWrite(buzzer, LOW);
    delay(500);
  }
  else{
    digitalWrite(buzzer, HIGH);
    }
  return validData;
}

#endif
