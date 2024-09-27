#ifndef DHT11_H
#define DHT11_H

#include <Bonezegei_DHT11.h>

extern Bonezegei_DHT11 dht;  //เป็นการอิงจาก


bool readDHT11(String &temperature, String &humidity) {

  if (dht.getData()) {                         // get All data from DHT11
    temperature = dht.getTemperature();      // return temperature in celsius
    humidity = dht.getHumidity();               // return humidity
    String str  = " temperature: ";
           str += temperature;
           str += "°C  ";
           str += "  humidity: ";
           str += humidity;
    Serial.print(str.c_str());
  }
 return readDHT11;
}

#endif  // ปิดเงื่อนไขการตรวจสอบ #ifndef
