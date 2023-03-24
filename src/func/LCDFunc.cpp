#include "LCDFunc.h"
#include <Arduino.h>
#include "../Var.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 4);

void initLcd()
{
    Wire.begin();
    lcd.init();
    lcd.backlight();
    return;
}

void printTextLcd(int row, int columb, String text)
{
    int textStart = 0;
    if (row > 1)
        textStart = -4;
    lcd.setCursor(textStart + columb, row);
    lcd.print(text);
    return;
}

void blankLcdByRow(int row)
{
    printTextLcd(row, 0, "                  ");
    return;
}

void ClearLcd()
{
    lcd.clear();
    return;
}
