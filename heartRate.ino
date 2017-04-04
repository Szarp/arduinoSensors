/*----------------------
|Pulse pin label  | Pin Function         |Arduino Connection|
|-----------------|:--------------------:|-----------------:|
| VCC             | Supply               |  +3.3V             |             
| SCL             | Serial clock         |  A5              |
| SDA             | Serial data          |  A4              |
| GND             |                      |  GND             |
| INT             | Interrupt            |  D2              |
-------------------------------------------------------------
*/
#include "heartRateLib.h"
#include <Wire.h>

MAX30100 sensor;
uint8_t maxValueIndex[5]; //wait for 5 measurements
uint8_t maxValuePointer;
int x = 0;
int last=0; //last avarage
uint16_t avarage;
int pointer=0; //what do next
int meassureMode=0; //looking for falling in avarge
int dataIndex; //index or every mesurment came in
void setup() {
  Wire.begin();
  Serial.begin(57600);
  pinMode(2,INPUT);
  //getPulse();
  sensor.shutdown();
}
void ifBigger(){ //check if avarge is rising or falling
 // pointer=1; 
  if(meassureMode==0){
    if(avarage>last){
      //Serial.println("Start finding max value");
      meassureMode=1;//looking for loss
    }
  }
  else{
    if(avarage < last){
      meassureMode=0;
      saveMaxValue(); //index of that measurement
    }  
  }
  last=avarage;
}
void saveMaxValue(){
  maxValuePointer[maxValueIndex]=dataIndex;
  maxValuePointer++;
  if(maxValuePointer >5){
    pointer=3; //stop measure
  }
}
void btnClick(){//interrupt function
  x++;
  pointer=2;
}

void loop() {
  whatNext();
}
void startMeassure(){
  dataIndex=0;
  //stopIndex=0;
  maxValuePointer=0;
  avarage=sensor.dataContainer[0];
  last=avarage+1;
}
void getPulse(){ //run settings for heart rate
    sensor.IRsettings();
    //sensor.clearInt();
    sensor.clearFIFO();
}
void whatNext(){ //modes of working
  if(pointer == 0){ //start
    //messureMode
    attachInterrupt(0,btnClick,FALLING);
    Serial.println("Meassure Mode");
     getPulse();
     //delay(10);
     pointer=1;
     x=0;
 }
  if(pointer == 1){
    //waitForInterup

  }
  if(pointer == 2){ //after interrupt
    pointer=1;
    sensor.clearInt();
    sensor.readFullFIFO();
    if(x==1){
      Serial.println("Start Maessure");
      startMeassure();
    }
    for(int i=0;i<15;i++){
      dataIndex++;
      avarage= avarage*0.99+sensor.dataContainer[i]*0.01;
      ifBigger();
    }
  }
  if(pointer == 3){
   Serial.println("END");
     detachInterrupt(0);    
     sensor.shutdown(); 
     for(int i=1;i<5;i++){
       //heart rate(by index of sample);
      Serial.println(maxValueIndex[i]-maxValueIndex[i-1]); 
     }
     pointer=0; //repeat measure or set in to 4 
  
  }
  if(pointer == 4){
   //sleepMode 
  }
   
}
