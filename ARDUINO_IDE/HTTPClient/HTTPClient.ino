#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "ASRON";
const char* password = "88888888";

const char* serverName = "http://192.168.137.1:3000/api/data"; // Replace with your server IP

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
    int temperature = 25; // Example temperature
    int humidity = 60;    // Example humidity

    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity)  + "}";
    
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

  delay(5000); // Send data every 60 seconds
}
