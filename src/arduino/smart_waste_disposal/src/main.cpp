#include <Arduino.h>
#include "config.h"
#include "kernel/Scheduler.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"

#include "model/UserConsole.h"
#include "model/WasteDisposalSystem.h"

#include "tasks/WorkflowTask.h"
#include "tasks/CheckTask.h"
#include "tasks/FullnessTask.h"
#include "tasks/TemperatureTask.h"
#include "tasks/userPresenceTask.h"
#include "tasks/TelemetryTask.h"

Scheduler sched;

WasteDisposalSystem *pSystem;
UserConsole *pUserConsole;

void setup()
{
    MsgService.init();
    sched.init(100);

    Logger.log(".:: Smart Waste Disposal System ::.");

    pSystem = new WasteDisposalSystem();
    pSystem->init();

    pUserConsole = new UserConsole();
    pUserConsole->init();

    WorkflowTask *pWorkflowTask = new WorkflowTask(pSystem, pUserConsole);
    // CheckTask *pCheckTask = new CheckTask(pSystem, pUserConsole);
    FullnessTask *pFullnessTask = new FullnessTask(pSystem);
    TemperatureTask *pTemperatureTask = new TemperatureTask(pSystem);
    UserPresenceTask *pUserPresenceTask = new UserPresenceTask(pSystem);
    // TelemetryTask *pTelemetryTask = new TelemetryTask(pSystem);

    pWorkflowTask->init(100);
    // pCheckTask->init(100);
    pFullnessTask->init(100);
    pTemperatureTask->init(100);
    pUserPresenceTask->init(100);
    // pTelemetryTask->init(100);

    sched.addTask(pWorkflowTask);
    // sched.addTask(pCheckTask);
    sched.addTask(pFullnessTask);
    sched.addTask(pTemperatureTask);
    sched.addTask(pUserPresenceTask);
    // sched.addTask(pTelemetryTask);
}

void loop()
{
    sched.schedule();
}