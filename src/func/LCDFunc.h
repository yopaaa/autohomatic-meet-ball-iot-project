#ifndef LCD_FUNC
#define LCD_FUNC

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void initLcd();
void printTextLcd(int row, int columb, String text);
void blankLcdByRow(int row);
void ClearLcd();

#endif