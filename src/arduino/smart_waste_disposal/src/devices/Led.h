#ifndef LED_H
#define LED_H

#include "Light.h"

class Led : public Light
{
private:
    int pin;

public:
    Led(const int pin);
    void switchOn();
    void switchOff();
};

#endif