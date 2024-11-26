#include "UserConsole.h"
#include <Arduino.h>
#include "devices/ButtonImpl.h"
#include "kernel/MsgService.h"
#include <avr/sleep.h>
#include "config.h"
#include "kernel/Logger.h"

UserConsole::UserConsole()
{
    this->pLcd = new LiquidCrystal_I2C(0x27, 20, 4);
    this->pOpenButton = new ButtonImpl(OPEN_DOOR_PIN);
    this->pCloseButton = new ButtonImpl(CLOSE_DOOR_PIN);
}

void UserConsole::turnOnDisplay()
{
    this->pLcd->display();
    this->pLcd->clear();
}

void UserConsole::turnOffDisplay()
{
    this->pLcd->noDisplay();
}

void UserConsole::init()
{
    this->pLcd->init();
    this->pLcd->backlight();
    this->pLcd->noDisplay();
}

void UserConsole::sync()
{
    this->pOpenButton->sync();
    this->pCloseButton->sync();
}

bool UserConsole::openDoorSignal()
{
    return this->pOpenButton->isPressed();
}

bool UserConsole::closeDoorSignal()
{
    return this->pCloseButton->isPressed();
}

void UserConsole::displayWelcome()
{
    this->pLcd->clear();
    this->pLcd->setCursor(0, 0);
    // pLcd->print("Welcome");
}

void UserConsole::displayReadyToDispose()
{
    this->pLcd->clear();
    this->pLcd->setCursor(0, 0);
}

void UserConsole::displayWaitToDispose()
{
    this->pLcd->clear();
    this->pLcd->setCursor(0, 0);
}

void UserConsole::displayDisposing()
{
    this->pLcd->clear();
    this->pLcd->setCursor(0, 0);
}

void UserConsole::displayDisposingTimeOut()
{
    this->pLcd->clear();
    this->pLcd->setCursor(0, 0);
}

void UserConsole::displayDisposingEnding()
{
    this->pLcd->clear();
    this->pLcd->setCursor(0, 0);
}

void UserConsole::prepareToSleep()
{
    this->pLcd->noDisplay();
}

void UserConsole::resumeFromSleeping()
{
    this->pLcd->display();
}
