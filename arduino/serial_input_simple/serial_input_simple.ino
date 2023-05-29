#include <SPI.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
const int rs = 7, en = 8, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SoftwareSerial mySerial(6, A7,true);

char read=0;
int light=0;
int lightA=0;
int lightB=0;
double temp=0;
int battery=0;
int tempA=0;
int batteryA=0;
int tempB=0;
int batteryB=0; 
int x=0;
int closest[2]={0,1000};
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

void setup() {
pinMode(A0,OUTPUT);
pinMode(9,OUTPUT);
lcd.begin(16, 2);  // put your setup code here, to run once:
Serial.begin(2400);
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
mySerial.begin(2400);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (mySerial.available()) {
    read=mySerial.read();
    //Serial.println(read);
    if (read!=-1){
      Serial.println(read);
      //Serial.println(int(read));
      x=0;
      while (72 == int(read) and x!=6 ){
        //Serial.print("match");
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
      lcd.setCursor(0, 0);
      lcd.print(light);
      lcd.print(" ");
      lcd.print(temp);
      lcd.print(" ");
      lcd.print(battery);
    }
  }
}
