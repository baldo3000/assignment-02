#include "ButtonImpl.h"
#include "Arduino.h"

ButtonImpl::ButtonImpl(const int pin) : pin(pin)
{
    pinMode(pin, INPUT);
    sync();
}

bool ButtonImpl::isPressed()
{
    return this->pressed;
}

void ButtonImpl::sync()
{
    const long curTime = millis();
    if (curTime - lastTimeSync > DEBOUNCE_TIME)
    {
        this->pressed = digitalRead(this->pin);
        this->lastTimeSync = curTime;
    }
}
