//#include <DallasTemperature.h>

//Библиотеки

#include <Wire.h>                  //библиотека для работы I²C
#include "TroykaRTC.h"             // библиотека для работы с часами реального времени
#include <microDS18B20.h>          // библиотека для погружного датчика температуры
#include <OneWire.h>               // Подключаем библиотеку для взаимодействия с устройствами, работающими на шине и по протоколу 1-Wire
#include <Wire.h>                  // библиотека для протокола I2C 
#include <LiquidCrystal_I2C.h>      // подключаем библиотеку для LCD
LiquidCrystal_I2C lcd(0x27,20,4);   // присваиваем имя LCD для дисплея
#include <DHT.h> 

//Датчик рельного времени

#define LEN_TIME 12                // размер массива для времени
#define LEN_DATE 12                // размер массива для даты
#define LEN_DOW 12                // размер массива для дня недели

RTC clock;                        // создаём объект для работы с часами реального времени

char time[LEN_TIME];              // массив для хранения текущего времени
char date[LEN_DATE];              // массив для хранения текущей даты
char weekDay[LEN_DOW];            // массив для хранения текущего дня недели

//DHT датчик-*
#define DHTPIN 2           // цифровой пин, к которому подключен контакт данных датчика
#define DHTTYPE DHT22      // константа для быстро переключение типов датчиков, например, можно переопределить DHT11
DHT dht(DHTPIN, DHTTYPE);   // инициализируем датчик


//pH датчик
#define SensorPin        A0
#define printInterval    800
#define samplingInterval 20
float calibration_value = 21.34;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;

//Реле
#define RELAY_1 6     // Реле 1
#define RELAY_2 7     // Реле 2
#define RELAY_3 8     // Реле 2
#define RELAY_4 9      // Реле 2

//Герконы
#define Ger_1 11       //Геркона1 для реле2 (включает и выключает)
boolean G1;           // Переменная для Ger_1. Тип Boolean т.е. (Да/Нет)
#define Ger_2 12       //Геркона1 для реле2 (включает и выключает)
boolean G2;           // Переменная для Ger_1. Тип Boolean т.е. (Да/Нет)

//Кнопки
#define Button_1 5       //Кнопка1 для реле1 (включает и выключает)
boolean butt_flag_1 = 0;   // вспомогательная переменная для уапрвления реле2
boolean btState_1;        // считывание хранение состояния кпопки 1
boolean relay_flag = 0;     //вспомогательная переменная для уапрвления реле2
unsigned long last_press_1;   

// Термодатчик DS18B20
MicroDS18B20 <3> qwerty;

/*#define ONE_WIRE_BUS 7                  // Указываем пин подключения data-вывода датчика температуры
#define term_power 8                   // Указываем пин подключения питания датчика температуры
OneWire oneWire(ONE_WIRE_BUS);          // Сообщаем библиотеке об устройстве, работающем по протоколу 1-Wire
DallasTemperature sensors(&oneWire);    // Связываем функции библиотеки DallasTemperature с нашим 1-Wire устройством (DS18B20)
*/

void setup()
{
  Serial.begin(9600);
  clock.begin();                    // инициализация часов 
  lcd.init();                       // Инициализация lcd     
 // clock.set(__TIMESTAMP__);         // метод установки времени и даты автоматически при компиляции
  dht.begin();                      // запускаем датчик DHT22
  lcd.backlight();                  // Включаем подсветку
  lcd.setCursor(5, 1);              //Указываем координаты Aquaponics
  lcd.print("Aquaponics");         // Выводим надпись Aquaponics
  lcd.setCursor(6, 2);             // Указываем координаты для beta_0.1
  lcd.print("beta_1.5");           // Выводим надпись beta_0.1
 //sensors.begin();                  // Запускаем библиотеку измерения температуры DS18B20  
 //pinMode(term_power, OUTPUT);      // Определяем пин подключения питания датчика температуры DS18B20
  pinMode(RELAY_1, OUTPUT); // Определяем пин реле1 как выход
  pinMode(RELAY_2, OUTPUT); // Определяем пин реле2 как выход
  pinMode(RELAY_3, OUTPUT); // Определяем пин реле2 как выход
  pinMode(RELAY_4, OUTPUT); // Определяем пин реле2 как выход
  pinMode(13, OUTPUT); // объявляем пин 13 как выход для динамика
  pinMode(Ger_1, INPUT_PULLUP); // Определяем пин геркона как вход
  pinMode(Ger_2, INPUT_PULLUP); // Определяем пин геркона как вход
  pinMode(Button_1, INPUT_PULLUP); // Определяем пин кнопки как вход со стягивающим резистором
  delay(2000);  // Задержка в 2 секунды
  lcd.clear(); // Очистка lcd от текста
  delay(500);  // Задержка в 0,5 секунды
}


