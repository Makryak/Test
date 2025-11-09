#include <LiquidCrystal_I2C.h>      // подключаем библиотеку дисплея
LiquidCrystal_I2C lcd(0x27, 16, 2); // адрес, столбцов, строк
#include <microDS18B20.h>
MicroDS18B20<3> sensor;     // указываем пин
void setup() {
  lcd.init();           // инициализация
  lcd.backlight();      // включить подсветку
}
void loop() {
  sensor.requestTemp();   // запросить температуру
  delay(1000);            // ждём ответа
  lcd.home();             // курсор в 0,0
  lcd.print(sensor.getTemp());  // выводим температуру
  lcd.write(223);         // символ градуса
  lcd.print("C  ");       // С и пара пробелов для очистки
  
}
