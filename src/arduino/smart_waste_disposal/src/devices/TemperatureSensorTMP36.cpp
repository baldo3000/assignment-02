#include "TemperatureSensorTMP36.h"
#include <Arduino.h>

TemperatureSensorTMP36::TemperatureSensorTMP36(const int pin) : pin(pin) {
    this->sensor = new DHT(pin, DHTTYPE);
    this->sensor->begin();
}

float TemperatureSensorTMP36::getTemperature()
{
    // int value = analogRead(this->pin);
    // float voltage = (value / 1024.0) * VCC;
    // //Serial.println(analogRead(this->pin));
    // float tempC = (voltage - 0.5) * 100.0;
    // return 0;
    // //return (tempC - 32.0) * 5.0 / 9.0; // this useless convertion is to make up for my faulty TMP36 sensor
    return this->sensor->readTemperature();
}