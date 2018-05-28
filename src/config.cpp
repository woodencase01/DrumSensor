/*
  -=-=-=- Configuration -=-=-=-
  byte 0: Pad type [0: No configuration set, 1: Single Zone pad, 2: Multi Zone Pad, 3: Single Zone Cymbal, 4: Multi Zone Cymbal]
  *1: Single Zone pad : Bass drum
  *2: Multi Zone Pad : Snare, Toms
  *3: Single Zone Cymbal : Splash
  *4: Multi Zone Cymbal : Ride, Crash, etc
  byte 1: Sensor 1 threshold [Pad sensor 1, Cymbal edge]
  byte 2: Sensor 2 threshold [Pad sensor 2, Cymbal bow]
  byte 3: Sensor 3 threshold [Pad sensor 3, Cymbal bell]
  byte 4: Sensor 4 threshold [Frame sensor]
  byte 5: Sensor 5 threshold [Pad Rim sensor, Cymbal not used]
  byte 6: Center threshold [0 - 100]
*/
#include <Arduino.h>
#include <EEPROM.h>
#include "config.h"

byte threshold[5];
byte padType;
byte nbPadSensors;
const int numberBytes = 7;
const int defaultConfig[numberBytes] = {0, 2, 2, 2, 2, 2, 10};

void configureSensors()
{
  if (EEPROM.read(0) == 0) // Only configure if unchanged
  {
    for (int i = 0; i < numberBytes; i++)
      EEPROM.update(i, defaultConfig[i]);
  }
}

void readConfig()
{
  padType = EEPROM.read(0);

  switch (padType)
  {
  case 1:
    nbPadSensors = 1;
  case 2:
    nbPadSensors = 3;
  case 3:
    nbPadSensors = 1;
  case 4:
    nbPadSensors = 3;
    break;
  default:
  }

  for (int i = 0; i <= 5; i++)
  {
    threshold[i] = EEPROM.read(i + 1);
  }

  centerThreshold = EEPROM.read(6);
}

void initializeModule()
{
  bool initializeDone = false;
  uint32_t now = millis();

  while (!initializeDone)
  {
    int blankData[] = {0, 0};
    Serial.write(blankData, 2); // send a blank message
    delay(25);

    int timePassed = 0;
    while (Serial.available() < 2 && timePassed < 50)
    {
      timePassed++;
    }

    initializeDone = true;
  }
}