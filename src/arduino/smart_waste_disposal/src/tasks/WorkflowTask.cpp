#include "WorkFlowTask.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"
#include "config.h"
#include "hardwareConfig.h"
#include <Arduino.h>
#include <avr/sleep.h>
#include <EnableInterrupt.h>

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

void wakeUpNow() {}

void WorkflowTask::tick()
{
    switch (this->state)
    {
    case IDLE:
        logOnce("[WF] idle");
        this->pSystem->idle();
        this->pUserConsole->displayWelcome();
        this->pSystem->setLed1On(true);
        this->pSystem->setLed2On(false);
        setState(WAITING_FOR_USER);
        break;

    case WAITING_FOR_USER:
        logOnce("[WF] waiting for user");
        this->pSystem->waitForUser();
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
        logOnce("[WF] user detected");
        this->pSystem->userDetected();
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
        logOnce("[WF] door opening");
        this->pSystem->dispose();
        this->pSystem->openDoor();
        setState(DISPOSING);
        break;

    case DISPOSING:
        logOnce("[WF] disposing");
        if (elapsedTimeInState() > DISPOSE_MAX_TIME || pUserConsole->closeDoorSignal() || this->pSystem->isFull() || this->pSystem->isOverheated())
        {
            setState(DOOR_CLOSING);
        }
        break;

    case DOOR_CLOSING:
        logOnce("[WF] door closing");
        this->pSystem->closeDoor();
        if (this->pSystem->isFull() || this->pSystem->isOverheated())
        {
            setState(PROBLEM_DETECTED);
        }
        else
        {
            setState(WAITING_FOR_USER);
        }
        break;

    case PROBLEM_DETECTED:
        this->pSystem->setLed1On(false);
        this->pSystem->setLed2On(true);
        logOnce("[WF] problem detected");
        if (this->pSystem->isFull() && checkEmptyMsg())
        {
            setState(IDLE);
        }
        if (this->pSystem->isOverheated() && checkResetMsg())
        {
            setState(IDLE);
        }
        break;

    case PREPARE_FOR_SLEEP:
        logOnce("[WF] preparing for sleep");
        this->pUserConsole->prepareToSleep();
        this->pSystem->prepareToSleep();
        enableInterrupt(PIR_PIN, wakeUpNow, RISING);
        setState(SLEEP);
        break;

    case SLEEP:
        logOnce("[WF] going to sleep");
        // TODO: to be implemented
        delay(15);
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        sleep_mode();
        setState(RESTORE_FROM_SLEEP);
        break;

    case RESTORE_FROM_SLEEP:
        logOnce("[WF] restoring from sleep");
        this->pUserConsole->resumeFromSleeping();
        this->pSystem->resumeFromSleeping();
        disableInterrupt(PIR_PIN);
        setState(IDLE);
        break;
    }
}

bool WorkflowTask::checkEmptyMsg()
{
    bool empty = false;
    if (MsgService.isMsgAvailable())
    {
        Msg *msg = MsgService.receiveMsg();
        if (msg != NULL)
        {
            Logger.log(msg->getContent());
            if (msg->getContent() == "emptied")
            {
                empty = true;
            }
            delete msg;
        }
    }
    return empty;
}

bool WorkflowTask::checkResetMsg()
{
    bool reset = false;
    if (MsgService.isMsgAvailable())
    {
        Msg *msg = MsgService.receiveMsg();
        if (msg != NULL)
        {
            Logger.log(msg->getContent());
            if (msg->getContent() == "reset")
            {
                reset = true;
            }
            delete msg;
        }
    }
    return reset;
}