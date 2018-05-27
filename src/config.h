/*
  -=-=-=- Configuration -=-=-=-
  byte 0: Pad type [0: No configuration set, 1: Single Zone pad, 2: Multi Zone Pad, 3: Single Zone Cymbal, 4: Multi Zone Cymbal]
  byte 1: Sensor 1 threshold [Pad sensor 1, Cymbal edge]
  byte 2: Sensor 2 threshold [Pad sensor 2, Cymbal bow]
  byte 3: Sensor 3 threshold [Pad sensor 3, Cymbal bell]
  byte 4: Sensor 4 threshold [Frame sensor]
  byte 5: Sensor 5 threshold [Pad Rim sensor, Cymbal not used]
  byte 6: Center threshold [0 - 100]
*/
#include <Arduino.h>

byte padType;
int threshold[5];
const int numberPads = 5;
const int numberBytes = 7;
const int defaultConfig[] = {1, 2, 2, 2, 2, 2, 10};
byte centerThreshold;

void configureSensors();
void readConfig();
