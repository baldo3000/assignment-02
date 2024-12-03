#ifndef TEMPERATURESENSORTMP36_H
#define TEMPERATURESENSORTMP36_H

#include "TemperatureSensor.h"

#define VCC 5.0F

class TemperatureSensorTMP36 : public TemperatureSensor
{
private:
    int pin;

public:
    TemperatureSensorTMP36(int pin);
    float getTemperature();
};

#endif