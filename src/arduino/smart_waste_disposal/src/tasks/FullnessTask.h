#ifndef FULLNESSTASK_H
#define FULLNESSTASK_H

#include "kernel/Task.h"
#include "model/WasteDisposalSystem.h"

class FullnessTask : public Task
{
private:
    WasteDisposalSystem *pSystem;

public:
    FullnessTask(WasteDisposalSystem *pSystem);
    void tick();
};

#endif