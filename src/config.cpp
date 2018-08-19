/*
  -=-=-=- Configuration -=-=-=-
  byte 0: Pad type [0: No configuration set, 1: Single Zone pad, 2: Multi Zone Pad, 3: Single Zone Cymbal, 4: Multi Zone Cymbal]
  *1: Single Zone pad : Bass drum
  *2: Multi Zone Pad : Snare, Toms
  *3: Single Zone Cymbal : Splash
  *4: Multi Zone Cymbal : Ride, Crash, etc
  byte 1: Sensor 1 threshold [Pad sensor 1]
  byte 2: Sensor 2 threshold [Pad sensor 2]
  byte 3: Sensor 3 threshold [Pad sensor 3]
  byte 5: Sensor 4 threshold [Rim sensor, Cymbal choke]  
  byte 4: Sensor 5 threshold [Frame sensor]
  byte 6: Center threshold [0 - 100]
  byte 7: Pad ID [0 - 255]
*/

#include "config.h"
#include <EEPROM.h>

#define DEBUG

byte threshold[5];
byte padType;
int nbPadSensors;
byte centerThreshold;
byte padid;

const int numberBytes = 8;
const byte defaultConfig[numberBytes] = {0, 2, 2, 2, 2, 2, 10, 0};

void configureSensors()
{
  if (EEPROM.read(0) > 4) // Only configure if unchanged
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
  case 0:
    nbPadSensors = 1;
    break;
  case 1:
    nbPadSensors = 1;
    break;
  case 2:
    nbPadSensors = 3;
    break;
  case 3:
    nbPadSensors = 1;
    break;
  case 4:
    nbPadSensors = 3;
    break;
  default:
    nbPadSensors = 1;
    break;
  }

  for (int i = 1; i <= 5; i++)
  {
    threshold[i-1] = EEPROM.read(i);
  }

  centerThreshold = EEPROM.read(6);

  padid = EEPROM.read(7);
}