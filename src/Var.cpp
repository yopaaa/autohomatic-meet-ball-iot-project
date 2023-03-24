#include "Var.h"
#include <Arduino.h>
#include "./func/Mode.h"

// const int NUM_LEDS = 2;
const int irPin = 23;
const int buzzerPin = 2;
// const int neoLedPin = 12;
const int button1 = 25;
const int button2 = 33;
const int button3 = 32;
const int button4 = 35;
const int relay1 = 4;
const int relay2 = 5;

String ssid = "";
String password = "";
const char *APssid = "smkn1sliat";
const char *APpassword = "zxcvbnm123";

bool isPause = true;

bool isApWifiMode = false;

int objectCount = 0;
unsigned long startTime = 0;         // variable to store the start time
unsigned long timerTarget = 1200;    // variable to store the start time
int countTarget = 100;               // variable to store the start time
String countMode = "limit";