void ph_sensor()
{
static unsigned long printTime    = millis();
static unsigned long samplingTime = millis(); 
static float ph_act, volt;
  
  lcd.setCursor(0, 0);
  lcd.print("pH value: ");

if(millis() - samplingTime > samplingInterval){ 
  for(int i=0;i<10;i++) 
   {
    buffer_arr[i] = analogRead(A0);
   }
 for(int i=0;i<9;i++)
  {
    for(int j=i+1;j<10;j++)
     {
      if(buffer_arr[i]>buffer_arr[j])
       {
        temp=buffer_arr[i];
        buffer_arr[i]=buffer_arr[j];
        buffer_arr[j]=temp;
       }
     }
   }
 avgval=0;
 for(int i=2;i<8;i++)
 {
 avgval+=buffer_arr[i];
 volt=(float)avgval*5.0/1024/6;
 ph_act = -7.3 * volt + calibration_value;
 }
 samplingTime = millis(); 
}

 //  Выводим показания в LCD:
 if(millis() - printTime > printInterval)
 {
  lcd.print(ph_act);
  
  Serial.print(ph_act,0);                                                                                          //Вывод в Excel
  Serial.print(",");
  printTime = millis(); 
 }
}


 void dht22()
 {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  lcd.setCursor(0,2);
  
 // Serial.print("Humidity: ");
 // Serial.print(h,0);                                                                                          //Вывод в Excel
 // Serial.print(",");
    
  lcd.print("Humidity: ");
  lcd.print(h,1);
  lcd.print("%  ");
  
  //Serial.print("Temperature: ");
 // Serial.print(t,0);                                                                                           //Вывод в Excel
//  Serial.print(",");
    
  lcd.setCursor(0, 3);
  lcd.print("Temperature: ");
  lcd.print(t,1);                                                                                        
 }

 void ds18b20()
 {
  qwerty.requestTemp();   // запросить температуру
  delay(1000);            // ждём ответа
  lcd.setCursor(0,1);             // курсор в 0,0 
  lcd.print("Water T: ");
  lcd.print(qwerty.getTemp());  // выводим температуру на lcd 
 // Serial.print("Water T: ");                                                                                   //Вывод в Excel 
//  Serial.print(qwerty.getTemp(),0);
 // Serial.println(",");
  lcd.write(223);         // символ градуса
  lcd.print("C  ");       // С и пара пробелов для очистки
 }


void loop()
{ 

//Датчик рельного времени
{
  clock.read();          // запрашиваем данные с часов
  clock.getTimeStamp(time, date, weekDay); // сохраняем текущее время, дату и день недели в переменные
  float Hour = clock.getHour();
  // выводим в serial порт текущее время, дату и день недели
  Serial.print(time);
  Serial.print("\t");
  Serial.print(date);
  Serial.print("\t");
  Serial.print(weekDay);
 
  delay(1000);           // ждём одну секунду 
  
 //Реле на свет под упр реального времени
float y = 7;
float n = 20;
  if( Hour < n and Hour > y)
  {
    digitalWrite(RELAY_2, LOW);
     digitalWrite(RELAY_4, LOW);
  }
  else
  {
    digitalWrite(RELAY_2, HIGH);
    digitalWrite(RELAY_4, HIGH);
  }
}


ds18b20();
dht22();
ph_sensor();
              //DHT 
  float h = dht.readHumidity();
 // Serial.print(dht.readHumidity(),0);                                                                                          //Вывод в Excel
 // Serial.print(",");
  float t = dht.readTemperature();
 // Serial.print(dht.readTemperature(),0);                                                                                          //Вывод в Excel 
//  Serial.print(",");
   static uint32_t printErrorDHT = millis();
  if(h > 60)
  {
    digitalWrite(RELAY_3, LOW);
  }
  else
  {
    digitalWrite(RELAY_3, HIGH);
  }


           // Кнопка 1
btState_1 = !digitalRead(Button_1);  // читаем инвертированное значение для удобства
 if (btState_1 == 1 && butt_flag_1 == 0 && millis() - last_press_1 > 100)  // обработчик нажатия
  {  
    butt_flag_1 = 1;
    relay_flag = !relay_flag;
    digitalWrite(RELAY_2, relay_flag);
    last_press_1 = millis();
  } 
 if (btState_1 == 0 && butt_flag_1 == 1)  // обработчик отпускания
  {  
    butt_flag_1 = 0;
  }

         // Герконы
G2 = digitalRead(Ger_2);
  if (G2 == HIGH) 
  { 
  lcd.setCursor(17,0);
  lcd.print("MAX");
  }
  else
  {
  lcd.setCursor(17,0);
  lcd.print("   ");
  }

  if( G2 == LOW && G1 == HIGH)
  {
  lcd.setCursor(18,1);
  lcd.print("*");
  }
  else{
  lcd.setCursor(17,1);
  lcd.print("  ");
  }


   G1 = digitalRead(Ger_1);
  if (G1 == LOW) 
  { 
  lcd.setCursor(17,2);
  lcd.print("MIN");
  tone (13, 600);
  }
  else
  {
  lcd.setCursor(17,2);
  lcd.print("   ");
  noTone (13);
  } 
}
 
 
