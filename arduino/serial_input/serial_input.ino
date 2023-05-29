#include <SPI.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
const int rs = 7, en = 8, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SoftwareSerial mySerial(6, A7);
int store[5];
//char store[8];
int val=0;
int read=0;
int avg=0;
int matches=0;
int threes=0;
int light=0;
int temp=0;
int battery=0;
int x=0;
int last=0;
void setup() {
pinMode(A0,OUTPUT);
pinMode(9,OUTPUT);
lcd.begin(16, 2);  // put your setup code here, to run once:
Serial.begin(1200);
while(!Serial){
  ;
}
    x=0;
    while(x<200){
      digitalWrite(9,1);
      digitalWrite(A0,0);
      delay(1);
      digitalWrite(A0,1);
      digitalWrite(9,0);
      delay(1);
      x=x+1;
    }
    x=0;
    digitalWrite(9,0);
    digitalWrite(A0,0);
mySerial.begin(1200);
     // Start the receiver PLL running
}

void loop() {
  lcd.setCursor(9, 0);
  val=0;
  //lcd.autoscroll();
  if (mySerial.available()) {
    read=mySerial.read();
    //Serial.println(read);
    if (read!=-1){
      if (read==195){// and read<=205){
        matches=0;
        Serial.println(read);
        store[0]=read;
        x=x+1;
        delay(1);
        if (mySerial.available()){
          read=mySerial.read();
          Serial.println(read);
          store[1]=read;
        }
        delay(1);
        if (mySerial.available()){
          read=mySerial.read();
          Serial.println(read);
          store[2]=read;
        }
        delay(1);
        if (mySerial.available()){
          read=mySerial.read();
          Serial.println(read);
          store[3]=read;
        }
        delay(1);
        if (mySerial.available()){
          read=mySerial.read();
          Serial.println(read);
          store[4]=read;
        }
        for (int i=0;i<5;i++){
          if (store[i]==195){
              matches=matches+1;
          }
        }
        //avg=matches;
        if (matches>=3){
          //Serial.println("MATCH!!!!!!!");
          lcd.setCursor(0, 0);
          lcd.print("match!");
          x=0;
          while (read==195){
            if (mySerial.available()){
              read=mySerial.read();
              Serial.print(x);
              Serial.print(":");
              Serial.println(read);
              x=x+1;
            }
            delay(1);
          }
          light=read;
          delay(1); //not sure why this is here
          if (mySerial.available()){
            read=mySerial.read();
            Serial.println(" ");
            Serial.println(read);
            temp=read;
          }
          delay(1);
          if (mySerial.available()){
            read=mySerial.read();
            Serial.println(read);
            battery=read;
          }
          x=0;
          while(x<10){
            if(mySerial.available()){
              read=mySerial.read();
              Serial.print(x);
              Serial.print(" ");
              Serial.println(read);
              x=x+1;
            }
            delay(1);
          }
          delay(100);
        }
        lcd.setCursor(0, 1);
        lcd.print(light);
        lcd.print("  ");
        lcd.print(temp);
        lcd.print("  ");
        lcd.print(battery);
        
        }
      }  
     }  
  }
