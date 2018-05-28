#include <Arduino.h>

byte padType;
int threshold[];
int nbPadSensors;
byte centerThreshold;

void configureSensors();

void initializeModule();

void readConfig();
