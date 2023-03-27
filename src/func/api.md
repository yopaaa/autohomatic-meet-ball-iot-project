## API Documentation

### Description
This API provides endpoints for controlling a device.

### Endpoints


## POST /wifi/set
Sets the WiFi credentials for the device.
#### Example Request:
```bash
POST /wifi/set HTTP/1.1
Host: <target ip>:3000
Content-Type: application/json

{
    "ssid": "your_ssid" ,
    "password": "your_password"
}
```
#### Example Response:
```bash
HTTP/1.1 200 OK
Content-Type: application/json

{
    "code": 200 ,
    "message": "OK" ,
    "url": "/wifi/set" ,
    "payload": {
        "ssid": "your_ssid"
    }
}
```
---

## POST /count
to start count mode
#### Example Request:
```bash
POST /count HTTP/1.1
Host: <target ip>:3000
Content-Type: application/json

{
    "target": 10
}
```
#### Example Response:
```bash
HTTP/1.1 200 OK
Content-Type: application/json

{
    "code": 200 ,
    "message": "OK" ,
    "url": "/count" ,
    "payload": {
        "target": 10
    }
}
```
---


## POST /timer
to start timer mode
#### Example Request:
```bash
POST /timer HTTP/1.1
Host: <target ip>:3000
Content-Type: application/json

{
    "targetM": 5 ,
    "targetS": 30
}
```
#### Example Response:
```bash
HTTP/1.1 200 OK
Content-Type: application/json

{
    "code": 200 ,
    "message": "OK" ,
    "url": "/timer" ,
    "payload": {
        "target": 330
    }
}
```
---


## POST /stop
Stops the start mode.
#### Example Request:
```bash
POST /stop HTTP/1.1
Host: <target ip>:3000
Content-Type: application/json

{
}
```
#### Example Response:
```bash
HTTP/1.1 200 OK
Content-Type: application/json

{
    "code": 200 ,
    "message": "OK" ,
    "url": "/stop" ,
    "payload": {
        "isPause": true
    }
}
```
---

## POST /objectCountDelay
Sets the infrared scan delay 
#### Example Request:
```bash
POST /objectCountDelay HTTP/1.1
Host: <target ip>:3000
Content-Type: application/json

{
    "delay": 10
}
```
#### Example Response:
```bash
HTTP/1.1 200 OK
Content-Type: application/json

{
    "code": 200 ,
    "message": "OK" ,
    "url": "/objectCountDelay",
    "payload": {
        "delay": 1000 ,
    }
}
```
---

## GET /ping
Pings the device to check if it is online.
#### Example Request:
```bash
GET /ping HTTP/1.1
Host: <target ip>:3000
```
#### Example Response:
```bash
HTTP/1.1 200 OK
Content-Type: application/json

{
    "code": 200
    "message": "OK"
    "url": "/ping"
    "payload": {}
}
```
---


## POST /restart
to restarts the device
#### Example Request:
```bash
POST /restart HTTP/1.1
Host: <target ip>:3000
```
#### Example Response:
```bash
HTTP/1.1 200 OK
Content-Type: application/json

{
    "status": "OK"
}
```
---

## POST /AsyncElegantOTA
to start AsyncElegantOTAs the device
#### Example Request:
```bash
POST /AsyncElegantOTA HTTP/1.1
Host: <target ip>:3000
```
#### Example Response:
```bash
HTTP/1.1 200 OK
Content-Type: application/json

{
    "status": "OK"
}
```
---
