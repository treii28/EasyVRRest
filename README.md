# EasyVRRest
ESP32-AsyncWebServer based REST translation layer for the EasyVR

This code depends upon the existing Veeer EasyVR library and the main function call expects the request pointer from the incoming handler to be passed it so it can parse the GET parameters.

Called with the command in question (in my case, I'm extracting it off the end of the url)

```C++
void handleEasyVR(AsyncWebServerRequest *request) {
    // url should be in format: "/easyvr/command"
    const char* command = request->url().substring(8).c_str();
    String json = evRest.handleRequest(command, request);
}

// in http setup:
    httpserver.on("/easyvr", HTTP_GET, handleEasyVR);
```
