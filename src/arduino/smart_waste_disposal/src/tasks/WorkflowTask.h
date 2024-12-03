#ifndef WORKFLOWTASK_H
#define WORKFLOWTASK_H

#include "kernel/Task.h"
#include "model/WasteDisposalSystem.h"
#include "model/UserConsole.h"
#include "WarningTask.h"

class WorkflowTask : public Task
{
private:
    enum State
    {
        IDLE,
        WAITING_FOR_USER,
        USER_DETECTED,
        PREPARE_FOR_SLEEP,
        SLEEP,
        RESTORE_FROM_SLEEP,
        DOOR_OPENING,
        DISPOSING,
        DOOR_CLOSING,
        FULL,
        EMPTYING,
        PROBLEM_DETECTED
    } state;

    void setState(State state);
    void restorePrevState();
    long elapsedTimeInState();
    bool doOnce();
    bool checkEmptyMsg();
    bool checkResetMsg();

    State prevState;
    long stateTimestamp;
    bool justEntered;

    WasteDisposalSystem *pSystem;
    UserConsole *pUserConsole;
    WarningTask *pWarningTask;

public:
    WorkflowTask(WasteDisposalSystem *pSystem, UserConsole *pUserConsole, WarningTask *pWarningTask);
    void tick();
};

#endif