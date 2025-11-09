// DHT22 d5
// LCD2004 (SCL d22 | SDA d21) - i2c
// PH meter vp
// DS18b20 d2
// Реле_3 d15
// Реле_4 d18
// L298n d14 - enable | d27 - motor
// Герконы d13 | d12

//Библиотеки*******************************************************************************************************
//   #include <Wire.h>                    // библиотека для работы I²C
//   #include <microDS18B20.h>            // библиотека для погружного датчика температуры DS18B20
#include <iocontrol.h>                // библиотека для работы через интернет с панелями на сайте ioControl
#include <WiFi.h>                     // библиотека для работы с интернетом
#include <DallasTemperature.h>        // библиотека для погружного датчика температуры DS18B20
#include "RTClib.h"                   // библиотека для работы с часами реального времени
#include <OneWire.h>                  // Подключаем библиотеку для взаимодействия с устройствами, работающими на шине и по протоколу 1-Wire
#include "DHT.h"                      // подключаем библиотеку для работы с датчиком DHT22
#include <LiquidCrystal_I2C.h>        // подключаем библиотеку для LCD

//Глобальные общие переменные**************************************************************************************
unsigned long UpdateTimer;

//Интернет*********************************************************************************************************

const char* myPanelName = "Aquaponics";       // Название панели на сайте iocontrol.ru
const char* myVarHumidity = "Humidity";
const char* myVarTemperature = "Temperature";
const char* myVarWaterTemperature = "WaterTemperature";
const char* myVarPH = "pH_metter";
const char* myVarPlantsLight = "Plants_Light";

const char* ntpServer = "pool.ntp.org";         // Сервер времени

const char* ssid = "POCO";
const char* password = "2e7x278uvvpige8";
WiFiClient client;                              // Создаём объект клиента класса WiFiClient

iocontrol mypanel(myPanelName, client);         // Создаём объект iocontrol, передавая в конструктор название панели и клиента


//Рестарт**********************************************************************************************************
int counter = 1;

//L298n************************************************************************************************************
int motor1Pin1 = 27; 
int enable1Pin = 14;

const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle;

unsigned long LightTimer;

//LCD дисплей******************************************************************************************************
int lcdColumns = 20;
int lcdRows = 4;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);     // присваиваем имя LCD для дисплея

//Датчик рельного времени******************************************************************************************

