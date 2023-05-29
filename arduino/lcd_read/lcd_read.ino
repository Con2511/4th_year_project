/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 8, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int val =0;
int fourth=0;
int i=0;
int decode[8];
int store =0;
int x=0;
int read=0.0;
double display=0.0;
const int check=6;
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(check, INPUT);
  // Print a message to the LCD.
  //lcd.print("hello world!");
}

void loop() {
  read=digitalRead(check);
  if (read==1 and x==0){
    x=x+1;
    //lcd.setCursor(8, 0);
    //lcd.print("yes");
    delay(15);
  }
  else if (read==0 and x==1){
    x=x+1;
    lcd.setCursor(7, 1);
    lcd.print("yes");
    delay(15);
  }
  else if (read==1 and x==2){
    x=0;
    //decode=0;
    delay(15);
    while (x<8){
      read=digitalRead(check);
      //decode=decode+pow(2,(7-x))*read;
      decode[x]=read;
      x=x+1;
      lcd.setCursor(7+x,0);
      lcd.print(read);
      delay(15);
    }
    fourth=store;
    store=val;
    val=round(display);
    display=0.0;
    for (int i=0.0; i<8; i=i+1){
      display=display+pow(2,i)*decode[7-i];
    }
    
    
    lcd.setCursor(8, 1);
    lcd.print("   ");
    lcd.setCursor(8, 1);
    lcd.print(display);
    lcd.setCursor(0, 0);
    lcd.print("   ");
    lcd.setCursor(0, 0);
    lcd.print(val);
    lcd.setCursor(0, 1);
    lcd.print("   ");
    lcd.setCursor(0, 1);
    lcd.print(store);
    lcd.setCursor(5, 0);
    lcd.print("   ");
    lcd.setCursor(5, 0);
    lcd.print(fourth);
    delay(100);             //Maybe need to tone down this delay? or add foot to ensure no run on
  }
  else{
    x=0;
    lcd.setCursor(5, 1);
    lcd.print(read);
  }
  
  
  
}

