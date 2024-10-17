#include <WiFi.h>
#include <HTTPClient.h>
//#include <SoftwareSerial.h>

const char *ssid = "RMUTSV_IoT";
const char *password = "CoE39201";
//const char* ssid = "vivoV27";
//const char* password = "0887528810";

// const char* serverName = "http://localhost:3000/api/data"; // Replace with your server IP  API
const char *serverName = "http://172.25.60.224:3000/api/data"; // Replace with your server IP  API

void setup()
{

  Serial.begin(115200);

  while (!Serial)
    ;
  // เริ่มต้นการใช้งาน Serial1 (TX = GPIO 7, RX = GPIO 6)
    Serial1.begin(9600, SERIAL_8N1, 6, 7);  // RX, TX สำหรับ PMS
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop()
{

  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;

    int index = 0;
    char value;
    char previousValue;
    String pm1 ;
    String pm2_5;
    String pm10;

    while (Serial1.available())
    {
      value = Serial1.read();
      if ((index == 0 && value != 0x42) || (index == 1 && value != 0x4d))
      {
        Serial.println("Cannot find the data header.");
        break;
      }

      if (index == 4 || index == 6 || index == 8 || index == 10 || index == 12 || index == 14)
      {
        previousValue = value;
      }
      else if (index == 5)
      {
        pm1 = 256 * previousValue + value;
        Serial.print("pm1: ");
        Serial.print(pm1);
        Serial.print("  ");
      }
      else if (index == 7)
      {
        pm2_5 = 256 * previousValue + value;
        Serial.print("pm2_5: ");
        Serial.print(pm2_5);
        Serial.print("  ");
      }
      else if (index == 9)
      {
        pm10 = 256 * previousValue + value;
        Serial.print("pm10: ");
        Serial.print(pm10);
      }
      else if (index > 15)
      {
        break;
      }
      index++;
    }
    while (Serial1.available())
      Serial1.read();

    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"pm1\":" + String(pm1) + ",\"pm2_5\":" + String(pm2_5) + ",\"pm10\":" + String(pm10) + "}";
    int httpResponseCode = http.POST(jsonData);
    Serial.print(jsonData);
    if (httpResponseCode > 0)
    {
      String response = http.getString();
      Serial.print("    ");
      Serial.println(response);
    }
    else
    {
      Serial.print("    Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }

  delay(5000); // Send data every 5 seconds
}
