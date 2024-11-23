#ifndef TEMPERATURESENSORTMP36_H
#define TEMPERATURESENSORTMP36_H

#include "TemperatureSensor.h"
#include <TMP36.h>

#define VCC ((float)5)

class TemperatureSensorTMP36 : public TemperatureSensor
{
private:
    TMP36 *sensor;

public:
    TemperatureSensorTMP36(int pin);
    ~TemperatureSensorTMP36();
    float getTemperature();
};

#endif