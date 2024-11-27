#include "WasteDisposalSystem.h"
#include "devices/TemperatureSensorTMP36.h"
#include "devices/ServoMotorImpl.h"
#include "hardwareConfig.h"
#include <Arduino.h>

WasteDisposalSystem::WasteDisposalSystem() {}

void WasteDisposalSystem::init()
{
    this->pProximitySensor = new Sonar(SONAR_TRIG_PIN, SONAR_ECHO_PIN);
    this->pPresenceSensor = new Pir(PIR_PIN);
    this->pTemperatureSensor = new TemperatureSensorTMP36(TEMP_SENSOR_PIN);
    this->pGreenLed = new Led(LED_GREEN_PIN);
    this->pRedLed = new Led(LED_RED_PIN);
    this->pDoorMotor = new ServoMotorImpl(DOOR_PIN);

    // Logger.log("Calibrating sensors in plant...");
    this->pPresenceSensor->calibrate();
    this->pDoorMotor->on();

    this->fullness = 0;
    this->temperature = 0;
    this->userPresence = false;

    this->state = IDLE;
}

// TODO: Implement what to do before sleeping (turning off things)
void WasteDisposalSystem::prepareToSleep() {}
// TODO: Implement what to do after sleeping (restoring)
void WasteDisposalSystem::resumeFromSleeping() {}

void WasteDisposalSystem::idle()
{
    this->state = IDLE;
}

bool WasteDisposalSystem::isIdle()
{
    return this->state == IDLE;
}

void WasteDisposalSystem::waitForUser()
{
    this->state = WAITING_FOR_USER;
}

bool WasteDisposalSystem::isWaitingForUser()
{
    return this->state == WAITING_FOR_USER;
}

void WasteDisposalSystem::userDetected()
{
    this->state = USER_DETECTED;
}

bool WasteDisposalSystem::isUserDetected()
{
    return this->state == USER_DETECTED;
}

void WasteDisposalSystem::readyToDispose()
{
    this->state = READY_TO_DISPOSE;
}

bool WasteDisposalSystem::isReadyToDispose()
{
    return this->state == READY_TO_DISPOSE;
}

void WasteDisposalSystem::dispose()
{
    openDoor();
    this->state = DISPOSING;
}

bool WasteDisposalSystem::isDisposing()
{
    return this->state == DISPOSING;
}

void WasteDisposalSystem::disposeCompleted()
{
    closeDoor();
    this->state = DISPOSING_COMPLETED;
}

bool WasteDisposalSystem::isDisposeCompleted()
{
    return this->state == DISPOSING_COMPLETED;
}

void WasteDisposalSystem::setInMaintenance()
{
    this->state = MAINTENANCE;
}

bool WasteDisposalSystem::isInMaintenance()
{
    return this->state == MAINTENANCE;
}

void WasteDisposalSystem::maintenanceDone()
{
    this->state = IDLE;
}

double WasteDisposalSystem::getCurrentFullness()
{
    return this->fullness;
}

bool WasteDisposalSystem::detectedUserPresence()
{
    return this->userPresence;
}

double WasteDisposalSystem::getCurrentTemperature()
{
    return this->temperature;
}

// hardware control
void WasteDisposalSystem::openDoor()
{
    this->pDoorMotor->setPosition(180);
}

void WasteDisposalSystem::closeDoor()
{
    this->pDoorMotor->setPosition(90);
}

void WasteDisposalSystem::sampleUserPresence()
{
    this->pPresenceSensor->sync();
    this->userPresence = this->pPresenceSensor->isMotionDetected();
}

void WasteDisposalSystem::sampleFullness()
{
    // TODO: Implement fullness boundaries
    this->fullness = this->pProximitySensor->getDistance();
}

void WasteDisposalSystem::sampleTemperature()
{
    this->temperature = this->pTemperatureSensor->getTemperature();
}