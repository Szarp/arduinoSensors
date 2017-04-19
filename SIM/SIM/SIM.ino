//connect to port 2,3 rx and tx
#include <SoftwareSerial.h>

String str =""; //input form device
String number = ""; //cut input from str
String book = "";
String nr="";
int isInContact= -1;
SoftwareSerial mySerial(2,3);

void setup()
{
  mySerial.begin(9600);                 
}

void loop()
{
  if (mySerial.available()){
    str=mySerial.read();
    if(str=="RING"){
      mySerial.write("AT+CLIP");
      delay(10); //wait for response
      number=mySerial.read();
      number=number.substring(0,9);//get number only
          check();
          mySerial.write("AT+CHUP");//end call
          if(isInContact >0){
            String toprint="ATD "+isInContact; //comand to make call from phone book
            mySerial.println(toprint);//execute command
          }
      } 
    }
  }


void check(){
    int comma=0;
    int enter=0;
    int index=0;
    isInContact=-1;
    mySerial.write("AT+CPBR");
    delay(10);
    book=mySerial.read();
    do{
      comma=book.indexOf(",");
      enter=book.indexOf("/n");
      nr=book.substring(comma,comma+9);
      if(nr==number){
        isInContact=index;
      }
      index++;
    }while(comma >-1 && enter>-1);
    
}