RTC_DS1307 DS1307_RTC;
char Week_days[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; 
unsigned long Time; 


//DHT датчик********************************************************************************************************
uint8_t DHTPin = 5;            // цифровой пин, к которому подключен контакт данных датчика
#define DHTTYPE DHT22          // константа для быстро переключение типов датчиков, например, можно переопределить DHT11
DHT dht(DHTPin, DHTTYPE);      // инициализируем датчик
float Temperature;
float Humidity;;


//pH датчик**********************************************************************************************************
const int potPin=A0;
float ph;
float Value=0;

/*
//#define printInterval    800
#define samplingInterval 20  //20
float calibration_value = 60;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[50],temp;

static unsigned long printTime    = millis();
static unsigned long samplingTime = millis(); 
static float ph_act, volt;

unsigned long PHTimer;
*/

//Реле**************************************************************************************************************
#define RELAY_2 18     // Пин для реле 2
#define RELAY_3 15     // Пин для реле 3

//Герконы************************************************************************************************************
#define Ger_1 12       // Геркона1 для ... (включает и выключает) (Тот который сверху)
boolean G1;            // Переменная для Ger_1. Тип Boolean т.е. (Да/Нет)
#define Ger_2 13       // Геркона2 для ... (включает и выключает) (Тот который снизу)
boolean G2;            // Переменная для Ger_2. Тип Boolean т.е. (Да/Нет)
int var = 0;
unsigned long LOOPTimer;

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
float temperatureC;
//MicroDS18B20 <3> qwerty;       // Создаем термометр без адреса на пине D2
//int w;

unsigned long timing;            // Переменная для хранения точки отсчета




void setup()
{
  Serial.begin(115200);             // задает скорость передачи данных в бит/c (бод)
  delay(500);
  
//Заставка логотипа и версии прошивки*********************************************************************************************
  lcd.begin(lcdColumns, lcdRows);                      // инициализация lcd   
  lcd.backlight();                  // включаем подсветку
  lcd.setCursor(5, 0);              // указываем координаты Aquaponics
  lcd.print("Aquaponics");          // выводим надпись Aquaponics
  lcd.setCursor(5, 1);              // указываем координаты для версии аквапоники
  lcd.print("beta 0.1.6");          // выводим надпись версии аквапоники

//Подключаемся к интернету********************************************************************************************************
  WiFi.begin(ssid, password);
  lcd.setCursor(0, 3);              // указываем координаты для версии аквапоники
  lcd.print("Connecting WiFi ");
  while (WiFi.status() != WL_CONNECTED)          // Ждём подключения
   {  
     Serial.print(".");
     delay(1000); 
   } 
   
   
//  Serial.println();
    
  mypanel.begin();                              // Вызываем функцию первого запроса к сервису
//  myButton = !mypanel.readBool(myVarName);

  dht.begin();                      // запускаем датчик DHT22
  pinMode(potPin,INPUT);            //Определяем пин potPin как вход
  pinMode(DHTPin, INPUT);           //Определяем пин DHTPin как вход
  sensors.begin();                  // Запускаем библиотеку измерения температуры DS18B20  
//  pinMode(term_power, OUTPUT);    // Определяем пин подключения питания датчика температуры DS18B20

  if (!DS1307_RTC.begin()) 
  {
    lcd.clear();
    lcd.setCursor(2, 2); 
    lcd.print("Couldn't find RTC");
    while(1);
  }
  
DS1307_RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));       // считывает время с компьютера при загрузке скетча
                                                               // (Разцензурить строчку при калибровке времени, потом зацензурить эту строчку)
                                                               // Если оставить эту строчку разцензуренной, то время при перезагрузке ESP32 будет постоянно обнуляться до того времени которое было запомнено)

  pinMode(motor1Pin1, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(enable1Pin, pwmChannel);
  digitalWrite(motor1Pin1, HIGH);

  pinMode(RELAY_2, OUTPUT);              // Определяем пин реле2 как выход
  pinMode(RELAY_3, OUTPUT);              // Определяем пин реле3 как выход
  pinMode(Ger_1, INPUT_PULLUP);          // Определяем пин геркона как вход / если нет резистора в схеме, то нужно написать (Ger_1, INPUT)
  pinMode(Ger_2, INPUT_PULLUP);          // Определяем пин геркона как вход / если нет резистора в схеме, то нужно написать (Ger_2, INPUT)
//  pinMode(Button_1, INPUT_PULLUP);     // Определяем пин кнопки как вход со стягивающим резистором
  delay(2000);                           // задержка 2 секунды
  lcd.clear();                           // очистка lcd от текста  
}


