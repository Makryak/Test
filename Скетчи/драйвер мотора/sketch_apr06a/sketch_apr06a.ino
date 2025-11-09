int motor1Pin1 = 27; 

int enable1Pin = 13; 
int var = 0;
//byte ena = 13;
//byte in1 = 26;
//byte in2 = 5;
const int freq = 30000;
const int pwmChannel = 10;
const int resolution = 8;
int dutyCycle = 0;


void setup() {
    Serial.begin(115200); 
  pinMode(motor1Pin1, OUTPUT);

  pinMode(enable1Pin, OUTPUT);
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(enable1Pin, pwmChannel);
  Serial.begin(115200); 
  ledcWrite(pwmChannel, dutyCycle);
}
void loop() 
{   
  digitalWrite(motor1Pin1, HIGH);
  Serial.println(dutyCycle);
  
  switch (var) {
    case 1:
    ledcWrite(pwmChannel, dutyCycle);
    dutyCycle = dutyCycle + 10;
    //digitalWrite(motor1Pin1, HIGH);
    delay(1000);
    if(dutyCycle>250){
      var ++; 
    }
    break;
    case 2:
    ledcWrite(pwmChannel, dutyCycle);
    dutyCycle = dutyCycle - 10;

    delay(1000); 
    if(dutyCycle<=1){
    var --; 
    }
    break;
  }
}
