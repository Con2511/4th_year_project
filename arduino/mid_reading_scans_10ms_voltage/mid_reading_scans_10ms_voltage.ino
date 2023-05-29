// include the library code:
#include <LiquidCrystal.h>
#include <SD.h>
//#include <SPI.h>
File myFile;
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 8, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int val =0;
int tally=0;
int fourth=0;
int i=0;
int decode[8];
int decodetemp[8];
int readings[15];
int store =0;
int x=0;
int read=0.0;
int readtemp=0;
double display=0.0;
double displaytemp=0.0;
double batterylvl=0;
const int check=6;
int review =0;
int pages[5][4]={
  {1,2,3,4},
  {2,3,4,5},
  {3,4,5,6},
  {4,5,6,7},
  {5,6,7,8}
};
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
int page=0;
int decode0=0;
int closest[2]={0,1000};
int sndclosest[2]={0,1000};

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(check, INPUT);
  pinMode(A1,INPUT); //Page up
  pinMode(A2,INPUT); //Page down
  pinMode(9,OUTPUT);
  pinMode(A0, OUTPUT);
  // while (!Serial) {
  // ; // wait for serial port to connect. Needed for native USB port only
  // }
  if (!SD.begin(10)){
    digitalWrite(9,1);
    lcd.setCursor(0, 0);
    lcd.print("Card setup not worked");
    while(1);
  }
  myFile= SD.open("reading.txt", FILE_WRITE);
  //pinMode(A0,INPUT);
  if(myFile){
    digitalWrite(9,0);
    myFile.println("No repetition");
    myFile.close();
  }
  else{
    lcd.setCursor(0, 0);
    lcd.print("File not open");
    while(1);
  }
}

