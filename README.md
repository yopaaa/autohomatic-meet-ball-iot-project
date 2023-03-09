
# Documentation
Please make sure to update the documentation when you add or modify code. This includes adding comments to your code, updating the README file, and providing examples if appropriate.

# API Documentation

This document provides a description of the available API endpoints for the server.

---

## POST /wifi/set
This endpoint sets the Wi-Fi credentials for the device. The request must contain the ssid and password parameters. If the values are successfully saved, the response will have a status code of 200 and a JSON object with a "status": "OK" message. If the values are not provided, the response will have a status code of 400 and a JSON object with a "status": "Bad request" message.

#### Example Request:
```bash
POST /wifi/set HTTP/1.1
Host: localhost
Content-Type: application/json

{
  "ssid": "wifi_network",
  "password": "secret_password"
}
```

#### Example Response:
```bash
HTTP/1.1 200 OK
Content-Type: application/json

{"status": "OK"}
```
---

## POST /isPause
This endpoint sets the pause state of the device. The request must contain the val parameter, which should be a boolean value ("true" or "false"). If the value is "true", the device will set the isPause variable to false, activate the buzzer for 100ms, reset the timer, clear the LCD display, and turn on the relay. If the value is "false", the device will set the isPause variable to true. The response will have a status code of 200 and a JSON object with a "status": "OK" message. If the val parameter is not provided, the response will have a status code of 400 and a JSON object with a "status": "Bad request" message.

#### Example Request:
```bash
POST /isPause HTTP/1.1
Host: localhost
Content-Type: application/json

{
  "val": "true"
}
```

#### Example Response:
```bash
HTTP/1.1 200 OK
Content-Type: application/json

{"status": "OK"}
```
---

## POST /mode/timer
This endpoint sets the mode of the device to timer mode. The device will clear the LCD display, set the mainFunc variable to TimerMode, and set the isPause variable to true. The response will have a status code of 200 and a JSON object with a "status": "OK" message.

#### Example Request:

```bash
POST /mode/timer HTTP/1.1
Host: localhost
Content-Type: application/json
```
#### Example Response:
```bash
Copy code
HTTP/1.1 200 OK
Content-Type: application/json

{"status": "OK"}
```
---

## POST /mode/count
This endpoint sets the mode of the device to count mode. The device will clear the LCD display, set the mainFunc variable to CountMode, and set the isPause variable to true. The response will have a status code of 200 and a JSON object with a "status": "OK" message.

#### Example Request:
```bash
POST /mode/count HTTP/1.1
Host: localhost
Content-Type: application/json
```
#### Example Response:
```bash
Copy code
HTTP/1.1 200 OK
Content-Type: application/json

{"status": "OK"}
```

---

## POST /restart
This endpoint restarts the device. The response will have a status code of 200 and a JSON object with a "status": "OK" message.

#### Example Request:
```bash
POST /restart HTTP/1.1
Host: localhost
Content-Type: application/json
```
#### Example Response:
```bash
HTTP/1.1 200 OK
Content-Type: application/json

{"status": "OK"}
```


# Contributing Guidelines
Thank you for considering contributing to our project! We appreciate your time and effort. To ensure a smooth and effective collaboration, please take a moment to review these guidelines before you start.

# Code of Conduct
Our project follows the Contributor Covenant Code of Conduct. By participating, you are expected to uphold this code. Please report any unacceptable behavior to the project maintainers.

# How to Contribute
## Reporting Issues
If you find a bug, have a feature request, or encounter any other issue, please report it in the issue tracker with a detailed description of the problem, including any relevant logs, screenshots, and steps to reproduce the issue. Please make sure to search existing issues before creating a new one.


# License
By contributing to our project, you agree to license your contribution under the same license as the project.