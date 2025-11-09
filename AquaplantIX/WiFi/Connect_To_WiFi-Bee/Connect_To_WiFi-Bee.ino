#include <ESP8266WiFi.h>

const char* ssid ="TP-Link_A39E";
const char* password =  "75845100";
byte tries = 10;  // Попыткок подключения к точке доступа
void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (--tries && WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Non Connecting to WiFi..");
  }
  else
  {
    // Иначе удалось подключиться отправляем сообщение
    // о подключении и выводим адрес IP
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

}

void loop() {}