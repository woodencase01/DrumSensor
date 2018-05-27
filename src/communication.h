/*
  -=-=-=- Data protocol -=-=-=-
  byte 0: Start byte [255]
  byte 1: Pad id
  byte 2: Sensor strength [0 - 200]
  byte 3: Sensor id [0: Pad / Cymbal edge, 1: Rim / Cymbal bow, 2: Cymbal Bell]
*/
bool senddata = false;

void readBuffer();
void sendBuffer();
void sendStroke(byte padstroke, byte padid);