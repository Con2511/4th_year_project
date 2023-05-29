//header and variable assignations
  #include <SoftwareSerial.h> //sets up the relevant packages for modules
  #include <LiquidCrystal.h>
  #include <SD.h>
  #include "RTClib.h"
  //#include<avr/sleep.h>
  #define interruptPin 2
  RTC_DS3231 rtc;
  File myFile;
  const int rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
  LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
  SoftwareSerial mySerial(A2, A7,true);
  int slp=0;
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
  char date[10];
  char times[8];
  //char date[13];
  int min;
  int hr;
  int day;
  int sec;
  int month;
  int year;
  //For sleep mode
  int start_sec;
  int start_min;
  int comp_sec;
  int comp_min;
  int fwd[7]={27,26,25,31,30,29,28};
  int bck[7]={30,29,28,27,26,25,31};
  int index=0;
  int footer;
  volatile int y;
  int PP3_bat;
//battery symbols
  byte vert_60[8]{
  0b00100,
  0b11111,
  0b10001,
  0b10001,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
byte vert_80[8]{
  0b00100,
  0b11111,
  0b10001,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
byte vert_40[8]{
  0b00100,
  0b11111,
  0b10001,
  0b10001,
  0b10001,
  0b11111,
  0b11111,
  0b11111
};  
byte vert_20[8]{
  0b00100,
  0b11111,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b11111,
  0b11111
};
byte vert_100[8]{
  0b00100,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
byte connected[8]{
  0b00011,
  0b00011,
  0b00011,
  0b00011,
  0b01111,
  0b01111,
  0b11111,
  0b11111
};
byte disconnected[8]{
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11000,
  0b11111
};
int noise=1;

void setup() {
  // put your setup code here, to run once:
  pinMode(A0,OUTPUT); //For piezo buzzer
  pinMode(A1,OUTPUT);
  pinMode(A3, INPUT);
  //pinMode(2,INPUT_PULLUP); //Page up
  pinMode(3,INPUT); //Page down
  pinMode(interruptPin,INPUT_PULLUP);
  lcd.begin(16, 2);  //sets LCD screen
  lcd.createChar(0, vert_100);
  lcd.createChar(1, vert_80);
  lcd.createChar(2, vert_60);
  lcd.createChar(3, vert_40);
  lcd.createChar(4, vert_20);
  lcd.createChar(5, connected);
  lcd.createChar(6, disconnected);
  tally=0;
  y=0;
  if (!SD.begin(10)){
    digitalWrite(9,1);
    lcd.setCursor(0, 0);
    lcd.print(F("Card-Restart me"));
    //while(1);
  }
  if (! rtc.begin()) {
    lcd.setCursor(0, 0);
    lcd.print(F("Clock issue"));
    while (1) delay(10);
  }
    if (rtc.lostPower()) {
      //commenting this out to see if time is carried
      //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    gettime();
    // start_sec=sec;
    // start_min=min; 
    makedatefile();
    //makedate();
    myFile=SD.open(date_file, FILE_WRITE);//date_file,FILE_WRITE);
    
    if(myFile){
      myFile.println(F("------------"));
      myFile.close();
    }
    else{
      lcd.setCursor(0, 0);
      lcd.print(F("File-Restart me"));
      //lcd.setCursor(0, 1);
      //lcd.print(date_file);
      //while(1);
    }
 
  lcd.setCursor(0, 1);
  lcd.print(date_file);
  //makedate();
  if (digitalRead(2)==1 or digitalRead(3)==1){
    noise=0;
  }   
  beep();//beep to show is on
  //gettime();
  mySerial.begin(2400);//starts Software Serial port for serial input
  PP3_bat=analogRead(A3);
  lcd.print(F(" "));
  //lcd.print(PP3_bat);
  if (PP3_bat>=890){
    lcd.write(byte(0));
  }
  else if (PP3_bat >=870){
    lcd.write(byte(1));
  }
  else if (PP3_bat >=850){
    lcd.write(byte(2));
  } 
  else if (PP3_bat >=850){
    lcd.write(byte(3));
  }
  else{
    lcd.clear();
    lcd.print(F(" Please charge"));
    lcd.setCursor(0, 1);
    lcd.print(F("battery soon!"));
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(date_file);
    lcd.print(F(" "));
    lcd.write(byte(4));
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (y==0 and tally==0){
    gettime();
    //maketime();
    lcd.setCursor(0, 0);
    lcd.print(times);
    //lcd.setCursor(0, 1);
    //lcd.print(fwd[index]);
    //lcd.print(bck[index]);
  }
  if (mySerial.available()) {
    read=mySerial.read();
    if (int(read)==78){
      lcd.setCursor(15, 1);
      lcd.write(byte(5));
    }
    else if (int(read)!=78 and int(read)!=72 and int(read)!=84){
      lcd.setCursor(15, 1);
      lcd.write(byte(6));
    }
    if (int(read)==72){
      x=0;
      pages_up=0;
      while (72 == int(read) and x!=7 ){
        slp=0;
        if (tally==4){
          y=1;
          tally=0;
        }
        if(mySerial.available() and x==0){
          lightA=mySerial.read();
          x=x+1;
        }
        if(mySerial.available() and x==1){
          lightB=mySerial.read();
          x=x+1;
        }
        if(mySerial.available() and x==2){
          tempA=mySerial.read();
          x=x+1;
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
        if (mySerial.available() and x==6){
          footer=mySerial.read();
          x=x+1;
        }
      }
      if (int(footer)==73){
        y=1;
        light=lightA+lightB*256;
        if (light>=0){
          temp=tempA+tempB*256;
          battery=batteryA+batteryB*256;
          battery=100*(battery/400);
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
          incoming();
          beep();
          if (battery<60){
            lcd.clear();
            lcd.print(F("Please swap"));
            lcd.setCursor(0,1);
            lcd.print(F("Tx soon"));
            delay(1000);
            incoming();
          }
          if (tally==4){
            for(int i=1;i<5;i++){
              for (int j=0;j<4;j++){
                pages[5-i][j]=pages[5-i-1][j];
              }
            }
            pages[0][0]=lstfour[0];
            pages[0][1]=lstfour[1];
            pages[0][2]=lstfour[2];
            pages[0][3]=lstfour[3];
            gettime();
            makedate();
            sdwrite();
          }
        }
      }
    }
    if (int(read)==84){
      x=0;
      if (mySerial.available()) {
        read=mySerial.read();}
      while ((int(read)==84 or slp==1) and x<1000){
        if (slp==0){
          slp=1;
          y=1; 
          lcd.clear();
          lcd.print(F("Please wake"));
          lcd.setCursor(0, 1);
          lcd.print(F("transmitter"));
           x=x+1;       
        }
        if (mySerial.available()){
          read=mySerial.read();
        }
        if (int(read)==78) {
          slp=0;
          incoming();
        }
    }

    }
  if ((digitalRead(2)==1 or digitalRead(3)==1) and pages_up==0){
    y=1;
    page=0;
    pages_up=1;
    display_page();
    //sdwrite();
    delay(100);    
  }
  else if(digitalRead(2)==1 and page!=4 and pages_up==1){
    page+=1;
    display_page();
    delay(100);
  }
  else if(digitalRead(3)==1 and page!=0 and pages_up==1){
    page-=1;
    display_page();
    delay(100);
  }        
  //else{
   // lcd.setCursor(15, 1);
    //lcd.print(F("D"));
  //   gettime();
  //   //introduce sleep function here
   //}        
  }
}

void gettime() {
  DateTime now =rtc.now();
  min=now.minute();
  day=now.day();
  sec=now.second();
  month=now.month();
  year=now.year();
  hr=now.hour();
//low effort date time  
  // if ((now.month()>=3) and (now.month()<11)){
  //   hr=hr+1;    
  // }
  //conversion to GMT from BST, if in GMT then remove
  hr=hr-1;
  //datetime adjustments to complete
  index=((now.year()-2022)%7+int((now.year()-2020)/4))%7;
  if (index>6){
    index=index-6;
  }
  if (month>=3 and month<11){
    if (month>3 and month<10){
      hr=hr+1;
    }
    else if (month==10 and day<bck[index]){
      hr=hr+1;
    }
    else if(month==3 and day>fwd[index]){
      hr=hr+1;
    }  
  }
  if (hr>23){
    hr=0;
    day=day+1;   
  }  
  if (day>31 and (month==1 or month==3 or month==5 or month==7 or month==8 or month==10)){
    day=1;
    month=month+1;    
  }
  else if (day>30 and (month==4 or month==6 or month ==9 or month==11)){
    day=1;
    month=month+1;    
  }  
  else if(day>28 and month==2){
    day=1;
    month=month+1;    
  }
  else if (day>31 and month==12){
    day=1;
    month=1;
    year=year+1;
  }    //   else{now.day()<fwd[index]
  //     if (now.hour()==23){
  //       day=now.day()+1;
  //       hr=0;
  //     }
  //     else{
  //       hr=now.hour()+1;
  //       day=now.day();
  //     }
  //   }
  // }
  // else{
  //   hr=now.hour();
  //   day=now.day();
  // } 
  //sprintf(times, "%02d:%02d:%02d", hr, min, sec);
  maketime();  
  yr=now.year()-2000;
  //makedate();
  //sprintf(date_file, "%02d_%02d_%02d.txt", day, month, yr);//sets the file name as the date
  //sprintf(date, "%02d/%02d/%02d", day, month, yr);
  //return 0; 
}
void display_page() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("1:"));
  lcd.print(pages[page][0]);
  lcd.setCursor(0,1);
  lcd.print(F("3:"));
  lcd.print(pages[page][2]);
  lcd.setCursor(7,0);
  lcd.print(F("2:"));
  lcd.print(pages[page][1]);
  lcd.setCursor(7,1);
  lcd.print(F("4:"));
  lcd.print(pages[page][3]);
  lcd.setCursor(13, 0);
  lcd.print(F("Pg:"));
  lcd.setCursor(14, 1);
  lcd.print(page+1);
  delay(100);  
}
void beep() {
  if (noise==1){
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
  }
}
void incoming() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("LST:"));
  lcd.print(lstfour[3]);
  lcd.setCursor(8,0);
  lcd.print(lstfour[2]);
  lcd.setCursor(4,1);
  lcd.print(lstfour[1]);
  lcd.setCursor(8,1);
  lcd.print(lstfour[0]);
  lcd.setCursor(13, 0);
  lcd.print(round(floor(temp)));
  lcd.print(F("C"));
  lcd.setCursor(13,1);
  //lcd.print(round(battery));
  symbol();
  //lcd.print(F("%"));
  lcd.setCursor(0,1);
  lcd.print(F("#:"));
  lcd.print(tally);
}
void sdwrite() {
  //gettime();
  myFile=SD.open(date_file,FILE_WRITE);
  //makedate();
  myFile.println(date); //maybe change this to alter shape of writing?
  myFile.println(times);
  myFile.print(F("Temp:"));
  myFile.println(temp);
  myFile.print(F("1:"));
  myFile.print(pages[0][0]);
  myFile.print(F("     2:"));
  myFile.println(pages[0][1]);
  myFile.print(F("3:"));
  myFile.print(pages[0][2]);
  myFile.print(F("     4:"));
  myFile.println(pages[0][3]);
  myFile.println(F("---------"));
  myFile.close();
}
// void Going_To_Sleep(){
//   sleep_enable();
//   attachInterrupt(0, wakeUp, RISING);
//   //digitalWrite(LED_BUILTIN,LOW);
//   delay(2000);
//   set_sleep_mode(SLEEP_MODE_PWR_DOWN);
//   //delay(1000);
//   sleep_cpu();
//   //digitalWrite(LED_BUILTIN,HIGH);
//   delay(1000);
// }
// void wakeUp(){
//   sleep_disable();
//   detachInterrupt(0);
// } 
void maketime(){
  sprintf(times, "%02d:%02d:%02d", hr, min, sec);  
} 
void makedatefile(){
   sprintf(date_file, "%02d_%02d_%02d.txt", day, month, yr);//sets the file name as the date 
}
void makedate(){
  sprintf(date, "%02d/%02d/%02d", day, month, yr);
}
void symbol(){
  if (battery>=90){
    lcd.write(byte(0));
  }
  else if (battery >=80){
    lcd.write(byte(1));
  }
  else if (battery >=70){
    lcd.write(byte(2));
  } 
  else if (battery >=60){
    lcd.write(byte(3));
  }
  else{
    lcd.write(byte(4));
  }  
}