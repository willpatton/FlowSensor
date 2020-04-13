/**
 * @author: Will Patton https://github.com/willpatton
 * @date:   April 9, 2020
 * @license: MIT
 * 
 * 
 * FLOW SENSOR             
 * Sensirion             
 * SFM3200-250-AW, Digital Flow Meter for medical applications           
 * https://www.sensirion.com/en/download-center/mass-flow-meters/flow-sensor-sfm3200-for-a-superior-performance-at-low-flows/
 * 
 * DATASHEET
 * 100msec startup time
 * 5.0V DC
 * 
 * 
 * I2C INTERFACE DOC
 * https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/5_Mass_Flow_Meters/Application_Notes/Sensirion_Mass_Flo_Meters_SFM3xxx_I2C_Functional_Description.pdf
 * 
 * This device's wiring should not be longer than 30 cm. A cap/cable range extender may be required for signal integrity.
 * 
 */

#include <Arduino.h>
#include <Wire.h>
#include "sfm3200.h"      //driver for Sensirion SFM3200-250-AW flow sensor

//DISPLAY or PLOT value
#define DISPLAY false


//OBJECTS
sfm3200 sfm;
  
//FLOW SENSOR
#define ADDR = 0x40           //0x40 is address 64
uint16_t flow = 0;


/**
 * setup
 */
void setup()
{

  //SERIAL
  Serial.begin(115200);
  delay(1000);                //serial delay

  //I2C
  Wire.begin();               // begin i2c bus (address optional for master)
  
  //FLOW SENSOR
  sfm.begin();                //initialize flow sensor
  if(DISPLAY){
    Serial.print("SFM3200 serial number: ");    //Show serial number
    Serial.println(sfm.readSernum(), HEX);
  }
}

/**
 * loop
 */
void loop()
{
  
  //READ FLOW SENSOR
  flow = sfm.readFlow();

  //DISPLAY or PLOT
  Serial.println(flow);

  delay(10);                   //loop, not too fast!

}