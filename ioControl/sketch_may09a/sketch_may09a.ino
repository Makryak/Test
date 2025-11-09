#include <iocontrol.h>
#include <WiFi.h>
// Если ESP8266, вместо WiFi.h:
// #include <ESP8266WiFi.h>

const int LED = 2;
bool myButton;
// Название панели на сайте iocontrol.ru
const char* myPanelName = "Aquaponics";
// Название переменной с типом кнопки на сайте iocontrol.ru
const char* myVarName = "OnOff";
// Если панель использует ключ
// const char* key = "ключ";

const char* ssid = "Qw-12";
const char* password = "12345678";

// Создаём объект клиента класса WiFiClient
WiFiClient client;
// Создаём объект iocontrol, передавая в конструктор название панели и клиента
iocontrol mypanel(myPanelName, client);
// Если панель использует ключ
// iocontrol mypanel(myPanelName, key, client);

void setup()
{
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    WiFi.begin(ssid, password);

    // Ждём подключения
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    // Вызываем функцию первого запроса к сервису
    mypanel.begin();
  myButton = !mypanel.readBool(myVarName);
}

void loop()
{
    // Обновляем переменные в памяти и записываем
  // статус запроса в переменную
    // Записываем состояние переменной
    mypanel.write(myVarName, myButton);

    // Обновляем переменные на сайте
    int status = mypanel.writeUpdate();

    // Если статус равен константе OK...
    if (status == OK) {
        // Меняем состояние переменной
        if(myButton){
          digitalWrite(LED, HIGH);
        }else{
          digitalWrite(LED, LOW);
        }
    }
}
