int x=0;
void setup() {
  // put your setup code here, to run once:
pinMode(9,OUTPUT);
while(x<500){
  digitalWrite(9,1);
  delay(1);
  digitalWrite(9,0);
  delay(1);
  x=x+1;
}
delay(1000);
x=0;
while(x<1000){
  digitalWrite(9,1);
  delay(0.55);
  digitalWrite(9,0);
  delay(0.55);
  x=x+1;
}
}

void loop() {
  // put your main code here, to run repeatedly:
//
}
