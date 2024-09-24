#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "RMUTSV_IoT";
const char *password = "CoE39201";

//const char* serverName = "http://localhost:3000/api/data"; // Replace with your server IP  API
const char* serverName = "http://172.25.62.67:3000/api/data"; // Replace with your server IP  API

void setup() {
  Serial.begin(115200);

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

    // Your data to send (replace with your actual sensor readings)
    int pm1 = 25;
    int pm2_5 = 60;
    int pm10 = 55;

    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"pm1\":" + String(pm1) + ",\"pm2_5\":" + String(pm2_5) + ",\"pm10\":" + String(pm10) + "}";
    
    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }

  delay(5000); // Send data every 5 seconds
}
