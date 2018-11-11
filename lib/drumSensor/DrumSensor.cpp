#include "DrumSensor.h"
#include "avdweb_AnalogReadFast.h"

/**
 * @file DrumSensor.cpp
 * @author Philippe Carrier
 */

DrumSensor::DrumSensor()
{
}

DrumSensor::DrumSensor(byte sensorPin)
{
	_sensorPin = sensorPin;
}

void DrumSensor::readSensor()
{
	analogReadFast(_sensorPin); //flush first value, wait ~25µs
	_val = analogReadFast(_sensorPin);
	if (_val > _minThreshold)
	{
		_isStruck = true;
	}
	else
	{
		_isStruck = false;
	}
}

bool DrumSensor::isStruck()
{
	return _isStruck;
}

uint16_t DrumSensor::getSensorValue()
{
	return _val;
}

void DrumSensor::setMinThreshold(byte minThreshold)
{
	_minThreshold = minThreshold;
}

void DrumSensor::setMaxThreshold(byte maxThreshold)
{
	_maxThreshold = maxThreshold;
}

byte DrumSensor::getMinThreshold()
{
	return _minThreshold;
}

byte DrumSensor::getMaxThreshold()
{
	return _maxThreshold;
}
