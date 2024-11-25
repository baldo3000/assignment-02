#ifndef SONAR_H
#define SONAR_H

#include "ProximitySensor.h"

class Sonar : public ProximitySensor
{
private:
    float temperature;
    float soundSpeed;
    int trigPin, echoPin;

    void updateSoundSpeed();

public:
    Sonar(int trigPin, int echoPin);
    float getDistance();
    void setTemperature(float temp);
};

#endif