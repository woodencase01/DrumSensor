#ifndef _DRUMSENSOR_H_
#define _DRUMSENSOR_H_

/**
 * @file DrumSensor.h
 * @author Philippe Carrier
 */

#include <Arduino.h>

class DrumSensor
{

  public:
	DrumSensor();
	DrumSensor(byte sensorPin);

	void readSensor();
	uint16_t getSensorValue();
	void setMinThreshold(byte minthreshold);
	void setMaxThreshold(byte maxthreshold);
	byte getMinThreshold();
	byte getMaxThreshold();
	bool isStruck();

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