void loop()
{
  
/*
   // Обновляем переменные в памяти и записываем
   // статус запроса в переменную
   // Записываем состояние переменной
    mypanel.write(myVarName, Hum);

    // Обновляем переменные на сайте
    int status = mypanel.writeUpdate();

    // Если статус равен константе OK...
    if (status == OK) 
    {
    //      String myString = mypanel.readString(HUM);
       Hum = mypanel.readFloat(HUM);
    //        Serial.println(myString);
      uint8_t prec = mypanel.getFloatPrec(HUM);
      Serial.println(Hum, prec);     
    }
*/

//********************************************************************************************************************
//********************************************************************************************************************
//********************************************************************************************************************
  if(millis() - UpdateTimer > 1000)
   {
     UpdateTimer = millis();
     dht22();
     ds18b20();
     ph_sensor();
//   }
//  if(millis() - PHTimer > 1000)
 //  {
 //   ph_sensor();
//    PHTimer = millis();
   }
   
if(millis() - LOOPTimer > 500)
  {
    LOOPTimer = millis();
    
    mypanel.write(myVarPlantsLight,dutyCycle); 
    mypanel.write(myVarWaterTemperature,temperatureC);
    mypanel.write(myVarHumidity, Humidity);
    mypanel.write(myVarTemperature, Temperature);
    mypanel.write(myVarPH, ph);
    int status = mypanel.writeUpdate();
    
    DateTime now = DS1307_RTC.now();
//Реле, которое управляется часами реального времени***********************************************
    float y = 7;                             // час, когда включиться свет
    float n = 20;                            // час, когда выключиться свет

//    Serial.print(now.year(), DEC);
//    Serial.print('/');
//    Serial.print(now.month(), DEC);
//    Serial.print('/');
//    Serial.print(now.day(), DEC);
//    Serial.print(" (");
    lcd.setCursor(0, 0);
//  lcd.print(Week_days[now.dayOfTheWeek()]);
//  lcd.print(" ");
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    lcd.print(now.second(), DEC);
    float Hour = now.hour();
   if( Hour < 20 and Hour > 7)
    {
      dawn();   
      digitalWrite(RELAY_2, LOW);
    }
    else
    {
      sunset();
      digitalWrite(RELAY_2, HIGH);   
    }
  
//Работа с сенсором DHT_22*************************************************************************
// static uint32_t printErrorDHT = millis();
  if(Humidity > 60)
  {
    digitalWrite(RELAY_3, LOW);
  }
  else
  {
    digitalWrite(RELAY_3, HIGH);
  }

/*// Термодатчик*****************************************************************************************
   float e = qwerty.getTemp();
 if(e > 29)
  {
//    tone (13, 600);                   // включаем пищалку 
  }
  else
  {
  //  noTone (13);                   // выключаем пищалку
  }*/
  
// Герконы | Попловки **************************************************************************************
  G1 = digitalRead(Ger_1);            //счмтываем и запоминаем значение геркона
  G2 = digitalRead(Ger_2);            //счмтываем и запоминаем значение геркона
if(G1 == LOW) 
{
  var = 1;
}
else if(G2 == HIGH)
{
  var = 3;
}
else if ( G2 == LOW && G1 == HIGH)
{
  var = 2;
}
switch (var){
    case 1:
        lcd.setCursor(4,1);              // задаём начальные координаты для надписи на LCD дисплее
        lcd.print("MAX");                 // выводим надпись на LCD дисплей
      break;
    case 2:
        lcd.setCursor(4,1);              // задаём начальные координаты для надписи на LCD дисплее
        lcd.print(" * ");                   // выводим надпись на LCD дисплей
       break;
    case 3:
        lcd.setCursor(4,1);              // задаём начальные координаты для надписи на LCD дисплее
        lcd.print("MIN");                 // выводим надпись на LCD дисплей
        //  tone (13, 600);                   // включаем пищалку
      break;
    }
  }
}

//********************************************************************************************************************
//************************************************************************************ void ( PH сенсор )
//********************************************************************************************************************
void ph_sensor()
{
    Value= analogRead(potPin);
 /*   Serial.print(Value);
    Serial.print(" | ");*/
    float voltage=Value*(3.3/4095.0);
    ph=(5.5*voltage);  //3,3
    lcd.setCursor(0, 3); 
    lcd.print("pH value: ");
    lcd.print(ph,2);
}
/*
  
if(millis() - samplingTime > samplingInterval){ 
  for(int i=0;i<10;i++) 
   {
    buffer_arr[i] = analogRead(potPin);
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
 
  lcd.setCursor(0, 0);
  lcd.print("pH value: ");
  lcd.setCursor(10, 0);
  lcd.print(ph_act, 2);
 samplingTime = millis(); 
}

 //  Выводим показания в LCD:
 if(millis() - printTime > printInterval)
 {
  lcd.setCursor(10, 0);
  lcd.print(ph_act, 2);
  
//  Serial.print(ph_act,0);                                                                                          //Вывод в Excel
//  Serial.print(",");
  printTime = millis(); 
 }*/

