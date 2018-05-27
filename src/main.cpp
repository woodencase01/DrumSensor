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
#include "config.h"
#include "communication.h"

#define DEBUG

unsigned long sensorsdecayend[] = {0, 0, 0, 0, 0};
unsigned long startread[] = {0, 0, 0, 0, 0};

#ifdef DEBUG
int numberreads[] = {0, 0, 0, 0, 0};
#endif

void setup() {

  configureSensors();
  readConfig();

  Serial.begin(9600);

#ifdef DEBUG
  Serial.print("Pad type: ");
  Serial.println(padType);
  Serial.print("Pad thresholds: ");
  for (int i = 0; i < numberPads; i++) {
    Serial.print(threshold[i]);
    Serial.print(", ");
  }
  Serial.println;
  Serial.print("Center threshold: ");
  Serial.println(centerThreshold);
  Serial.println("Drum Sensor Ready");
#endif
}

void loop() {

  readBuffer();

  if (senddata) {
    sendBuffer();
  }

  manageSensors();
  readSensors();
}
