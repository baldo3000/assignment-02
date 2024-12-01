#ifndef TELEMETRYTASK_H
#define TELEMETRYTASK_H

#include "kernel/Task.h"
#include "model/WasteDisposalSystem.h"
#include "model/UserConsole.h"

#define SENDING_DATA_TIMEOUT 500

class TelemetryTask : public Task
{

public:
    TelemetryTask(WasteDisposalSystem *pPlant, UserConsole *pUserConsole);
    void tick();

private:
    enum State
    {
        IDLE,
        SENDING_DATA
    } state;
    long stateTimestamp;
    bool justEntered;

    void setState(State state);
    long elapsedTimeInState();
    void logOnce(const String &msg);

    WasteDisposalSystem *pSystem;
    UserConsole *pUserConsole;
};

#endif