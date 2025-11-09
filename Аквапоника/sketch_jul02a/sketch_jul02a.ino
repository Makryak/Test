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

const int gerkon1 = 8;   //геркона 1
const int gerkon2 = 9;   //геркона 2
const int gerkon3 = 10;  //геркона 3

bool g1 = 1;   // переменная геркона 1
bool g2 = 1;   // переменная геркона 2
bool g3 = 1;   // переменная геркона 3

unsigned long prev_ms1 = 0;
unsigned long prev_ms2 = 0;
unsigned long prev_ms3 = 0;

const int On1 = 1000;                  // Светит
const int Off1 = 500;                  // Не светит
const int On2 = 2000;                  // Светит
const int Off2 = 500;                  // Не светит
const int On3 = 3000;                  // Светит
const int Off3 = 500;                  // Не светит

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
  
  pinMode(gerkon1, INPUT_PULLUP);
  pinMode(gerkon2, INPUT_PULLUP);
  pinMode(gerkon3, INPUT_PULLUP);
  delay(2000);
}


float temperature() 
{                                                // Измеряем температуру 10 секунд
  digitalWrite(term_power, HIGH);                // Включаем питание датчика температуры
  delay(100);                                    // Задержка перед первым измерением 
  sensors.requestTemperatures();                 // Запрос на измерение температуры (1-й ошибочный)
  delay(500);                                    // Задержка перед повторным измерением
  sensors.requestTemperatures();                 // Запрос на измерение температуры (повторный)
  float t = float(sensors.getTempCByIndex(0));   // Получаем значение температуры
  digitalWrite(term_power, LOW);                 // Отключаем питание датчика температуры
  delay(9400);                                   // Задержка, чтобы датчик не нагревался от частых измерений
  return(t);                                     // Возвращаем значение температуры в место вызова функции
}



void loop()
{  
 lcd.clear();             // Очистка экрана
 
// DHT22 сенсор 
  lcd.setCursor(0,1);
  int chk = DHT.read22(DHT22_PIN);
  lcd.print("Temp: ");
  lcd.print(DHT.temperature, 1);
  lcd.print(" C");
  lcd.setCursor(0,2);
  lcd.print("Hum: ");
  lcd.print(DHT.humidity, 1);
  lcd.print(" %");
  
  int adcSensor = analogRead(pinSensor);                                // Считываем аналоговое значение с датчика кислотности жидкости
  float voltageSensor = adcSensor * OPERATING_VOLTAGE / 1023;           // Переводим данные сенсора в напряжение
  float pHSensor = CALIBRATION_FACTOR * (voltageSensor + ZERO_SHIFT);   // Конвертируем напряжение в концентрацию pH

// PH сенсор
  lcd.setCursor(0,3);
  lcd.print("Vo: ");
  lcd.print(voltageSensor);
  lcd.print("V");
  lcd.print(" Va: ");
  lcd.print(pHSensor);
  lcd.print("pH");

  // DS18B20 сенсор
  lcd.setCursor(0,0);
  lcd.print("W tem: ");
  lcd.print(temperature(),1); 
  lcd.print(" C");
  Serial.println(temperature(),1);

unsigned long currentTime = millis(); // текущее время в миллисекундах
 g1 = digitalRead(gerkon1);

  if ((g1 == HIGH) && (currentTime - prev_ms1 >= On1))
  {
  g1 == LOW;
  prev_ms1 = currentTime;             // запоминаем время
  lcd.setCursor(18, 0);
  lcd.print("g1");
  Serial.println("g1");
  }else if ((g1 == 0) && (currentTime - prev_ms1 >= Off1)){
  prev_ms1 = currentTime;             // запоминаем время
  lcd.setCursor(18, 0);
  lcd.print("");
  Serial.println("152");
 }

 
  if (g1 == HIGH) {
  lcd.setCursor(18, 0);
  lcd.print("g1");
  g1 == LOW;
  Serial.println("g1");
  } else {
  lcd.setCursor(18, 0);
  lcd.print("");
  Serial.println("152");
    }
}

 /* g2 = digitalRead(gerkon2);
   if (g2 == HIGH) 
   {
    lcd.setCursor(18, 1);
    lcd.print("g2");
    delay(400);
   }else{
    
   }
}


 g3 = digitalRead(gerkon3);
  if (g3 == HIGH) 
  { 
   lcd.setCursor(18, 2);
   lcd.print("g3");
   delay(400);
  }else{

  }*/
