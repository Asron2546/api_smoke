unsigned int pm1 = 0;
unsigned int pm2_5 = 0;
unsigned int pm10 = 0;

void setup()
{
    Serial.begin(115200);  // Serial Monitor
    while (!Serial)
        ;

    // เริ่มต้นการใช้งาน Serial1 (TX = GPIO 7, RX = GPIO 6)
    Serial1.begin(9600, SERIAL_8N1, 6, 7);  // RX, TX สำหรับ PMS
}

void loop()
{
    int index = 0;
    char value;
    char previousValue;

    while (Serial1.available())  // อ่านจาก Serial1 แทน mySerial
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
            Serial.print("{ ");
            Serial.print("\"pm1\": ");
            Serial.print(pm1);
            Serial.print(" ug/m3");
            Serial.print(", ");
        }
        else if (index == 7)
        {
            pm2_5 = 256 * previousValue + value;
            Serial.print("\"pm2_5\": ");
            Serial.print(pm2_5);
            Serial.print(" ug/m3");
            Serial.print(", ");
        }
        else if (index == 9)
        {
            pm10 = 256 * previousValue + value;
            Serial.print("\"pm10\": ");
            Serial.print(pm10);
            Serial.print(" ug/m3");
        }
        else if (index > 15)
        {
            break;
        }
        index++;
    }
    while (Serial1.available())
        Serial1.read();
    Serial.println(" }");
    delay(1000);
}
