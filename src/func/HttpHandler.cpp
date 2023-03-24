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

AsyncWebServer server(3000);

void handleRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    // Check if this is the first data packet
    if (index == 0)
    {
        // Create a DynamicJsonDocument object with the same size as the incoming JSON data
        DynamicJsonDocument jsonDoc(len);
        // Parse the incoming JSON data into the DynamicJsonDocument object
        DeserializationError error = deserializeJson(jsonDoc, data);
        // Check if the JSON parsing was successful
        if (error)
        {
            request->send(400, "text/plain", "Bad Request");
            return;
        }

        DynamicJsonDocument json(1024);
        // json["name"] = jsonDoc["name"].as<String>();
        // json["age"] = jsonDoc["age"].as<int>();
        json["code"] = 200;
        json["message"] = "OK";
        json["version"] = request->version();
        json["method"] = request->method();
        json["url"] = request->url();
        json["host"] = request->host();
        json["contentType"] = request->contentType();
        json["contentLength"] = request->contentLength();
        json["multipart"] = request->multipart();

        const String url = request->url();
        if (url == "/led2")
        {
            json["pesan"] = jsonDoc["pesan"].as<String>();
            // LED2_State = !LED2_State;
            // digitalWrite(2, LED2_State);
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
    json["message"] = "OK";
    // json["CodeVersion"] = CodeVersion;
    json["CycleCount"] = ESP.getCycleCount();
    json["ChipModel"] = ESP.getChipModel();
    json["SketchSize"] = ESP.getSketchSize();
    json["version"] = request->version();
    json["method"] = request->method();
    json["url"] = request->url();
    json["host"] = request->host();
    json["contentType"] = request->contentType();
    json["contentLength"] = request->contentLength();
    json["multipart"] = request->multipart();

    String jsonString;
    serializeJson(json, jsonString);

    request->send(200, "application/json", jsonString);
}
// WEB SERVER FUNCTION
void httpHandler()
{
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "content-type");

    server.onRequestBody(handleRequest);
    server.on("/ping", HTTP_GET, handlePing);

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
        ClearLcd();
        SetMainFunc("timer");
            // mainFunc = TimerMode;
        isPause = true;
        myBuzzer.beep(100);
        request->send(200, "application/json", "{\"status\": \"OK\"}"); });

    server.on("/mode/count", HTTP_POST, [](AsyncWebServerRequest *request)
              {
        ClearLcd();
        SetMainFunc("count");
        // mainFunc = CountMode;
        isPause = true;
        myBuzzer.beep(100);
        request->send(200, "application/json", "{\"status\": \"OK\"}"); });

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
