#define MQ135 2

void setup()
{  
  Serial.begin(115200);   
  pinMode(MQ135,INPUT);                         
 }
void loop()
{
  int MQ135_value = analogRead(MQ135);       
  Serial.print(MQ135_value);               
  Serial.println(" PPM");
  delay(800);     
}                          
