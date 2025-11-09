#define BTN_PIN 8
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


#include <GyverOS.h>
GyverOS<9> OS;


void setup() {
  Serial.begin(9600);
  sensors.begin();                // Запускаем библиотеку измерения температуры
  pinMode(term_power, OUTPUT);    // Определяем пин подключения питания датчика температуры

}
float temperature() { // Измеряем температуру 10 секунд
  digitalWrite(term_power, HIGH);                // Включаем питание датчика температуры
  delay(100);                                    // Задержка перед первым измерением 
  sensors.requestTemperatures();                 // Запрос на измерение температуры (1-й ошибочный)
  delay(500);                                    // Задержка перед поторным измерением
  sensors.requestTemperatures();                 // Запрос на измерение температуры (повторный)
  float t = float(sensors.getTempCByIndex(0));   // Получаем значение температуры
  digitalWrite(term_power, LOW);                 // Отключаем питание датчика температуры
  delay(1000);                                   // Задержка, чтобы датчик не нагревался от частых измерений
  return(t);                                     // Возвращаем значение температуры в место вызова функции
}

void task1(){
  static float average  = 0;
  static byte counter  = 0;
}

void loop() {
OS.tick();
Serial.println(temperature(),1);                // Выводим текущее значение температуры датчика, округленное до десятых

}
