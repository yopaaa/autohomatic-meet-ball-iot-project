#include "EEPROMFunc.h"
#include <EEPROM.h>
#include <Arduino.h>
#include "../Var.h"

#define SSID_ADDRESS 400
#define PASSWORD_ADDRESS 432
#define EEPROM_SIZE 512

void eepromInit()
{
    EEPROM.begin(EEPROM_SIZE);
}

void loadVariableFromEeprom()
{
    objectCountDelay = EEPROM.read(OBJECT_COUNT_DELAY_ADDRESS) * 100;
}

void saveWifiCredentials(const char *ssid, const char *password)
{
    // Save the SSID to EEPROM
    int i = 0;
    for (; i < strlen(ssid); i++)
    {
        EEPROM.write(SSID_ADDRESS + i, ssid[i]);
    }
    for (; i < 32; i++)
    {
        EEPROM.write(SSID_ADDRESS + i, 0);
    }

    // Save the password to EEPROM
    i = 0;
    for (; i < strlen(password); i++)
    {
        EEPROM.write(PASSWORD_ADDRESS + i, password[i]);
    }
    for (; i < 64; i++)
    {
        EEPROM.write(PASSWORD_ADDRESS + i, 0);
    }

    // Commit the changes to EEPROM
    EEPROM.commit();
}

// Read the saved SSID and password from EEPROM
void readWifiCredentials()
{
    for (int i = SSID_ADDRESS; i < SSID_ADDRESS + 32; i++)
    {
        ssid += char(EEPROM.read(i));
    }
    for (int i = PASSWORD_ADDRESS; i < PASSWORD_ADDRESS + 64; i++)
    {
        password += char(EEPROM.read(i));
    }
}