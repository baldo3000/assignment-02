#ifndef CHECKTASK_H
#define CHECKTASK_H

#include "kernel/Task.h"
#include "model/WasteDisposalSystem.h"
#include "model/UserConsole.h"

class CheckTask : public Task
{
private:
    enum State
    {
        CHECKING_PROBLEMS,
        OVERHEATED,
        FULL,
    } state;

    void setState(State state);
    long elapsedTimeInState();
    void logOnce(const String &msg);

    long stateTimestamp;
    long elapsedTimeOverheated;
    bool overHeated;
    bool justEntered;

    WasteDisposalSystem *pSystem;
    UserConsole *pUserConsole;

public:
    CheckTask(WasteDisposalSystem *pSystem, UserConsole *pUserConsole);
    void tick();
};

#endif