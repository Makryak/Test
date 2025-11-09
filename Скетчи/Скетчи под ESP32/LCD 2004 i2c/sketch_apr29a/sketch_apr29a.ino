#include <RTClib.h>

#include <LiquidCrystal_I2C.h>

RTC_DS1307 DS1307_RTC;
char Week_days[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


// set the LCD number of columns and rows
int lcdColumns = 20;
int lcdRows = 4;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

void setup(){
    Serial.begin(115200);
  // initialize LCD
  lcd.begin();
  // turn on LCD backlight                      
  lcd.backlight();

  if (!DS1307_RTC.begin()) 
  {
    Serial.println("Couldn't find RTC");
    while(1);
  }
   DS1307_RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
}


void loop(){
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
 

  // clears the display to print new message
  lcd.clear();
  // set cursor to first column, second row
  lcd.setCursor(0,1);
  lcd.print("Hello, World!");

  lcd.clear(); 


  DateTime now = DS1307_RTC.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    lcd.setCursor(0, 0);
    lcd.print(Week_days[now.dayOfTheWeek()]);
    lcd.print(" ");
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    lcd.print(now.second(), DEC);
    Serial.println();

    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");

    // calculate a date which is 7 days, 12 hours, 30 minutes, 6 seconds into the future
    DateTime future (now + TimeSpan(7,12,30,6));

    Serial.print(" now + 7d + 12h + 30m + 6s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);

    Serial.println();
    Serial.println();
    delay(5000);
}
