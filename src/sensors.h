//sensors.h

#ifndef _SENSORS_H
#define _SENSORS_H

/*
  -=-=-=- Sensors -=-=-=-
  0: Waiting for decay
  1: First read
  2: Successive readings
  3: Final value
*/

void manageSensors();
void readSensors();
void resetSensor();
void setPadState(byte state);
void sensorAnalogRead();
void registerValues();
void setCalibration(bool state);

#endif