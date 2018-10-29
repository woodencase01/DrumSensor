#ifndef _DRUMSENSOR_H_
#define _DRUMSENSOR_H_

/**
 * @file DrumSensor.h
 * @author Philippe Carrier
 */

#include <Arduino.h>
#include "avdweb_AnalogReadFast.h"

class DrumSensor {

	public:

		DrumSensor(byte sensorPin);

		void readSensor();
		uint16_t getSensorValue();
		void setMinThreashold(byte minthreshold);
		void setMaxThreashold(byte maxthreshold);

	private:

		//	VARIABLES
		byte _sensorPin;
		byte _minThreshold;
		byte _maxThreshold;
		bool _isStruck;
		uint16_t _val;

		//	CONSTANTS

		//	OBJECTS

};

#endif