#include "HttpHandler.h"
#include <Arduino.h>
#include "../Var.h"
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <AsyncElegantOTA.h>
#include "buzzer.h"
#include "EEPROMFunc.h"
#include "LCDFunc.h"
#include "Mode.h"
#include <EEPROM.h>

AsyncWebServer server(3000);

void handleRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    if (index == 0)
    {
        DynamicJsonDocument jsonDoc(len);
        DeserializationError error = deserializeJson(jsonDoc, data);
        if (error)
        {
            request->send(400, "text/plain", "Bad Request");
            return;
        }

        DynamicJsonDocument json(1024);
        JsonObject payload = json.createNestedObject("payload");
        json["code"] = 200;
        json["message"] = "OK";
        // json["method"] = request->method();
        json["url"] = request->url();

        const String url = request->url();
        if (url == "/wifi/set")
        {
            if ((jsonDoc.containsKey("ssid")) && (jsonDoc.containsKey("password")))
            {
                String ssid = jsonDoc["ssid"].as<String>();
                String password = jsonDoc["password"].as<String>();

                saveWifiCredentials(ssid.c_str(), password.c_str());
                payload["ssid"] = ssid;
            }
            else
            {
                json["code"] = 400;
                json["message"] = "Bad Request";
            }
        }
        else if (url == "/count")
        {
            if ((jsonDoc.containsKey("target")))
            {
                SetMainFunc("count");
                isPause = false;

                blankLcdByRow(2);
                digitalWrite(relay1, LOW); // set on relay

                int target = jsonDoc["target"].as<int>();

                countTarget = target;
                payload["target"] = target;
            }
            else
            {
                json["code"] = 400;
                json["message"] = "Bad Request";
            }
        }
        else if (url == "/timer")
        {
            if ((jsonDoc.containsKey("targetM")) && (jsonDoc.containsKey("targetS")))
            {
                SetMainFunc("timer");
                startTime = millis();
                isPause = false;

                blankLcdByRow(2);
                digitalWrite(relay1, LOW); // set on relay

                int targetM = jsonDoc["targetM"].as<int>();
                int targetS = jsonDoc["targetS"].as<int>();

                timerTarget = (targetM * 60) + targetS;
                payload["target"] = timerTarget;
            }
            else
            {
                json["code"] = 400;
                json["message"] = "Bad Request";
            }
        }
        else if (url == "/stop")
        {
            // startTime = millis();
            isPause = true;
            digitalWrite(relay1, HIGH); // set off relay
            payload["isPause"] = isPause;
        }
        else if (url == "/objectCountDelay")
        {
            if ((jsonDoc.containsKey("delay")))
            {
                int delay = jsonDoc["delay"].as<int>();
                objectCountDelay = delay * 100;
                EEPROM.write(OBJECT_COUNT_DELAY_ADDRESS, delay);
                EEPROM.commit();
                payload["delay"] = delay * 100;
            }
            else
            {
                json["code"] = 400;
                json["message"] = "Bad Request";
            }
        }
        else
        {
            json["code"] = 404;
            json["message"] = "NOT FOUND";
        }

        String jsonString;
        serializeJson(json, jsonString);
        request->send(json["code"].as<int>(), "application/json", jsonString);
    }
}

void handlePing(AsyncWebServerRequest *request)
{
    DynamicJsonDocument json(1024);
    JsonObject payload = json.createNestedObject("payload");
    json["code"] = 200;
    json["message"] = "OK";
    // json["method"] = request->method();
    json["url"] = request->url();


    String jsonString;
    serializeJson(json, jsonString);
    request->send(200, "application/json", jsonString);
}

void httpHandler()
{
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "content-type");

    server.onRequestBody(handleRequest);
    server.on("/ping", HTTP_GET, handlePing);

    server.on("/restart", HTTP_POST, [](AsyncWebServerRequest *request)
              {
          request->send(200, "application/json", "{\"status\": \"OK\"}");
          delay(500);
          ESP.restart(); });

    server.onNotFound([](AsyncWebServerRequest *request)
                      { request->send(404, "application/json", "{\"status\": \"Not found\"}"); });

    AsyncElegantOTA.begin(&server, APssid, APpassword);
    server.begin();
    Serial.println("Server started");
}
