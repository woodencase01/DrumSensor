#include "drumSensor.h"

/**
 * @file drumSensor.cpp
 * @author Philippe Carrier
 */

DrumSensor::DrumSensor(byte sensorPin)
{
	_sensorPin = sensorPin;
	// initialize variables
}

void DrumSensor::readSensor()
{
	analogReadFast(_sensorspin); //flush first value, wait ~25µs
	_val = analogReadFast(_sensorspin);
	if (_val > _minThreshold)
	{
		_isStruck = true;
	}
	else
	{
		_isStruck = false;
	}
}

bool isStruck()
{
	return _isStruck
}

uint16_t getSensorValue()
{
	return _val
}

void setMinThreashold(byte minThreshold)
{
	_minThreshold = minThreshold;
}

void setMaxThreashold(byte maxThreshold;)
{
	_maxThreshold = maxThreshold;
}