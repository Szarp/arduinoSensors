// 
//    FILE:
// VERSION: 0.1.00
// PURPOSE: HC SR501 PIR  logger  for imwave
//
//
// HISTORY:
//

#ifndef compasstest_h
#define compasstest_h

#include "Arduino.h"
#include "imframe.h"
#include "imdebug.h"
#include "imatmega.h"
#include <Wire.h>
// Setup a oneWire instance to communicate with ANY OneWire devices
#define compass_address 0x1E       // The I2C address of the Magnetometer
#define configuration_bits 0b01110001




void SetupCOMPASS()
{
  //pinMode(SOIL_BUS,INPUT);
  DBGINFO("bus:");
  //DBGINFO(SOIL_BUS);
  Wire.begin();
  Serial.begin(9600);

}




bool DataCOMPASS(IMFrame &frame)
{
   IMFrameData *data =frame.Data();
 Wire.beginTransmission(compass_address);
  Wire.write(0x02);
  Wire.write(configuration_bits); //te bity sugeruj� jak co� szybko mo�na odczytywa�
  // Writing the register value 0000 0000 for continous mode
  // Writing the register value 0000 0001 for single
  Wire.endTransmission();
  Wire.requestFrom(compass_address,6);

  if (6 <= Wire.available()){

    data->w[2] = Wire.read()<<8 | Wire.read();

    data->w[3] = Wire.read()<<8 | Wire.read();

    data->w[4] = Wire.read()<<8 | Wire.read();

  }
    return true;


}





#endif
//
// END OF FILE
//
