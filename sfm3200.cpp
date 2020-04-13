/**
	sfm3200.cpp

	@author: Will Patton: https://github.com/willpatton
	@date: April 7, 2020
	@license: MIT
*/

#include "Arduino.h"
#include <Wire.h>
#include "sfm3200.h"


/*!
	@brief  Begin SFM3200
 */
void sfm3200::begin()
{	
	//ensure a 100ms delay since power-on
	//delay(100);

    //read serial number
	readSernum();
    delayMicroseconds(500);

	//send measurement cmd, then read 1st measurement data and discard (1st reading is assumed invalid)
  	readFlow();				      
    delayMicroseconds(500);

    //send measurement cmd, then read 2nd measure data, it should be valid
    readFlow();

    //SFM3200
    //FYI - chip will remain in the measurement command mode until overwritten by another command
}


/*!
	 @brief  Send the 16-bit measurement command, only
*/
void sfm3200::sendMeasCmd(){
	Wire.beginTransmission((uint8_t)SFM3200_ADDR); 	// send 7-bit address + write bit=0, ACK
	Wire.write((uint8_t)(CMD_MEASURE >> 8));      	// write command MSB, ACK
	Wire.write((uint8_t)CMD_MEASURE);     			//	  "		"    LSB, ACK
	Wire.endTransmission();				 
}


/*! 
 	@brief  Read the 16-bit flow measurement
	assumes measurement command was previously sent
 	this chip will likely return 0xFFFF if its locked-up or not ready(invalid).
*/
uint16_t sfm3200::readFlow()
{
    uint16_t flow;							//measurement: flow data.
	uint8_t crc;							//data CRC
	
	//COMMAND
    sendMeasCmd();							
    delayMicroseconds(200);					//required.  120us too fast and occassionally put out spikes of 0xFFFF

    //DATA
	Wire.requestFrom(SFM3200_ADDR, 3);  	//send 7-bit addr + read bit=1 and request 3-bytes, ACK
	flow = (uint16_t)(Wire.read() << 8 | (Wire.read() )); 	//read 16-bit: MSB +ACK, then LSB + ACK
	crc = (uint8_t)Wire.read();								//read CRC, NACK	
	//CRC - discarded  	

	return flow;
}


/*!
	 @brief  Read the 32-bit serial number
*/
uint32_t sfm3200::readSernum()
{
  	uint32_t sernum;						//serial number
  	uint8_t crcH, crcL;						//CRC for high and low 16-bit word read

  	//COMMAND
	Wire.beginTransmission((uint8_t)SFM3200_ADDR);	//send 7-bit addr + write bit=0, ACK
	Wire.write((uint8_t)(CMD_SERNUM >> 8));			//write command MSB, ACK
	Wire.write((uint8_t)CMD_SERNUM & 0xFF);		    //  "   "		LSB, ACK
	Wire.endTransmission();
	delayMicroseconds(500);
	
	//DATA
	Wire.requestFrom(SFM3200_ADDR, 6);  	//send 7-bit addr + read bit=1 and request 6-bytes (2 frames of 3-bytes each)
	sernum = (uint32_t)(Wire.read() << 8 | (Wire.read() ));  	//MSB BYTE3 +ACK, BYTE2 +ACK 
	crcH = (uint8_t)Wire.read();    							//CRC high word, ACK
	sernum  = (uint32_t)(sernum << 8 | Wire.read() << 8 | (Wire.read() )); 	//BYTE1 +ACK, LSB BYTE0 +ACK 
	crcL  = (uint8_t)Wire.read();    							//CRC low word, NACK
	//CRC - discarded

	return sernum;
}


/*!
	@brief  Send the 16-bit soft reset command
	this command only flushes the status registers
	this command will not reset the chip if it is locked-up
*/
void sfm3200::reset()
{
	Wire.beginTransmission((uint8_t)SFM3200_ADDR);	//send 7-bit addr + write bit=0, ACK
	Wire.write((uint8_t)(CMD_RESET >> 8));			//write command MSB, ACK
	Wire.write((uint8_t)CMD_RESET & 0xFF);			//  "   "		LSB, ACK
	Wire.endTransmission();				    
}


