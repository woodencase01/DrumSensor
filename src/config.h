//config.h

#ifndef _config_h
#define _config_h

#include <Arduino.h>

#define DEBUG

extern byte threshold[5];
extern byte upperThreshold[5];
extern byte padType;
extern int nbPadSensors;
extern byte centerThreshold;
extern byte padid;

const byte readCom = 10;
const byte activeCom = 9;

const byte sensor1 = 7;

void configureSensors(void);

void initializeModule(void);

void readConfig(void);

void configurePins(void);

#endif