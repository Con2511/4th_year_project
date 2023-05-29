//#include <SPI.h>
#include <SoftwareSerial.h> //sets up the relevant packages for modules
#include <LiquidCrystal.h>
#include <SD.h>
#include "RTClib.h"
RTC_DS3231 rtc;
File myFile;
const int rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SoftwareSerial mySerial(A2, A7,true);

char read=0; //reads the H which gets sent
int light=0; //These variables are to turn 2x8bit to one integer
int lightA=0;
int lightB=0;
double temp=0;
double battery=0;
int tempA=0;
int batteryA=0;
int tempB=0;
int batteryB=0; 
int x=0;            //Useful for counts, like for buzzer
int closest[2]={0,1000};  //These variables are used for temp adc=>temp
int sndclosest[2]={0,1000};
const double LUT[19][2]={
  {-30,45.3759},
  {-25,61.5347},
  {-20,82.1434},
  {-15,108.3483},
  {-10,140.4144},
  {-5,179.6145},
  {0,224.7438},
  {5,275.8695},
  {10,331.8211},
  {15,390.78},
  {20,451.4394},
  {25,512},
  {30,570.7279},
  {35,627.1897},
  {40,679.3154},
  {45,726.1072},
  {50,767.8292},
  {55,804.7594},
  {60,836.8064}
};
int pages[5][4]={ //These variables are to do with the page display
  {1,2,3,4},
  {2,3,4,5},
  {3,4,5,6},
  {4,5,6,7},
  {5,6,7,8}
};
int lstfour[4]={0,0,0,0};
int tally=0;
int pages_up=0; //goes to one if the stored data is being viewed
int page=0;
int yr;//these variables will be used for the RTC and file interface
char date_file[12];
char times[8];
char date[13];
int min=0;
int hr=0;
int day=0;
int second=0;
int month=0;
int year=0;
const int fwd[7]={27,26,25,31,30,29,28};
const int bck[7]={30,29,28,27,26,25,31};
int index=0;

void setup() {
pinMode(A0,OUTPUT); //For piezo buzzer
pinMode(A1,OUTPUT);
pinMode(2,INPUT); //Page up
pinMode(3,INPUT); //Page down
lcd.begin(16, 2);  //sets LCD screen
tally=0;
if (!SD.begin(10)){
  digitalWrite(9,1);
  lcd.setCursor(0, 0);
  lcd.print("Card-Restart me");
  while(1);
}
if (! rtc.begin()) {
    while (1) delay(10);
}
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  DateTime now = rtc.now();
  yr=now.year()-2000;
  sprintf(date_file, "%02d_%02d_%02d.txt", now.day(), now.month(), yr);//sets the file name as the date
    
  lcd.setCursor(0, 1);
  lcd.print(date_file);
  myFile=SD.open(date_file,FILE_WRITE);
  if(myFile){
    myFile.close();
  }
  else{
    lcd.setCursor(0, 0);
    lcd.print("File-Restart me");
    while(1);
  }
  x=0;
  while(x<200){    //beep to show is on
    digitalWrite(A1,1);
    digitalWrite(A0,0);
    delay(1);
    digitalWrite(A0,1);
    digitalWrite(A1,0);
    delay(1);
    x=x+1;
  }
  x=0;
  digitalWrite(A1,0);
  digitalWrite(A0,0);
mySerial.begin(2400);  //starts Software Serial port for serial input
}

