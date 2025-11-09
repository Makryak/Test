#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);
#include <dht.h> 
dht DHT; 
#define DHT22_PIN 12                    //DHT data pin here I used D7 you can change it
#define ONE_WIRE_BUS 2                  // Указываем пин подключения data-вывода датчика температуры
#define term_power 4                    // Указываем пин подключения питания датчика температуры
OneWire oneWire(ONE_WIRE_BUS);          // Сообщаем библиотеке об устройстве, работающем по протоколу 1-Wire
DallasTemperature sensors(&oneWire);    // Связываем функции библиотеки DallasTemperature с нашим 1-Wire устройством (DS18B20)
#if defined(__AVR__)
#define OPERATING_VOLTAGE   5.0
#define ZERO_SHIFT          0
#else
#define OPERATING_VOLTAGE   3.3
#define ZERO_SHIFT          1.1
#endif
#define CALIBRATION_FACTOR  3.5          // Коэффициент перевода напряжения в концентрацию pH
constexpr auto pinSensor = A0;           // Назначаем пин для подключения датчика


unsigned long prev_ms1 = 0;
unsigned long prev_ms2 = 0;
unsigned long prev_ms3 = 0;

int gerkon1 = 8;
int gerkon2 = 9;
int gerkon3 = 10;

bool g1 = false;
bool g2 = false;
bool g3 = false;

void setup() 
{
  Serial.begin(9600);
  sensors.begin();                  // Запускаем библиотеку измерения температуры
  pinMode(term_power, OUTPUT);      // Определяем пин подключения питания датчика температуры
  lcd.init();                       // Инициализация lcd             
  lcd.backlight();                  // Включаем подсветку
  lcd.setCursor(5, 1);              //Устанавливаем координаты текста
  lcd.print("Aquaponics"); 
  lcd.setCursor(6, 2);
  lcd.print("beta_0.1");     
  
  pinMode(gerkon1, INPUT);
  pinMode(gerkon2, INPUT);
  pinMode(gerkon3, INPUT);

  delay(2000);
}
void loop() 
{
  
  lcd.clear();

  
//unsigned long currentTime1 = millis();
 if(millis() - prev_ms1 >= 10)
 {
  g1 = digitalRead(gerkon1);
  if (g1 == HIGH){
    prev_ms1 = millis();            
    lcd.setCursor(0,0);
    lcd.print("g1");
    g1 = LOW;
    Serial.println("g1"); 
        delay(200);      
  }
    else
    {

    }
 }

//unsigned long currentTime2 = millis();
if (millis() - prev_ms2 >= 10) 
{
    g2 = digitalRead(gerkon2);
  prev_ms2 = millis();
   if (g2 == HIGH) 
   {
    lcd.setCursor(0,1);
    lcd.print("g2");
    Serial.println("g2");
    delay(200);
    }
    else
    {

    }
}

//unsigned long currentTime3 = millis();
 if(millis() - prev_ms3 >= 10)
 {
      g3 = digitalRead(gerkon3);
   prev_ms3 = millis(); 
    if (g3 == HIGH)
   {    
    lcd.setCursor(0,2);
    lcd.print("g3");
    Serial.println("g3");
        delay(200);
   }
    else
   {

   }
}
}




     
