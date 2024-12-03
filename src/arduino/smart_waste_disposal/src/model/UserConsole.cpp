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

bool UserConsole::openDoorSignal()
{
    this->pOpenButton->sync();
    return this->pOpenButton->isPressed();
}

bool UserConsole::closeDoorSignal()
{
    this->pCloseButton->sync();
    return this->pCloseButton->isPressed();
}

void UserConsole::displayWelcome()
{
    this->pLcd->clear();
    this->pLcd->setCursor(0, 0);
    this->pLcd->print("WELCOME");
}

void UserConsole::displayReadyToDispose()
{
    //this->pLcd->clear();
    this->pLcd->setCursor(0, 0);
    this->pLcd->print("PRESS OPEN TO ENTER WASTE");
}

void UserConsole::displayDisposing()
{
    //this->pLcd->clear();
    this->pLcd->setCursor(0, 0);
    this->pLcd->print("PRESS CLOSE WHEN DONE");
}

void UserConsole::displayDisposingEnd()
{
    //this->pLcd->clear();
    this->pLcd->setCursor(0, 0);
    this->pLcd->print("WASTE RECEIVED");
}

void UserConsole::displayFull()
{
    //this->pLcd->clear();
    this->pLcd->setCursor(0, 0);
    this->pLcd->print("CONTAINER FULL");
}

void UserConsole::displayProblem()
{
    //this->pLcd->clear();
    this->pLcd->setCursor(0, 0);
    this->pLcd->print("PROBLEM DETECTED");
}

void UserConsole::prepareToSleep()
{
    this->pLcd->clear();
    this->pLcd->noDisplay();
    this->pLcd->noBacklight();
}

void UserConsole::resumeFromSleeping()
{
    this->pLcd->clear();
    this->pLcd->display();
    this->pLcd->backlight();
}
