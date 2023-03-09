#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <FastLED.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <FS.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include "./func/buzzer.h"
#include "./var.h"

bool isApWifiMode = false;
String ssid = "";
String password = "";
const char *APssid = "smkn1sliat";
const char *APpassword = "zxcvbnm123";
IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

AsyncWebServer server(3000);
const int SSID_ADDRESS = 400;
const int PASSWORD_ADDRESS = 432;

int objectCount = 0;
unsigned long startTime = 0;         // variable to store the start time
bool isStartCountingButton1 = false; // variable to track if we are currently counting or not
bool isPause = true;
unsigned long timerTarget = 1200; // variable to store the start time
int countTarget = 100;            // variable to store the start time
String countMode = "limit";
LiquidCrystal_I2C lcd(0x27, 16, 4);
Buzzer myBuzzer(buzzerPin);
CRGB leds[NUM_LEDS];

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

void (*mainFunc)() = TimerMode;

void initPins()
{
  pinMode(irPin, INPUT);
  pinMode(button1, INPUT_PULLDOWN);
  pinMode(button2, INPUT_PULLDOWN);
  pinMode(button3, INPUT_PULLDOWN);
  pinMode(button4, INPUT_PULLDOWN);

  pinMode(relay1, OUTPUT);
  // pinMode(relay2, OUTPUT);
  pinMode(neoLedPin, OUTPUT);

  digitalWrite(relay1, HIGH); // set off relay
  // digitalWrite(relay2, HIGH); // set off relay

  leds[0] = CRGB(6, 255, 6);
  FastLED.show();
}

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  EEPROM.begin(512);
  FastLED.addLeds<WS2812B, neoLedPin, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  lcd.init();
  lcd.backlight();
  initPins();

  // Read the saved SSID and password from EEPROM
  for (int i = SSID_ADDRESS; i < SSID_ADDRESS + 32; i++)
  {
    ssid += char(EEPROM.read(i));
  }
  for (int i = PASSWORD_ADDRESS; i < PASSWORD_ADDRESS + 64; i++)
  {
    password += char(EEPROM.read(i));
  }

  myBuzzer.on();
  printTextLcd(0, 0, "Hello world 1");
  printTextLcd(1, 0, "Hello world 2");
  printTextLcd(2, 0, "Hello world 3");
  printTextLcd(3, 0, "Hello world 4");
  delay(1000);
  myBuzzer.off();
  lcd.clear();
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
      if ((digitalRead(button1) == HIGH) && (elapsedTime >= 5000))
      {
        leds[0] = CRGB(0, 255, 0);
        FastLED.show();
        mainFunc = WifiMode;
        Serial.println("mainFunc = TimerMode;");
        myBuzzer.beep(300);
        lcd.clear();
        isStartCountingButton1 = false;
        isPause = true;
        startWifi(0);
        return;
      }
      Serial.println("button1 presses on 3 second");
      if (mainFunc == TimerMode)
      {
        leds[0] = CRGB(255, 0, 0);
        mainFunc = CountMode;
        Serial.println("mainFunc = CountMode");
      }
      else if (mainFunc == CountMode)
      {
        leds[0] = CRGB(0, 255, 0);
        FastLED.show();
        mainFunc = WifiMode;
        Serial.println("mainFunc = WifiMode;");
        myBuzzer.beep(300);
        lcd.clear();
        isStartCountingButton1 = false;
        isPause = true;
        startWifi(1);
        return;
      }
      else if (mainFunc == WifiMode)
      {
        leds[0] = CRGB(0, 0, 255);
        mainFunc = TimerMode;
        Serial.println("mainFunc = TimerMode;");
      }

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

      lcd.clear();
      FastLED.show();
      isStartCountingButton1 = false;
      isPause = true;
      myBuzzer.beep(300);
      return;
    }
    if (digitalRead(button1) == LOW)
    {
      // isPause = !isPause;
      isStartCountingButton1 = false;
      // myBuzzer.beep(500);
      return;
    }
  }
  else
  {
    mainFunc();
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
      startWifi(1);
    }
    else
    {
      startWifi(0);
    }
    delay(100);
    return;
  }
}

// WIFI FUNCTION
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

void startWifi(int wifiMode)
{
  if (wifiMode == 1)
  { // wifi access point mode if mode = 1
    lcd.clear();
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
  else
  { // connect to access point mode if mode = 2
    // Connect to Wi-Fi using the saved SSID and password
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    int trying = 1;

    lcd.clear();
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
        startWifi(1);
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
}

// WEB SERVER FUNCTION
void httpHandler()
{
  server.on("/wifi/set", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    bool valuesExist = (request->hasArg("ssid")) && (request->hasArg("password"));

    if (valuesExist){
      String ssid = request->arg("ssid");
      String password = request->arg("password");

      saveWifiCredentials(ssid.c_str(), password.c_str());
      request->send(200, "application/json", "{\"status\": \"OK\"}"); 
    } else {
      request->send(400, "application/json", "{\"status\": \"Bad request\"}");
    } });

  server.on("/isPause", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    bool valuesExist = (request->hasArg("val"));

    if (valuesExist){
      String val = request->arg("val");
      val.toLowerCase();
      if (val == "true")
      {
        isPause = false;
        myBuzzer.beep(100);
        startTime = millis();
        blankLcdByRow(2);
        digitalWrite(relay1, LOW); // set on relay
        delay(100);
      }
      else
      {
        isPause = true;
      }

      request->send(200, "application/json", "{\"status\": \"OK\"}"); 
    } else {
      request->send(400, "application/json", "{\"status\": \"Bad request\"}");
    } });

  server.on("/mode/timer", HTTP_POST, [](AsyncWebServerRequest *request)
            {
        lcd.clear();
        mainFunc = TimerMode;
        isPause = true;
        myBuzzer.beep(100);
        request->send(200, "application/json", "{\"status\": \"OK\"}"); });

  server.on("/mode/count", HTTP_POST, [](AsyncWebServerRequest *request)
            {
        lcd.clear();
        mainFunc = CountMode;
        isPause = true;
        myBuzzer.beep(100);
        request->send(200, "application/json", "{\"status\": \"OK\"}"); });

  server.on("/restart", HTTP_POST, [](AsyncWebServerRequest *request)
            {
          request->send(200, "application/json", "{\"status\": \"OK\"}");
          delay(500);
          ESP.restart(); });

  server.on("/ping", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "application/json", "{\"status\": \"OK\"}"); });

  server.onNotFound([](AsyncWebServerRequest *request)
                    { request->send(404, "application/json", "{\"status\": \"Not found\"}"); });

  server.begin();
  Serial.println("Server started");
}
