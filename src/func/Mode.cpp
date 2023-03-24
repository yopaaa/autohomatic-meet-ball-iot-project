#include "Mode.h"
#include <Arduino.h>
#include "buzzer.h"
#include "../Var.h"
#include "LCDFunc.h"
#include "WiFiFunc.h"

String mode = "timer";

void TimerMode()
{
    if (digitalRead(button2) == HIGH)
    {
        myBuzzer.beep(100);
        startTime = millis();
        isPause = !isPause;
        if (!isPause)
            digitalWrite(relay1, LOW); // set on relay
        if (isPause)
            digitalWrite(relay1, HIGH); // set off relay
        delay(100);
        return;
    }
    if (isPause)
    {
        int minute = static_cast<int>(timerTarget / 60); // timerTarget / 60; // calculate the number of minutes
        int second = timerTarget - (minute * 60);        // timerTarget % 60; // calculate the number of seconds
        printTextLcd(0, 0, "Mode: Timer");
        printTextLcd(1, 0, "Target: ");
        lcd.setCursor(8, 1); // Target: 20m 35s
        lcd.print(minute);
        printTextLcd(1, 10, "m");

        lcd.setCursor(12, 1); // Target: 20m 35s
        lcd.print(second);
        printTextLcd(1, 14, "s");

        Serial.println(timerTarget);

        if (digitalRead(button3) == HIGH)
        {
            myBuzzer.beep(100);
            delay(100);
            blankLcdByRow(1);
            if (digitalRead(button3) == HIGH)
            {
                timerTarget -= 60;
                return;
            }
            timerTarget--;
            return;
        }
        if (digitalRead(button4) == HIGH)
        {
            myBuzzer.beep(100);
            delay(100);
            blankLcdByRow(1);
            if (digitalRead(button4) == HIGH)
            {
                timerTarget += 60;
                return;
            }
            timerTarget++;
            return;
        }
    }
    else
    {
        unsigned long currentTime = millis();
        unsigned long elapsedTime = (currentTime - startTime) / 1000;
        int minute = static_cast<int>(elapsedTime / 60); // calculate the number of minutes
        int second = elapsedTime - (minute * 60);        // calculate the number of seconds
        if (elapsedTime >= timerTarget)
        {
            isPause = true;
            digitalWrite(relay1, HIGH); // set off relay
            myBuzzer.beep(1000);
            return;
        }
        else
        {
            // Current:10m 10s
            blankLcdByRow(2);
            printTextLcd(2, 0, "Current:");
            lcd.setCursor(4, 2); // Target: 20m 35s
            lcd.print(minute);
            lcd.setCursor(6, 2); // Target: 20m 35s
            lcd.print("m");

            lcd.setCursor(8, 2); // Target: 20m 35s
            lcd.print(second);
            lcd.setCursor(10, 2); // Target: 20m 35s
            lcd.print("s");
            delay(500);
        }
    }
}

void CountMode()
{
    if (digitalRead(button2) == HIGH)
    {
        myBuzzer.beep(100);
        startTime = millis();
        isPause = !isPause;
        if (!isPause)
        {
            blankLcdByRow(2);
            // digitalWrite(relay2, LOW); // set on relay
            digitalWrite(relay1, LOW); // set on relay
        }
        if (isPause)
        {
            // digitalWrite(relay2, HIGH); // set off relay
            digitalWrite(relay1, HIGH); // set off relay
        }

        delay(100);
        return;
    }
    if (isPause)
    {
        printTextLcd(0, 0, "Mode: ");
        lcd.setCursor(6, 0);
        lcd.print(countMode);

        printTextLcd(1, 0, "Target: ");
        lcd.setCursor(8, 1);
        lcd.print(countTarget);

        if (digitalRead(button3) == HIGH)
        {
            myBuzzer.beep(100);
            delay(100);
            blankLcdByRow(1);
            if (digitalRead(button3) == HIGH)
            {
                countTarget -= 10;
                return;
            }
            countTarget--;
            return;
        }
        if (digitalRead(button4) == HIGH)
        {
            myBuzzer.beep(100);
            delay(100);
            blankLcdByRow(1);
            if (digitalRead(button4) == HIGH)
            {
                countTarget += 10;
                return;
            }
            countTarget++;
            return;
        }
    }
    else
    {
        int irValue = digitalRead(irPin);
        if (irValue == LOW)
        {
            objectCount++;
            printTextLcd(2, 0, "Current: ");
            lcd.setCursor(5, 2);
            lcd.print(objectCount);
            if (objectCount >= countTarget)
            {
                objectCount = 0;
                printTextLcd(2, 0, "Current: finish");
                isPause = true;
                // digitalWrite(relay2, HIGH); // set on relay
                digitalWrite(relay1, HIGH); // set on relay
                myBuzzer.beep(1000);
                return;
            }
        }
        delay(20);
    }
}

void WifiMode()
{
    if (digitalRead(button2) == HIGH)
    {
        myBuzzer.beep(100);
        isApWifiMode = !isApWifiMode;
        if (isApWifiMode)
        {
            WiFiApStart();
        }
        else
        {
            WiFiSTAStart();
        }
        delay(100);
        return;
    }
}

void (*mainFunc)() = TimerMode;

void StartMainFunc()
{
    mainFunc();
    return;
}

void SetMainFunc(String mode)
{
    if (mode == "timer")
    {
        mode = "timer";
        mainFunc = TimerMode;
    }
    else if (mode == "count")
    {
        mode = "count";
        mainFunc = CountMode;
    }
    else if (mode == "wifi")
    {
        mode = "wifi";
        mainFunc = WifiMode;
    }
    else
    {
        mode = "timer";
        mainFunc = TimerMode;
    }
}
