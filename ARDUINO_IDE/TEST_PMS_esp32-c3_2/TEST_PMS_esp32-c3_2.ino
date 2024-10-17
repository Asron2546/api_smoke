#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "vivoV27";
const char* password = "0887528810";

//const char *ssid = "Ronvisly";
//const char *password = "88888888";

const char *serverName = "http://172.25.60.224:3000/api/data"; // Replace with your server IP or API URL

void setup() {
  Serial.begin(115200);

  // Initialize Serial1 (TX = GPIO 7, RX = GPIO 6)
  Serial1.begin(9600, SERIAL_8N1, 6, 7);  // RX, TX for PMS sensor

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    int index = 0;
    char value;
    char previousValue = 0;
    int pm1 = 0;
    int pm2_5 = 0;
    int pm10 = 0;

    while (Serial1.available()) {
      value = Serial1.read();
      
      // Check data header
      if ((index == 0 && value != 0x42) || (index == 1 && value != 0x4d)) {
        Serial.println("Cannot find the data header.");
        break;
      }

      // Read PM values
      if (index == 4 || index == 6 || index == 8 || index == 10 || index == 12 || index == 14) {
        previousValue = value;
      } else if (index == 5) {
        pm1 = 256 * previousValue + value;
        Serial.print("PM1.0: ");
        Serial.print(pm1);
        Serial.print("  ");
      } else if (index == 7) {
        pm2_5 = 256 * previousValue + value;
        Serial.print("PM2.5: ");
        Serial.print(pm2_5);
        Serial.print("  ");
      } else if (index == 9) {
        pm10 = 256 * previousValue + value;
        Serial.print("PM10: ");
        Serial.println(pm10);
      } else if (index > 15) {
        break;
      }
      index++;
    }

    // Clear remaining bytes in Serial1 buffer
    while (Serial1.available()) {
      Serial1.read();
    }

    // Send data to server
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"pm1\":" + String(pm1) + ",\"pm2_5\":" + String(pm2_5) + ",\"pm10\":" + String(pm10) + "}";
    int httpResponseCode = http.POST(jsonData);
    Serial.print("Sending data: ");
    Serial.println(jsonData);
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("Response: ");
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }

  delay(5000); // Send data every 5 seconds
}
