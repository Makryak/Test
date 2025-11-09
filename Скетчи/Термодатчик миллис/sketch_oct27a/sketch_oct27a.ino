#include <DS_raw.h>
#include <microDS18B20.h>
#include <microOneWire.h>
#include <LiquidCrystal_I2C.h>      // подключаем библиотеку для LCD
LiquidCrystal_I2C lcd(0x27,20,4);   // присваиваем имя LCD для дисплея
#include <Wire.h>                  // библиотека для протокола I2C 

MicroDS18B20<3> ds;
void setup()
{
  Serial.begin(9600);

  lcd.init();                       // Инициализация lcd       
   lcd.backlight();                  // Включаем подсветку      
}
void loop()
{  
  static uint32_t timer = millis();
if (millis() - timer >= 1000) 
 {
   timer = millis();
   if (ds.readTemp())
    {
      lcd.setCursor(0,1);
     lcd.print("Water temp: ");
      lcd.println(ds.getTemp(), 1);

     }
    ds.requestTemp();
  }
}
