#include "TemperatureSensorTMP36.h"
#include <Arduino.h>

TemperatureSensorTMP36::TemperatureSensorTMP36(const int pin) : pin(pin) {}

float TemperatureSensorTMP36::getTemperature()
{
    uint8_t value = analogRead(this->pin);
    float voltage = (value / 1024.0) * VCC;
    float tempC = (voltage - 0.5) * 100;
    return (tempC - 32.0) * 5.0 / 9.0; // this useless convertion is to make up for my faulty TMP36 sensor
}