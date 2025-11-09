#include <Wire.h>                    // библиотека для работы I²C
#include <LiquidCrystal_I2C.h>        // подключаем библиотеку для LCD

const int potPin=A0;
float ph;
float Value=0;

int lcdColumns = 20;
int lcdRows = 4;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);     // присваиваем имя LCD для дисплея

 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
lcd.begin();                      // инициализация lcd   
  lcd.backlight();                  // включаем подсветку
  
  pinMode(potPin,INPUT);
  delay(1000);
}
 void loop(){
 
    Value= analogRead(potPin);
 /*   Serial.print(Value);
    Serial.print(" | ");*/
    float voltage=Value*(3.3/4095.0);
    ph=(3.3*voltage);
    lcd.setCursor(5, 1); 
    lcd.print(ph);
    delay(500);
 }
