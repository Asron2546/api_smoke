#include <WiFi.h>
#include <HTTPClient.h>
#include "SensorReader.h"  // include ไฟล์ใหม่ที่แยกออกมา
#include "DHT11.h"
#include "MQ135.h"

#define led_state D9

Bonezegei_DHT11 dht(D3);        // ขา D3 สำหรับ DHT11
const int buzzer = D10;          // ขา D10 สำหรับ buzzer
long last_time;
#define sound_sensor A1 
volatile bool sound_state = 0;
unsigned long sound_time = 0; // ใช้สำหรับจับเวลา

const char *ssid = "RMUTSV_IoT";
const char *password = "CoE39201";
//const char *ssid = "Error404_2.4GHz";
//const char *password = "10011001";
//const char *ssid = "Ronvisly";    // ใส่ชื่อ Wi-Fi
//const char *password = "88888888";  // ใส่รหัสผ่าน Wi-Fi
//const char *ssid = "VIRUS_2.4GHz";
//const char *password = "073332227";

const char *serverName = "http://172.16.8.155:5000/devices/data/device001";
  
// ฟังก์ชันสำหรับการเชื่อมต่อ WiFi
void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(led_state, LOW);
    delay(250);
    digitalWrite(led_state, HIGH);
    delay(250);
    digitalWrite(led_state, LOW);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  for(int i = 0; i < 4; i++){
      digitalWrite(led_state, HIGH);
      delay(100);
      digitalWrite(led_state, LOW);
      delay(100);
  }
}

// ฟังก์ชันสำหรับการส่งข้อมูลไปยัง API
void sendData( String pm1, String pm2_5, String pm10, String temperature, String humidity, String co2, String ammonia, String gas) {
  HTTPClient http;
  http.begin(serverName);
  http.setTimeout(10000);  // ตั้งเวลา Timeout เป็น 10 วินาที
  http.addHeader("Content-Type", "application/json");

//  String jsonData = "{\"pm1\":" + pm1 + 
//                    ",\"pm2_5\":" + pm2_5 + 
//                    ",\"pm10\":" + pm10 +
//                    ",\"temperature\":" + temperature +
//                    ",\"humidity\":" + humidity +
//                    ",\"co2\":" + co2 +
//                    ",\"ammonia\":" + ammonia +
//                    ",\"gas\":" + gas + "}";  
String jsonData = "{\"pm1\":" + String(pm1) + 
                  ",\"pm2_5\":" + String(pm2_5) + 
                  ",\"pm10\":" + String(pm10) +
                  ",\"temperature\":" + String(temperature) +
                  ",\"humidity\":" + String(humidity) +
                  ",\"co2\":" + String(co2) +
                  ",\"ammonia\":" + String(ammonia) +
                  ",\"gas\":" + String(gas) + "}";

  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.println("Error on sending POST: " + String(httpResponseCode));
  }
  
  http.end();
}

void IRAM_ATTR soundSensor(){
  if(sound_state == 1){
    sound_state = 0;
    digitalWrite(buzzer, LOW);
    sound_time = millis();
  }
}

  
void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 6, 7); // ใช้ Serial1 แทน SoftwareSerial
  pinMode(buzzer, OUTPUT);        // buzzer
  digitalWrite(buzzer, HIGH);
  pinMode(led_state, OUTPUT);     // led_state
  digitalWrite(led_state, LOW);
  pinMode(sound_sensor, INPUT_PULLUP);      // PULLUP
  attachInterrupt(digitalPinToInterrupt(sound_sensor), soundSensor, FALLING);    // เปลียนจาก 1 -> 0

  setupMQ135();
  connectWiFi();
}

void loop() {
  if(sound_state == 0 && (millis()- sound_time >= 500)){
    sound_state = 1;
    digitalWrite(buzzer, HIGH);
    }
  if(millis() - last_time > 5000){
    if(WiFi.status() == WL_CONNECTED) {
      String pm1 = "0";
      String pm2_5 = "0";
      String pm10 = "0";
      String temperature = "0";
      String humidity = "0";
      
      
      // อ่านข้อมูลจากเซ็นเซอร์ PM และ DHT11
      readSensor(pm1, pm2_5, pm10);
      readDHT11(temperature, humidity);
 
      // อ่านค่า MQ135 และคำนวณค่า co2, ammonia, gas
      int sensorValue = analogRead(MQ_135);
      String co2 = String(analysisco2(sensorValue), 3);
      String ammonia = String(analysisammonia(sensorValue), 3);
      String gas = String(analysisgas(sensorValue), 3);
      Serial.println("\nco2: " + co2 + "  ammonia: " + ammonia +"  gas: " + gas);
        
      // ส่งข้อมูลไปยัง API
      sendData(pm1, pm2_5, pm10, temperature, humidity, co2, ammonia, gas);
      
      digitalWrite(led_state, HIGH);
      delay(50);
      digitalWrite(led_state, LOW);
      last_time = millis();
     }
  }
}