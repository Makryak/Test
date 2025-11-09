
#define PIN_RELAY 10 // Определяем пин, используемый для подключения реле

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_RELAY, OUTPUT); // Объявляем пин реле как выход
}
void loop()
{
  digitalWrite(PIN_RELAY, HIGH); // Отключаем реле - посылаем высокий уровень сигнала
  delay(1000); 
  digitalWrite(PIN_RELAY, LOW);
  delay(1000);
}
