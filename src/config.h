//config.h

#ifndef _config_h
#define _config_h

#include <Arduino.h>
#include <EEPROM.h>
#include <EEWrap.h>

void configureSensors(void);

void initializeModule(void);

void readConfig(void);

void configurePins(void);

#endif