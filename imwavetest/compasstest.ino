
#include <imframe.h>

// Data wire is plugged into pin 2 on the Arduino
#define SOIL_BUS A2







//#.define DBGLVL 2
#include "imdebug.h"
#include <imatmega.h>
#include "compasstest.h"
#include <Wire.h>





/******************************** Configuration *************************************/

#define MMAC 0x103080  // My MAC
#define ServerMAC 0xA000  // Server  MAC
#define MDEVICE 9     //Type of device



/************************* Module specyfic functions **********************/


#include "imtrans.h"
#include "imtimer.h"


IMCC1101  cc1101;
Transceiver trx;







void pciSetup(byte pin)
{
  *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
  PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
  PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}





void OnRead(byte state)
{
  DBGINFO(state);
  if (state==2)
  {
    return ;
  }
  trx.timer.doneListen();
}





void SendData()
{
  if (trx.Connected())
  {
    if (trx.CycleData()){
      //        DBGINFO(millis());     
      //        DBGINFO(">");     
      static IMFrame frame;
      frame.Reset();
      if (DataCOMPASS(frame)){

                  Serial.print("SendData(2,3,4) ");

        trx.SendData(frame);
        trx.Transmit();
        //          ERRFLASH();
      } 
    } 

  } 
  else {
    trx.ListenBroadcast();
  }

}



void ReceiveData()
{
  static IMFrame rxFrame;
  if (trx.GetFrame(rxFrame))
  {
    if (!trx.ParseFrame(rxFrame))
    {
      DBGINFO(" rxGET ");
    }
  }
  DBGINFO("\r\n");


}


void PrintStatus()
{
  DBGINFO("\r\n");
  DBGINFO(" Status ");
  trx.printStatus();
  DBGINFO("\r\n");

}

void stageloop(byte stage)
{
  //  DBGINFO("stage=");  DBGINFO(millis());
  //  DBGINFO(":");  DBGINFO(stage);
  switch (stage)
  {
  case STARTBROADCAST:  
    trx.ListenBroadcast();      
    break;
  case STOPBROADCAST:  
    trx.Knock();      
    break;
  case STARTDATA: 
    SendData();
    break;
  case STOPDATA:   
    trx.StopListen();      
    break;
  case LISTENDATA : 
    ReceiveData();
    break;
  case LISTENBROADCAST : 
    ReceiveData();
    break;
    case IMTimer::PERIOD : 
//    PrintStatus();
    break;
  case IMTimer::IDDLESTAGE : {
     DBGINFO("***IDDLE DATA");
     DBGINFO(millis());

       ReceiveData();break;
     }
    
  case CRONHOUR : 
    PrintStatus();
    break;

  default:
    break;
  }
}





void setup()
{
  INITDBG();
  ERRLEDINIT(); 
  ERRLEDOFF();
  SetupCOMPASS();

  interrupts ();
  trx.Init(cc1101);
  trx.myMAC=MMAC;
  trx.myDevice=MDEVICE;
  trx.onEvent=OnRead;
  trx.timer.onStage=stageloop;
  pciSetup(9);
}

void loop()
{
  ERRFLASH();

  byte xstage;
  do{

    xstage=trx.timer.WaitStage();
    stageloop(xstage);
  }
  while( xstage!=IMTimer::PERIOD);

}

