#include <LiquidCrystal.h>
#include "RTClib.h"
#include <TimeLib.h>
RTC_DS3231 rtc;
const int rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  if (! rtc.begin()) {
    lcd.print("Couldn't find RTC");
    while (1) delay(10);
  }
  //if (rtc.lostPower()) {
    //Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  //}
  DateTime now =rtc.now();
  //setSyncProvider(rtc.get);
}

void loop() {
  // put your main code here, to run repeatedly:
  DateTime now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print("                    ");
  lcd.setCursor(0, 0);
  lcd.print(now.hour(),DEC);
  lcd.print(":");
  lcd.print(now.minute(),DEC);
  lcd.print(":");
  lcd.print(now.second(),DEC);
  lcd.print(now.month(),DEC);
  //lcd.print(":");
  delay(1000);

}

