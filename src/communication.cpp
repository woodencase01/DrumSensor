/*
  -=-=-=- Data protocol -=-=-=-
  byte 0: Start byte [255]
  byte 1: Pad id
  byte 2: Sensor strength [0 - 200]
  byte 3: Sensor id 
  *0-99: Pad / Cymbal bow (Position sensitive)
  *100: Pad / Cymbal (Single zone pad)
  *101: Rim

  -=-=-=- Configuration Protocol -=-=-=-
  byte 0: 
  254 : Start byte 
  byte 1: 
    0 : Reset configuration to default
    1 : Set padID
      byte 2: Pad ID
  254 : Read configuration
  255 : Store Configuration in EEPROM
*/
#include <Arduino.h>
#include "config.h"
#include "communication.h"
#include "settings.h"

const byte databytes = 4;
byte databuffer[databytes];
byte datastroke[databytes];

void readBuffer()
{

  int buffersize = Serial.available();

  if (buffersize >= databytes)
  {

    /*#ifdef DEBUG
      Serial.print("Current buffer: ");
      Serial.println(buffersize);
      #endif */

    bool goodbuffer = false;

    while (Serial.available() >= databytes && !goodbuffer)
    {
      byte bufferread = Serial.read();

      if (bufferread >= 254) // Either a stroke or a configuration
      {
        goodbuffer = true;
        databuffer[0] = bufferread;
      }
    }

    if (goodbuffer)
    {
      for (byte i = 1; i < databytes; i++)
      {
        databuffer[i] = Serial.read();
      }

#ifdef DEBUG
      Serial.print(F("Received: "));
      for (byte i = 0; i < databytes - 1; i++)
      {
        Serial.print(databuffer[i]);
        Serial.print(F(","));
      }
      Serial.println(databuffer[databytes - 1]);
#endif
      if (databuffer[0] == 254) // Configuration command received
      {
        switch (databuffer[1])
        {
        case 0: // Reset configuration to default
          loadFactoryDefaults();
#ifdef DEBUG
          Serial.println(F("Loaded factory defaults"));
#endif
          break;
        case 1: // Change padID value
          setPadId(databuffer[2]);
#ifdef DEBUG
          Serial.print(F("Pad ID set as "));
          Serial.println(databuffer[2]);
#endif
          break;
        case 254: // List configuration
#ifdef DEBUG      // Only available in debug
          listConfig();
#endif
          break;
        case 255: // Store Configuration in EEPROM
          storeConfig();
#ifdef DEBUG
          Serial.println(F("Stored configuration in EEPROM"));
#endif
          break;
        }
      }
    }
  }
}

void sendStroke(byte strokeStrength, byte strokeID)
{
  datastroke[0] = 255;
  datastroke[1] = padId;
  datastroke[2] = strokeStrength;
  datastroke[3] = strokeID;
  while (digitalRead(readCom))
  {
    // Wait until the communication pin goes low
  }
  digitalWrite(activeCom, HIGH);
  Serial.write(datastroke, databytes);
  digitalWrite(activeCom, LOW);

#ifdef DEBUG
  Serial.println();
  Serial.print(F("Sent: "));
  for (int i = 0; i < databytes - 1; i++)
  {
    Serial.print(datastroke[i]);
    Serial.print(F(","));
  }
  Serial.println(datastroke[databytes - 1]);
#endif
}
