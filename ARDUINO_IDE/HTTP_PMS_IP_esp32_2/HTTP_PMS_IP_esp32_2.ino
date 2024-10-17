#include <VIRUS_WIFI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(21, 22); // RX, TX


const char *ssid = "RMUTSV_IoT";
const char *password = "CoE39201";
//const char *ssid = "Ronvisly";
//const char *password = "88888888";

const char *serverName = "http://192.168.1.109:3000/api/data"; // Replace with your server IP or API
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

// ฟังก์ชันสำหรับการอ่านค่าฝุ่นจากเซนเซอร์
bool readSensor(String &pm1, String &pm2_5, String &pm10) {
  int index = 0;
  char value;
  char previousValue;
  bool validData = true;

  while (mySerial.available()) {
    value = mySerial.read();
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
      Serial.print("pm1: ");
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
    }
    else if (index > 15) {
      break;
    }
    index++;
  }

  while (mySerial.available())
    mySerial.read();  // เคลียร์ buffer

  return validData;
}

// ฟังก์ชันสำหรับการส่งข้อมูลไปยัง API
void sendData(String smoke_detector, String pm1, String pm2_5, String pm10) {
  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");

  // สร้าง JSON ข้อมูล
  String jsonData = "{\"smoke_detector\":\"" + String(smoke_detector) + "\",\"pm1\":" + String(pm1) + ",\"pm2_5\":" + String(pm2_5) + ",\"pm10\":" + String(pm10) + "}";
  Serial.println(jsonData);  // พิมพ์ JSON ที่จะส่งออกไป                 

  // ส่งข้อมูลแบบ POST
  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.print("    ");
    Serial.println(response);
  }
  else {
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

    // อ่านข้อมูลจากเซนเซอร์
    if (readSensor(pm1, pm2_5, pm10)) {
      // ส่งข้อมูลไปยัง API
      sendData(smoke_detector, pm1, pm2_5, pm10);
    }
  }

  delay(5000); // รอ 5 วินาทีก่อนส่งข้อมูลใหม่
}
