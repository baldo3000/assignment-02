#include "FullnessTask.h"

FullnessTask::FullnessTask(WasteDisposalSystem *pSystem) : pSystem(pSystem) {}

void FullnessTask::tick()
{
    pSystem->sampleFullness();
}