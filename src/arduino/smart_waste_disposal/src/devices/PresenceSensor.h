#ifndef PRESENCESENSOR_H
#define PRESENCESENSOR_H

class PresenceSensor
{
public:
    virtual bool isMotionDetected() = 0;
};

#endif