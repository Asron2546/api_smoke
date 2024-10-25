#define MQ_135 A2
float analysisCO2(int);
float analysisNH3(int);
float analysisAcetone(int);
float Rs; 

void setup() {
  Serial.begin(115200);
  pinMode(MQ_135, INPUT);
}

void loop() {
  // อ่านค่าจากเซ็นเซอร์
  int sensorValue = analogRead(MQ_135);
  
  // คำนวณค่า CO2
  Serial.print("ADC: ");
  Serial.print(sensorValue);
  Serial.print("  CO2: ");
  Serial.print(analysisCO2(sensorValue), 3);  // คำนวณค่า CO2
  Serial.print(" ppm");
  
  // คำนวณค่า NH3
  Serial.print("  NH3: ");
  Serial.print(analysisNH3(sensorValue), 3);  // คำนวณค่า NH3
  Serial.print(" ppm");
  
  // คำนวณค่า Acetone
  Serial.print("  Acetone: ");
  Serial.print(analysisAcetone(sensorValue), 3);  // คำนวณค่า Acetone
  Serial.println(" ppm");
  
  delay(500);
}

// ฟังก์ชันสำหรับคำนวณค่า CO2
float analysisCO2(int adc) {
  float slope = -0.32;  // Slope สำหรับ CO2
  float A = 1.5;        // ค่า A สำหรับ CO2
  float Rseries = 1000; // ค่าความต้านทานตัวต้านทานภายนอก
  float V_Rseries = ((float)adc * 5) / 1023;  // คำนวณแรงดันตกคร่อมที่ตัวต้านทาน
  Rs = ((5 - V_Rseries) / V_Rseries) * Rseries;  // คำนวณค่าความต้านทานของเซ็นเซอร์
  float R0 = 1809.52;   // ค่าความต้านทานในสภาวะอากาศสะอาด

  float Y = Rs / R0;
  float CO2_gas = pow(10, (log10(Y / A) / slope));  // คำนวณความเข้มข้นของแก๊ส CO2 (ppm)
  return CO2_gas;
}

// ฟังก์ชันสำหรับคำนวณค่า NH3
float analysisNH3(int adc) {
  float slope = -0.45;  // Slope สำหรับแอมโมเนีย
  float A = 1.9;        // ค่า A สำหรับแอมโมเนีย
  float Rseries = 1000; // ค่าความต้านทานตัวต้านทานภายนอก
  float V_Rseries = ((float)adc * 5) / 1023;  // คำนวณแรงดันตกคร่อมที่ตัวต้านทาน
  Rs = ((5 - V_Rseries) / V_Rseries) * Rseries;  // คำนวณค่าความต้านทานของเซ็นเซอร์
  float R0 = 1809.52;   // ค่าความต้านทานในสภาวะอากาศสะอาด

  float Y = Rs / R0;
  float Ammonia_gas = pow(10, (log10(Y / A) / slope));  // คำนวณความเข้มข้นของแก๊ส NH3 (ppm)
  return Ammonia_gas;
}

// ฟังก์ชันสำหรับคำนวณค่า Acetone
float analysisAcetone(int adc) {
  float slope = -0.2884453282;  // Slope สำหรับ Acetone
  float A = 2.720027932;        // ค่า A สำหรับ Acetone
  float Rseries = 1000;        // ค่าความต้านทานตัวต้านทานภายนอก
  float V_Rseries = ((float)adc * 5) / 1023;  // คำนวณแรงดันตกคร่อมที่ตัวต้านทาน
  Rs = ((5 - V_Rseries) / V_Rseries) * Rseries;  // คำนวณค่าความต้านทานของเซ็นเซอร์
  float R0 = 1809.52;           // ค่าความต้านทานในสภาวะอากาศสะอาด

  float Y = Rs / R0;
  float Acetone_gas = pow(10, (log10(Y / A) / slope));  // คำนวณความเข้มข้นของแก๊ส Acetone (ppm)
  return Acetone_gas;
}
