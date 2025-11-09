// библиотека для работы с HTTP-протоколом
#include <HTTPClient.h>
// вводим имя и пароль точки доступа
const char* ssid = "WIFINAME";
const char* password = "WIFIPASSWORD";
 
void setup() {
    // иницилизируем монитор порта
    Serial.begin(115200);
    // запас времени на открытие монитора порта — 5 секунд
    delay(5000);
    // подключаемся к Wi-Fi сети
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to Wi-Fi..");
    }
    Serial.println("Connected to the Wi-Fi network");
}
 
void loop() {
    // выполняем проверку подключения к беспроводной сети
    if ((WiFi.status() == WL_CONNECTED)) {
        // создаем объект для работы с HTTP
        HTTPClient http;
        // подключаемся к тестовому серверу с помощью HTTP
        http.begin("http://httpbin.org/");
        // делаем GET запрос
        int httpCode = http.GET();
        // проверяем успешность запроса
        if (httpCode > 0) {
            // выводим ответ сервера
            String payload = http.getString();
            Serial.println(httpCode);
            Serial.println(payload);
        }
        else {
            Serial.println("Error on HTTP request");
        }
        // освобождаем ресурсы микроконтроллера
        http.end();
    }
    delay(10000);
}
