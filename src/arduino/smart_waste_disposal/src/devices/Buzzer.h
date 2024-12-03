#ifndef BUZZER_H
#define BUZZER_H

class Buzzer
{
private:
    int pin;

public:
    Buzzer(int pin);
    void playSound(unsigned int frequency);
    void noSound();
};

#endif