#include "WasteDisposalSystem.h"
#include "devices/TemperatureSensorTMP36.h"
#include "devices/ServoMotorImpl.h"
#include "hardwareConfig.h"
#include "config.h"
#include "kernel/Logger.h"
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
    this->pBuzzer = new Buzzer(BUZZER_PIN);

    Logger.log("Calibrating sensors in plant...");
    this->pPresenceSensor->calibrate();
    this->pDoorMotor->on();
    Logger.log("Calibration done");

    this->fullness = 0;
    this->temperature = 0;
    this->userPresence = false;

    this->state = IDLE;
}

void WasteDisposalSystem::prepareToSleep()
{
    this->pDoorMotor->off();
}

void WasteDisposalSystem::resumeFromSleeping()
{
    this->pDoorMotor->on();
}

bool WasteDisposalSystem::isIdle()
{
    return this->state == IDLE;
}

bool WasteDisposalSystem::isWaitingForUser()
{
    return this->state == WAITING_FOR_USER;
}

bool WasteDisposalSystem::isUserDetected()
{
    return this->state == USER_DETECTED;
}

bool WasteDisposalSystem::isDisposing()
{
    return this->state == DISPOSING;
}

void WasteDisposalSystem::idle()
{
    this->state = IDLE;
}

void WasteDisposalSystem::waitForUser()
{
    this->state = WAITING_FOR_USER;
}

void WasteDisposalSystem::userDetected()
{
    this->state = USER_DETECTED;
}

void WasteDisposalSystem::dispose()
{
    this->state = DISPOSING;
}

void WasteDisposalSystem::full()
{
    this->state = FULL;
}

void WasteDisposalSystem::overheated()
{
    this->state = OVERHEATED;
}

bool WasteDisposalSystem::isFull()
{
    return this->state == FULL;
}

bool WasteDisposalSystem::isOverheated()
{
    return this->state == OVERHEATED;
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

void WasteDisposalSystem::openReverseDoor()
{
    this->pDoorMotor->setPosition(0);
}

void WasteDisposalSystem::setLed1On(const bool on)
{
    if (on)
    {
        this->pGreenLed->switchOn();
    }
    else
    {
        this->pGreenLed->switchOff();
    }
}

void WasteDisposalSystem::setLed2On(const bool on)
{
    if (on)
    {
        this->pRedLed->switchOn();
    }
    else
    {
        this->pRedLed->switchOff();
    }
}

void WasteDisposalSystem::playSound(const unsigned int frequency)
{
    this->pBuzzer->playSound(frequency);
}

void WasteDisposalSystem::stopSound()
{
    this->pBuzzer->noSound();
}

void WasteDisposalSystem::sampleUserPresence()
{
    this->pPresenceSensor->sync();
    this->userPresence = this->pPresenceSensor->isMotionDetected();
}

double doubleMap(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double WasteDisposalSystem::distanceToFullness(const double distance)
{
    double correctedDistance = distance;
    if (distance > MIN_FULLNESS_DISTANCE || distance == NO_OBJ_DETECTED)
    {
        correctedDistance = MIN_FULLNESS_DISTANCE;
    }
    else if (distance < MAX_FULLNESS_DISTANCE)
    {
        correctedDistance = MAX_FULLNESS_DISTANCE;
    }
    return 100.0 - doubleMap(correctedDistance, MAX_FULLNESS_DISTANCE, MIN_FULLNESS_DISTANCE, 0.0, 100);
}

void WasteDisposalSystem::sampleFullness()
{
    const double distance = this->pProximitySensor->getDistance();
    this->fullness = distanceToFullness(distance);
}

void WasteDisposalSystem::sampleTemperature()
{
    this->temperature = this->pTemperatureSensor->getTemperature();
}