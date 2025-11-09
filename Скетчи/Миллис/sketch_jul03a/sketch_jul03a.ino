unsigned long last_time1;
unsigned long last_time2;
unsigned long last_time3;

#include <OneWire.h> 

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);
int gerkon1 = 8;   //геркона 1
int gerkon2 = 9;   //геркона 2
int gerkon3 = 10;  //геркона 3

int g1 = 0;   // переменная геркона 1
int g2 = 0;   // переменная геркона 2
int g3 = 0;   // переменная геркона 3


void setup() {

  Serial.begin(9600);

  lcd.init();                       // Инициализация lcd             
  lcd.backlight();                  // Включаем подсветку
  lcd.setCursor(5, 1);              //Устанавливаем координаты текста
  lcd.print("Aquaponics"); 
  lcd.setCursor(6, 2);
  lcd.print("beta_0.1");     
  delay(2000);
  

  pinMode(gerkon1, INPUT);
  pinMode(gerkon2, INPUT);
  pinMode(gerkon3, INPUT);
}

void loop(){
  lcd.clear();             // Очистка экрана
  
  if(millis() - last_time1 > 200){
    last_time1 = millis();
    g1 = digitalRead(gerkon1); 
    if (g1 == HIGH)
  {
    lcd.setCursor(18,0);
    lcd.print("g1");
    Serial.println("g1");
  }
  else
  {
    
  }
}
  if(millis() - last_time2 > 200){
    last_time2 = millis();
    g2 = digitalRead(gerkon2); 
    if (g2 == HIGH)
  {
    lcd.setCursor(18,0);
    lcd.print("g2");
    Serial.println("g2");
  }
  else
  {
    
  }
}
  if(millis() - last_time3 > 200){
    last_time3 = millis();
    g3 = digitalRead(gerkon3); 
    if (g3 == HIGH)
  {
    lcd.setCursor(18,0);
    lcd.print("g3");
    Serial.println("g3");
  }
  else
  {
    
  }
}
}
