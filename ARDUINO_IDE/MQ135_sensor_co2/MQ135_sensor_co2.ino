#define MQ_135 2
float analysis(int);
float Rs; 

void setup() {
  Serial.begin(115200);
}

void loop() {
  int sensorValue = analogRead(MQ_135);
  Serial.print("ADC : ");
  Serial.print(sensorValue);
  Serial.print("\t");
  Serial.print("CO2 : ");
  Serial.print(analysis(sensorValue), 3);  // คำนวณค่า CO2
  Serial.print(" ppm");
  Serial.print("\t");
  Serial.print("Sensor Resistance : ");
  Serial.println(Rs);
  delay(500);
}

float analysis(int adc){
  // ปรับพารามิเตอร์ให้ตรงกับข้อมูล CO2 จากกราฟ calibration curve
  float slope = -0.32;  // Slope สำหรับ CO2 (ต้องปรับตามข้อมูลจาก datasheet)
  float A = 1.5;        // ค่า A สำหรับ CO2 (ปรับตาม calibration curve)
  float Rseries = 1000; // ค่าความต้านทานตัวต้านทานภายนอก
  float V_Rseries = ((float)adc * 5) / 1023;  // คำนวณแรงดันตกคร่อมที่ตัวต้านทาน
  Rs = ((5 - V_Rseries) / V_Rseries) * Rseries;  // คำนวณค่าความต้านทานของเซ็นเซอร์
  float R0 = 1809.52;   // ค่าความต้านทานในสภาวะอากาศสะอาด (ต้องปรับจูน)

  /*
   * ควรปรับจูนค่า R0 โดยการสอบเทียบกับเซ็นเซอร์ที่มีมาตรฐาน
   */
  
  float Y = Rs / R0;
  float CO2_gas = pow(10, (log10(Y / A) / slope));  // คำนวณความเข้มข้นของแก๊ส (ppm)
  return CO2_gas;
}
