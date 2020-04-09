/**

sfm3200.h

@author: Will Patton: https://github.com/willpatton
@date: April 7, 2020
@license: MIT

*/
#ifndef SFM3200_FLOW_SENSOR_H
#define SFM3200_FLOW_SENSOR_H

#define 	FLOW_I2C_ADDR		0x40			//SFM3200 I2C address (7 bit) where 0x40 is address 64
#define 	FLOW_CMD_START		0x1000			//measurement command
#define 	FLOW_CMD_RESET		0x2000			//reset command
#define 	FLOW_CMD_READ_SN	0x31AE			//read SN command


class sfm3200 {
	public:

		void		    begin();
		void 			flowSendMeasCmd();	    //send measurement command, but does not read.
		uint16_t 	    flowReadMeas();			// 
		uint32_t		flowReadSN();          	//reads the SN
		void 		    flowReset();

	private:
		volatile uint16_t measurement;					//hold the latest flow measurement
		volatile uint32_t sernum;      					//serial number

};//end class

#endif //SFM3200_FLOW_SENSOR_H
