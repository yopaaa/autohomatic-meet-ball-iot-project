#ifndef VAR
#define VAR
#include <Arduino.h>

// extern void (*mainFunc)();
// void SetMainFunc(void (*x)());

#define NUM_LEDS 2
#define LEDS_PIN 12
#define LEDS_TYPE WS2812B

extern const int irPin;
extern const int buzzerPin;
extern const int button1;
extern const int button2;
extern const int button3;
extern const int button4;
extern const int relay1;
extern const int relay2;

extern String ssid;
extern String password;
extern const char *APssid;
extern const char *APpassword;

extern bool isPause;
extern bool isApWifiMode;
extern unsigned long startTime;
extern unsigned long timerTarget;
extern int countTarget;
extern int objectCount;
extern String countMode;

#endif
