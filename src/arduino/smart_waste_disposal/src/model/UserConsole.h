#ifndef USERCONSOLE_H
#define USERCONSOLE_H

#include "hardwareConfig.h"
#include "devices/ButtonImpl.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class UserConsole
{
private:
    ButtonImpl *pOpenButton;
    ButtonImpl *pCloseButton;
    LiquidCrystal_I2C *pLcd;

public:
    UserConsole();

    void init();

    void turnOffDisplay();
    void turnOnDisplay();

    bool openDoorSignal();
    bool closeDoorSignal();

    void displayWelcome();
    void displayReadyToDispose();
    void displayDisposing();
    void displayDisposingEnd();
    void displayFull();
    void displayProblem();

    void prepareToSleep();
    void resumeFromSleeping();
};

#endif