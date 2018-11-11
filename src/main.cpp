/*

  Drum Sensor - A serial drum trigger

  -=-=-=- Flow chart -=-=-=-

  1. Read and send buffer

  2. Check inputs

  3. Send new data

  -=-=-=- General features -=-=-=-
  Prioritize I/O for low latency
  Parallel reading for sensors
*/

#include <Arduino.h>
#include "sensors.h"
#include "settings.h"
#include "communication.h"
#include "config.h"

void setup()
{

  configureSensors(); // Make sure EEPROM is configured correctly after first flash
  readConfig();       // Assign variables based on EEPROM
  configurePins();

  Serial.begin(115200);

#ifdef DEBUG
  listConfig();
#endif
  sensorAnalogRead(); // Prepare the ADC and avoid false triggers at start
}

void loop()
{
  manageSensors();
  readSensors();
  readBuffer();
}
