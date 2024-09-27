#include <VIRUS_WIFI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SoftwareSerial.h>
#include "SensorReader.h"  // include ไฟล์ใหม่ที่แยกออกมา
#include "DHT11.h"

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
void sendData(String smoke_detector, String pm1, String pm2_5, String pm10, String temperature, String humidity) {
  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");

  //String jsonData = "{\"smoke_detector\":\"" + String(smoke_detector) + "\",\"pm1\":" + String(pm1) + ",\"pm2_5\":" + String(pm2_5) + ",\"pm10\":" + String(pm10) + "}";
  String jsonData = "{\"smoke_detector\":\"" + smoke_detector + 
                  "\",\"pm1\":" + pm1 + 
                  ",\"pm2_5\":" + pm2_5 + 
                  ",\"pm10\":" + pm10 +
                  ",\"temperature\":" + temperature +
                  ",\"humidity\":" + humidity + "}";  
                  
  Serial.println(jsonData);  

  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.print("    ");
    Serial.println(response);
  } else {
    Serial.print("    Error on sending POST: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
}

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);

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

    // อ่านข้อมูลจากเซนเซอร์
    if (readSensor(pm1, pm2_5, pm10) && readDHT11(temperature, humidity)) {
      // ส่งข้อมูลไปยัง API
      sendData(smoke_detector, pm1, pm2_5, pm10, temperature, humidity);
    }
  }

  delay(5000); 
}
