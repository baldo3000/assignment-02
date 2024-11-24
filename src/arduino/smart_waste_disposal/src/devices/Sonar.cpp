#include "Sonar.h"
#include <Arduino.h>

Sonar::Sonar(const int trigPin, const int echoPin, const long timeOut) : trigPin(trigPin), echoPin(echoPin), timeOut(timeOut)
{
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    setTemperature(20);
}

void Sonar::updateSoundSpeed()
{
    this->soundSpeed = 331.5 + 0.6 * temperature;
}

void Sonar::setTemperature(const float temp)
{
    this->temperature = temp;
    updateSoundSpeed();
}

float Sonar::getDistance()
{
    digitalWrite(this->trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(this->trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(this->trigPin, LOW);

    noInterrupts();
    float d = pulseIn(this->echoPin, HIGH, 23529.4); // max sensor dist ~4m
    interrupts();
    d = d != 0 ? d / 58.8235 : NO_OBJ_DETECTED;
    return d;
}