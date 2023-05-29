#include <LiquidCrystal.h>
const int rs = 7, en = 8, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int analogPin= A0;
int val =0;
int i=0;
int decode[8];
int store[4];
int x=0;
int page=4;
int reading_number=0;
int read=0.0;
int pages[5][4]={
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0}
};
double display=0.0;
const int check=6; //pin which is receiving binary (will eventually be reciever input)

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  pinMode(check, INPUT);
  pinMode(10,OUTPUT);
  lcd.setCursor(0,0);
  lcd.print("1:");
  lcd.setCursor(0,1);
  lcd.print("2:");
  lcd.setCursor(8,0);
  lcd.print("3:");
  lcd.setCursor(8,1);
  lcd.print("4:");
}

void loop() {
  // put your main code here, to run repeatedly:
  val=analogRead(analogPin);
  read=digitalRead(check);
  if (reading_number<4){
    if (read==1 and x==0){
      x=x+1;
      delay(10);
    }
    else if (read==0 and x==1){
      x=x+1;
      delay(10);
    }
    else if (read==1 and x==2){
      x=0;
      delay(10);
      while (x<8){
        read=digitalRead(check);
        decode[x]=read;
        x=x+1;
        //lcd.setCursor(7+x,0);
        //lcd.print(read);
        delay(10);
      }
      display=0.0;
      for (int i=0.0; i<8; i=i+1){
       display=display+pow(2,i)*decode[7-i];
      }
      if (display>=30){
        digitalWrite(10, 1);
      }
      else{
        digitalWrite(10,0);
      }
      
      if (reading_number==0){
        lcd.setCursor(2, 0);
        lcd.print("   ");
        lcd.setCursor(2, 1);
        lcd.print("   ");
        lcd.setCursor(10, 0);
        lcd.print("   ");
        lcd.setCursor(10, 1);
        lcd.print("   ");
      }
      store[reading_number]=round(display);
      reading_number=reading_number+1;
      for (int i=0; i<4;i=i+1){
        pages[4][i]=store[i];
      }
      delay(50);             //Maybe need to tone down this delay? or add foot to ensure no run on
    }
    else{
      x=0;
      //lcd.setCursor(5, 1);
      //lcd.print(read);
    }
  }
  else{
    reading_number=0;
    for (int i=0;i<4;i=i+1){
      for (int j=0;j<4;j=j+1){
        pages[i][j]=pages[i+1][j]; //sets the pages up one page
      }
    }

    for (int i=0;i<4;i=i+1){
          pages[4][i]=0;
          store[i]=0;             //sets the bottom page to the 4th and resets this might need to move up 
        }
    }
  
  
  lcd.setCursor(2, 0);
  lcd.print(pages[page][0]);
  lcd.setCursor(2, 1);
  lcd.print(pages[page][1]);
  lcd.setCursor(10, 0);
  lcd.print(pages[page][2]);
  lcd.setCursor(10, 1);
  lcd.print(pages[page][4]);
  lcd.setCursor(14, 1);
  lcd.print(reading_number);
}
