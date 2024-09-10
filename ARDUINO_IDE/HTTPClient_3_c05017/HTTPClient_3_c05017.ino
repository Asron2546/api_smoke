#include <WiFi.h>
#include <HTTPClient.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(21,22); // RX, TX

unsigned int pm1 = 0;
unsigned int pm2_5 = 0;
unsigned int pm10 = 0;

const char* ssid = "Gg";
const char* password = "12345678";

//const char* serverName = "http://localhost:3000/api/data"; // Replace with your server IP  API
const char* serverName = "http://192.168.43.232:3000/api/data"; // Replace with your server IP  API

void setup() {
  Serial.begin(115200);
  while (!Serial) ;
  mySerial.begin(9600);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  
  int index = 0;
  char value;
  char previousValue;
  
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
   
    while (mySerial.available()) {           // อ่านค่า
    value = mySerial.read();
    
    if ((index == 0 && value != 0x42) || (index == 1 && value != 0x4d)){
       Serial.println("Cannot find the data header.");
       break;
    }
    if (index == 4 || index == 6 || index == 8 || index == 10 || index == 12 || index == 14) {
       previousValue = value;
    }
    else if (index == 5) {
       pm1 = 256 * previousValue + value;
       Serial.print("{ ");
       Serial.print("\"pm1\": ");
       Serial.print(pm1);
       Serial.print(" ug/m3");
       Serial.print(", ");
    }
    else if (index == 7) {
       pm2_5 = 256 * previousValue + value;
       Serial.print("\"pm2_5\": ");
       Serial.print(pm2_5);
       Serial.print(" ug/m3");
       Serial.print(", ");
    }
    else if (index == 9) {
       pm10 = 256 * previousValue + value;
       Serial.print("\"pm10\": ");
       Serial.print(pm10);
       Serial.print(" ug/m3");
    }
    else if (index > 15) {
       break;
    }
    index++;
    }
    while(mySerial.available()) mySerial.read();
    Serial.println(" }");

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
