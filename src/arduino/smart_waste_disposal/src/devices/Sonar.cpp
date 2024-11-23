#include "Sonar.h"
#include <Arduino.h>

Sonar::Sonar(const int echoPin, const int trigPin, const long timeOut) : echoPin(echoPin), trigPin(trigPin), timeOut(timeOut)
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
    delayMicroseconds(3);
    digitalWrite(this->trigPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(this->trigPin, LOW);

    const float timeMicroseconds = pulseIn(this->echoPin, HIGH, this->timeOut);
    if (timeMicroseconds == 0)
    {
        return NO_OBJ_DETECTED;
    }
    else
    {
        const float timeSeconds = timeMicroseconds / 1000.0 / 1000.0 / 2;
        const float distance = timeSeconds * this->soundSpeed;
        return distance;
    }
}