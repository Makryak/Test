//Библиотеки*******************************************************************************************************

#include <Wire.h>                   // библиотека для работы I²C
#include <DallasTemperature.h>      // библиотека для работы с датчиком температуры DS18B20
#include <RTClib.h>                 // библиотека для работы с часами реального времени
#include <microDS18B20.h>           // библиотека для погружного датчика температуры
#include <OneWire.h>                // библиотека для взаимодействия с устройствами по шине 1-Wire
#include "DHT.h"                    // библиотека для работы с датчиком DHT22
#include <LiquidCrystal_I2C.h>      // библиотека для LCD

//Рестарт**********************************************************************************************************
int counter = 1;

//LCD дисплей******************************************************************************************************
int lcdColumns = 20;
int lcdRows = 4;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);     // присваиваем имя LCD для дисплея

//Датчик реального времени******************************************************************************************
RTC_DS1307 DS1307_RTC;
char Week_days[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//DHT датчик********************************************************************************************************
uint8_t DHTPin = 5;            // цифровой пин, к которому подключен контакт данных датчика
#define DHTTYPE DHT22          // константа для быстрого переключения типов датчиков, например, можно переопределить DHT11
DHT dht(DHTPin, DHTTYPE);      // инициализируем датчик
float Temperature;
float Humidity;
unsigned long timeDHT; 

//pH датчик**********************************************************************************************************
#define printInterval    800
#define samplingInterval 20
float calibration_value = 70;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10], temp;

//Реле**************************************************************************************************************
#define RELAY_3 15     // Пин для реле 3
//#define RELAY_2 16     // Добавляем пин для реле 2!!!!!!!!!!!!!!
//#define RELAY_4 17     // Добавляем пин для реле 4!!!!!!!!!!!!!

//Герконы************************************************************************************************************
#define Ger_1 12       // Геркон 1
boolean G1;            // Переменная для Ger_1. Тип Boolean т.е. (Да/Нет)
#define Ger_2 14       // Геркон 2
boolean G2;            // Переменная для Ger_2. Тип Boolean т.е. (Да/Нет)

//Кнопки************************************************************************ (Не задействованы на данный момент)
#define Button_1 5           // Кнопка 1 для реле 1 (включает и выключает)
boolean butt_flag_1 = 0;     // вспомогательная переменная для управления реле 2
boolean btState_1;           // считывание состояния кнопки 1
boolean relay_flag = 0;      // вспомогательная переменная для управления реле 2
unsigned long last_press_1;   

// Термодатчик DS18B20***********************************************************************************************
const int oneWireBus = 2;   
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
//MicroDS18B20 <3> qwerty;       // Создаем термометр без адреса на пине D2
//int w;
unsigned long timing;          // Переменная для хранения точки отсчета
/*#define ONE_WIRE_BUS 7               // Указываем пин подключения data-вывода датчика температуры
#define term_power 8                   // Указываем пин подключения питания датчика температуры
OneWire oneWire(ONE_WIRE_BUS);         // Сообщаем библиотеке об устройстве, работающем по протоколу 1-Wire
DallasTemperature sensors(&oneWire);   // Связываем функции библиотеки DallasTemperature с нашим 1-Wire устройством (DS18B20)
*/

