#ifndef TEMPERATURESENSORTMP36_H
#define TEMPERATURESENSORTMP36_H

#include "TemperatureSensor.h"
#include <DHT.h>

#define VCC 5.0
#define DHTTYPE DHT11 // DHT 11

class TemperatureSensorTMP36 : public TemperatureSensor
{
private:
    int pin;
    DHT *sensor;

public:
    TemperatureSensorTMP36(int pin);
    float getTemperature();
};

#endif