#ifndef PIR_H
#define PIR_H

#include "PresenceSensor.h"

#define CALIBRATION_TIME 10000

class Pir : public PresenceSensor
{
private:
    int pin;
    bool motionDetected;

public:
    Pir(const int pin);
    void calibrate();
    bool isMotionDetected();
    void sync();
};

#endif