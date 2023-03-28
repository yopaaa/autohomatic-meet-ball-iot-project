#include <Arduino.h>
#include <FastLED.h>
#include <FS.h>
#include "./func/buzzer.h"
#include "./func/EEPROMFunc.h"
#include "./func/WiFiFunc.h"
#include "./func/LCDFunc.h"
#include "./func/HttpHandler.h"
#include "./func/Mode.h"
#include "./Var.h"

bool isStartCountingButton1 = false;
CRGB leds[NUM_LEDS];

void initPins()
{
  pinMode(irPin, INPUT);
  pinMode(button1, INPUT_PULLDOWN);
  pinMode(button2, INPUT_PULLDOWN);
  pinMode(button3, INPUT_PULLDOWN);
  pinMode(button4, INPUT_PULLDOWN);

  pinMode(relay1, OUTPUT);
  // pinMode(relay2, OUTPUT);
  pinMode(LEDS_PIN, OUTPUT);

  digitalWrite(relay1, HIGH); // set off relay
  // digitalWrite(relay2, HIGH); // set off relay

  leds[0] = CRGB(6, 255, 6);
  FastLED.show();
}

void setup()
{
  Serial.begin(9600);
  eepromInit();
  initLcd();
  FastLED.addLeds<LEDS_TYPE, LEDS_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  initPins();

  readWifiCredentials();

  myBuzzer.on();
  printTextLcd(0, 0, "Hello world 1");
  printTextLcd(1, 0, "Hello world 2");
  printTextLcd(2, 0, "Hello world 3");
  printTextLcd(3, 0, "Hello world 4");
  delay(1000);
  myBuzzer.off();

  ClearLcd();
  // SetMainFunc("timer");
}

void loop()
{
  if ((digitalRead(button1) == HIGH) && !isStartCountingButton1)
  {
    isStartCountingButton1 = true;
    startTime = millis();
  }
  if (isStartCountingButton1)
  {
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - startTime;
    Serial.print("button 1 pressed in: ");
    Serial.println(elapsedTime);

    if ((digitalRead(button1) == HIGH) && (elapsedTime >= 3000))
    {
      Serial.println("button1 presses on 3 second");
      // timer > count > wifi
      if (mode == "timer")
      {
        leds[0] = CRGB(255, 0, 0);
        SetMainFunc("count");
        Serial.println("mainFunc = CountMode");
      }
      else if (mode == "count")
      {
        leds[0] = CRGB(0, 255, 0);
        SetMainFunc("wifi");
        Serial.println("wifi mode");
      }
      else if (mode == "wifi")
      {
        leds[0] = CRGB(0, 0, 255);
        SetMainFunc("timer");
        Serial.println("mainFunc = TimerMode;");
      }

      mode == "wifi" ? WiFiApStart() : WifiStop();
      ClearLcd();
      FastLED.show();
      isStartCountingButton1 = false;
      isPause = true;
      myBuzzer.beep(300);
      return;
    }
  }
  else
  {
    StartMainFunc();
  }
}
