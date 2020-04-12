/**

sfm3200.h

	@author 	Will Patton: https://github.com/willpatton
	@date 		April 7, 2020
	@license 	MIT

*/
#ifndef SFM3200_FLOW_SENSOR_H
#define SFM3200_FLOW_SENSOR_H

//ADDRESS
#define 	SFM3200_ADDR	0x40				//SFM3200 I2C address (7 bit) where 0x40 is address 64

//COMMANDS
#define 	CMD_MEASURE		0x1000				//measurement command
#define 	CMD_RESET		0x2000				//reset command
#define 	CMD_SERNUM		0x31AE				//read serial number command


class sfm3200 {

	public:
		void		    begin();
		uint16_t 	    readFlow();				//send measurement command and read back data
		uint32_t		readSernum();          	//send SN cmd and read back data
		void 		    reset();

	private:
		void            sendMeasCmd();			//send measurement command 
		volatile uint16_t flow;					//hold the latest flow measurement
		volatile uint32_t sernum;      			//serial number

};//end class

#endif //SFM3200_FLOW_SENSOR_H
