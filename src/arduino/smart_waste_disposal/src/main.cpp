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
    pUserConsole->turnOnDisplay();

    FullnessTask *pFullnessTask = new FullnessTask(pSystem);
    TemperatureTask *pTemperatureTask = new TemperatureTask(pSystem);
    UserPresenceTask *pUserPresenceTask = new UserPresenceTask(pSystem);
    WorkflowTask *pWorkflowTask = new WorkflowTask(pSystem, pUserConsole);
    CheckTask *pCheckTask = new CheckTask(pSystem, pUserConsole);
    TelemetryTask *pTelemetryTask = new TelemetryTask(pSystem, pUserConsole);

    pFullnessTask->init(200);
    pTemperatureTask->init(100);
    pUserPresenceTask->init(100);
    pWorkflowTask->init(100);
    pCheckTask->init(100);
    pTelemetryTask->init(100);

    sched.addTask(pFullnessTask);
    sched.addTask(pTemperatureTask);
    sched.addTask(pUserPresenceTask);
    sched.addTask(pWorkflowTask);
    sched.addTask(pCheckTask);
    sched.addTask(pTelemetryTask);
}

void logSensors()
{
    Logger.log("Temperature: " + String(pSystem->getCurrentTemperature()) + "°C" +
               " Fullness: " + String(pSystem->getCurrentFullness()) + "%" +
               " User Presence: " + String(pSystem->detectedUserPresence()));
}

void loop()
{
    sched.schedule();
    // logSensors();
    // Logger.log(String(pUserConsole->closeDoorSignal()));
    // Serial.println(millis());
}