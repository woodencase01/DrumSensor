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

const byte sensorPin0 = 7;
const byte sensorPin1 = 6;
const byte sensorPin2 = 5;
const byte sensorPin3 = 4;
const byte sensorPin4 = 3;

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
const byte defaultLowerThreshold[] = {4, 4, 4, 4, 4};
const byte defaultUpperThreshold[] = {150, 150, 150, 150, 150}; // Upper threshold needs to be < 255. They are multiplied by 4 in the sensor loop to match the 10 bits ADC values

// Quadruplets at 250 BPM correspond to 60 ms between strokes
//                 A flam correspond to 40 ms between strokes
const int minDecay = 5;  // Minimum decay time (ms)
const int maxDecay = 20; // Maximum decay time (ms)

extern byte padType;
extern byte rimPad;
extern byte nbPadSensors;
extern byte centerThreshold;
extern byte padId;

extern DrumSensor sensor[5];

#endif