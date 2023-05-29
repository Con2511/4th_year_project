// include the library code:
#include <LiquidCrystal.h>

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
int store =0;
int x=0;
int read=0.0;
double display=0.0;
double displaytemp=0.0;
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
int T;
int closest[2]={0,1000};
int sndclosest[2]={0,1000};

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(check, INPUT);
  pinMode(A1,INPUT); //Page up
  pinMode(A2,INPUT); //Page down
  pinMode(9,OUTPUT);
  //pinMode(A0,INPUT);
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
    //lcd.setCursor(7, 1);
    //lcd.print("yes");
    delay(15);
  }
  else if (read==1 and x==2){
    x=x+1;
    delay(15);
  }
  else if (read==1 and x==3){
    x=x+1;
    delay(15);
  }
  else if (read==1 and x==4){
    x=x+1;
    delay(15);
  }
  else if (read==0 and x==5){
    x=x+1;
    delay(15);
  }
  else if (read==0 and x==6){
    x=0;
    review=0;
    digitalWrite(9,1);
    //decode=0;
    delay(15);
    while (x<10){
      read=digitalRead(check);
      //decode=decode+pow(2,(7-x))*read;
      decode[x]=read;
      x=x+1;
      //lcd.setCursor(7+x,0);
      //lcd.print(read);
      delay(15);
    }
    x=0;
    while (x<10){
      read=digitalRead(check);
      //decode=decode+pow(2,(7-x))*read;
      decodetemp[x]=read;
      x=x+1;
      //lcd.setCursor(7+x,0);
      //lcd.print(read);
      delay(15);
    }
    fourth=store;
    store=val;
    val=round(display);
    display=0.0;
    displaytemp=0.0;
    for (int i=0.0; i<10; i=i+1){
      display=display+pow(2,i)*decode[9-i];
      displaytemp=displaytemp+pow(2,i)*decodetemp[9-i];
    }
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
    lcd.setCursor(4, 1);
    lcd.print(store);
    lcd.setCursor(8, 1);
    lcd.print(fourth);
    lcd.setCursor(13,0);
    lcd.print("   ");
    lcd.setCursor(12,0);
    lcd.print(displaytemp);
    if (tally==3){
      tally=0;
      for(int i=0;i<4;i=i+1){
        for(int j=0;j<4;j=j+1){
          pages[i][j]=pages[i+1][j]; //sets down a page
        }
      }
      pages[4][0]=fourth;
      pages[4][1]=store;
      pages[4][2]=val;
      pages[4][3]=round(display);
    }
    else{
      tally=tally+1;
    }
    delay(100);             //Maybe need to tone down this delay? or add foot to ensure no run on
  }
  else{
    //T=analogRead(A0);
    digitalWrite(9,0);
    lcd.setCursor(13,1);
    lcd.print("   ");
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

