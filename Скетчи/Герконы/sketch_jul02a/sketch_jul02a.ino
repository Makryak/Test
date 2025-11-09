int Door_Led_Pin = 13; // светодиод

int Door_Sensor_Pin1 = 8;   //геркона 1
int Door_Sensor_Pin2 = 9;   //геркона 2
int Door_Sensor_Pin3 = 10;  //геркона 3

int g1 = 0;   // переменная геркона 1
int g2 = 0;   // переменная геркона 2
int g3 = 0;   // переменная геркона 3
 
void setup() 
{
  Serial.begin(9600);
  pinMode(Door_Led_Pin, OUTPUT);
  pinMode(Door_Sensor_Pin1, INPUT);
  pinMode(Door_Sensor_Pin2, INPUT);
  pinMode(Door_Sensor_Pin3, INPUT);
}
 
void loop()
{
 g1 = digitalRead(Door_Sensor_Pin1); 
  if (g1 == HIGH) 
  {
    digitalWrite(Door_Led_Pin, LOW); 
        Serial.println("g1");
  }else{
    digitalWrite(Door_Led_Pin, HIGH); 
  }
  
  
  
 g2 = digitalRead(Door_Sensor_Pin2);
  if (g2 == HIGH) 
  {
   Serial.println("g2");
  }else{
   Serial.println("Ng2");
  }
  
  
    
 g3 = digitalRead(Door_Sensor_Pin3);
  if (g3 == HIGH) 
  { 
    digitalWrite(Door_Led_Pin, LOW);
        Serial.println("g3"); 
  }else{
    digitalWrite(Door_Led_Pin, HIGH);
  }
}
