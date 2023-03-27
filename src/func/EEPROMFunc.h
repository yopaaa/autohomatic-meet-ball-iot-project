#ifndef EEPROM_FUNC
#define EEPROM_FUNC

#include <Arduino.h>

void saveWifiCredentials(const char *ssid, const char *password);
void readWifiCredentials();
void eepromInit();
void loadVariableFromEeprom();

#endif