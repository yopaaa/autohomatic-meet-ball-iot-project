#ifndef MODE
#define MODE

#include <Arduino.h>

extern String mode;

void TimerMode();
void CountMode();

void StartMainFunc();
void SetMainFunc(String mode);

#endif