void loop() {
  if (mySerial.available()) {
    read=mySerial.read();
    //Serial.println(read);
      //Serial.println(read);
      //Serial.println(int(read));
    if (int(read)==72){
      x=0;
      pages_up=0;
      while (72 == int(read) and x!=6 ){
        //Serial.print("match");

        if (tally==4){
          tally=0;
        }
        if(mySerial.available() and x==0){
          lightA=mySerial.read();
          x=x+1;
          //read=0;
        }
        if(mySerial.available() and x==1){
          lightB=mySerial.read();
          x=x+1;
          //read=0;
        }
        if(mySerial.available() and x==2){
          tempA=mySerial.read();
          x=x+1;
          //read=0;
        }
        if(mySerial.available() and x==3){
          tempB=mySerial.read();
          x=x+1;
        }
        if(mySerial.available() and x==4){
          batteryA=mySerial.read();
          x=x+1;
        }
        if(mySerial.available() and x==5){
          batteryB=mySerial.read();
          x=x+1;
        }
      }
      
      light=lightA+lightB*256;
      temp=tempA+tempB*256;
      battery=batteryA+batteryB*256;
      battery=100*(battery/307.2);
      closest[1]=1000;
      sndclosest[1]=1000;
      //Goes through LUT to see what interval the temperature reading is in
      for (int i=0; i<19; i=i+1){
        if (abs((LUT[i][1]-temp))<closest[1]){
          closest[1]=abs((LUT[i][1]-temp));
          closest[0]=i;
        }
        else if (abs((LUT[i][1]-temp))<sndclosest[1]){
          sndclosest[1]=abs((LUT[i][1]-temp));
          sndclosest[0]=i;
        }
      }
      //Use Linear regression to get temperature
      temp=((temp-LUT[closest[0]][1])/(LUT[sndclosest[0]][1]-LUT[closest[0]][1]))*(LUT[sndclosest[0]][0]-LUT[closest[0]][0])+LUT[closest[0]][0];
      tally=tally+1;
      for (int i=0; i<3;i++){
        lstfour[i]=lstfour[i+1];
      }
      lstfour[3]=light;
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("LST:");
      lcd.print(lstfour[3]);
      lcd.setCursor(8,0);
      lcd.print(lstfour[2]);
      lcd.setCursor(4,1);
      lcd.print(lstfour[1]);
      lcd.setCursor(8,1);
      lcd.print(lstfour[0]);
      lcd.setCursor(13, 0);
      lcd.print(round(temp));
      lcd.print("C");
      lcd.setCursor(13,1);
      lcd.print(round(battery));
      lcd.print("%");
      lcd.setCursor(0,1);
      lcd.print("#:");
      lcd.print(tally);
      x=0;
      while(x<150){    //beep to show received
        digitalWrite(A1,1);
        digitalWrite(A0,0);
        delay(1);
        digitalWrite(A0,1);
        digitalWrite(A1,0);
        delay(1);
        x=x+1;
      }
      x=0;
      digitalWrite(A0,0);
      digitalWrite(A1,0);
      if (tally==4){
        for(int i=1;i<5;i++){
          for (int j=0;j<4;j++){
            pages[i][j]=pages[i-1][j];
          }
        }
        pages[0][0]=lstfour[0];
        pages[0][1]=lstfour[1];
        pages[0][2]=lstfour[2];
        pages[0][3]=lstfour[3];
        
        DateTime now=rtc.now();
        min=now.minute();
        second=now.second();
        month=now.month();
        year=now.year();
        index=((now.year()-2022)%7+int((now.year()-2020)/4))%7;
        if (index>6){
          index=index-6;
        }
        if (now.month()>=3 and now.month()<11){
          if (now.month()==3 and now.day()<fwd[index]){
            hr=now.hour();
            day=now.day();
          }
          else if (now.month()==10 and now.day()>bck[index]){
            hr=now.hour();
            day=now.day();
          }
          else{
            if (now.hour()==23){
              day=now.day()+1;
              hr=0;
            }
            else{
              hr=now.hour()+1;
              day=now.day();
            }
          }
        }
        else{
          hr=now.hour();
          day=now.day();
        }
        //not sure why it doesn't work?
        //sprintf(date, "%02d/%02d/%02d", now.day(), now.month(), now.year());
        myFile=SD.open(date_file,FILE_WRITE);
        //sprintf(date, "%02d/%02d/%02d", day, month, year);
        //myFile.print(date);
        //myFile.print("  ");
        sprintf(times, "%02d:%02d:%02d", hr, min,second);
        myFile.println(times);
        myFile.print("Temp:");
        myFile.println(temp);
        myFile.print("1:");
        myFile.print(pages[0][0]);
        myFile.print("     2:");
        myFile.println(lstfour[1]);
        myFile.print("3:");
        myFile.print(lstfour[2]);
        myFile.print("     4:");
        myFile.println(lstfour[3]);
        myFile.close();
      }
    }
    //If the header doesn't match the H
    if ((digitalRead(2)==1 or digitalRead(3)==1) and pages_up==0){
      page=0;
      pages_up=1;
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0,0);
      lcd.print("1:");
      lcd.print(pages[page][0]);
      lcd.setCursor(0,1);
      lcd.print("2:");
      lcd.print(pages[page][1]);
      lcd.setCursor(7,0);
      lcd.print("3:");
      lcd.print(pages[page][2]);
      lcd.setCursor(7,1);
      lcd.print("4:");
      lcd.print(pages[page][3]);
      lcd.setCursor(13, 0);
      lcd.print("Pg:");
      lcd.setCursor(15, 1);
      lcd.print(page+1);
      delay(500);  
    }
    else if(digitalRead(2)==1 and page!=4 and pages_up==1){
      page+=1;
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0,0);
      lcd.print("1:");
      lcd.print(pages[page][0]);
      lcd.setCursor(0,1);
      lcd.print("2:");
      lcd.print(pages[page][1]);
      lcd.setCursor(7,0);
      lcd.print("3:");
      lcd.print(pages[page][2]);
      lcd.setCursor(7,1);
      lcd.print("4:");
      lcd.print(pages[page][3]);
      lcd.setCursor(13, 0);
      lcd.print("Pg:");
      lcd.setCursor(15, 1);
      lcd.print(page+1);
      delay(500);
    }
    else if(digitalRead(3)==1 and page!=0 and pages_up==1){
      page-=1;
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0,0);
      lcd.print("1:");
      lcd.print(pages[page][0]);
      lcd.setCursor(0,1);
      lcd.print("2:");
      lcd.print(pages[page][1]);
      lcd.setCursor(7,0);
      lcd.print("3:");
      lcd.print(pages[page][2]);
      lcd.setCursor(7,1);
      lcd.print("4:");
      lcd.print(pages[page][3]);
      lcd.setCursor(13, 0);
      lcd.print("Pg:");
      lcd.setCursor(15, 1);
      lcd.print(page+1);
      delay(500);
    }
  }
}
