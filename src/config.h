//config.h

#ifndef _CONFIG_h
#define _CONFIG_h

void configureSensors();
void initializeModule();
void readConfig();
void configurePins();
void loadFactoryDefaults();
void setPadId(byte id);
void storeConfig();
void listConfig();
void prepareSensors();

#endif