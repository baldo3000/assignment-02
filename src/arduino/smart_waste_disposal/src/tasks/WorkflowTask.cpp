#include "WorkFlowTask.h"
#include "kernel/Logger.h"
#include "config.h"
#include <Arduino.h>

WorkflowTask::WorkflowTask(WasteDisposalSystem *pSystem, UserConsole *pUserConsole) : pSystem(pSystem), pUserConsole(pUserConsole)
{
    setState(IDLE);
}

void WorkflowTask::setState(const State state)
{
    this->state = state;
    this->stateTimestamp = millis();
    this->justEntered = true;
}

long WorkflowTask::elapsedTimeInState()
{
    return millis() - this->stateTimestamp;
}

void WorkflowTask::logOnce(const String &msg)
{
    if (this->justEntered)
    {
        Logger.log(msg);
        this->justEntered = false;
    }
}

void WorkflowTask::tick()
{
    switch (this->state)
    {
    case IDLE:
        logOnce("IDLE");
        setState(WAITING_FOR_USER);
        break;

    case WAITING_FOR_USER:
        logOnce("WAITING_FOR_USER");
        if (elapsedTimeInState() > TIME_TO_SLEEP)
        {
            setState(PREPARE_FOR_SLEEP);
        }
        else if (pSystem->detectedUserPresence())
        {
            setState(USER_DETECTED);
        }
        break;

    case USER_DETECTED:
        logOnce("USER_DETECTED");
        if (!pSystem->detectedUserPresence())
        {
            setState(WAITING_FOR_USER);
        }
        else if (pUserConsole->openDoorSignal())
        {
            setState(DOOR_OPENING);
        }
        break;

    case DOOR_OPENING:
        logOnce("DOOR_OPENING");
        this->pSystem->openDoor();
        setState(DISPOSING);
        break;

    case DISPOSING:
        logOnce("DISPOSING");
        if (elapsedTimeInState() > DISPOSE_MAX_TIME || pUserConsole->closeDoorSignal() || this->pSystem->isInMaintenance())
        {
            setState(DOOR_CLOSING);
        }
        break;

    case DOOR_CLOSING:
        logOnce("DOOR_CLOSING");
        this->pSystem->closeDoor();
        if (this->pSystem->isInMaintenance())
        {
            setState(PROBLEM_DETECTED);
        }
        else
        {
            setState(WAITING_FOR_USER);
        }
        break;

    case PROBLEM_DETECTED:
        logOnce("PROBLEM_DETECTED");
        break;

    case PREPARE_FOR_SLEEP:
        logOnce("PREPARE_FOR_SLEEP");
        this->pUserConsole->prepareToSleep();
        break;

    case SLEEP:
        logOnce("SLEEP");
        // TODO: to be implemented
        Serial.println("Faking sleeping...");
        break;

    case RESTORE_FROM_SLEEP:
        logOnce("RESTORE_FROM_SLEEP");
        this->pUserConsole->resumeFromSleeping();
        break;
    }
}