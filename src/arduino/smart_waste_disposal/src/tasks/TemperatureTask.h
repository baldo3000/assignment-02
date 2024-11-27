#ifndef TEMPERATURETASK_H
#define TEMPERATURETASK_H

#include "kernel/Task.h"
#include "model/WasteDisposalSystem.h"

class TemperatureTask : public Task
{
private:
    WasteDisposalSystem *pSystem;

public:
    TemperatureTask(WasteDisposalSystem *pSystem);
    void tick();
};

#endif