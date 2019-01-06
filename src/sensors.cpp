/*
  -=-=-=- Sensors -=-=-=-
  0: Waiting for decay
  1: First read
  2: Successive readings
  3: Final value
*/
#include <Arduino.h>
#include "communication.h"
#include "sensors.h"
#include "settings.h"

const int readtimes = 75; // Number of times the sensor is read

#ifdef DEBUG1
uint16_t sensorVal[5][readtimes];
#endif

#ifdef DEBUG
uint32_t struckTimeMicro;
uint32_t sentTimeMicro;
#endif

uint16_t maxsensorvalue[] = {0, 0, 0, 0, 0};
byte sensorstroke[] = {0, 0, 0, 0, 0};
byte padState = 0;
bool isStruck = false;
byte sensorReadTimes = 0;
uint32_t sensorsDecayEnd;
uint32_t startRead;

uint16_t calibratingRatio[2]; // Only compairing sensors 2 and 3 over sensor 1, 2 ratios are needed
uint8_t calibratingCycle;
bool calibration;

void manageSensors()
{

  unsigned long now = millis();

  if (padState == 0) //0: Waiting for decay
  {
    if (sensorsDecayEnd < now)
      setPadState(1);
  }

  /* +----------------------------------
     | Single zone pad or cymbal
     +---------------------------------- */
  if ((padType == 0 || padType == 1 || padType == 3) && padState == 3)
  {

    sensorstroke[0] = map(maxsensorvalue[0], 0, 200, 0, 200); //upperThreshold[0] * 4

    if (sensorstroke[0] > 0) // Avoid sending an empty signal
    {
      sendStroke(sensorstroke[0], 100);

#ifdef DEBUG
      sentTimeMicro = micros();
      Serial.print(F("Time: "));
      Serial.println(sentTimeMicro - struckTimeMicro);
#endif
    }

    sensorsDecayEnd = map(sensorstroke[0], 0, 200, minDecay, maxDecay) + now;

    resetSensor();
  }
  /* +----------------------------------
     | Multi zone pad or cymbal
     +---------------------------------- */
  if ((padType == 2 || padType == 4) && padState == 3)
  {
    for (byte i = 0; i < nbPadSensors; i++)
    {
      sensorstroke[i] = map(maxsensorvalue[i], 0, sensor[i].getMaxThreshold() * 4, 1, 200);
    }

    uint16_t averageStroke = (sensorstroke[0] + sensorstroke[1] + sensorstroke[2]) / 3;

#ifdef DEBUG
    Serial.print(F("Pad max values: "));
    for (byte i = 0; i < nbPadSensors; i++)
    {
      Serial.print(maxsensorvalue[i]);
      Serial.print(F(", "));
    }
    Serial.println();
    Serial.print(F("Pad stroke: "));
    for (byte i = 0; i < nbPadSensors; i++)
    {
      Serial.print(sensorstroke[i]);
      Serial.print(F(", "));
    }
    Serial.println();
#endif

    sendStroke(averageStroke, 100);

#ifdef DEBUG
    sentTimeMicro = micros();
    Serial.print(F("Time: "));
    Serial.println(sentTimeMicro - struckTimeMicro);
#endif

    sensorsDecayEnd = map(averageStroke, 0, 200, minDecay, maxDecay) + now;

    if (calibration)
    {
      for (byte i = 0; i < 2; i++) // Get all ratio based on sensor 1
      {
        calibratingRatio[i] += maxsensorvalue[0] * 64 / maxsensorvalue[i + 1];
      }
#ifdef DEBUG
      Serial.print(F("Calibration: "));
      Serial.print(calibratingCycle);
      Serial.print(F(", "));
      Serial.print(calibratingRatio[0]);
      Serial.print(F(", "));
      Serial.print(calibratingRatio[1]);
      Serial.println();
#endif
      calibratingCycle++;

      if (calibratingCycle == calibratingValues) // Last value is reached, time to calculate
      {
        for (byte i = 0; i < 2; i++)
        {
          // Calculate the new threshold
          calibratingRatio[i] = (uint32_t)calibratingValues * sensor[i + 1].getMaxThreshold() * 64 / calibratingRatio[i];
#ifdef DEBUG
          Serial.print(F("New threshold for sensor "));
          Serial.print(i + 1);
          Serial.print(F("is: "));
          Serial.println(calibratingRatio[i]);
#endif
          // Set the new threshold value
          sensor[i + 1].setMaxThreshold(calibratingRatio[i]);
        }
        calibratingCycle = 0;
        for (byte i = 0; i < 2; i++)
        {
          calibratingRatio[i] = 0;
        }
        setCalibration(false);
      }
    }

    resetSensor();
  }
}

