bool flag = false;

void setup() {
  Serial.begin(9600);
  pinMode(7, INPUT_PULLUP);
}

void loop() 
{
  bool btnState = !digitalRead(7);  // читаем инвертированное значение для удобства
  if (btnState && !flag) // обработчик нажатия
  {  
    flag = true;
    Serial.println("press");
  }
  if (!btnState && flag) // обработчик отпускания
  {  
    flag = false;
  }
}
