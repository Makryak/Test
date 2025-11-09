
int gerkon1 = 8;
int gerkon2 = 9;
int gerkon3 = 10;

bool g1 = false; 
bool g2 = false; 
bool g3 = false;

unsigned long prev_ms1 = 0;
unsigned long prev_ms2 = 0;
unsigned long prev_ms3 = 0;

const int On1 = 1000;                  // Светит
const int Off1 = 500;                  // Не светит
const int On2 = 2000;                  // Светит
const int Off2 = 500;                  // Не светит
const int On3 = 3000;                  // Светит
const int Off3 = 500;                  // Не светит

bool BTN_state1 = false;
bool BTN_state2 = false;
bool BTN_state3 = false;
 
void setup() {


 pinMode(gerkon1, INPUT_PULLUP);
 pinMode(gerkon2, INPUT_PULLUP);
 pinMode(gerkon3, INPUT_PULLUP);
}
 
void loop() {
 unsigned long currentTime = millis(); // текущее время в миллисекундах
 BTN_state1 = digitalRead(BTN);
 
 if((led1State == 1) && (currentTime - prev_ms1 >= On1))
 {
    led1State = 0;                      // выключаем
    prev_ms1 = currentTime;             // запоминаем время
    digitalWrite(Led1, led1State);      // Гасим светодиод
 }
  else if ((led1State == 0) && (currentTime - prev_ms1 >= Off1))
  {
    led1State = 1;                      // включаем
    prev_ms1 = currentTime;             // запоминаем время
    digitalWrite(Led1, led1State);      // Зажигаем светодиод
  }
 
 if((led2State == 1) && (currentTime - prev_ms2 >= On2))
 {
    led2State = 0;                      // выключаем
    prev_ms2 = currentTime;             // запоминаем время
    digitalWrite(Led2, led2State);      // Гасим светодиод
 }
  else if ((led2State == 0) && (currentTime - prev_ms2 >= Off2))
  {
    led2State = 1;                       // включаем
    prev_ms2 = currentTime;              // запоминаем время
    digitalWrite(Led2, led2State);       // Зажигаем светодиод
  }
 if((led3State == 1) && (currentTime - prev_ms3 >= On3))
 {
    led3State = 0;                      // выключаем
    prev_ms3 = currentTime;             // запоминаем время
    digitalWrite(Led3, led3State);      // Гасим светодиод
 }
  else if ((led3State == 0) && (currentTime - prev_ms3 >= Off3))
  {
    led3State = 1;                       // включаем
    prev_ms3 = currentTime;              // запоминаем время
    digitalWrite(Led3, led3State);       // Зажигаем светодиод
  }
    
 if (BTN_state == HIGH) {
    digitalWrite(Led4, LOW);
  } else {
    digitalWrite(Led4, HIGH);
    }
}
