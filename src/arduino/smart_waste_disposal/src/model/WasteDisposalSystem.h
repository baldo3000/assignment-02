#ifndef WASTEDISPOSALSYSTEM_H
#define WASTEDISPOSALSYSTEM_H

#include "devices/ButtonImpl.h"
#include "devices/Led.h"
#include "devices/Pir.h"
#include "devices/ServoMotor.h"
#include "devices/Sonar.h"
#include "devices/TemperatureSensor.h"
#include "devices/Buzzer.h"

class WasteDisposalSystem
{
private:
    Led *pGreenLed;
    Led *pRedLed;
    Pir *pPresenceSensor;
    ServoMotor *pDoorMotor;
    Sonar *pProximitySensor;
    TemperatureSensor *pTemperatureSensor;
    Buzzer *pBuzzer;

    double fullness;
    double temperature;
    bool userPresence;

    enum State
    {
        IDLE,
        WAITING_FOR_USER,
        USER_DETECTED,
        DISPOSING,
        OVERHEATED,
        FULL
    } state;

    double distanceToFullness(double distance);

public:
    WasteDisposalSystem();

    void init();

    // sleep
    void prepareToSleep();
    void resumeFromSleeping();

    bool isIdle();
    bool isWaitingForUser();
    bool isUserDetected();
    bool isDisposing();
    bool isFull();
    bool isOverheated();

    void idle();
    void waitForUser();
    void userDetected();
    void dispose();
    void full();
    void overheated();

    double getCurrentFullness();
    bool detectedUserPresence();
    double getCurrentTemperature();

    // hardware control
    void openDoor();
    void closeDoor();
    void openReverseDoor();
    void setLed1On(bool on);
    void setLed2On(bool on);
    void playSound(unsigned int frequency);
    void stopSound();
    void sampleUserPresence();
    void sampleFullness();
    void sampleTemperature();
};

#endif