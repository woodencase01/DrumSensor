//sensors.h

#ifndef _sensors_h
#define _sensors_h

#include <Arduino.h>
#include "communication.h"

/*
  -=-=-=- Sensors -=-=-=-
  0: Waiting for decay
  1: First read
  2: Successive readings
  3: Final value
*/

void manageSensors(void);
void readSensors(void);

#endif