#include "Pir.h"
#include <Arduino.h>

Pir::Pir(const int pin) : pin(pin)
{
    pinMode(pin, INPUT);
}

void Pir::calibrate()
{
    delay(CALIBRATION_TIME);
}

bool Pir::isMotionDetected()
{
    return this->motionDetected;
}

void Pir::sync()
{
    this->motionDetected = digitalRead(this->pin);
}
