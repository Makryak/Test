const int gerkon1 = 8;   //геркона 1
const int gerkon2 = 9;   //геркона 2
const int gerkon3 = 10;  //геркона 3

bool g1 = false;         // переменная геркона 1
bool g2 = false;         // переменная геркона 2
bool g3 = false;         // переменная геркона 3

unsigned long prev_ms1;
unsigned long prev_ms2;
unsigned long prev_ms3;

void setup() {
Serial.begin(9600);
pinMode(gerkon1, INPUT);
pinMode(gerkon2, INPUT);
}

void loop(){
if (digitalRead(gerkon1) == HIGH) 
{
  Serial.println("g1");
  g1 = true;
  prev_ms1 = millis();
}

if (digitalRead(gerkon2) == HIGH) 
{
  Serial.println("g2");
  g2 = true;
  prev_ms2 = millis();
}
if(g1)
 if(millis() - prev_ms1 > 30000) {
  digitalWrite(g1, LOW);
  g1 = false;
}
if(g2)
  if(millis() - prev_ms2 > 30000) {
  digitalWrite(g2, LOW);
  g2 = false;
 }
}
