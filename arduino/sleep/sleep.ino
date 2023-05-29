#include<avr/sleep.h>
#define interruptPin 2
#include "RTClib.h"
#include <SPI.h>
RTC_DS3231 rtc;
int min;
int sec;
int start_sec;
int start_min;
int comp_sec;
int comp_min;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Serial Connected");
  pinMode(2,INPUT_PULLUP);
  pinMode(interruptPin,INPUT_PULLUP);
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  if (! rtc.begin()) {
    while (1) delay(10);
}
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  DateTime now = rtc.now();
  start_min=now.minute();
  start_sec=now.second();
}

void loop() {
  // put your main code here, to run repeatedly:
  //delay(5000);
  DateTime now=rtc.now();
  sec=now.second();
  min=now.minute();
  Serial.print(min);
  Serial.print(" ");
  Serial.println(sec);
  if (min-start_min<0){
    comp_min=min-start_min+60;
  
  }
  else{
    comp_min=min-start_min; 
  }  
  if (sec-start_sec<0){
    comp_sec=sec-start_sec+60;
    comp_min=comp_min-1;    
    //min=min+1;
  } 
  else{
    comp_sec=sec-start_sec;  
  }
  Serial.print(comp_min);
  Serial.print(" ");
  Serial.print(comp_sec);  
  Serial.println(" ");
  if (comp_min>=1){
    Going_To_Sleep();
    DateTime now = rtc.now();
    start_min=now.minute();
    start_sec=now.second(); 
    //delay(1000) ;
  }  
  //delay(1000);  
}

void Going_To_Sleep(){
  sleep_enable();
  attachInterrupt(0, wakeUp, RISING);
  digitalWrite(LED_BUILTIN,LOW);
  delay(2000);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_cpu();
  digitalWrite(LED_BUILTIN,HIGH);
  delay(2000);
  
}
void wakeUp(){
  sleep_disable();
  detachInterrupt(0);
}