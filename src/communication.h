//communication.h
#ifndef _communication_h
#define _communication_h

#include <Arduino.h>
#include "config.h"

void readBuffer();
void sendBuffer();
void sendStroke(byte padstroke, byte padid);

#endif