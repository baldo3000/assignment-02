#include "TemperatureSensorTMP36.h"

TemperatureSensorTMP36::TemperatureSensorTMP36(const int pin)
{
    this->sensor = new TMP36(pin, VCC);
}

TemperatureSensorTMP36::~TemperatureSensorTMP36()
{
    delete (this->sensor);
}

float TemperatureSensorTMP36::getTemperature()
{
    return this->sensor->getTempC();
}