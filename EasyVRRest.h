//
// Created by swood on 12/22/18.
//
#if defined(ARDUINO)
#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif
#else
#include <Arduino.h>
#endif

#ifndef I2SDAC_EASYVRREST_H
#define I2SDAC_EASYVRREST_H

#pragma once

#ifndef _EASYVR_LIBRARY_H
#include "EasyVR.h"
#endif
#ifndef ARDUINOJSON_VERSION
#include <ArduinoJson.h>
#endif
#ifndef _ESPAsyncWebServer_H_
#include "ESPAsyncWebServer.h"
#endif

#include <Stream.h>
#include <stdint.h>
#include <stdio.h>
#include <string>

class EasyVRRest {
private:
    int error;
protected:
    Stream *_s; // communication interface for the EasyVR module
    EasyVR *_e; // EasyVR Object
    StaticJsonBuffer<200> jsonBuffer;// json buffer for input and output
public:
    enum parseErrors
    {
        PARSE_FAILED          = 1,
        PARSE_NO_COMMAND      = 2,
        PARSE_COMMAND_EMPTY   = 3,
        PARSE_COMMAND_INVALID = 4,
        PARAMETER_MISSING     = 5
    };

    EasyVRRest(EasyVR& e, Stream& s) : _e(&e), _s(&s) { };

    int getLastError() { return error; }
    bool parseBooleanParam(const char* val) { return ((val == "true") || (val == "True") || (val == "yes") || (val == "Yes") || (val == "y") || (val == "Y") || (val == "1")); };

    String handleRequest(const char* command, AsyncWebServerRequest *request);

    String paramMissingError(const char* param);
};


#endif //I2SDAC_EASYVRREST_H