void loop() {
  read=digitalRead(check);
  if (read==1 and x==0){
    x=x+1;
    //lcd.setCursor(8, 0);
    //lcd.print("yes");
    delay(10);
  }
  else if (read==0 and x==1){
    x=x+1;
    //lcd.setCursor(7, 1);
    //lcd.print("yes");
    delay(10);
  }
  else if (read==1 and x==2){
    x=x+1;
    delay(10);
  }
  else if (read==1 and x==3){
    x=x+1;
    delay(10);
  }
  else if (read==1 and x==4){
    x=x+1;
    delay(10);
  }
  else if (read==0 and x==5){
    x=x+1;
    delay(10);
  }
  else if (read==0 and x==6){
    x=x+1;
    delay(10);
  }
  else if (read==1 and x==7){
    x=x+1;
    delay(10);
  }
  else if (read==0 and x==8){
    x=x+1;
    delay(10);
  }
  else if (read==1 and x==9){
    x=x+1;
    delay(10);
  }
  else if (read==1 and x==10){
    x=x+1;
    delay(10);
  }
  else if (read==0 and x==11){
    x=0;
    review=0;
    //digitalWrite(9,1);
    fourth=store;
    store=val;
    val=round(display);
    display=0.0;
    //decode=0;
    delay(10);
    while (x<10){
      for (int i=0;i<10;i=i+1){
      read=digitalRead(check);
      readings[i]=read;
      delay(1);
      }
      read=0;
      for (int i=0;i<10;i=i+1){
        read=read+readings[i];
      }
      if (read>=4){
        read=1;
      }
      else{
        read=0;
      }
      decode[x]=read;
      if (x==0){
        decode0=read;
      }
      //Serial.print(read);
      display=display+pow(2,9-x)*read;
      x=x+1;
      //delay(15);
    }
    //Serial.println(" ");
    //Serial.print(decode0);
    //Serial.print("fix");
    //Serial.println(" ");
    x=0;
    while (x<10){
      for (int i=0;i<10;i=i+1){
      readtemp=digitalRead(check);
      readings[i]=readtemp;
      delay(1);
      }
      readtemp=0;
      for (int i=0;i<10;i=i+1){
        readtemp=readtemp+readings[i];
      }
      if (readtemp>=5){
        readtemp=1;
      }
      else{
        readtemp=0;
      }
      //decode=decode+pow(2,(7-x))*read;
      decodetemp[x]=readtemp;
      //Serial.print(readtemp);
      x=x+1;
      //lcd.setCursor(7+x,0);
      //lcd.print(read);
      //delay(15);
    }
    //Serial.println(" ");
    x=0;
    batterylvl=0;
    while (x<9){
      for (int i=0;i<10;i=i+1){
      readtemp=digitalRead(check);
      readings[i]=readtemp;
      delay(1);
      }
      readtemp=0;
      for (int i=0;i<10;i=i+1){
        readtemp=readtemp+readings[i];
      }
      if (readtemp>=5){
        readtemp=1;
      }
      else{
        readtemp=0;
      }
      batterylvl=batterylvl+pow(2,8-x)*readtemp;
      
      x=x+1;
    }
    displaytemp=0.0;
    batterylvl=100*(batterylvl/307.2);
    if (batterylvl<50){

    }
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0,0);
    lcd.print("Lst:");
    lcd.print(round(display));
    lcd.setCursor(0,1);
    lcd.print("4");
    lcd.setCursor(8, 0);
    lcd.print(val);
    lcd.print("|");
    lcd.setCursor(4, 1);
    lcd.print(store);
    lcd.setCursor(8, 1);
    lcd.print(fourth);
    lcd.print("|");
    lcd.setCursor(13,0);
    lcd.print("   ");
    lcd.setCursor(12,1);
    lcd.print(batterylvl);
    displaytemp=1*decodetemp[9]+2*decodetemp[8]+4*decodetemp[7]+8*decodetemp[6]+16*decodetemp[5]+32*decodetemp[4]+64*decodetemp[3]+128*decodetemp[2]+256*decodetemp[1]+512*decodetemp[0];
    //}
    //Linear regression line for -15 to 50
    //displaytemp=(displaytemp-239.85)/10.728;
    
    //LUT approach
    closest[1]=1000;
    sndclosest[1]=1000;
    //Goes through LUT to see what interval the temperature reading is in
    for (int i=0; i<19; i=i+1){
      if (abs((LUT[i][1]-displaytemp))<closest[1]){
        closest[1]=abs((LUT[i][1]-displaytemp));
        closest[0]=i;
      }
      else if (abs((LUT[i][1]-displaytemp))<sndclosest[1]){
        sndclosest[1]=abs((LUT[i][1]-displaytemp));
        sndclosest[0]=i;
      }
    }
    //Use Linear regression to get temperature
    displaytemp=((displaytemp-LUT[closest[0]][1])/(LUT[sndclosest[0]][1]-LUT[closest[0]][1]))*(LUT[sndclosest[0]][0]-LUT[closest[0]][0])+LUT[closest[0]][0];

   
    lcd.setCursor(12,0);
    lcd.print(displaytemp);
    if (tally==3){
      tally=0;
      for(int i=0;i<4;i=i+1){
        for(int j=0;j<4;j=j+1){
          pages[i][j]=pages[i+1][j]; //sets down a page
        }
      }
      myFile= SD.open("reading.txt", FILE_WRITE);
      myFile.print("Date   Temp:");
      myFile.println(displaytemp);
      myFile.print("1:");
      myFile.print(fourth);
      myFile.print("     2:");
      myFile.println(store);
      myFile.print("3:");
      myFile.print(val);
      myFile.print("     4:");
      myFile.println(round(display));
      myFile.close();
      pages[4][0]=fourth;
      pages[4][1]=store;
      pages[4][2]=val;
      pages[4][3]=round(display);
    }
    else{
      tally=tally+1;
    }
    //digitalWrite(9,0);
    //delay(700);
    x=0;
    while(x<300){
      digitalWrite(9,1);
      digitalWrite(A0,0);
      delay(1);
      digitalWrite(A0,1);
      digitalWrite(9,0);
      delay(1);
      x=x+1;
    }
    digitalWrite(9,0);
    digitalWrite(A0,0);             //Maybe need to tone down this delay? or add foot to ensure no run on
  }
  else{
    //T=analogRead(A0);
    digitalWrite(9,0);
    lcd.setCursor(2,1);
    lcd.print("");
    //lcd.setCursor(13,1);
    //lcd.print(T);
    x=0;
    lcd.setCursor(1, 1);
    lcd.print(" ");
    
    if ((digitalRead(A1)==1 or digitalRead(A2)==1)and review==0){
      page=4;
      review=1;
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
      lcd.setCursor(8,0);
      lcd.print("3:");
      lcd.print(pages[page][2]);
      lcd.setCursor(8,1);
      lcd.print("4:");
      lcd.print(pages[page][3]);
      lcd.setCursor(13, 0);
      lcd.print("#:");
      lcd.print(page+1);
      delay(500);
    }
    else if (digitalRead(A1)==1 and page!=4 and review==1){
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
      lcd.setCursor(8,0);
      lcd.print("3:");
      lcd.print(pages[page][2]);
      lcd.setCursor(8,1);
      lcd.print("4:");
      lcd.print(pages[page][3]);
      lcd.setCursor(13, 0);
      lcd.print("#:");
      lcd.print(page+1);
      delay(500);
    }
    else if (digitalRead(A2)==1 and page!=0 and review==1){
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
      lcd.setCursor(8,0);
      lcd.print("3:");
      lcd.print(pages[page][2]);
      lcd.setCursor(8,1);
      lcd.print("4:");
      lcd.print(pages[page][3]);
      lcd.setCursor(13, 0);
      lcd.print("#:");
      lcd.print(page+1);
      delay(500);
    }
  }
  
  
  
}

