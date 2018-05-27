/*
  -=-=-=- Sensors -=-=-=-
  0: Waiting for decay
  1: First read
  2: Successive readings
  3: Final value
*/
#include <Arduino.h>
#include "communication.h"
#include "config.h"

const int nbsensors = 1;

const int sensorspin[] = {0, 1, 2, 3, 4};

const int peakread = 1; // time in millis before a peak is read
const int mindecay = 2;   // Maximum decay time (ms)
const int maxdecay = 15;   // Maximum decay time (ms)

int maxsensorvalue[] = {0, 0, 0, 0, 0};
byte sensorstroke[] = {0, 0, 0, 0, 0};
byte sensorstate[] = {0, 0, 0, 0, 0};

unsigned long sensorsdecayend[nbsensors];
unsigned long startread[nbsensors];

void manageSensors() {

  unsigned long now = millis();

  // First check and adjust the sensors state
  for (int i = 0; i < nbsensors; i++) {

    if (sensorstate[i] == 0) { //0: Waiting for decay
      if (sensorsdecayend[i] < now) sensorstate[i] = 1;
    }

    //1: First read : managed in read sensors

    if (sensorstate[i] == 2) { //2: Successive readings
      if (startread[i] + peakread < now) sensorstate[i] = 3;
    }

    if (sensorstate[i] == 3) { //3: Final value

      sensorstroke[i] = map(maxsensorvalue[i], 0, 1023, 0, 200);
      sensorsdecayend[i] = map(maxsensorvalue[i], 0, 1023, mindecay, maxdecay) + now;

      if (sensorstroke[i] > 0) {
        sendStroke(sensorstroke[i],0);
      }

      maxsensorvalue[i] = 0;
      sensorstate[i] = 0;

#ifdef DEBUG
      Serial.print("Number of reads: ");
      Serial.println(numberreads[i]);
      numberreads[i] = 0;
#endif DEBUG
    }
  }
}

void readSensors() {

  unsigned long now = millis();

  for (int i = 0; i < nbsensors; i++) {

    if (sensorstate[i] == 1  ) { //1: First read
      int val = analogRead(sensorspin[i]);
      if (val > threshold[i]) {
        if (val > maxsensorvalue[i]) maxsensorvalue[i] = val;

#ifdef DEBUG
        numberreads[i]++;
#endif DEBUG

        sensorstate[i] = 2;
        startread[i] = now;
      }
    }

    if (sensorstate[i] == 2 ) {
      int val = analogRead(sensorspin[i]);
      if (val > maxsensorvalue[i]) maxsensorvalue[i] = val;

#ifdef DEBUG
      numberreads[i]++;
#endif DEBUG
    }
  }
}
