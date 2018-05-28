/*
  -=-=-=- Data protocol -=-=-=-
  byte 0: Start byte [255]
  byte 1: Pad id (Always 0 as the first pad)
  byte 2: Sensor strength [0 - 200]
  byte 3: Sensor id 
  *0-99: Pad / Cymbal bow (Position sensitive)
  *100: Pad / Cymbal (Single zone pad)
  *101: Rim
*/
#include <Arduino.h>
#include "communication.h"

const int databytes = 4;
byte databuffer[databytes];
byte datastroke[databytes];

void readBuffer() {

  int buffersize = Serial.available();

  if (buffersize >= databytes) {

    /*#ifdef DEBUG
      Serial.print("Current buffer: ");
      Serial.println(buffersize);
      #endif */

    bool goodbuffer = false;

    while (Serial.available() >= databytes) {
      byte bufferread = Serial.read();

      /*#ifdef DEBUG
        Serial.print("Current read: ");
        Serial.println(bufferread);
        #endif */

      if (bufferread == 255) {
        goodbuffer = true;
        break;
      }
    }

    if (goodbuffer) {
      for (int i = 1; i < databytes; i++) {
        databuffer[i] = Serial.read();
      }
      senddata = true;

#ifdef DEBUG
      Serial.print("Received: ");
      for (int i = 0; i < databytes - 1; i++) {
        Serial.print(databuffer[i]);
        Serial.print(",");
      }
      Serial.println(databuffer[databytes - 1]);
#endif
    }
  }
}

void sendBuffer() {
  databuffer[0] = 255;
  databuffer[1]++;
  Serial.write(databuffer, databytes);
  senddata = false;

#ifdef DEBUG
  Serial.print("Sent: ");
  for (int i = 0; i < databytes - 1; i++) {
    Serial.print(databuffer[i]);
    Serial.print(",");
  }
  Serial.println(databuffer[databytes - 1]);
#endif
}

void sendStroke(byte strokeStrength, byte strokeID) {
  datastroke[0] = 255;
  datastroke[1] = 0;
  datastroke[2] = strokeStrength;
  datastroke[3] = strokeID;
  Serial.write(datastroke, databytes);

#ifdef DEBUG
  Serial.print("Sent: ");
  for (int i = 0; i < databytes - 1; i++) {
    Serial.print(datastroke[i]);
    Serial.print(",");
  }
  Serial.println(datastroke[databytes - 1]);
#endif
}
