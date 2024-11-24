#ifndef SONAR_H
#define SONAR_H

#include "ProximitySensor.h"

class Sonar : public ProximitySensor
{
private:
    float temperature;
    float soundSpeed;
    int trigPin, echoPin;
    long timeOut;

    void updateSoundSpeed();

public:
    Sonar(int trigPin, int echoPin, long timeOut);
    float getDistance();
    void setTemperature(float temp);
};

#endif