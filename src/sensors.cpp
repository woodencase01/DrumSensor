/*
  -=-=-=- Sensors -=-=-=-
  0: Waiting for decay
  1: First read
  2: Successive readings
  3: Final value
*/
#include "sensors.h"
#include "avdweb_AnalogReadFast.h"

const int numSensors = 5;
const int sensorspin[numSensors] = {sensor1, 1, 2, 3, 4};

const int readtimes = 10; // Number of times the sensor is read
#ifdef DEBUG
int sensor1val[readtimes];
#endif
const int mindecay = 2;  // Maximum decay time (ms)
const int maxdecay = 15; // Maximum decay time (ms)

int maxsensorvalue[] = {0, 0, 0, 0, 0};
byte sensorstroke[] = {0, 0, 0, 0, 0};
byte sensorstate[] = {0, 0, 0, 0, 0};
byte sensorreadtimes[] = {0, 0, 0, 0, 0};

unsigned long sensorsdecayend[numSensors];
unsigned long startread[numSensors];

void manageSensors()
{

  unsigned long now = millis();

  // First check and adjust individual sensors state
  for (int i = 0; i < nbPadSensors; i++)
  {

    if (sensorstate[i] == 0)
    { //0: Waiting for decay
      if (sensorsdecayend[i] < now)
        sensorstate[i] = 1;
    }

    /*
    if (sensorstate[i] == 1)
    { //1: First read : managed in read sensors
    }*/

    if (sensorstate[i] == 2)
    { //2: Successive readings
      if (sensorreadtimes[i] >= readtimes)
        sensorstate[i] = 3;
    }
  }

  // Then manage the strokes
  if ((padType == 0 || padType == 1 || padType == 3) && sensorstate[0] == 3) // Single zone pad or cymbal
  {

    sensorstroke[0] = map(maxsensorvalue[0], 0, 200, 0, 200); //upperThreshold[0] * 4

    if (sensorstroke[0] > 0) // Avoid sending an empty signal
    {
      sendStroke(sensorstroke[0], 100);

#ifdef DEBUG
      for (byte i = 0; i < readtimes; i++)
      {
        Serial.print(sensor1val[i]);
        Serial.print(", ");
      }
      Serial.println("");
#endif
    }

    sensorsdecayend[0] = map(maxsensorvalue[0], 0, 1023, mindecay, maxdecay) + now;

    maxsensorvalue[0] = 0;
    sensorstate[0] = 0;
    sensorreadtimes[0] = 0;
  }

  if ((padType == 2 || padType == 4) && (sensorstate[0] == 3 && sensorstate[1] == 3 && sensorstate[2] == 3)) // Multi zone pad or cymbal
  {
    //To do
  }

  if ((padType == 1 || padType == 2) && sensorstate[4] == 3) // Single or multi zone pad
  {
    sensorstroke[4] = map(maxsensorvalue[4], 0, 1023, 0, 200);
    sensorsdecayend[4] = map(maxsensorvalue[4], 0, 1023, mindecay, maxdecay) + now;

    if (sensorstroke[4] > 0)
    {
      sendStroke(sensorstroke[4], 101);
    }

    maxsensorvalue[4] = 0;
    sensorstate[4] = 0;
  }
}

void readSensors()
{

  unsigned long now = millis();

  for (int i = 0; i < nbPadSensors; i++)
  {

    if (sensorstate[i] == 1)
    { //1: First read
      int val = analogReadFast(sensorspin[i]);
      if (val > threshold[i])
      {
#ifdef DEBUG
        sensor1val[0] = val;
#endif

        if (val > maxsensorvalue[i])
        {
          maxsensorvalue[i] = val;

          sensorreadtimes[i]++;

          sensorstate[i] = 2;
          startread[i] = now;
        }
      }
    }

    if (sensorstate[i] == 2)
    {
      //int val = analogRead(sensorspin[i]);
      analogReadFast(sensorspin[i]); //Flush value
      int val = analogReadFast(sensorspin[i]);

#ifdef DEBUG
      sensor1val[sensorreadtimes[i]] = val;
#endif

      if (val > maxsensorvalue[i])
        maxsensorvalue[i] = val;

      sensorreadtimes[i]++;
    }
  }
}
