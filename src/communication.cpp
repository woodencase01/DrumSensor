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
  byte 0: Start byte [254]

*/
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

      if (bufferread == 255)
      {
        goodbuffer = true;
      }
    }

    if (goodbuffer)
    {
      for (byte i = 1; i < databytes; i++)
      {
        databuffer[i] = Serial.read();
      }

#ifdef DEBUG
      Serial.print("Received: ");
      for (byte i = 0; i < databytes - 1; i++)
      {
        Serial.print(databuffer[i]);
        Serial.print(",");
      }
      Serial.println(databuffer[databytes - 1]);
#endif
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
  Serial.print("Sent: ");
  for (int i = 0; i < databytes - 1; i++)
  {
    Serial.print(datastroke[i]);
    Serial.print(",");
  }
  Serial.println(datastroke[databytes - 1]);
#endif
}
