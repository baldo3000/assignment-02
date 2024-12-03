#ifndef WARNINGTASK_H
#define WARNINGTASK_H

#include "kernel/Task.h"
#include "model/WasteDisposalSystem.h"

#define WARNING_TONE 2500U

class WarningTask : public Task
{
private:
    enum State
    {
        PLAYING,
        MUTED
    } state;
    bool isPlayingSound;
    WasteDisposalSystem *pSystem;

public:
    WarningTask(WasteDisposalSystem *pSystem);
    void tick();
};

#endif