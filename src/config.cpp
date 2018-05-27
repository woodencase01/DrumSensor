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
#include <EEPROM.h>
#include "config.h"

void configureSensors() {
  if (EEPROM.read(0) == 0) {
    for (int i = 0; i < numberBytes; i++)
      EEPROM.update(i, defaultConfig[i]);
  }
}

void readConfig() {
  padType = EEPROM.read(0);

  for (int i = 0; i < numberPads; i++) {
    threshold[i] = EEPROM.read(i + 1);
  }

  centerThreshold = EEPROM.read(6);
}
