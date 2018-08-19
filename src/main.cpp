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

void setup()
{

  configureSensors();
  readConfig();

  Serial.begin(115200);

#ifdef DEBUG
  Serial.print("Pad type: ");
  Serial.println(padType);
  Serial.print("Pad thresholds: ");
  for (int i = 0; i < nbPadSensors; i++)
  {
    Serial.print(threshold[i]);
    Serial.print(", ");
  }
  Serial.println();
  Serial.print("Center threshold: ");
  Serial.println(centerThreshold);
  Serial.println("Drum Sensor Ready");
#endif
}

void loop()
{
  readBuffer();
  manageSensors();
  readSensors();
}
