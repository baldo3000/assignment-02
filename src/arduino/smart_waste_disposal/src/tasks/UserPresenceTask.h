#ifndef USERPRESENCETASK_H
#define USERPRESENCETASK_H

#include "kernel/Task.h"
#include "model/WasteDisposalSystem.h"

class UserPresenceTask : public Task
{
private:
    WasteDisposalSystem *pSystem;

public:
    UserPresenceTask(WasteDisposalSystem *pSystem);
    void tick();
};

#endif