
#define PIN_RELAY 15 // Определяем пин, используемый для подключения реле

void setup()
{
  Serial.begin(115200);
  pinMode(PIN_RELAY, OUTPUT); // Объявляем пин реле как выход
}
void loop()
{
  digitalWrite(PIN_RELAY, HIGH); // Отключаем реле - посылаем высокий уровень сигнала
  delay(3000); 
  digitalWrite(PIN_RELAY, LOW);
  delay(3000);
}
