#include "Buzzer.h"

#include <Arduino.h>

Buzzer::Buzzer(const int pin) : pin(pin)
{
    pinMode(pin, OUTPUT);
    noSound();
}

void Buzzer::playSound(const unsigned int frequency)
{
    tone(this->pin, frequency);
}

void Buzzer::noSound()
{
    noTone(this->pin);
}