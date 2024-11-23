#ifndef SONAR_H
#define SONAR_H

#include "ProximitySensor.h"

class Sonar : public ProximitySensor
{
private:
    float temperature;
    float soundSpeed;
    int echoPin, trigPin;
    long timeOut;

    void updateSoundSpeed();

public:
    Sonar(int echoPin, int trigPin, long timeOut);
    float getDistance();
    void setTemperature(float temp);
};

#endif