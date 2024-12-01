#include "tasks/TelemetryTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"

TelemetryTask::TelemetryTask(WasteDisposalSystem *pSystem, UserConsole *pUserConsole) : pSystem(pSystem), pUserConsole(pUserConsole)
{
    setState(IDLE);
}

void TelemetryTask::tick()
{
    switch (this->state)
    {
    case IDLE:
    {
        // logOnce("[TT] idle");
        if (elapsedTimeInState() > SENDING_DATA_TIMEOUT)
        {
            setState(SENDING_DATA);
        }
        break;
    }
    case SENDING_DATA:
    {
        // logOnce("[TT] sending data");
        int stateCode = 0;

        if (this->pSystem->isIdle())
        {
            stateCode = 0;
        }
        else if (this->pSystem->isWaitingForUser())
        {
            stateCode = 1;
        }
        else if (this->pSystem->isUserDetected())
        {
            stateCode = 2;
        }
        else if (this->pSystem->isDisposing())
        {
            stateCode = 3;
        }
        else if (this->pSystem->isFull())
        {
            stateCode = 4;
        }
        else if (this->pSystem->isOverheated())
        {
            stateCode = 5;
        }

        const double fullness = pSystem->getCurrentFullness();
        const double currentTemp = pSystem->getCurrentTemperature();

        MsgService.sendMsg(String("wd:st:") + String(stateCode) + ":" + String(fullness) + ":" + String(currentTemp));
        setState(IDLE);
        break;
    }
    }
}

void TelemetryTask::setState(const State s)
{
    this->state = s;
    this->stateTimestamp = millis();
    this->justEntered = true;
}

long TelemetryTask::elapsedTimeInState()
{
    return millis() - this->stateTimestamp;
}

void TelemetryTask::logOnce(const String &msg)
{
    if (this->justEntered)
    {
        Logger.log(msg);
        this->justEntered = false;
    }
}