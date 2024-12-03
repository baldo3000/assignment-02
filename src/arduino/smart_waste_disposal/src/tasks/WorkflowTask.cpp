#include "WorkFlowTask.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"
#include "config.h"
#include "hardwareConfig.h"
#include <Arduino.h>
#include <avr/sleep.h>
#include <EnableInterrupt.h>

#define LOG_TAG "[WF] "

WorkflowTask::WorkflowTask(WasteDisposalSystem *pSystem, UserConsole *pUserConsole, WarningTask *pWarningTask) : pSystem(pSystem), pUserConsole(pUserConsole), pWarningTask(pWarningTask)
{
    setState(IDLE);
}

void WorkflowTask::setState(const State state)
{
    if (state == PREPARE_FOR_SLEEP)
    {
        this->prevState = this->state;
    }
    this->state = state;
    this->stateTimestamp = millis();
    this->justEntered = true;
}

void WorkflowTask::restorePrevState()
{
    setState(this->prevState);
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
        this->pWarningTask->setActive(false);
        this->pWarningTask->reset();
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
        if (elapsedTimeInState() > TIME_TO_SLEEP)
        {
            setState(PREPARE_FOR_SLEEP);
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
            this->pWarningTask->setActive(true);
            Logger.log(String(LOG_TAG) + "problem detected");
        }
        if (elapsedTimeInState() > TIME_TO_SLEEP)
        {
            setState(PREPARE_FOR_SLEEP);
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
        enableInterrupt(0, wakeUpNow, RISING);
        setState(SLEEP);
        break;

    case SLEEP:
        // Logger.log(String(LOG_TAG) + "sleeping");
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        delay(15);
        sleep_mode();
        delay(15);
        sleep_disable();
        setState(RESTORE_FROM_SLEEP);
        break;

    case RESTORE_FROM_SLEEP:
        Logger.log(String(LOG_TAG) + "restoring from sleep");
        this->pUserConsole->resumeFromSleeping();
        this->pSystem->resumeFromSleeping();
        disableInterrupt(PIR_PIN);
        disableInterrupt(0);
        restorePrevState();
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
            if (msg->getContent() == "Cemptied" || msg->getContent() == "emptied")
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
            if (msg->getContent() == "Creset" || msg->getContent() == "reset")
            {
                reset = true;
            }
            delete msg;
        }
    }
    return reset;
}