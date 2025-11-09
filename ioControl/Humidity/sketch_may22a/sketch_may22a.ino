#include <iocontrol.h>
#include <WiFi.h>
#include "DHT.h"                      // подключаем библиотеку для работы с датчиком DHT22

const char* myPanelName = "MyAquaponics";
const char* myVarName = "Humidity";

const char* ssid = "CTPO";
const char* password = "8967452310";

WiFiClient client;
iocontrol mypanel(myPanelName, client);

uint8_t DHTPin = 5;            // цифровой пин, к которому подключен контакт данных датчика
#define DHTTYPE DHT22          // константа для быстро переключение типов датчиков, например, можно переопределить DHT11
DHT dht(DHTPin, DHTTYPE);      // инициализируем датчик
float Humidity;
float Temperature;



void setup()
{
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    dht.begin();                    
    pinMode(DHTPin, INPUT);
    mypanel.begin();
}

void loop()
{


    Humidity = dht.readHumidity(); 
    Serial.println(Humidity,1);
    mypanel.write(myVarName, Humidity);


    int status = mypanel.writeUpdate();

    if (status == OK) 
    {

        Serial.println("Updated");

    }
    
}
