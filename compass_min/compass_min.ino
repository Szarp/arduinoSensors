/* Author = helscream (Omer Ikram ul Haq)
Last edit date = 2014-06-22
Website: http://hobbylogs.me.pn/?p=17
Location: Pakistan
Ver: 0.1 beta --- Start
Ver: 0.2 beta --- Debug feature included
*/
#include "compass.h"
#include <Wire.h>
#define compass_address 0x1E       
#define Task_t 10          // Task Time in milli seconds
#define configuration_bits 0b01110001
/*
   A A                        DO2 DO1 DO0      Sample Rate [Hz]      MS1 MS0    Measurment Mode
   0 0 = No Average            0   0   0   =   0.75                   0   0   = Normal  
   0 1 = 2 Sample average      0   0   1   =   1.5                    0   1   = Positive Bias
   1 0 = 4 Sample Average      0   1   0   =   3                      1   0   = Negative Bais
   1 1 = 8 Sample Average      0   1   1   =   7.5                    1   1   = -
   1   0   0   =   15 (Default)
   1   0   1   =   30
   1   1   0   =   75
   1   1   1   =   -
   */
// Main code -----------------------------------------------------------------
void setup(){
  Serial.begin(9600);
  // Serial.print("Setting up I2C ........\n");
  Wire.begin();
};

// Main loop 
// Main loop -----------------------------------------------------------------
void loop(){  
  compass_read_XYZdata();
  Serial.print("x = ");
  Serial.println(compass_x);
  Serial.print("y = ");
  Serial.println(compass_y);
  Serial.print("z = ");
  Serial.println(compass_z);
    delay(3000);  
}











