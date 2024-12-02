#include "CheckTask.h"
#include "kernel/Logger.h"
#include "config.h"
#include <Arduino.h>

CheckTask::CheckTask(WasteDisposalSystem *pSystem, UserConsole *pUserConsole) : pSystem(pSystem), pUserConsole(pUserConsole)
{
    setState(CHECKING_PROBLEMS);
}

void CheckTask::setState(const State state)
{
    this->state = state;
    this->stateTimestamp = millis();
    this->justEntered = true;
}

long CheckTask::elapsedTimeInState()
{
    return millis() - this->stateTimestamp;
}

void CheckTask::logOnce(const String &msg)
{
    if (this->justEntered)
    {
        Logger.log(msg);
        this->justEntered = false;
    }
}

void CheckTask::tick()
{
    switch (this->state)
    {
    case CHECKING_PROBLEMS:
        // logOnce("[CHK] checking problems");
        if (pSystem->getCurrentTemperature() >= OVERHEAT_THRESHOLD)
        {
            this->pSystem->overheated();
            setState(OVERHEATED);
        }
        else if (pSystem->getCurrentFullness() >= FULL_THRESHOLD)
        {
            this->pSystem->full();
            setState(FULL);
        }
        break;

    case OVERHEATED:
        // logOnce("[CHK] overheated");
        if (!this->pSystem->isOverheated())
        {
            setState(CHECKING_PROBLEMS);
        }
        break;

    case FULL:
        // logOnce("[CHK] full");
        if (!this->pSystem->isFull())
        {
            setState(CHECKING_PROBLEMS);
        }
        break;
    }
}
