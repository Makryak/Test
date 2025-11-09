//Библиотеки*******************************************************************************************************

#include <Wire.h>                   // библиотека для работы I²C
#include <DallasTemperature.h>
#include <RTClib.h>                   // библиотека для работы с часами реального времени
#include <microDS18B20.h>           // библиотека для погружного датчика температуры
#include <OneWire.h>                // Подключаем библиотеку для взаимодействия с устройствами, работающими на шине и по протоколу 1-Wire
#include "DHT.h"                   // подключаем библиотеку для работы с датчиком DHT22
#include <LiquidCrystal_I2C.h>     // подключаем библиотеку для LCD

//Рестарт**********************************************************************************************************
int counter = 1;

//LCD дисплей******************************************************************************************************
int lcdColumns = 20;
int lcdRows = 4;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);     // присваиваем имя LCD для дисплея

//Датчик рельного времени******************************************************************************************
RTC_DS1307 DS1307_RTC;
char Week_days[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//DHT датчик********************************************************************************************************
uint8_t DHTPin = 5;            // цифровой пин, к которому подключен контакт данных датчика
#define DHTTYPE DHT22      // константа для быстро переключение типов датчиков, например, можно переопределить DHT11
DHT dht(DHTPin, DHTTYPE);  // инициализируем датчик
float Temperature;
float Humidity;
unsigned long timeDHT; 

//pH датчик**********************************************************************************************************
#define printInterval    800
#define samplingInterval 20
float calibration_value = 70;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;

//Реле**************************************************************************************************************
#define RELAY_3 15     // Пин для реле 3


//Герконы************************************************************************************************************
#define Ger_1 12       // Геркона1 для ... (включает и выключает)
boolean G1;            // Переменная для Ger_1. Тип Boolean т.е. (Да/Нет)
#define Ger_2 14       // Геркона2 для ... (включает и выключает)
boolean G2;            // Переменная для Ger_2. Тип Boolean т.е. (Да/Нет)

//Кнопки************************************************************************ ( Не задействован на данный момент )
#define Button_1 5           // Кнопка1 для реле1 (включает и выключает)
boolean butt_flag_1 = 0;     // вспомогательная переменная для уапрвления реле2
boolean btState_1;           // считывание хранение состояния кпопки 1
boolean relay_flag = 0;      // вспомогательная переменная для уапрвления реле2
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
  Serial.begin(115200);               // задает скорость передачи данных в бит/c (бод)
  delay(100);
  
  lcd.begin();                       // инициализация lcd   
  lcd.backlight();                  // включаем подсветку
  lcd.setCursor(5, 1);              // указываем координаты Aquaponics
  lcd.print("Aquaponics");          // выводим надпись Aquaponics
  lcd.setCursor(5, 2);              // указываем координаты для версии аквапоники
  lcd.print("beta 0.0.1");          // выводим надпись версии аквапоники
  pinMode(DHTPin, INPUT);
  dht.begin();                      // запускаем датчик DHT22

  sensors.begin();                  // Запускаем библиотеку измерения температуры DS18B20  
//  pinMode(term_power, OUTPUT);      // Определяем пин подключения питания датчика температуры DS18B20

  pinMode(RELAY_3, OUTPUT);         // Определяем пин реле2 как выход


  pinMode(Ger_1, INPUT);           // Определяем пин геркона как вход / если нет резистора в схеме, то нужно написать (Ger_1, INPUT_PULLUP)
  pinMode(Ger_2, INPUT);           // Определяем пин геркона как вход / если нет резистора в схеме, то нужно написать (Ger_2, INPUT_PULLUP)
//  pinMode(Button_1, INPUT_PULLUP);  // Определяем пин кнопки как вход со стягивающим резистором
  delay(2000);                       // задержка 2 секунды
  lcd.clear();                      // очистка lcd от текста   
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
  float Hour = clock.getHour();            // переменная, которая запониманает текущий час
  float y = 7;                             // час, когда включиться свет
  float n = 20;                            // час, когда выключиться свет
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
  
//Работа с сенсором DHT_22*************************************************************************

  static uint32_t printErrorDHT = millis();
  if(Humidity > 60)
  {
    digitalWrite(RELAY_3, LOW);
  }
  else
  {
    digitalWrite(RELAY_3, HIGH);
  }

// Термодатчик*****************************************************************************************
   float e = qwerty.getTemp();
 if(e > 29)
  {
    tone (13, 600);                   // включаем пищалку 
  }
  else
  {
    noTone (13);                   // выключаем пищалку
  }

// Герконы*****************************************************************************************
G2 = digitalRead(Ger_2);            //счмтываем и запоминаем значение геркона
 if (G2 == HIGH) 
 { 
  lcd.setCursor(17,0);              // задаём начальные координаты для надписи на LCD дисплее
  lcd.print("MAX");                 // выводим надпись на LCD дисплей
 }
 else
 {
  lcd.setCursor(17,0);              // задаём начальные координаты для надписи на LCD дисплее
  lcd.print("   ");                 // ничего не выводим на LCD дисплей
 }

 if( G2 == LOW && G1 == HIGH)
 {
  lcd.setCursor(18,1);              // задаём начальные координаты для надписи на LCD дисплее
  lcd.print("*");                   // выводим надпись на LCD дисплей
 }
 else
 {
  lcd.setCursor(17,1);              // задаём начальные координаты для надписи на LCD дисплее
  lcd.print("  ");                  // ничего не выводим на LCD дисплей
 }

G1 = digitalRead(Ger_1);            //счмтываем и запоминаем значение геркона
 if (G1 == LOW) 
 { 
  lcd.setCursor(17,2);              // задаём начальные координаты для надписи на LCD дисплее
  lcd.print("MIN");                 // выводим надпись на LCD дисплей
//  tone (13, 600);                   // включаем пищалку
 }
 else
 {
  lcd.setCursor(17,2);              // задаём начальные координаты для надписи на LCD дисплее
  lcd.print("   ");                 // ничего не выводим на LCD дисплей
//  noTone (13);                      // выключаем пищалку
 } 

 
}

