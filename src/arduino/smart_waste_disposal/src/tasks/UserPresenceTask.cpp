#include "UserPresenceTask.h"

UserPresenceTask::UserPresenceTask(WasteDisposalSystem *pSystem) : pSystem(pSystem) {}

void UserPresenceTask::tick()
{
    pSystem->sampleUserPresence();
}