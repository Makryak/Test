
#if defined(__AVR__)
#define OPERATING_VOLTAGE   5.0
#define ZERO_SHIFT          0
#else
#define OPERATING_VOLTAGE   3.3
#define ZERO_SHIFT          1.1
#endif
 
// Коэффициент перевода напряжения в концентрацию pH
#define CALIBRATION_FACTOR  0.6
 
// Назначаем пин для подключения датчика
constexpr auto pinSensor = 4 ;
 
void setup() {
  // Открываем Serial-порт
  Serial.begin(115200);
}
 
void loop() {
  // Считываем аналоговое значение с датчика кислотности жидкости
  int adcSensor = analogRead(pinSensor);
  // Переводим данные сенсора в напряжение
  float voltageSensor = adcSensor * OPERATING_VOLTAGE / 1023;
  // Конвертируем напряжение в концентрацию pH
  float pHSensor = CALIBRATION_FACTOR * (voltageSensor + ZERO_SHIFT);
  // Выводим данные в Serial-порт
  
  Serial.print("Voltage: ");
  Serial.print(voltageSensor);
  Serial.print(" V");
  Serial.print("\t");
  Serial.print("Value: ");
  Serial.print(pHSensor);
  Serial.println(" pH");
  delay(1000);
}
