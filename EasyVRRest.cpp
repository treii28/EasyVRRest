//
// Created by swood on 12/22/18.
//

#include "EasyVRRest.h"

#ifndef _EASYVR_LIBRARY_H
#include "EasyVR.h"
#endif

String EasyVRRest::paramMissingError(const char* param) {
    JsonObject &error = jsonBuffer.createObject();
    error["type"] = parseErrors::PARAMETER_MISSING;
    error["message"] = sprintf("'%s' parameter missing", param);
    String errorJson;
    error.printTo(errorJson);
    return errorJson;
}

String EasyVRRest::handleRequest(const char* command, AsyncWebServerRequest *request) {
    JsonObject &response = jsonBuffer.createObject();

    if (strcmp(command, "") == 0) {
        response["error"] = jsonBuffer.createObject();
        response["error"]["type"] = parseErrors::PARSE_COMMAND_EMPTY;
        response["error"]["message"] = "Command Parameter Empty";
        goto createJson;
    }

    response["command"] = command;

    // parameter-less functions
    if (strcmp(command, "detect") == 0)
        response["result"] = _e->detect();
    else if (strcmp(command, "stop") == 0)
        response["result"] = _e->stop();
    else if (strcmp(command, "getID") == 0)
        response["ID"] = _e->getID();
    else if (strcmp(command, "hasFinished") == 0)
        response["isFinished"] = _e->hasFinished();
    else if (strcmp(command, "getCommand") == 0)
        response["commandIndex"] = _e->getCommand();
    else if (strcmp(command, "getWord") == 0)
        response["commandIndex"] = _e->getWord();
    else if (strcmp(command, "getToken") == 0)
        response["commandIndex"] = _e->getToken();
    else if (strcmp(command, "getError") == 0)
        response["errorCode"] = _e->getError();
    else if (strcmp(command, "isTimeout") == 0)
        response["isTimeout"] = _e->isTimeout();
    else if (strcmp(command, "isAwakened") == 0)
        response["isAwakened"] = _e->isAwakened();
    else if (strcmp(command, "isConflict") == 0)
        response["isConflict"] = _e->isConflict();
    else if (strcmp(command, "isMemoryFull") == 0)
        response["isMemoryFull"] = _e->isMemoryFull();
    else if (strcmp(command, "isInvalid") == 0)
        response["isInvalid"] = _e->isMemoryFull();
    else if (strcmp(command, "checkMessages") == 0)
        response["result"] = _e->checkMessages();
    else if (strcmp(command, "getGrammarsCount") == 0)
        response["count"] = _e->getGrammarsCount();

    else if (strcmp(command, "dumpSoundTable") == 0)
    {
        char* name;
        int16_t count;
        bool res = _e->dumpSoundTable(name,count);
        response["result"] = res;
        if(res) {
            response["name"] = name;
            response["count"] = count;
        }
    }
    else if (strcmp(command, "fetchMouthPosition") == 0)
    {
        int8_t value;
        bool res = _e->fetchMouthPosition(value);
        response["result"] = res;
        if(res) response["value"] = value;
    }

    else if (strcmp(command, "getNextWordLabel") == 0) {
        char *name;
        bool res = _e->getNextWordLabel(name);
        response["result"] = res;
        if (res) response["name"] = name;
    }
    else if (strcmp(command, "getGroupMask") == 0) {
        uint32_t mask;
        bool res = _e->getGroupMask(mask);
        response["result"] = res;
        if (res) response["mask"] = mask;
    }

    // unsupported functions
    else if (strcmp(command, "exportCommand") == 0)
        response["error"] = "Not yet supported";
    else if (strcmp(command, "importCommand") == 0)
        response["error"] = "Not yet supported";
    else if (strcmp(command, "bridgeRequested") == 0)
        response["error"] = "Not supported";
    else if (strcmp(command, "bridgeLoop") == 0)
        response["error"] = "Not supported";

    // commands with parameters in json data
    //   we only need to parse if not one of the above commands
    else if (strcmp(command, "setLanguage") == 0) {
        if (request->hasParam("lang")) {
            int8_t lang = request->getParam("lang")->value().toInt();
            response["result"] = _e->setLanguage(lang);
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("lang"));
    } else if (strcmp(command, "setTimeout") == 0) {
        if (request->hasParam("seconds")) {
            int8_t seconds = request->getParam("seconds")->value().toInt();
            response["result"] = _e->setTimeout(seconds);
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("seconds"));
    } else if (strcmp(command, "setMicDistance") == 0) {
        if (request->hasParam("dist")) {
            int8_t dist = request->getParam("dist")->value().toInt();
            response["result"] = _e->setMicDistance(dist);
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("dist"));
    } else if (strcmp(command, "setKnob") == 0) {
        if (request->hasParam("knob")) {
            int8_t knob = request->getParam("knob")->value().toInt();
            response["result"] = _e->setKnob(knob);
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("knob"));
    } else if (strcmp(command, "setTrailingSilence") == 0) {
        if (request->hasParam("dur")) {
            int8_t dur = request->getParam("dur")->value().toInt();
            response["result"] = _e->setTrailingSilence(dur);
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("dur"));
    } else if (strcmp(command, "setLevel") == 0) {
        if (request->hasParam("level")) {
            int8_t level = request->getParam("level")->value().toInt();
            response["result"] = _e->setLevel(level);
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("level"));
    } else if (strcmp(command, "setCommandLatency") == 0) {
        if (request->hasParam("mode")) {
            int8_t mode = request->getParam("mode")->value().toInt();
            response["result"] = _e->setCommandLatency(mode);
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("mode"));
    } else if (strcmp(command, "setDelay") == 0) {
        if (request->hasParam("millis")) {
            uint16_t millis = request->getParam("millis")->value().toInt();
            response["result"] = _e->setDelay(millis);
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("millis"));
    } else if (strcmp(command, "changeBaudrate") == 0) {
        if (request->hasParam("baud")) {
            int8_t baud = request->getParam("baud")->value().toInt();
            response["result"] = _e->changeBaudrate(baud);
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("baud"));
    } else if (strcmp(command, "sleep") == 0) {
        if (request->hasParam("mode")) {
            int8_t mode = request->getParam("mode")->value().toInt();
            response["result"] = _e->sleep(mode);
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("mode"));
    } else if (strcmp(command, "addCommand") == 0) {
        if (request->hasParam("group")) {
            int8_t group = request->getParam("group")->value().toInt();
            if (request->hasParam("index")) {
                int8_t index = request->getParam("index")->value().toInt();
                response["result"] = _e->addCommand(group, index);
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("index"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("group"));
    } else if (strcmp(command, "removeCommand") == 0) {
        if (request->hasParam("group")) {
            int8_t group = request->getParam("group")->value().toInt();
            if (request->hasParam("index")) {
                int8_t index = request->getParam("index")->value().toInt();
                response["result"] = _e->removeCommand(group, index);
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("index"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("group"));
    } else if (strcmp(command, "setCommandLabel") == 0) {
        if (request->hasParam("group")) {
            int8_t group = request->getParam("group")->value().toInt();
            if (request->hasParam("index")) {
                int8_t index = request->getParam("index")->value().toInt();
                if (request->hasParam("name")) {
                    const char *name = request->getParam("name")->value().c_str();
                    response["result"] = _e->setCommandLabel(group, index, name);
                } else response["error"] = jsonBuffer.parseObject(paramMissingError("name"));
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("index"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("group"));
    } else if (strcmp(command, "eraseCommand") == 0) {
        if (request->hasParam("group")) {
            int8_t group = request->getParam("group")->value().toInt();
            if (request->hasParam("index")) {
                int8_t index = request->getParam("index")->value().toInt();
                response["result"] = _e->eraseCommand(group, index);
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("index"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("group"));
    }
    else if (strcmp(command, "getCommandCount") == 0) {
        if (request->hasParam("group")) {
            int8_t group = request->getParam("group")->value().toInt();
            response["count"] = _e->getCommandCount(group);
        }
        response["error"] = jsonBuffer.parseObject(paramMissingError("group"));
    }
    else if (strcmp(command, "dumpCommand") == 0) {
        if (request->hasParam("group")) {
            int8_t group = request->getParam("group")->value().toInt();
            if (request->hasParam("index")) {
                int8_t index = request->getParam("index")->value().toInt();
                char *name;
                uint8_t training;
                bool res = _e->dumpCommand(group, index, name, training);
                response["result"] = res;
                if (res) {
                    response["name"] = name;
                    response["training"] = training;io
                }
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("index"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("group"));
    }
    else if (strcmp(command, "dumpGrammar") == 0) {
        if (request->hasParam("grammar")) {
            int8_t grammar = request->getParam("grammar")->value().toInt();
            uint8_t flags;
            uint8_t count;
            bool res = _e->dumpGrammar(grammar, flags, count);
            response["result"] = res;
            if (res) {
                response["flags"] = flags;
                response["count"] = count;
            }
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("grammar"));
    }
    else if (strcmp(command, "trainCommand") == 0) {
        if (request->hasParam("group")) {
            int8_t group = request->getParam("group")->value().toInt();
            if (request->hasParam("index")) {
                int8_t index = request->getParam("index")->value().toInt();
                _e->trainCommand(group, index);
                response["result"] = "Training started";
                response["isFinished"] = false;
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("index"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("group"));

    }
    else if (strcmp(command, "recognizeCommand") == 0) {
        if (request->hasParam("group")) {
            int8_t group = request->getParam("group")->value().toInt();
            _e->recognizeCommand(group);
            response["result"] = "Recognition started";
            response["isFinished"] = false;
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("group"));

    }
    else if (strcmp(command, "recognizeWord") == 0) {
        if (request->hasParam("wordset")) {
            int8_t wordset = request->getParam("wordset")->value().toInt();
            _e->recognizeWord(wordset);
            response["result"] = "Recognition started";
            response["isFinished"] = false;
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("wordset"));

    }
    else if (strcmp(command, "setPinOutput") == 0) {
        if (request->hasParam("pin")) {
            int8_t pin = request->getParam("pin")->value().toInt();
            if (request->hasParam("value")) {
                int8_t value = request->getParam("value")->value().toInt();
                response["result"] = _e->setPinOutput(pin, value);
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("value"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("pin"));

    }
    else if (strcmp(command, "getPinInput") == 0) {
        if (request->hasParam("pin")) {
            int8_t pin = request->getParam("pin")->value().toInt();
            if (request->hasParam("config")) {
                int8_t config = request->getParam("config")->value().toInt();
                response["value"] = _e->getPinInput(pin, config);
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("config"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("pin"));

    }
    else if (strcmp(command, "detectToken") == 0) {
        if (request->hasParam("pin")) {
            int8_t pin = request->getParam("pin")->value().toInt();
            if (request->hasParam("rejection")) {
                int8_t rejection = request->getParam("rejection")->value().toInt();
                if (request->hasParam("timeout")) {
                    uint16_t timeout = request->getParam("timeout")->value().toInt();
                    _e->detectToken(pin, rejection, timeout);
                    response["result"] = "listening";
                } else response["error"] = jsonBuffer.parseObject(paramMissingError("timeout"));
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("rejection"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("pin"));
    }
    else if (strcmp(command, "sendTokenAsync") == 0) {
        if (request->hasParam("bits")) {
            int8_t bits = request->getParam("bits")->value().toInt();
            if (request->hasParam("token")) {
                uint8_t token = request->getParam("token")->value().toInt();
                _e->sendTokenAsync(bits, token);
                response["result"] = "starting playback";
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("token"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("bits"));
    }
    else if (strcmp(command, "sendToken") == 0) {
        if (request->hasParam("bits")) {
            int8_t bits = request->getParam("bits")->value().toInt();
            if (request->hasParam("token")) {
                uint8_t token = request->getParam("token")->value().toInt();
                _e->sendToken(bits, token);
                response["result"] = "playing token";
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("token"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("bits"));
    }
    else if (strcmp(command, "embedToken") == 0) {
        if (request->hasParam("bits")) {
            int8_t bits = request->getParam("bits")->value().toInt();
            if (request->hasParam("token")) {
                uint8_t token = request->getParam("token")->value().toInt();
                if (request->hasParam("delay")) {
                    uint16_t delay = request->getParam("delay")->value().toInt();
                    _e->embedToken(bits, token, delay);
                    response["result"] = "scheduling playback";
                } else response["error"] = jsonBuffer.parseObject(paramMissingError("delay"));
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("token"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("bits"));
    }
    else if (strcmp(command, "playSoundAsync") == 0) {
        if (request->hasParam("index")) {
            uint16_t index = request->getParam("index")->value().toInt();
            if (request->hasParam("volume")) {
                int8_t volume = request->getParam("volume")->value().toInt();
                _e->playSoundAsync(index, volume);
                response["result"] = "starting playback";
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("volume"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("index"));
    }
    else if (strcmp(command, "playSound") == 0) {
        if (request->hasParam("index")) {
            uint16_t index = request->getParam("index")->value().toInt();
            if (request->hasParam("volume")) {
                int8_t volume = request->getParam("volume")->value().toInt();
                _e->playSound(index, volume);
                response["result"] = "playing sound";
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("volume"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("index"));
    }
    else if (strcmp(command, "playPhoneTone") == 0)
    {
        if (request->hasParam("tone")) {
            int8_t tone = request->getParam("tone")->value().toInt();
            if (request->hasParam("duration")) {
                uint8_t duration = request->getParam("duration")->value().toInt();
                response["result"] = _e->playPhoneTone(tone, duration);
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("duration"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("tone"));
    }
    else if (strcmp(command, "resetAll") == 0)
    {
        if (request->hasParam("wait")) {
            bool wait = parseBooleanParam(request->getParam("wait")->value().c_str());
            response["result"] = _e->resetAll(wait);
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("wait"));
    }
    else if (strcmp(command, "resetCommands") == 0)
    {
        if (request->hasParam("wait")) {
            bool wait = parseBooleanParam(request->getParam("wait")->value().c_str());
            response["result"] = _e->resetCommands(wait);
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("wait"));
    }
    else if (strcmp(command, "resetMessages") == 0)
    {
        if (request->hasParam("wait")) {
            bool wait = parseBooleanParam(request->getParam("wait")->value().c_str());
            response["result"] = _e->resetMessages(wait);
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("wait"));
    }
    else if (strcmp(command, "fixMessages") == 0)
    {
        if (request->hasParam("wait")) {
            bool wait = parseBooleanParam(request->getParam("wait")->value().c_str());
            response["result"] = _e->fixMessages(wait);
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("wait"));
    }
    else if (strcmp(command, "recordMessageAsync") == 0)
    {
        if (request->hasParam("index")) {
            int8_t index = request->getParam("index")->value().toInt();
            if (request->hasParam("bits")) {
                int8_t bits = request->getParam("bits")->value().toInt();
                if (request->hasParam("timeout")) {
                    int8_t timeout = request->getParam("timeout")->value().toInt();
                    _e->recordMessageAsync(index, bits, timeout);
                    response["result"] = "recording started";
                } else response["error"] = jsonBuffer.parseObject(paramMissingError("timeout"));
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("bits"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("index"));
    }
    else if (strcmp(command, "playMessageAsync") == 0)
    {
        if (request->hasParam("index")) {
            int8_t index = request->getParam("index")->value().toInt();
            if (request->hasParam("speed")) {
                int8_t speed = request->getParam("speed")->value().toInt();
                if (request->hasParam("atten")) {
                    int8_t atten = request->getParam("atten")->value().toInt();
                    _e->playMessageAsync(index, speed, atten);
                    response["result"] = "playback started";
                } else response["error"] = jsonBuffer.parseObject(paramMissingError("atten"));
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("speed"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("index"));
    }
    else if (strcmp(command, "eraseMessageAsync") == 0)
    {
        if (request->hasParam("index")) {
            int8_t index = request->getParam("index")->value().toInt();
            _e->eraseMessageAsync(index);
            response["result"] = "erasing message";
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("index"));
    }
    else if (strcmp(command, "dumpMessage") == 0)
    {
        if (request->hasParam("index")) {
            int8_t index = request->getParam("index")->value().toInt();
            int8_t type;
            int32_t length;
            bool res =_e->dumpMessage(index, type, length);
            response["result"] = res;
            response["type"] = type;
            response["length"] = length;
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("index"));
    }
    else if (strcmp(command, "realtimeLipsync") == 0)
    {
        if (request->hasParam("threshold")) {
            int16_t threshold = request->getParam("threshold")->value().toInt();
            if (request->hasParam("timeout")) {
                uint8_t timeout = request->getParam("timeout")->value().toInt();
                response["result"] = _e->realtimeLipsync(threshold, timeout);
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("timeout"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("threshold"));
    }
    else if (strcmp(command, "verifyCommand") == 0)
    {
        if (request->hasParam("group")) {
            int8_t group = request->getParam("group")->value().toInt();
            if (request->hasParam("index")) {
                int8_t index = request->getParam("index")->value().toInt();
                _e->verifyCommand(group,index);
                response["result"] = "verifying";
                response["isFinished"] = false;
            } else response["error"] = jsonBuffer.parseObject(paramMissingError("index"));
        } else response["error"] = jsonBuffer.parseObject(paramMissingError("group"));
    }
    else
    {
        response["error"] = jsonBuffer.createObject();
        response["error"]["type"] = parseErrors::PARSE_COMMAND_INVALID;
        response["error"]["message"] = "Invalid Command Parameter";
        goto createJson;
    }

    createJson:
    String responseJson;
    response.printTo(responseJson);
    return responseJson;
}

