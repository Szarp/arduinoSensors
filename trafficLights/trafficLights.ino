/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */
  int red=6;
  int yellow=7;
  int green=8;
  int btnPush=0;
  unsigned long lightTime=0;
  unsigned long currentTime=0;

// the setup function runs once when you press reset or power the board
void setup() {
   Serial.begin(57600);
  // initialize digital pin 13 as an output.
  pinMode(red, OUTPUT);//gora
  pinMode(yellow, OUTPUT);//środek
  pinMode(green, OUTPUT);//dół
  pinMode(5, INPUT_PULLUP); 
  
  startDriving();
}

// the loop function runs over and over again forever
void loop() {
  if(digitalRead(5)==0){
     btnPush=1;
     Serial.print("jest");
   }
   currentTime=millis()-lightTime;
   if(btnPush==1 && currentTime>10000){
    stopDriving();
   } 
}

void startDriving(){
  digitalWrite(yellow,HIGH);
  delay(1000);
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(green,HIGH);
  lightTime=millis();  
}
void stopDriving(){
  digitalWrite(green,LOW);
  digitalWrite(yellow,HIGH);
  delay(1000);
  digitalWrite(yellow,LOW);
  digitalWrite(red,HIGH);
  delay(5000);
  btnPush=0;
  startDriving();
}
