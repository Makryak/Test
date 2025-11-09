//This code is to use with DHT22 module to measure temperature and humidiy and print the result on Arduino monitor
//It reads the value given by the sensor and print them on monitor, for the temperature it shows both in Celsius and Farnheit
//Refer to https://surtrtech.com/ for more information
#include <Wire.h> 

#include <dht.h> //DHT library works for both 11 and 22

dht DHT; //Declaring the DHT

#define DHT22_PIN 11 //DHT data pin here I used D7 you can change it

void setup(){
  Serial.begin(9600); //Serial baude rate
  
}

void loop()
{


  
  int chk = DHT.read22(DHT22_PIN); //Reading data from the sensor
  float t= DHT.temperature*(9/5) + 32;//Converting the Celsius to Farenheit
  Serial.print("Temperature = ");
  Serial.print(DHT.temperature);//Showing the temperature in Celsius
  Serial.print("°C ");
  Serial.print(t);//Showing the temperature in Farenheit
  Serial.println(" °F");
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);//Showing the humidity
  delay(2000);//Repeating every 2s make sure the reading is done every 2s or more as the sampling rate of the DHT22 is 0.5Hz
}
