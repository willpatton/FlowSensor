/**

sfm3200.cpp

@author: Will Patton: https://github.com/willpatton
@date: April 7, 2020
@license: MIT

*/

#include "Arduino.h"
#include <Wire.h>
#include "sfm3200.h"


/*
 begin SFM3200
 */
void sfm3200::begin()
{	
	//ensure a 100ms delay since power-on (required)
	delay(100);

    //read serial number
	flowReadSN();

	//write measurement command
	//this command will 
    flowSendMeasCmd();		
	delayMicroseconds(500);

	//read 1st measurement data and discard (1st reading is assumed invalid)
  	flowReadMeas();				      
    delayMicroseconds(500);

    //read 2nd measure data, it should be valid
    flowReadMeas();

    //SFM3200
    //FYI - chip will remain in the measurement command mode until overwritten by another command
}


/*
send measurement command
*/
void sfm3200::flowSendMeasCmd(){
	Wire.beginTransmission(FLOW_I2C_ADDR); 	// send address, ACK
	Wire.write(FLOW_CMD_START>>8);      	// write 16-bit command	ACK
	Wire.write(FLOW_CMD_START & 0xFF);      //						ACK
	Wire.endTransmission();				
}


/* 
 read flow measurement
 assumes measurement command was previously sent
*/
uint16_t sfm3200::flowReadMeas()
{
    uint8_t meas[2];							//measurement data array
    uint16_t * ptr_meas = (uint16_t) meas;		//16-bit pointer
	uint8_t crc;								
	
    //flowSendMeasCmd();							//optionally, comment this line if only reading forever
    //delayMicroseconds(200);

	Wire.requestFrom(FLOW_I2C_ADDR, 3);  		//send addr and request 3-bytes, ACK
	meas[1] = Wire.read();					    //read MSB, ACK
	meas[0] = Wire.read(); 					    //read LSB, ACK
	crc 	= Wire.read();						//read CRC, NACK
	//Wire.endTransmission();
	delayMicroseconds(200);			  	

	this->measurement =  *ptr_meas;				//pointer indirection to 16-bit value
	return  this->measurement;
}


/*
Read the 4-byte serial number
*/
uint32_t sfm3200::flowReadSN()
{
	
	uint8_t crcMSB, crcLSB;
  	uint8_t sn[4];							//serial number array
  	uint32_t *ptr_sn = (uint32_t) sn;		//32-bit pointer
  	uint32_t retval = 0;

  	//COMMAND
	Wire.beginTransmission(FLOW_I2C_ADDR);	//send addr, ACK
	Wire.write(FLOW_CMD_READ_SN>>8);		//write command MSB, ACK
	Wire.write(FLOW_CMD_READ_SN & 0xFF);	//  "   "		LSB, ACK
	Wire.endTransmission();
	delayMicroseconds(500);
	
	//DATA
	Wire.requestFrom(FLOW_I2C_ADDR, 6);  	//send addr and request 6-bytes (two frames of 8 bits)
	sn[3]   = Wire.read();    				//MSB, ACK
 	sn[2]   = Wire.read();    				//   , ACK
	crcMSB  = Wire.read();    				//CRC MSB, ACK
	sn[1]   = Wire.read();    				//   , ACK
	sn[0]   = Wire.read();    				//LSB, ACK
	crcLSB  = Wire.read();    				//CRC LSB, NACK
	//Wire.endTransmission();
    //delayMicroseconds(500);

    this->sernum = *ptr_sn;				    //save to this object
	return this->sernum;
}


/*
soft reset command
*/
void sfm3200::flowReset()
{
	Wire.beginTransmission(FLOW_I2C_ADDR);	//send addr, ACK
	Wire.write(FLOW_CMD_RESET>>8);			//write command MSB, ACK
	Wire.write(FLOW_CMD_RESET & 0xFF);		//  "   "		LSB, ACK
	Wire.endTransmission();				    //
    //delayMicroseconds(500);
}


