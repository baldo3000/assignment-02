#include "TemperatureTask.h"

TemperatureTask::TemperatureTask(WasteDisposalSystem *pSystem) : pSystem(pSystem) {}

void TemperatureTask::tick()
{
    pSystem->sampleTemperature();
}