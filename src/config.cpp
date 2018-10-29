#include "config.h"

byte padType;
byte rimPad;
byte nbPadSensors;
byte centerThreshold;
byte padid;
byte sensorId[5];

uint8_e *m_padType = (uint8_e *)0; // Always on byte 0
uint8_e m_rimPad EEMEM;
uint8_e m_centerThreshold EEMEM;
uint8_e m_padId EEMEM;
uint8_e m_lowerThreshold[5] EEMEM;
uint8_e m_upperThreshold[5] EEMEM;

void configureSensors()
{
#ifdef DEBUG // Force default config
  Serial.println(F("Loading Default configuration"));
  *m_padType = defaultPadType;
  m_rimPad = defaultRimPad;
  m_centerThreshold = defaultCenterThreshold;
  m_padId = defaultPadId;
  for (i = 0; i < 5; i++)
    m_lowerThreshold[i] = defaultLowerThreshold[i];
  for (i = 0; i < 5; i++)
    m_upperThreshold[i] = defaultUpperThreshold[i];
#endif

#ifndef DEBUG
  if (*m_padType == 0) // Only update to defaults if not yet configured
  {
    m_rimPad = defaultRimPad;
    m_centerThreshold = defaultCenterThreshold;
    m_padId = defaultPadId;
    for (i = 0; i < 5; i++)
      m_lowerThreshold[i] = defaultLowerThreshold[i];
    for (i = 0; i < 5; i++)
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
    nbPadSensors = 4;
  }

  for (int i = 0; i < 5; i++)
  {
    sensor[i].setMinThreashold = m_lowerThreshold[i];
    sensor[i].setMaxThreashold = m_upperThreshold[i];
  }

  centerThreshold = m_centerThreshold;

  padid = m_padId;
}

void configurePins()
{
  pinMode(readCom, INPUT);
  pinMode(activeCom, OUTPUT);
}