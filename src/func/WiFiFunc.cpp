#include "WiFiFunc.h"
#include <Arduino.h>
#include <WiFi.h>
#include "../Var.h"
#include "LCDFunc.h"
#include "buzzer.h"
#include "HttpHandler.h"

IPAddress local_ip(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

void WiFiApStart()
{
    ClearLcd();
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    WiFi.softAP(APssid, APpassword);

    Serial.println("");
    Serial.println(""); // print space
    printTextLcd(0, 0, "Mode: AP");
    Serial.println("Access Point started");

    Serial.print("ssid:");
    Serial.println(APssid);
    printTextLcd(1, 0, "ssid: ");
    lcd.setCursor(6, 1);
    lcd.print(APssid);

    Serial.print("pwd: ");
    Serial.println(APpassword);
    printTextLcd(2, 0, "pwd: ");
    lcd.setCursor(1, 2);
    lcd.print(APpassword);

    IPAddress localIP = WiFi.softAPIP();
    Serial.print("ip: ");
    Serial.println(localIP);
    printTextLcd(3, 0, "ip: ");
    lcd.setCursor(0, 3);
    lcd.print(localIP);
    httpHandler();
    return;
}

void WiFiSTAStart()
{
    // Connect to Wi-Fi using the saved SSID and password
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    int trying = 1;

    ClearLcd();
    printTextLcd(0, 0, "Mode: STA");
    while (WiFi.status() != WL_CONNECTED)
    {
        blankLcdByRow(1);
        blankLcdByRow(2);
        Serial.print("ssid:");
        Serial.println(ssid);
        printTextLcd(1, 0, "ssid: ");
        lcd.setCursor(6, 1);
        lcd.print(ssid);

        Serial.println("Connecting..." + String(trying));
        printTextLcd(2, 0, "Connecting..." + String(trying));

        if (digitalRead(button2) == HIGH)
        {
            myBuzzer.beep(100);
            isApWifiMode = true;
            WiFiApStart();
            delay(100);
            return;
        }
        if (trying == 50)
        {
            Serial.println("Cant connect to network");
            printTextLcd(2, 0, "Cant connect...");
            return;
        }
        trying++;
        delay(1000);
    }

    Serial.println("Connected to WiFi");
    printTextLcd(2, 0, "Connected to WiFi");

    IPAddress localIP = WiFi.localIP();
    Serial.print("ip: ");
    Serial.println(localIP);
    printTextLcd(3, 0, "ip: ");
    lcd.setCursor(0, 3);
    lcd.print(localIP);
    httpHandler();

    // print ip address to display
    delay(2000);
    return;
}

void WifiStop()
{
    int wifiMode = WiFi.getMode();
    // to stop is wifi is running
    if (wifiMode == WIFI_MODE_STA)
    {
        Serial.println("stop wifi in STA mode.");
        WiFi.disconnect(true);
    }
    else if (wifiMode == WIFI_MODE_AP)
    {
        Serial.println("stop wifi in AP mode.");
        WiFi.softAPdisconnect(true);
    }
    // Check current Wi-Fi mode
    if (wifiMode == WIFI_MODE_STA)
    {
        Serial.println("Currently in STA mode.");
    }
    else if (wifiMode == WIFI_MODE_AP)
    {
        Serial.println("Currently in AP mode.");
    }
    else if (wifiMode == WIFI_MODE_APSTA)
    {
        Serial.println("Currently in Station+AP mode.");
    }
    else
    {
        Serial.println("Wi-Fi interface not initialized.");
    }
    return;
}
