## API Documentation

### Description
This API provides endpoints for controlling a device.

### Endpoints

#### `/wifi/set`
##### Method: POST
Sets the WiFi credentials for the device.
##### Request Parameters:
* `ssid` (required): The name of the WiFi network.
* `password` (required): The password for the WiFi network.

##### Response:
Returns a JSON object with the following fields:
* `code`: The HTTP status code (200 for success, 400 for bad request).
* `message`: A description of the result.
* `ssis`: The name of the WiFi network that was set.

#### `/count`
##### Method: POST
Sets the device to count mode.
##### Request Parameters:
* `target` (required): The target count.

##### Response:
Returns a JSON object with the following fields:
* `code`: The HTTP status code (200 for success, 400 for bad request).
* `message`: A description of the result.
* `target`: The target count that was set.

#### `/timer`
##### Method: POST
Sets the device to timer mode.
##### Request Parameters:
* `targetM` (required): The target minutes.
* `targetS` (required): The target seconds.

##### Response:
Returns a JSON object with the following fields:
* `code`: The HTTP status code (200 for success, 400 for bad request).
* `message`: A description of the result.
* `target`: The target time that was set.

#### `/stop`
##### Method: POST
Stops the device.
##### Request Parameters:
None.

##### Response:
Returns a JSON object with the following fields:
* `code`: The HTTP status code (200 for success, 400 for bad request).
* `message`: A description of the result.
* `isPause`: A boolean indicating whether the device is paused.

#### `/ping`
##### Method: GET
Pings the device to check if it is online.

##### Response:
Returns a JSON object with the following fields:
* `message`: A description of the result.
* `CycleCount`: The number of cycles since the device was last restarted.
* `ChipModel`: The model of the ESP chip.
* `SketchSize`: The size of the sketch.
* `version`: The version of the HTTP protocol.
* `method`: The HTTP method used.
* `url`: The URL that was requested.
* `host`: The host of the device.
* `contentType`: The content type of the request.
* `contentLength`: The length of the request content.
* `multipart`: A boolean indicating whether the request was multipart.

#### `/isPause`
##### Method: POST
Pauses or resumes the device.
##### Request Parameters:
* `val` (required): A boolean indicating whether to pause or resume the device.

##### Response:
Returns a JSON object with the following fields:
* `status`: A description of the result.

#### `/restart`
##### Method: POST
Restarts the device.

##### Response:
Returns a JSON object with the following fields:
* `status`: A description of the result.
