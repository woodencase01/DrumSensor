/*
  -=-=-=- Sensors -=-=-=-
  0: Waiting for decay
  1: First read
  2: Successive readings
  3: Final value
*/
#include "sensors.h"
#include "settings.h"

const int readtimes = 75; // Number of times the sensor is read

#ifdef DEBUG
uint16_t sensorVal[5][readtimes];
#endif

uint16_t maxsensorvalue[] = {0, 0, 0, 0, 0};
byte sensorstroke[] = {0, 0, 0, 0, 0};
byte padState = 0;
bool isStruck = false;
byte sensorReadTimes = 0;
unsigned long sensorsDecayEnd = 0;
unsigned long startRead = 0;

void manageSensors()
{

  unsigned long now = millis();

  // First check and adjust individual sensors state
  if (padState == 0)
  { //0: Waiting for decay
    if (sensorsDecayEnd < now)
      padState = 1;
  }

  /*
    if (padState == 1)
    { //1: First read : managed in read sensors
    }*/

  if (padState == 2)
  { //2: Successive readings
    if (sensorReadTimes == readtimes)
      padState = 3;
  }

  // Then manage the strokes
  if ((padType == 0 || padType == 1 || padType == 3) && padState == 3) // Single zone pad or cymbal
  {

    sensorstroke[0] = map(maxsensorvalue[0], 0, 200, 0, 200); //upperThreshold[0] * 4

    if (sensorstroke[0] > 0) // Avoid sending an empty signal
    {
      sendStroke(sensorstroke[0], 100);

#ifdef DEBUG
      for (byte i = 0; i < readtimes; i++)
      {
        Serial.print(sensorVal[0][i]);
        Serial.print(", ");
      }
      Serial.println("");
#endif
    }

    sensorsDecayEnd = map(maxsensorvalue[0], 0, 1023, minDecay, maxDecay) + now;

    maxsensorvalue[0] = 0;
    padState = 0;
    sensorReadTimes = 0;
    isStruck = false;
  }

  if ((padType == 2 || padType == 4) && padState == 3) // Multi zone pad or cymbal
  {
    //To do
  }
}

void readSensors()
{

  unsigned long now = millis();

  for (int i = 0; i < nbPadSensors; i++) // Read sensors values
  {
    sensor[i].readSensor();
  }

  if (rimPad) // Read rim sensor if required
  {
    sensor[4].readSensor();
  }

  sensor[5].readSensor(); // Read frame sensor

  if (padState == 1) //1: First read
  {
    for (int i = 0; i < nbPadSensors; i++) // Check all sensors for value over threshold
    {
      if (sensor[i].isStruck())
      {
        isStruck = true;
      }
    }

    if (sensor[4].isStruck()) // Check rim sensor for value over threshold
    {
      isStruck = true;
    }

    if (isStruck) // Change state if anything has beed flagged as struck
    {
      padState = 2;
      startRead = now;
    }
  }

  if (padState == 2) // Read values until the number of times is reached
  {
#ifdef DEBUG // Register the struck value in the table for debugging
    for (int i = 0; i < 5; i++)
    {
      sensorVal[i][sensorReadTimes] = sensor[i].getSensorValue();
    }
#endif

    for (int i = 0; i < nbPadSensors; i++) // Register pad sensor value
    {
      uint16_t currenSensorValue = sensor[i].getSensorValue();
      if (currenSensorValue > maxsensorvalue[i])
      {
        maxsensorvalue[i] = currenSensorValue;
      }
    }

    if (rimPad) // Read rim sensor if required
    {
      uint16_t currenSensorValue = sensor[4].getSensorValue();
      if (currenSensorValue > maxsensorvalue[4])
      {
        maxsensorvalue[4] = currenSensorValue;
      }
    }

    uint16_t currenSensorValue = sensor[5].getSensorValue(); // Read frame sensor
    if (currenSensorValue > maxsensorvalue[5])
    {
      maxsensorvalue[5] = currenSensorValue;
    }

    sensorReadTimes++;
  }
}
