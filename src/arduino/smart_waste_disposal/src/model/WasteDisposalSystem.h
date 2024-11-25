#ifndef WASTEDISPOSALSYSTEM_H
#define WASTEDISPOSALSYSTEM_H

#include "devices/ButtonImpl.h"
#include "devices/Led.h"
#include "devices/Pir.h"
#include "devices/ServoMotor.h"
#include "devices/Sonar.h"
#include "devices/TemperatureSensor.h"

class WasteDisposalSystem
{
private:
    Led *pGreenLed;
    Led *pRedLed;
    // ButtonImpl *pOpenButton;
    // ButtonImpl *pCloseButton;
    Pir *pPresenceSensor;
    ServoMotor *pDoorMotor;
    Sonar *pProximitySensor;
    TemperatureSensor *pTemperatureSensor;

    double fullness;
    double temperature;
    bool userPresence;

    enum
    {
        IDLE,
        WAITING_FOR_USER,
        USER_DETECTED,
        READY_TO_DISPOSE,
        DISPOSING,
        DISPOSING_COMPLETED,
        MAINTENANCE,
    } state;

public:
    WasteDisposalSystem();

    void init();

    // sleep
    void prepareToSleep();
    void resumeFromSleeping();

    void idle();
    bool isIdle();

    void waitForUser();
    bool isWaitingForUser();

    void userDetected();
    bool isUserDetected();

    void readyToDispose();
    bool isReadyToDispose();

    void dispose();
    bool isDisposing();

    void disposeCompleted();
    bool isDisposeCompleted();

    void setInMaintenance();
    bool isInMaintenance();
    void maintenanceDone();

    double getCurrentFullness();
    bool detectedUserPresence();
    double getCurrentTemperature();

    // hardware control
    void openDoor();
    void closeDoor();
    void sampleUserPresence();
    void sampleFullness();
    void sampleTemperature();
};

#endif