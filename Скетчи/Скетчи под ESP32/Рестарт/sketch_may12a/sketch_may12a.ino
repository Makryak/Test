int counter = 3;

void setup() {
  Serial.begin(115200);
}

void loop() {

  Serial.println(counter);

  if (counter == 0) {
    Serial.println("Reset..");
    ESP.restart();
  }

  counter--;
  delay(2000);
}
