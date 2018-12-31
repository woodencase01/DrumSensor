#include "settings.h"

DrumSensor sensor[5] = {
    DrumSensor(sensorPin0),
    DrumSensor(sensorPin1),
    DrumSensor(sensorPin2),
    DrumSensor(sensorPin3),
    DrumSensor(sensorPin4),
};

byte padType;
byte rimPad;
byte nbPadSensors;
byte centerThreshold;
byte padId;