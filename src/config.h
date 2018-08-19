//config.h

#ifndef _config_h
#define _config_h

#include <Arduino.h>

extern byte threshold[5];
extern byte padType;
extern int nbPadSensors;
extern byte centerThreshold;
extern byte padid;

void configureSensors(void);

void initializeModule(void);

void readConfig(void);

#endif