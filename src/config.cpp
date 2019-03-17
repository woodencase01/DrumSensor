#include <Arduino.h>
#include <EEPROM.h>
#include <EEWrap.h>
#include "config.h"
#include "settings.h"

uint8_e m_padType EEMEM;
uint8_e m_rimPad EEMEM;
uint8_e m_centerThreshold EEMEM;
uint8_e m_padId EEMEM;
uint8_e m_lowerThreshold[5] EEMEM;
uint8_e m_upperThreshold[5] EEMEM;

void configureSensors()
{
#ifdef DEBUG // Force default config
  Serial.println(F("Loading Default configuration"));
  loadFactoryDefaults();
#endif

#ifndef DEBUG
  if (m_padType == 0) // Only update to defaults if not yet configured
  {
    m_rimPad = defaultRimPad;
    m_centerThreshold = defaultCenterThreshold;
    m_padId = defaultPadId;
    for (byte i = 0; i < 5; i++)
      m_lowerThreshold[i] = defaultLowerThreshold[i];
    for (byte i = 0; i < 5; i++)
      m_upperThreshold[i] = defaultUpperThreshold[i];
  }
#endif
}

void readConfig()
{
  padType = m_padType;
  rimPad = m_rimPad;

  if ((padType == 0) || (padType == 1) || (padType == 3)) // 1 zone sensor
  {
    nbPadSensors = 1;
  }

  if ((padType == 2) || (padType == 4)) // 3 zones sensor
  {
    nbPadSensors = 3;
  }

  for (byte i = 0; i < 5; i++)
  {
    sensor[i].setMinThreshold(m_lowerThreshold[i]);
    sensor[i].setMaxThreshold(m_upperThreshold[i]);
  }

  centerThreshold = m_centerThreshold;

  padId = m_padId;
}

void configurePins()
{
  pinMode(readCom, INPUT);
  pinMode(activeCom, OUTPUT);
  pinMode(resetPeakPin, OUTPUT);
}

void loadFactoryDefaults()
{
  m_padType = defaultPadType;
  m_rimPad = defaultRimPad;
  m_centerThreshold = defaultCenterThreshold;
  m_padId = defaultPadId;
  for (byte i = 0; i < 5; i++)
    m_lowerThreshold[i] = defaultLowerThreshold[i];
  for (byte i = 0; i < 5; i++)
    m_upperThreshold[i] = defaultUpperThreshold[i];
}

void setPadId(byte id)
{
  padId = id;
}

void storeConfig()
{
  m_padType = padType;
  m_rimPad = rimPad;
  m_centerThreshold = centerThreshold;
  m_padId = padId;
  for (byte i = 0; i < 5; i++)
    m_lowerThreshold[i] = sensor[i].getMinThreshold();
  for (byte i = 0; i < 5; i++)
    m_upperThreshold[i] = sensor[i].getMaxThreshold();
}

void listConfig()
{
#ifdef DEBUG
  Serial.print(F("Pad type: "));
  Serial.println(padType);
  Serial.print(F("Pad ID: "));
  Serial.println(padId);
  Serial.println(F("Pad thresholds: "));
  Serial.println(F("Min"));
  for (byte i = 0; i < 4; i++)
  {
    Serial.print(sensor[i].getMinThreshold());
    Serial.print(F(", "));
  }
  Serial.println(sensor[4].getMinThreshold());
  Serial.println(F("Max"));
  for (byte i = 0; i < 4; i++)
  {
    Serial.print(sensor[i].getMaxThreshold());
    Serial.print(F(", "));
  }
  Serial.println(sensor[4].getMaxThreshold());
  Serial.print(F("Center threshold: "));
  Serial.println(centerThreshold);
  Serial.print(F("Rim: "));
  Serial.println(rimPad);
  Serial.println(F("Drum Sensor Ready"));
#endif
}