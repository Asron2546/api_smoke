#ifndef MQ135_H
#define MQ135_H

#define MQ_135 34  // MQ_135 PIN 34 
float analysisco2(int);
float analysisammonia(int);
float analysisgas(int);
float Rs;

void setupMQ135() {
  pinMode(MQ_135, INPUT);
}

// ฟังก์ชันสำหรับคำนวณค่า co2
float analysisco2(int adc) {
  float slope = -0.32;  // Slope สำหรับ co2
  float A = 1.5;        // ค่า A สำหรับ co2
  float Rseries = 1000; // ค่าความต้านทานตัวต้านทานภายนอก
  float V_Rseries = ((float)adc * 5) / 1023;  // คำนวณแรงดันตกคร่อมที่ตัวต้านทาน
  Rs = ((5 - V_Rseries) / V_Rseries) * Rseries;  // คำนวณค่าความต้านทานของเซ็นเซอร์
  float R0 = 1809.52;   // ค่าความต้านทานในสภาวะอากาศสะอาด

  float Y = Rs / R0;
  float co2_gas = pow(10, (log10(Y / A) / slope));  // คำนวณความเข้มข้นของแก๊ส co2 (ppm)
  return co2_gas;
}

// ฟังก์ชันสำหรับคำนวณค่า ammonia
float analysisammonia(int adc) {
  float slope = -0.45;  // Slope สำหรับแอมโมเนีย
  float A = 1.9;        // ค่า A สำหรับแอมโมเนีย
  float Rseries = 1000; // ค่าความต้านทานตัวต้านทานภายนอก
  float V_Rseries = ((float)adc * 5) / 1023;  // คำนวณแรงดันตกคร่อมที่ตัวต้านทาน
  Rs = ((5 - V_Rseries) / V_Rseries) * Rseries;  // คำนวณค่าความต้านทานของเซ็นเซอร์
  float R0 = 1809.52;   // ค่าความต้านทานในสภาวะอากาศสะอาด

  float Y = Rs / R0;
  float Ammonia_gas = pow(10, (log10(Y / A) / slope));  // คำนวณความเข้มข้นของแก๊ส ammonia (ppm)
  return Ammonia_gas;
}

// ฟังก์ชันสำหรับคำนวณค่า gas
float analysisgas(int adc) {
  float slope = -0.2884453282;  // Slope สำหรับ gas
  float A = 2.720027932;        // ค่า A สำหรับ gas
  float Rseries = 1000;         // ค่าความต้านทานตัวต้านทานภายนอก
  float V_Rseries = ((float)adc * 5) / 1023;  // คำนวณแรงดันตกคร่อมที่ตัวต้านทาน
  Rs = ((5 - V_Rseries) / V_Rseries) * Rseries;  // คำนวณค่าความต้านทานของเซ็นเซอร์
  float R0 = 1809.52;           // ค่าความต้านทานในสภาวะอากาศสะอาด

  float Y = Rs / R0;
  float gas_gas = pow(10, (log10(Y / A) / slope));  // คำนวณความเข้มข้นของแก๊ส gas (ppm)
  return gas_gas;
}

#endif
