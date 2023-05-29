#include <LiquidCrystal.h>
const int rs = 7, en = 8, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int i=0;
int decode[8];
int store[4];
int x=0;
int page=3;
int reading_number=1;
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
  pinMode(11,INPUT); //Page up
  pinMode(12,INPUT); //Page down
  lcd.setCursor(0,0);
  lcd.print("1:    ");
  lcd.setCursor(0,1);
  lcd.print("2:    ");
  lcd.setCursor(8,0);
  lcd.print("3:    ");
  lcd.setCursor(8,1);
  lcd.print("4:    ");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(11)==1 and page!=4){  //Switches pages, can allow for new 
    page=page+1;
    lcd.setCursor(2, 0);
    lcd.print("     ");
    lcd.setCursor(2, 1);
    lcd.print("     ");
    lcd.setCursor(10, 0);
    lcd.print("     ");
    lcd.setCursor(10, 1);
    lcd.print("     ");

  }
  else if (digitalRead(12)==1 and page!=0){
    page=page-1;
    lcd.setCursor(2, 0);
    lcd.print("     ");
    lcd.setCursor(2, 1);
    lcd.print("     ");
    lcd.setCursor(10, 0);
    lcd.print("     ");
    lcd.setCursor(10, 1);
    lcd.print("     ");
  }
  read=digitalRead(check);
  if (reading_number<5){
    if (read==1 and x==0){  //first reading
      x=x+1;
      delay(15);
    }
    else if (read==0 and x==1){ //2nd reading
      x=x+1;
      delay(15);
    }
    else if (read==1 and x==2){ //if headers match
      x=0; //resets count
      delay(15); //waits for next bit
      while (x<8){
        read=digitalRead(check);
        //Serial.print(read);
        decode[x]=read;
        x=x+1;
      }
      display=0.0;
      for (int i=0; i<8; i=i+1){  //number turned into binary
        display=display+pow(2,i)*decode[7-i];
      } 
      if (reading_number==1){
        for(int i=0;i<4;i=i+1){
          store[i]=0;
          for(int j=0;j<4;j=j+1){
            pages[i][j]=pages[i+1][j]; //sets down a page
          }
        }
        for(int i=0;i<4;i=i+1){
          pages[4][i]=0;
        }
      }
      page=4; //Need to show most recent reading page
      x=0;
      store[reading_number-1]=round(display);
      reading_number=reading_number+1;
      for (int i=0;i<4;i=i+1){
        pages[4][i]=store[i];
      }
      delay(100);
    }
    else{ //when header not detected
      x=0;
      delay(15);
    }
  
  }
  else{
    delay(100);
    reading_number=1;
  }

  //delay(50); //will be removed with more code
  lcd.setCursor(2, 0);
  lcd.print(pages[page][0]);
  lcd.setCursor(2, 1);
  lcd.print(pages[page][1]);
  lcd.setCursor(10, 0);
  lcd.print(pages[page][2]);
  lcd.setCursor(10, 1);
  lcd.print(store[reading_number-1]);//pages[page][3]);
  lcd.setCursor(14, 1);
  lcd.print(reading_number);
  lcd.setCursor(14, 0);
  lcd.print(page);
}
