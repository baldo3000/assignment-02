#include "WarningTask.h"
#include "kernel/Logger.h"

WarningTask::WarningTask(WasteDisposalSystem *pSystem) : pSystem(pSystem)
{
    this->state = MUTED;
}

void WarningTask::tick()
{
    switch (this->state)
    {
    case MUTED:
        this->pSystem->stopSound();
        this->state = PLAYING;
        break;

    case PLAYING:
        this->pSystem->playSound(WARNING_TONE);
        this->state = MUTED;
        break;
    }
}

void WarningTask::reset()
{
    this->pSystem->stopSound();
    this->state = MUTED;
}