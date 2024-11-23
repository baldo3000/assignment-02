#ifndef PROXIMITYSENSOR_H
#define PROXIMITYSENSOR_H

#define NO_OBJ_DETECTED -1

class ProximitySensor
{
public:
    virtual float getDistance() = 0;
};

#endif