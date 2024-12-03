#ifndef WORKFLOWTASK_H
#define WORKFLOWTASK_H

#include "kernel/Task.h"
#include "model/WasteDisposalSystem.h"
#include "model/UserConsole.h"

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
    long elapsedTimeInState();
    void logOnce(const String &msg);
    bool checkEmptyMsg();
    bool checkResetMsg();

    long stateTimestamp;
    bool justEntered;

    WasteDisposalSystem *pSystem;
    UserConsole *pUserConsole;

public:
    WorkflowTask(WasteDisposalSystem *pSystem, UserConsole *pUserConsole);
    void tick();
};

#endif