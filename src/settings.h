#ifndef _SETTINGS_H_
#define _SETTINGS_H_

/**
 * @file settings.h
 * @author Philippe Carrier
 */

#include <Arduino.h>
#include "DrumSensor.h"

#define DEBUG  // Basic debug level
#define DEBUG1 // Debug level interfering with sensor read routines timings (adds 500 µs)

// ==== Pins ====

const byte sensorPin0 = 0;
const byte sensorPin1 = 1;
const byte sensorPin2 = 2;
const byte sensorPin3 = 3;
const byte sensorPin4 = 4;

const byte readCom = 10;
const byte activeCom = 9;

/* padType
  *0: No configuration set, default settings
  *1: Single Zone pad : Bass drum
  *2: Multi Zone Pad : Snare, Toms
  *3: Single Zone Cymbal : Splash
  *4: Multi Zone Cymbal : Ride, Crash, etc */
const byte defaultPadType = 2;

/* RimPad
  *0: No Rim
  *1: Has RIm */
const byte defaultRimPad = 1; // 
const byte defaultCenterThreshold = 0; //Center threshold [0 - 100]
const byte defaultPadId = 0; //Pad ID [0 - 255]
const byte defaultLowerThreshold[] = {10, 10, 10, 10, 10};
const byte defaultUpperThreshold[] = {255, 255, 255, 255, 255}; // Upper threshold needs to be < 255. They are multiplied by 4 in the sensor loop to match the 10 bits ADC values

const int minDecay = 5;  // Maximum decay time (ms)
const int maxDecay = 20; // Maximum decay time (ms)

extern byte padType;
extern byte rimPad;
extern byte nbPadSensors;
extern byte centerThreshold;

extern byte padId;

const uint8_t calibratingValues = 5;

extern DrumSensor sensor[5];

#endif