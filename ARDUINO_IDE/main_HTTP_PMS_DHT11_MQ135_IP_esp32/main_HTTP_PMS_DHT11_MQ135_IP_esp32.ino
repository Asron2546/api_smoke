#include <VIRUS_WIFI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SoftwareSerial.h>
#include "SensorReader.h"  // include ไฟล์ใหม่ที่แยกออกมา
#include "DHT11.h"
#include "MQ135.h"         // include ไฟล์ใหม่สำหรับ MQ135

// เซ็ตอัพเซ็นเซอร์และ DHT11
SoftwareSerial mySerial(21, 22); // RX, TX
Bonezegei_DHT11 dht(5);  // DHT11 PIN 5

//const char *ssid = "Ronvisly";
//const char *password = "88888888";
const char *serverName = "http://192.168.1.109:3000/api/data"; 
String smoke_detector = "001";

// ฟังก์ชันสำหรับการเชื่อมต่อ WiFi
void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

// ฟังก์ชันสำหรับการส่งข้อมูลไปยัง API
void sendData(String smoke_detector, String pm1, String pm2_5, String pm10, String temperature, String humidity, String co2, String ammonia, String gas) {
  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");

  String jsonData = "{\"smoke_detector\":\"" + smoke_detector + 
                    "\",\"pm1\":" + pm1 + 
                    ",\"pm2_5\":" + pm2_5 + 
                    ",\"pm10\":" + pm10 +
                    ",\"temperature\":" + temperature +
                    ",\"humidity\":" + humidity +
                    ",\"co2\":" + co2 +
                    ",\"ammonia\":" + ammonia +
                    ",\"gas\":" + gas + "}";  

  Serial.println(jsonData);

  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.println("Error on sending POST: " + String(httpResponseCode));
  }
  
  http.end();
}

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);

  // ตั้งค่า MQ135
  setupMQ135();

  // เชื่อมต่อ WiFi
  connectWiFi();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    String pm1 = "0";
    String pm2_5 = "0";
    String pm10 = "0";
    String temperature = "0";
    String humidity = "0";

    // อ่านข้อมูลจากเซ็นเซอร์ PM และ DHT11
    if (readSensor(pm1, pm2_5, pm10) && readDHT11(temperature, humidity)) {
      
      // อ่านค่า MQ135 และคำนวณค่า co2, ammonia, gas
      int sensorValue = analogRead(MQ_135);
      String co2 = String(analysisco2(sensorValue), 3);
      String ammonia = String(analysisammonia(sensorValue), 3);
      String gas = String(analysisgas(sensorValue), 3);
      
      Serial.println(" co2 : " + co2 + ", ammonia : " + ammonia +", gas : " + gas);
      Serial.println("Sensor Value: " + String(sensorValue));
      // ส่งข้อมูลไปยัง API
      sendData(smoke_detector, pm1, pm2_5, pm10, temperature, humidity, co2, ammonia, gas);
    }
  }

  delay(5000); 
}