void readSensors()
{

  unsigned long now = millis();

  sensorAnalogRead();

  /* +----------------------------------
   | First read
   | First value will be stored as state change
   +---------------------------------- */
  if (padState == 1)
  {
    for (int i = 0; i < nbPadSensors; i++) // Check all sensors for value over threshold
    {
      if (sensor[i].isStruck())
      {
        isStruck = true;
#ifdef DEBUG1
        Serial.print(F("Pad struck: "));
        Serial.println(i);
#endif
      }
    }

    if (sensor[3].isStruck()) // Check rim sensor for value over threshold
    {
      isStruck = true;
#ifdef DEBUG1
      Serial.print(F("Pad struck: "));
      Serial.println(3);
#endif
    }

    if (isStruck) // Change state if anything has beed flagged as struck
    {
      setPadState(2);
      startRead = now;
#ifdef DEBUG
      struckTimeMicro = micros();
#endif
    }
  }

  /* +----------------------------------
   | Read values until the number of times is reached
   +---------------------------------- */
  if (padState == 2)
  {
    registerValues();
    sensorReadTimes++;

    if (sensorReadTimes == (readtimes)) // Value won't be read oncec equal, no overflow in array will occur
    {
      setPadState(3);
    }
  }
}

void resetSensor()
{
  for (byte i = 0; i < nbPadSensors; i++)
  {
    maxsensorvalue[i] = 0;
  }
  maxsensorvalue[3] = 0;
  maxsensorvalue[4] = 0;

  setPadState(0);

  sensorReadTimes = 0;
  isStruck = false;

#ifdef DEBUG1
  for (byte j = 0; j < 5; j++)
  {
    for (byte i = 0; i < readtimes; i++)
    {
      Serial.print(sensorVal[j][i]);
      Serial.print(", ");
    }
    Serial.println("");
  }
#endif
}

void setPadState(byte state)
{
  padState = state;

#ifdef DEBUG1
  Serial.print(F("Pad state: "));
  Serial.println(state);
#endif
}

void sensorAnalogRead()
{
  for (int i = 0; i < nbPadSensors; i++) // Read sensors values
  {
    sensor[i].readSensor();
  }

  if (rimPad) // Read rim sensor if required
  {
    sensor[3].readSensor();
  }

  sensor[4].readSensor();
}

void registerValues()
{
  uint16_t currenSensorValue;

  for (int i = 0; i < nbPadSensors; i++) // Register pad sensor value
  {
    currenSensorValue = sensor[i].getSensorValue();
#ifdef DEBUG1 // Register the struck value in the table for debugging
    sensorVal[i][sensorReadTimes] = sensor[i].getSensorValue();
#endif
    if (currenSensorValue > maxsensorvalue[i])
    {
      maxsensorvalue[i] = currenSensorValue;
    }
  }

  if (rimPad) // Read rim sensor if required
  {
    currenSensorValue = sensor[3].getSensorValue();
#ifdef DEBUG1 // Register the struck value in the table for debugging
    sensorVal[3][sensorReadTimes] = sensor[3].getSensorValue();
#endif
    if (currenSensorValue > maxsensorvalue[3])
    {
      maxsensorvalue[3] = currenSensorValue;
    }
  }

  currenSensorValue = sensor[4].getSensorValue(); // Read frame sensor
#ifdef DEBUG1                                     // Register the struck value in the table for debugging
  sensorVal[4][sensorReadTimes] = sensor[4].getSensorValue();
#endif
  if (currenSensorValue > maxsensorvalue[4])
  {
    maxsensorvalue[4] = currenSensorValue;
  }
}

void setCalibration(bool state)
{
  calibration = state;
}