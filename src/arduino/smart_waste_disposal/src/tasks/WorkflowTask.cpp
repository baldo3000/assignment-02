#include "WorkFlowTask.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"
#include "config.h"
#include "hardwareConfig.h"
#include <Arduino.h>
#include <avr/sleep.h>
#include <EnableInterrupt.h>

#define LOG_TAG "[WF] "

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

bool WorkflowTask::doOnce()
{
    if (this->justEntered)
    {
        this->justEntered = false;
        return true;
    }
    return false;
}

void wakeUpNow() {}

void WorkflowTask::tick()
{
    switch (this->state)
    {
    case IDLE:
        Logger.log(String(LOG_TAG) + "idle");
        this->pSystem->idle();
        this->pUserConsole->displayWelcome();
        this->pSystem->setLed1On(true);
        this->pSystem->setLed2On(false);
        this->pSystem->closeDoor();
        setState(WAITING_FOR_USER);
        break;

    case WAITING_FOR_USER:
        if (doOnce())
        {
            Logger.log(String(LOG_TAG) + "waiting for user");
            this->pSystem->waitForUser();
            this->pUserConsole->displayWelcome();
            this->pUserConsole->displayReadyToDispose();
        }
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
        if (doOnce())
        {
            Logger.log(String(LOG_TAG) + "user detected");
            this->pSystem->userDetected();
            this->pUserConsole->displayReadyToDispose();
        }
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
        Logger.log(String(LOG_TAG) + "door opening");
        this->pSystem->dispose();
        this->pSystem->openDoor();
        this->pUserConsole->displayDisposing();
        setState(DISPOSING);
        break;

    case DISPOSING:
        if (doOnce())
        {
            Logger.log(String(LOG_TAG) + "disposing");
        }
        if (elapsedTimeInState() > DISPOSE_MAX_TIME || pUserConsole->closeDoorSignal() || this->pSystem->isFull() || this->pSystem->isOverheated())
        {
            setState(DOOR_CLOSING);
        }
        break;

    case DOOR_CLOSING:
        if (doOnce())
        {
            Logger.log(String(LOG_TAG) + "door closing");
            this->pSystem->closeDoor();
            this->pUserConsole->displayDisposingEnd();
        }
        if (this->pSystem->isFull())
        {
            setState(FULL);
            this->pUserConsole->displayFull();
        }
        else if (this->pSystem->isOverheated())
        {
            setState(PROBLEM_DETECTED);
            this->pUserConsole->displayProblem();
        }
        else
        {
            if (elapsedTimeInState() > RECYCLE_TIME)
            {
                setState(WAITING_FOR_USER);
            }
        }
        break;

    case FULL:
        if (doOnce())
        {
            this->pSystem->setLed1On(false);
            this->pSystem->setLed2On(true);
            Logger.log(String(LOG_TAG) + "full");
            this->pUserConsole->displayFull();
        }
        if (checkEmptyMsg())
        {
            setState(EMPTYING);
        }
        break;

    case EMPTYING:
        if (doOnce())
        {
            Logger.log(String(LOG_TAG) + "emptying");
            this->pSystem->openReverseDoor();
        }
        if (elapsedTimeInState() > EMPTYING_TIME)
        {
            setState(IDLE);
        }
        break;

    case PROBLEM_DETECTED:
        if (doOnce())
        {
            this->pSystem->setLed1On(false);
            this->pSystem->setLed2On(true);
            Logger.log(String(LOG_TAG) + "problem detected");
        }
        if (checkResetMsg())
        {
            setState(IDLE);
        }
        break;

    case PREPARE_FOR_SLEEP:
        Logger.log(String(LOG_TAG) + "preparing for sleep");
        this->pUserConsole->prepareToSleep();
        this->pSystem->prepareToSleep();
        enableInterrupt(PIR_PIN, wakeUpNow, RISING);
        setState(SLEEP);
        break;

    case SLEEP:
        Logger.log(String(LOG_TAG) + "going to sleep");
        // TODO: to be implemented
        delay(100);
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        sleep_mode();
        setState(RESTORE_FROM_SLEEP);
        break;

    case RESTORE_FROM_SLEEP:
        Logger.log(String(LOG_TAG) + "restoring from sleep");
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
            // Logger.log(msg->getContent());
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
            // Logger.log(msg->getContent());
            if (msg->getContent() == "reset")
            {
                reset = true;
            }
            delete msg;
        }
    }
    return reset;
}