/*
  -=-=-=- Configuration -=-=-=-
  byte 0: Pad type [0: No configuration set, 1: Single Zone pad, 2: Multi Zone Pad, 3: Single Zone Cymbal, 4: Multi Zone Cymbal]
  *1: Single Zone pad : Bass drum
  *2: Multi Zone Pad : Snare, Toms
  *3: Single Zone Cymbal : Splash
  *4: Multi Zone Cymbal : Ride, Crash, etc
  byte 01: Sensor 1 lower threshold [Pad sensor 1]
  byte 02: Sensor 2 lower threshold [Pad sensor 2]
  byte 03: Sensor 3 lower threshold [Pad sensor 3]
  byte 04: Sensor 4 lower threshold [Rim sensor, Cymbal choke]  
  byte 05: Sensor 5 lower threshold [Frame sensor]
  byte 06: Center threshold [0 - 100]
  byte 07: Pad ID [0 - 255]
  byte 08: Sensor 1 upper threshold [Pad sensor 1]
  byte 09: Sensor 2 upper threshold [Pad sensor 2]
  byte 10: Sensor 3 upper threshold [Pad sensor 3]
  byte 11: Sensor 4 upper threshold [Rim sensor, Cymbal choke]  
  byte 12: Sensor 5 upper threshold [Frame sensor]
*/

#include "config.h"
#include <EEPROM.h>

byte threshold[5];
byte upperThreshold[5];
byte padType;
int nbPadSensors;
byte centerThreshold;
byte padid;

const byte defaultConfig[] = {0, 2, 2, 2, 2, 2, 10, 0, 43, 255, 255, 255, 255}; 
// Upper threshold needs to be < 255. They are multiplied by 4 in the sensor loop to match the 10 bits ADC values

byte numberBytes = sizeof(defaultConfig);

void configureSensors()
{
  if (EEPROM.read(0) > 0) // Only configure if unchanged
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

 for (int i = 8; i <= 12; i++)
  {
    upperThreshold[i-8] = EEPROM.read(i);
  }
}

void configurePins(){
pinMode(readCom,INPUT);
pinMode(activeCom,OUTPUT);
}