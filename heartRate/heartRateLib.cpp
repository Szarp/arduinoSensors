

#include "Arduino.h"
#include <Wire.h>
#include "heartRateLib.h"

MAX30100::MAX30100(){

}
void MAX30100::shutdown(void){
  uint8_t reg = I2CreadByte(MAX30100_ADDRESS, MAX30100_MODE_CONFIG);  // Get the current register
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_MODE_CONFIG, reg | 0x80);   // mask the SHDN bit
}


void MAX30100::IRsettings(){ //heartRate settings 
  	//pinMode(led,OUTPUT);   
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_MODE_CONFIG, 0x02); // Heart rate only
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_INT_ENABLE, 0x80);// A_FULL_FIFO interrupt
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_LED_CONFIG, 0x0f); //IR power
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG, 0x03); //pulse width
}
void MAX30100::clearInt(void){ //clear pointer after read
  	uint8_t data; // `data` will store the register data
	Wire.beginTransmission(MAX30100_ADDRESS);         // Initialize the Tx buffer
	Wire.write(MAX30100_INT_STATUS);	                 // Put slave register address in Tx buffer
	Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
	Wire.requestFrom(MAX30100_ADDRESS, (uint8_t) 1);  // Read one byte from slave register address
	data = Wire.read();
}
void MAX30100::clearFIFO(void){//set pointers to 0 value
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_FIFO_WR_PTR, 0x00); // Heart rate only
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_OVRFLOW_CTR, 0x00);// A_FULL_FIFO
  I2CwriteByte(MAX30100_ADDRESS, MAX30100_FIFO_RD_PTR, 0x00); //IR power
}
void MAX30100::readFullFIFO(void){ //read all measurements
  for(int i=0;i<16;i++){
   dataContainer[i]=readIR(); 
  }
  //unit8_t 
}
int MAX30100::readIR(void){ //read only IR value
  
  int ret;
  uint8_t temp[4] = {0};  // Temporary buffer for read values
  I2CreadBytes(MAX30100_ADDRESS, MAX30100_FIFO_DATA, &temp[0], 4);  // Read four times from the FIFO
  ret = (temp[0]<<8) | temp[1];    // Combine values to get the actual number
  return ret;
  
}

// Wire.h read and write protocols
void MAX30100::I2CwriteByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
	Wire.beginTransmission(address);  // Initialize the Tx buffer
	Wire.write(subAddress);           // Put slave register address in Tx buffer
	Wire.write(data);                 // Put data in Tx buffer
	Wire.endTransmission();           // Send the Tx buffer
}

uint8_t MAX30100::I2CreadByte(uint8_t address, uint8_t subAddress)
{
	uint8_t data; // `data` will store the register data
	Wire.beginTransmission(address);         // Initialize the Tx buffer
	Wire.write(subAddress);	                 // Put slave register address in Tx buffer
	Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
	Wire.requestFrom(address, (uint8_t) 1);  // Read one byte from slave register address
	data = Wire.read();                      // Fill Rx buffer with result
	return data;                             // Return data read from slave register
}

void MAX30100::I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t * dest, uint8_t count)
{
	Wire.beginTransmission(address);   // Initialize the Tx buffer
	// Next send the register to be read. OR with 0x80 to indicate multi-read.
	Wire.write(subAddress);     // Put slave register address in Tx buffer
	Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
	uint8_t i = 0;
	Wire.requestFrom(address, count);  // Read bytes from slave register address
	while (Wire.available())
	{
		dest[i++] = Wire.read(); // Put read results in the Rx buffer
	}
}
