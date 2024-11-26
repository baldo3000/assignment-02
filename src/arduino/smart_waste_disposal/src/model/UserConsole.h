#ifndef USERCONSOLE_H
#define USERCONSOLE_H

#include "config.h"
#include "devices/ButtonImpl.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class UserConsole
{
private:
    ButtonImpl *pOpenButton;
    ButtonImpl *pCloseButton;
    LiquidCrystal_I2C *pLcd;

    void turnOffDisplay();
    void turnOnDisplay();

public:
    UserConsole();

    void init();
    void sync();

    bool openDoorSignal();
    bool closeDoorSignal();

    void displayWelcome();
    void displayReadyToDispose();
    void displayWaitToDispose();
    void displayDisposing();
    void displayDisposingTimeOut();
    void displayDisposingEnding();

    void prepareToSleep();
    void resumeFromSleeping();
};

#endif