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
int store =0;
int x=0;
int read=0.0;
double display=0.0;
const int check=6;
int review =0;
int pages[5][4]={
  {1,2,3,4},
  {2,3,4,5},
  {3,4,5,6},
  {4,5,6,7},
  {5,6,7,8}
};
int page=0;
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(check, INPUT);
  pinMode(11,INPUT); //Page up
  pinMode(12,INPUT); //Page down
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
    review=0;
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
    x=0;
    lcd.setCursor(5, 1);
    lcd.print(" ");
    if ((digitalRead(11)==1 or digitalRead(12)==1)and review==0){
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
    else if (digitalRead(11)==1 and page!=4 and review==1){
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
    else if (digitalRead(12)==1 and page!=0 and review==1){
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

