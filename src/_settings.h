#ifndef _SETTINGS_H_
#define _SETTINGS_H_

/**
 * @file settings.h
 * @author Philippe Carrier
 */

#include <Arduino.h>

//#define DEBUG

// ==== Pins ====

const byte sensorPin0 = 7;
const byte sensorPin1 = 6;
const byte sensorPin2 = 5;
const byte sensorPin3 = 4;
const byte sensorPin4 = 3;

const byte readCom = 10;
const byte activeCom = 9;

DrumSensor sensor[]
{
  sensorPin0;
  sensorPin1;
  sensorPin2;
  sensorPin3;
  sensorPin4;
}

/* padType
  *0: No configuration set, default settings
  *1: Single Zone pad : Bass drum
  *2: Multi Zone Pad : Snare, Toms
  *3: Single Zone Cymbal : Splash
  *4: Multi Zone Cymbal : Ride, Crash, etc */
const byte defaultPadType = 0;

/* RimPad
  *0: No Rim
  *1: Has RIm */
const byte defaultRimPad = 1; // 
const byte defaultCenterThreshold = 0; //Center threshold [0 - 100]
const byte defaultPadId = 0; //Pad ID [0 - 255]
const byte defaultLowerThreshold[] = {2, 2, 2, 2, 2};
const byte defaultUpperThreshold[] = {43, 255, 255, 255, 255}; // Upper threshold needs to be < 255. They are multiplied by 4 in the sensor loop to match the 10 bits ADC values

const int minDecay = 2;  // Maximum decay time (ms)
const int maxDecay = 15; // Maximum decay time (ms)

#endif