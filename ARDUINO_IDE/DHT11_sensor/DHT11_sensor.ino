#define MQ135 2

void setup()
{  
  Serial.begin(9600);   
  pinMode(MQ135,INPUT);                         
 }
void loop()
{
  int MQ135_value = analogRead(MQ135);       
  Serial.print(MQ135_value);               
  Serial.println(" PPM");
  delay(100);     
}                          