void setup()
{
  Serial.begin(115200);               // задает скорость передачи данных в бит/с (бод)
  delay(100);
  
  lcd.begin();                        // инициализация lcd   
  lcd.backlight();                    // включаем подсветку
  lcd.setCursor(5, 1);                // указываем координаты Aquaponics
  lcd.print("Aquaponics");            // выводим надпись Aquaponics
  lcd.setCursor(5, 2);                // указываем координаты для версии аквапоники
  lcd.print("beta 0.0.1");            // выводим надпись версии аквапоники

  if (!DS1307_RTC.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  pinMode(DHTPin, INPUT);
  dht.begin();                        // запускаем датчик DHT22

  sensors.begin();                    // Запускаем библиотеку измерения температуры DS18B20  
//  pinMode(term_power, OUTPUT);      // Определяем пин подключения питания датчика температуры DS18B20

  pinMode(RELAY_3, OUTPUT);           // Определяем пин реле 3 как выход
  //pinMode(RELAY_2, OUTPUT);           // Определяем пин реле 2 как выход!!!!!!!!!
  //pinMode(RELAY_4, OUTPUT);           // Определяем пин реле 4 как выход!!!!!!!!!!!

  pinMode(Ger_1, INPUT);              // Определяем пин геркона как вход
  pinMode(Ger_2, INPUT);              // Определяем пин геркона как вход

//  pinMode(Button_1, INPUT_PULLUP);  // Определяем пин кнопки как вход со стягивающим резистором
  delay(2000);                        // задержка 2 секунды
  lcd.clear();                        // очистка lcd от текста   
 // ESP.restart();                    // перезапуск ESP32
}

void loop()
{
  delay(1000);
  ds18b20();
  dht22();
  ph_sensor();
//  button();
//  real_time_clock();

//Реле, которое управляется часами реального времени***********************************************
  DateTime now = DS1307_RTC.now();
  float Hour = now.hour();            // переменная, которая запоминает текущий час
  float y = 7;                        // час, когда включиться свет
  float n = 20;                       // час, когда выключиться свет

  if (Hour < n && Hour > y)
  {
    //digitalWrite(RELAY_2, LOW);!!!!!!
    //digitalWrite(RELAY_4, LOW);!!!!!!
  }
  else
  {
    //digitalWrite(RELAY_2, HIGH);!!!!!
    //digitalWrite(RELAY_4, HIGH);!!!!!
  }
  
//Работа с сенсором DHT_22*************************************************************************

  static uint32_t printErrorDHT = millis();
  if (Humidity > 60)
  {
    digitalWrite(RELAY_3, LOW);
  }
  else
  {
    digitalWrite(RELAY_3, HIGH);
  }

// Термодатчик*****************************************************************************************
// float e = qwerty.getTemp(); // закомментированный код
// if(e > 29)
// {
//   tone(13, 600);                   // включаем пищалку 
// }
// else
// {
//   noTone (13);                   // выключаем пищалку
// }

// Герконы*****************************************************************************************
  G2 = digitalRead(Ger_2);            // считываем и запоминаем значение геркона
  if (G2 == HIGH) 
  { 
    lcd.setCursor(17, 0);              // задаём начальные координаты для надписи на LCD дисплее
    lcd.print("MAX");                 // выводим надпись на LCD дисплей
    tone (13, 600);
  }
  else
  {
    lcd.setCursor(17, 0);              // задаём начальные координаты для надписи на LCD дисплее
    lcd.print("   ");                 // ничего не выводим на LCD дисплей
    noTone (13);
  }

  G1 = digitalRead(Ger_1);            // считываем и запоминаем значение геркона
  if (G1 == LOW) 
  { 
    lcd.setCursor(17, 2);              // задаём начальные координаты для надписи на LCD дисплее
    lcd.print("MIN");                 // выводим надпись на LCD дисплей
    tone (13, 600);                   // включаем пищалку
  }
  else
  {
    lcd.setCursor(17, 2);              // задаём начальные координаты для надписи на LCD дисплее
    lcd.print("   ");                 // ничего не выводим на LCD дисплей
    noTone (13);                      // выключаем пищалку
  } 

  if (G2 == LOW && G1 == HIGH)
  {
    lcd.setCursor(18, 1);              // задаём начальные координаты для надписи на LCD дисплее
    lcd.print("*");                   // выводим надпись на LCD дисплей
  }
  else
  {
    lcd.setCursor(17, 1);              // задаём начальные координаты для надписи на LCD дисплее
    lcd.print("  ");                  // ничего не выводим на LCD дисплей
  }
}

//******************************************************************************************************************
//      В этой части будут храниться функции
//******************************************************************************************************************

void resetAll()
{
  ESP.restart();
}

void button()
{
btState_1 = !digitalRead(Button_1);              // читаем инвертированное значение для удобства
 if (btState_1 == 1 && butt_flag_1 == 0 && millis() - last_press_1 > 100)    // проверка нажатия
  {  
    butt_flag_1 = 1;
    relay_flag = !relay_flag;
    digitalWrite(RELAY_3, relay_flag);
    last_press_1 = millis();
  } 
 if (btState_1 == 0 && butt_flag_1 == 1)         // проверка отпускания
  {  
    butt_flag_1 = 0;
  }
}

void real_time_clock()
{
    DateTime now = DS1307_RTC.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(Week_days[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    delay(1000);
}

//Функция термодатчика DS18B20*************************************************************************
void ds18b20()
{
  sensors.requestTemperatures(); // Получаем измеренную температуру
  float tempC = sensors.getTempCByIndex(0); // Получаем температуру в Цельсиях с первого датчика (0)
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempC);
  lcd.print((char)223); // Символ градуса
  lcd.print("C");
}

//Функция DHT22*************************************************************************
void dht22()
{
  Temperature = dht.readTemperature(); // Считываем температуру с датчика DHT22
  Humidity = dht.readHumidity(); // Считываем влажность с датчика DHT22
  if (isnan(Temperature) || isnan(Humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(Temperature);
  lcd.print((char)223); // Символ градуса
  lcd.print("C");
  lcd.setCursor(0, 2);
  lcd.print("Hum: ");
  lcd.print(Humidity);
  lcd.print("%");
}

//Функция датчика pH*************************************************************************
void ph_sensor()
{
  for (int i = 0; i < 10; i++)
  {
    buffer_arr[i] = analogRead(A0); // Заполнение буфера данными с аналогового пина A0
    delay(30);
  }
  avgval = 0;
  for (int i = 0; i < 10; i++)
  {
    avgval += buffer_arr[i];
  }
  avgval /= 10;
  float volt = avgval * 5.0 / 1024;
  float ph_act = -5.70 * volt + calibration_value / 100;
  lcd.setCursor(0, 3);
  lcd.print("pH: ");
  lcd.print(ph_act, 2);
}