//********************************************************************************************************************
//*********************************************************************************** void (Reset)
//********************************************************************************************************************
void resetAll()
{
  ESP.restart();
}

//********************************************************************************************************************
//************************************************************************************ void ( кнопка )
//********************************************************************************************************************
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
//********************************************************************************************************************
//************************************************************************************ void ( real_time_clock сенсор )
//********************************************************************************************************************
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
//********************************************************************************************************************
//************************************************************************************ void ( DHT сенсор )
//********************************************************************************************************************
 void dht22()
 {
   if(millis() - timeDHT > 1000)
   {
      Temperature = dht.readTemperature(); // Gets the values of the temperature
      Humidity = dht.readHumidity(); // Gets the values of the humidity 
      lcd.setCursor(0,2);
//      Serial.print("Humidity: ");
//      Serial.print(h*100);  //**************Вывод в Excel****************
//      Serial.print(",");   
      lcd.print("Humidity: ");
      lcd.print(Humidity);
      lcd.print("%  ");
//      Serial.print("Temperature: ");
//      Serial.print(t*100); //**************Вывод в Excel****************
//      Serial.print(",");   
      lcd.setCursor(0, 3);
      lcd.print("Temperature: ");
      lcd.print(Temperature,1);
      lcd.write(223);                            // символ градуса
      lcd.print("C  ");
      timeDHT = millis();                        // обнуляем таймер
   }                                                                                      
 }
//********************************************************************************************************************
//************************************************************************************ void ( DS18B20 сенсор )
//********************************************************************************************************************
 void ds18b20()
 {
  if(millis() - timing > 1000)    // задержка в 1 секунду
  {
   sensors.requestTemperatures();             // запрашиваем новое измерение
   float temperatureC = sensors.getTempCByIndex(0);
   float temperatureF = sensors.getTempFByIndex(0);
   timing = millis();               // обнуляем таймер
   lcd.setCursor(0,1);                 
   lcd.print("Water T: ");                // выводим надпись Water T: на lcd 
   lcd.print(temperatureC,0);
//   Serial.print("Water T: ");
//   Serial.print((qwerty.getTemp())*100,0);      //**************Вывод в Excel****************
   lcd.write(223);                            // символ градуса
   lcd.print("C  ");
  }
 }
//********************************************************************************************************************
//************************************************************************************ void ( PH сенсор )
//********************************************************************************************************************
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
    buffer_arr[i] = analogRead(4);
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
  
//  Serial.print(ph_act,0);                                                                                          //Вывод в Excel
//  Serial.print(",");
  printTime = millis(); 
 }
}
