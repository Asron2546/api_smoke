#define MQ_135 2
float analysis(int);
float Rs; 

void setup() {
  Serial.begin(115200);
}

void loop() {
  int sensorValue = analogRead(MQ_135);
  Serial.print("adc : ");
  Serial.print(sensorValue);
  Serial.print("\t");
  Serial.print("Acetone : ");
  Serial.print(analysis(sensorValue),3);
  Serial.print("ppm");
  Serial.print("\t");
  Serial.print("Sensor resistance : ");
  Serial.println(Rs);
  delay(500);
}

float analysis(int adc){
  float slope = -0.2884453282;
  float A = 2.720027932;
  float Rseries = 1000;
  float V_Rseries = ((float)adc*5)/1023;
  Rs = ((5-V_Rseries)/V_Rseries)*Rseries;
  float R0 = 1809.52;
  /*
   * R0 คือ ค่าความต้านทานเซ็นเซอร์ขณะที่ Ammonia เท่ากับ 100 ppm ในอากาศที่สะอาด
   * ควรปรับจูนด้วยการสอบเทียบกับเซ็นเซอร์มาตรฐาน
   */
  float Y = Rs/R0;
  float Methane_gas = pow(10,(log10(Y/A)/slope));
  return Methane_gas;
}