//********************************************************************************************************************
//************************************************************************************ void ( DHT сенсор )
//********************************************************************************************************************
 void dht22()
 {
      Temperature = dht.readTemperature(); // Gets the values of the temperature
      Humidity = dht.readHumidity(); // Gets the values of the humidity 
      
      lcd.setCursor(9,0);
//      Serial.print("Humidity: ");
//      Serial.print(h*100);  //**************Вывод в Excel****************
//      Serial.print(",");   
      lcd.print("Hum: ");
      lcd.print(Humidity,1);
      lcd.print(" %");
//      Serial.print("Temperature: ");
//      Serial.print(t*100); //**************Вывод в Excel****************
//      Serial.print(","); 
  
      lcd.setCursor(9, 1);
      lcd.print("Tem: ");
      lcd.print(Temperature,1);
      lcd.write(223);                            // символ градуса
      lcd.print("C");                                                                                      
}
//********************************************************************************************************************
//************************************************************************************ void ( DS18B20 сенсор )
//********************************************************************************************************************
 void ds18b20()
 {
   sensors.requestTemperatures();             // запрашиваем новое измерение
   temperatureC = sensors.getTempCByIndex(0);
   lcd.setCursor(0,2);                 
   lcd.print("Water Tem: ");                // выводим надпись Water T: на lcd 
   lcd.print(temperatureC,0);
//   Serial.print("Water T: ");
//   Serial.print((qwerty.getTemp())*100,0);      //**************Вывод в Excel****************
   lcd.write(223);                            // символ градуса
   lcd.print("C");
 }
//*******************************************************************************************************************
//*************************************************************************************void ( L298n ) Рассвет
//******************************************************************************************************************
void dawn()
{    
  if(dutyCycle <= 255 && (millis() - LightTimer > 21177))    //   21177
    {
      LightTimer = millis();                        // обнуляем таймер
      ledcWrite(pwmChannel, dutyCycle);
      lcd.setCursor(0, 1); 
      lcd.print(dutyCycle);
      dutyCycle = dutyCycle + 1;
      /*
      lcd.setCursor(17, 3); 
      lcd.print(dutyCycle);
      if(dutyCycle<=99)
      {
      lcd.setCursor(19, 3); 
      lcd.print(" ");
      }
      if(dutyCycle<=9)
      {
      lcd.setCursor(18, 3); 
      lcd.print("  ");
      }
      dutyCycle = dutyCycle + 1;
      */
    }
}
//*******************************************************************************************************************
//*************************************************************************************void ( L298n ) Закат
//*******************************************************************************************************************
void sunset()
{    
  if(dutyCycle >= 0 && (millis() - LightTimer > 21600))    //  21600
    {
      LightTimer = millis();                        // обнуляем таймер
      ledcWrite(pwmChannel, dutyCycle);
      lcd.setCursor(0, 1); 
      lcd.print(dutyCycle);
      dutyCycle = dutyCycle - 1;
      /*
      lcd.setCursor(17, 3); 
      lcd.print(dutyCycle);
      if(dutyCycle<=99)
      {
      lcd.setCursor(19, 3); 
      lcd.print(" ");
      }
      if(dutyCycle<=9)
      {
      lcd.setCursor(18, 3); 
      lcd.print("  ");
      }
      dutyCycle = dutyCycle - 1;
      */
    }
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
//*********************************************************************************** void (Reset)
//********************************************************************************************************************
void resetAll()
{
  if(counter = 1)
  {
    ESP.restart();
    counter = counter - 1;
    delay(2000);
  }
}
